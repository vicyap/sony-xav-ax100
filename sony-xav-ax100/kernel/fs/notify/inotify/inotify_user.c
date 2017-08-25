/*
 * fs/inotify_user.c - inotify support for userspace
 *
 * Authors:
 *	John McCutchan	<ttb@tentacle.dhs.org>
 *	Robert Love	<rml@novell.com>
 *
 * Copyright (C) 2005 John McCutchan
 * Copyright 2006 Hewlett-Packard Development Company, L.P.
 *
 * Copyright (C) 2009 Eric Paris <Red Hat Inc>
 * inotify was largely rewriten to make use of the fsnotify infrastructure
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/file.h>
#include <linux/fs.h> /* struct inode */
#include <linux/fsnotify_backend.h>
#include <linux/idr.h>
#include <linux/init.h> /* module_init */
#include <linux/inotify.h>
#include <linux/kernel.h> /* roundup() */
#include <linux/namei.h> /* LOOKUP_FOLLOW */
#include <linux/sched.h> /* struct user */
#include <linux/slab.h> /* struct kmem_cache */
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/anon_inodes.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/wait.h>

#include "inotify.h"

#include <asm/ioctls.h>

#include <asm/warp.h>

#ifdef WARP_AM_INOTIFY

#include <linux/fdtable.h>
#include <asm/errno.h>
#include <asm/atomic.h>

#endif /* WARP_AM_INOTIFY */

/* these are configurable via /proc/sys/fs/inotify/ */
static int inotify_max_user_instances __read_mostly;
static int inotify_max_queued_events __read_mostly;
static int inotify_max_user_watches __read_mostly;

static struct kmem_cache *inotify_inode_mark_cachep __read_mostly;
struct kmem_cache *event_priv_cachep __read_mostly;

#ifdef WARP_AM_INOTIFY

//#define __WARP_DEBUG_LOG 1

#ifdef __WARP_DEBUG_LOG
#define __WARP_DEBUG_DUMP 1
#endif /* __WARP_DEBUG_LOG */

static bool warp_mode = false;
static LIST_HEAD(warp_inotify_list);
DEFINE_MUTEX(warp_mutex);
#define WARP_LOCK	if (!in_atomic()) mutex_lock(&warp_mutex)
#define WARP_UNLOCK	if (!in_atomic()) mutex_unlock(&warp_mutex)

#ifdef WARP_UMOUNT_RW
#define WARP_UMOUNT_RW_NUM (sizeof(warp_umount_rw) / sizeof(char *))
static const char *warp_umount_rw[] = {
		WARP_UMOUNT_RW
};
#endif

struct warp_inotify {
	struct list_head list;
	struct task_struct* task;
	pid_t pid;
	char comm[TASK_COMM_LEN];
	int group_no;
	int inotify_fd;
	int inode_based_fd;
	int inode_now_fd;
	u32 mask;
	struct fsnotify_group *group;
	char *pathname;
};

#endif /* WARP_AM_INOTIFY */

#ifdef CONFIG_SYSCTL

#include <linux/sysctl.h>

static int zero;

ctl_table inotify_table[] = {
	{
		.procname	= "max_user_instances",
		.data		= &inotify_max_user_instances,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
	},
	{
		.procname	= "max_user_watches",
		.data		= &inotify_max_user_watches,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
	},
	{
		.procname	= "max_queued_events",
		.data		= &inotify_max_queued_events,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero
	},
	{ }
};
#endif /* CONFIG_SYSCTL */

#ifdef WARP_AM_INOTIFY

#define WARP_PRINTK(fmt, ...) printk(KERN_INFO "%s(%d):"fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__)

#ifdef __WARP_DEBUG_LOG
#define WARP_DEBUG_PRINTK(fmt, ...) printk(KERN_INFO "%s(%d):"fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#else
#define WARP_DEBUG_PRINTK(fmt, ...)  
#endif /* __WARP_DEBUG_LOG */

#else

#define WARP_PRINTK(fmt, ...) 
#define WARP_DEBUG_PRINTK(fmt, ...)  

#endif /* WARP_AM_INOTIFY */

static inline __u32 inotify_arg_to_mask(u32 arg)
{
	__u32 mask;

	WARP_DEBUG_PRINTK("inotify: Call inotify_arg_to_mask(arg: [%d])\n", arg);

	/*
	 * everything should accept their own ignored, cares about children,
	 * and should receive events when the inode is unmounted
	 */
	mask = (FS_IN_IGNORED | FS_EVENT_ON_CHILD | FS_UNMOUNT);

	/* mask off the flags used to open the fd */
	mask |= (arg & (IN_ALL_EVENTS | IN_ONESHOT | IN_EXCL_UNLINK));

	WARP_DEBUG_PRINTK("inotify: arg: [%x] -> mask: [%x])\n", arg, mask);

	return mask;
}

static inline u32 inotify_mask_to_arg(__u32 mask)
{
	return mask & (IN_ALL_EVENTS | IN_ISDIR | IN_UNMOUNT | IN_IGNORED |
		       IN_Q_OVERFLOW);
}

/* intofiy userspace file descriptor functions */
static unsigned int inotify_poll(struct file *file, poll_table *wait)
{
	struct fsnotify_group *group = file->private_data;
	int ret = 0;

	poll_wait(file, &group->notification_waitq, wait);
	mutex_lock(&group->notification_mutex);
	if (!fsnotify_notify_queue_is_empty(group))
		ret = POLLIN | POLLRDNORM;
	mutex_unlock(&group->notification_mutex);

	return ret;
}

/*
 * Get an inotify_kernel_event if one exists and is small
 * enough to fit in "count". Return an error pointer if
 * not large enough.
 *
 * Called with the group->notification_mutex held.
 */
static struct fsnotify_event *get_one_event(struct fsnotify_group *group,
					    size_t count)
{
	size_t event_size = sizeof(struct inotify_event);
	struct fsnotify_event *event;

	if (fsnotify_notify_queue_is_empty(group))
		return NULL;

	event = fsnotify_peek_notify_event(group);

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	if (event->name_len)
		event_size += roundup(event->name_len + 1, event_size);

	if (event_size > count)
		return ERR_PTR(-EINVAL);

	/* held the notification_mutex the whole time, so this is the
	 * same event we peeked above */
	fsnotify_remove_notify_event(group);

	return event;
}

/*
 * Copy an event to user space, returning how much we copied.
 *
 * We already checked that the event size is smaller than the
 * buffer we had in "get_one_event()" above.
 */
static ssize_t copy_event_to_user(struct fsnotify_group *group,
				  struct fsnotify_event *event,
				  char __user *buf)
{
	struct inotify_event inotify_event;
	struct fsnotify_event_private_data *fsn_priv;
	struct inotify_event_private_data *priv;
	size_t event_size = sizeof(struct inotify_event);
	size_t name_len = 0;
#ifdef WARP_AM_INOTIFY
	struct list_head *p;
	struct warp_inotify *myobj;
	int wd;
#endif

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);
	WARP_DEBUG_PRINTK("group=%p event=%p\n", group, event);
	
	/* we get the inotify watch descriptor from the event private data */
	spin_lock(&event->lock);
	fsn_priv = fsnotify_remove_priv_from_event(group, event);
	spin_unlock(&event->lock);

	if (!fsn_priv)
		inotify_event.wd = -1;
	else {
		priv = container_of(fsn_priv, struct inotify_event_private_data,
				    fsnotify_event_priv_data);
#ifdef WARP_AM_INOTIFY
			
		wd = priv->wd;
		WARP_LOCK;
		list_for_each(p, &warp_inotify_list) {
			myobj = list_entry(p, struct warp_inotify, list);

			WARP_DEBUG_PRINTK("group=%p:%p wd=%d:%d\n"
								, myobj->group, group, myobj->inode_now_fd, priv->wd);
			
			
			if (myobj->group == group && myobj->inode_now_fd == priv->wd) {
				
				priv->wd = myobj->inode_based_fd;
				
				WARP_DEBUG_PRINTK("Bingo! wd:[%d]->[%d]", wd, priv->wd);
				
				break;
				
			}
		}
		WARP_UNLOCK;
		if (wd != priv->wd) {
			WARP_DEBUG_PRINTK("no items. wd=[%d]", priv->wd);
		}
#endif
		inotify_event.wd = priv->wd;
		inotify_free_event_priv(fsn_priv);
	}

	/*
	 * round up event->name_len so it is a multiple of event_size
	 * plus an extra byte for the terminating '\0'.
	 */
	if (event->name_len)
		name_len = roundup(event->name_len + 1, event_size);
	inotify_event.len = name_len;

	inotify_event.mask = inotify_mask_to_arg(event->mask);
	inotify_event.cookie = event->sync_cookie;

	/* send the main event */
#ifdef WARP_AM_INOTIFY
	if (copy_to_user(buf, &inotify_event, event_size)) {
		WARP_DEBUG_PRINTK("copy_event_to_user: -EFAULT(1)\n");
		return -EFAULT;
	}
#else
	if (copy_to_user(buf, &inotify_event, event_size))
		return -EFAULT;
#endif
	buf += event_size;

	/*
	 * fsnotify only stores the pathname, so here we have to send the pathname
	 * and then pad that pathname out to a multiple of sizeof(inotify_event)
	 * with zeros.  I get my zeros from the nul_inotify_event.
	 */
	if (name_len) {
		unsigned int len_to_zero = name_len - event->name_len;
		/* copy the path name */
#ifdef WARP_AM_INOTIFY
		if (copy_to_user(buf, event->file_name, event->name_len)) {
			WARP_DEBUG_PRINTK("copy_event_to_user: -EFAULT(2)\n");
			return -EFAULT;
		}
#else
		if (copy_to_user(buf, event->file_name, event->name_len))
			return -EFAULT;
#endif
		buf += event->name_len;

		/* fill userspace with 0's */
		if (clear_user(buf, len_to_zero))
			return -EFAULT;
		buf += len_to_zero;
		event_size += name_len;
	}

	return event_size;
}

static ssize_t inotify_read(struct file *file, char __user *buf,
			    size_t count, loff_t *pos)
{
	struct fsnotify_group *group;
	struct fsnotify_event *kevent;
	char __user *start;
	int ret;
	DEFINE_WAIT(wait);

	start = buf;
	group = file->private_data;

	while (1) {
		prepare_to_wait(&group->notification_waitq, &wait, TASK_INTERRUPTIBLE);

		mutex_lock(&group->notification_mutex);
		kevent = get_one_event(group, count);
		mutex_unlock(&group->notification_mutex);

		pr_debug("%s: group=%p kevent=%p\n", __func__, group, kevent);

		if (kevent) {
			ret = PTR_ERR(kevent);
			if (IS_ERR(kevent))
				break;
			ret = copy_event_to_user(group, kevent, buf);
			fsnotify_put_event(kevent);
			if (ret < 0)
				break;
			buf += ret;
			count -= ret;
			continue;
		}

		ret = -EAGAIN;
		if (file->f_flags & O_NONBLOCK)
			break;
		ret = -EINTR;
		if (signal_pending(current))
			break;

		if (start != buf)
			break;

		schedule();
	}

	finish_wait(&group->notification_waitq, &wait);
	if (start != buf && ret != -EFAULT)
		ret = buf - start;
	return ret;
}

static int inotify_fasync(int fd, struct file *file, int on)
{
	struct fsnotify_group *group = file->private_data;

	return fasync_helper(fd, file, on, &group->inotify_data.fa) >= 0 ? 0 : -EIO;
}

static int inotify_release(struct inode *ignored, struct file *file)
{
	struct fsnotify_group *group = file->private_data;

	pr_debug("%s: group=%p\n", __func__, group);

	fsnotify_clear_marks_by_group(group);

	/* free this group, matching get was inotify_init->fsnotify_obtain_group */
	fsnotify_put_group(group);

	return 0;
}

static long inotify_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct fsnotify_group *group;
	struct fsnotify_event_holder *holder;
	struct fsnotify_event *event;
	void __user *p;
	int ret = -ENOTTY;
	size_t send_len = 0;

	group = file->private_data;
	p = (void __user *) arg;

	pr_debug("%s: group=%p cmd=%u\n", __func__, group, cmd);

	switch (cmd) {
	case FIONREAD:
		mutex_lock(&group->notification_mutex);
		list_for_each_entry(holder, &group->notification_list, event_list) {
			event = holder->event;
			send_len += sizeof(struct inotify_event);
			if (event->name_len)
				send_len += roundup(event->name_len + 1,
						sizeof(struct inotify_event));
		}
		mutex_unlock(&group->notification_mutex);
		ret = put_user(send_len, (int __user *) p);
		break;
	}

	return ret;
}

static const struct file_operations inotify_fops = {
	.poll		= inotify_poll,
	.read		= inotify_read,
	.fasync		= inotify_fasync,
	.release	= inotify_release,
	.unlocked_ioctl	= inotify_ioctl,
	.compat_ioctl	= inotify_ioctl,
	.llseek		= noop_llseek,
};


/*
 * find_inode - resolve a user-given path to a specific inode
 */
static int inotify_find_inode(const char __user *dirname, struct path *path, unsigned flags)
{
	int error;
	
#ifdef WARP_AM_INOTIFY
	WARP_DEBUG_PRINTK("inotify_find_inode: call.\n");
	error = user_path_at(AT_FDCWD, dirname, flags, path);
	if (error) {
		WARP_DEBUG_PRINTK("inotify_find_inode: user_path_at(error :[%d])\n", error);
		return error;
	}
	/* you can only watch an inode if you have read permissions on it */
	error = inode_permission(path->dentry->d_inode, MAY_READ);
	if (error) {
		WARP_DEBUG_PRINTK("inotify_find_inode: inode_permission(error :[%d])\n", error);
		path_put(path);
	}
	WARP_DEBUG_PRINTK("inotify_find_inode: return(error :[%d])\n", error);

#else

	error = user_path_at(AT_FDCWD, dirname, flags, path);
	if (error)
		return error;
	/* you can only watch an inode if you have read permissions on it */
	error = inode_permission(path->dentry->d_inode, MAY_READ);
	if (error)
		path_put(path);
		
#endif

	return error;
}

static int inotify_add_to_idr(struct idr *idr, spinlock_t *idr_lock,
			      int *last_wd,
			      struct inotify_inode_mark *i_mark)
{
	int ret;

	do {
		if (unlikely(!idr_pre_get(idr, GFP_KERNEL)))
			return -ENOMEM;

		spin_lock(idr_lock);
		ret = idr_get_new_above(idr, i_mark, *last_wd + 1,
					&i_mark->wd);
		/* we added the mark to the idr, take a reference */
		if (!ret) {
			*last_wd = i_mark->wd;
			fsnotify_get_mark(&i_mark->fsn_mark);
		}
		spin_unlock(idr_lock);
	} while (ret == -EAGAIN);

	return ret;
}

static struct inotify_inode_mark *inotify_idr_find_locked(struct fsnotify_group *group,
								int wd)
{
	struct idr *idr = &group->inotify_data.idr;
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;
	struct inotify_inode_mark *i_mark;

	assert_spin_locked(idr_lock);

	i_mark = idr_find(idr, wd);
	if (i_mark) {
		struct fsnotify_mark *fsn_mark = &i_mark->fsn_mark;

		fsnotify_get_mark(fsn_mark);
		/* One ref for being in the idr, one ref we just took */
		BUG_ON(atomic_read(&fsn_mark->refcnt) < 2);
	}

	return i_mark;
}

static struct inotify_inode_mark *inotify_idr_find(struct fsnotify_group *group,
							 int wd)
{
	struct inotify_inode_mark *i_mark;
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;

	spin_lock(idr_lock);
	i_mark = inotify_idr_find_locked(group, wd);
	spin_unlock(idr_lock);

	return i_mark;
}

static void do_inotify_remove_from_idr(struct fsnotify_group *group,
				       struct inotify_inode_mark *i_mark)
{
	struct idr *idr = &group->inotify_data.idr;
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;
	int wd = i_mark->wd;

	assert_spin_locked(idr_lock);

	idr_remove(idr, wd);

	/* removed from the idr, drop that ref */
	fsnotify_put_mark(&i_mark->fsn_mark);
}

/*
 * Remove the mark from the idr (if present) and drop the reference
 * on the mark because it was in the idr.
 */
static void inotify_remove_from_idr(struct fsnotify_group *group,
				    struct inotify_inode_mark *i_mark)
{
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;
	struct inotify_inode_mark *found_i_mark = NULL;
	int wd;

	spin_lock(idr_lock);
	wd = i_mark->wd;

	/*
	 * does this i_mark think it is in the idr?  we shouldn't get called
	 * if it wasn't....
	 */
	if (wd == -1) {
		WARN_ONCE(1, "%s: i_mark=%p i_mark->wd=%d i_mark->group=%p"
			" i_mark->inode=%p\n", __func__, i_mark, i_mark->wd,
			i_mark->fsn_mark.group, i_mark->fsn_mark.i.inode);
		goto out;
	}

	/* Lets look in the idr to see if we find it */
	found_i_mark = inotify_idr_find_locked(group, wd);
	if (unlikely(!found_i_mark)) {
		WARN_ONCE(1, "%s: i_mark=%p i_mark->wd=%d i_mark->group=%p"
			" i_mark->inode=%p\n", __func__, i_mark, i_mark->wd,
			i_mark->fsn_mark.group, i_mark->fsn_mark.i.inode);
		goto out;
	}

	/*
	 * We found an mark in the idr at the right wd, but it's
	 * not the mark we were told to remove.  eparis seriously
	 * fucked up somewhere.
	 */
	if (unlikely(found_i_mark != i_mark)) {
		WARN_ONCE(1, "%s: i_mark=%p i_mark->wd=%d i_mark->group=%p "
			"mark->inode=%p found_i_mark=%p found_i_mark->wd=%d "
			"found_i_mark->group=%p found_i_mark->inode=%p\n",
			__func__, i_mark, i_mark->wd, i_mark->fsn_mark.group,
			i_mark->fsn_mark.i.inode, found_i_mark, found_i_mark->wd,
			found_i_mark->fsn_mark.group,
			found_i_mark->fsn_mark.i.inode);
		goto out;
	}

	/*
	 * One ref for being in the idr
	 * one ref held by the caller trying to kill us
	 * one ref grabbed by inotify_idr_find
	 */
	if (unlikely(atomic_read(&i_mark->fsn_mark.refcnt) < 3)) {
		printk(KERN_ERR "%s: i_mark=%p i_mark->wd=%d i_mark->group=%p"
			" i_mark->inode=%p\n", __func__, i_mark, i_mark->wd,
			i_mark->fsn_mark.group, i_mark->fsn_mark.i.inode);
		/* we can't really recover with bad ref cnting.. */
		BUG();
	}

	do_inotify_remove_from_idr(group, i_mark);
out:
	/* match the ref taken by inotify_idr_find_locked() */
	if (found_i_mark)
		fsnotify_put_mark(&found_i_mark->fsn_mark);
	i_mark->wd = -1;
	spin_unlock(idr_lock);
}

/*
 * Send IN_IGNORED for this wd, remove this wd from the idr.
 */
void inotify_ignored_and_remove_idr(struct fsnotify_mark *fsn_mark,
				    struct fsnotify_group *group)
{
	struct inotify_inode_mark *i_mark;
	struct fsnotify_event *ignored_event, *notify_event;
	struct inotify_event_private_data *event_priv;
	struct fsnotify_event_private_data *fsn_event_priv;
	int ret;

	ignored_event = fsnotify_create_event(NULL, FS_IN_IGNORED, NULL,
					      FSNOTIFY_EVENT_NONE, NULL, 0,
					      GFP_NOFS);
	if (!ignored_event)
		return;

	i_mark = container_of(fsn_mark, struct inotify_inode_mark, fsn_mark);

	event_priv = kmem_cache_alloc(event_priv_cachep, GFP_NOFS);
	if (unlikely(!event_priv))
		goto skip_send_ignore;

	fsn_event_priv = &event_priv->fsnotify_event_priv_data;

	fsn_event_priv->group = group;
	event_priv->wd = i_mark->wd;

	notify_event = fsnotify_add_notify_event(group, ignored_event, fsn_event_priv, NULL);
	if (notify_event) {
		if (IS_ERR(notify_event))
			ret = PTR_ERR(notify_event);
		else
			fsnotify_put_event(notify_event);
		inotify_free_event_priv(fsn_event_priv);
	}

skip_send_ignore:

	/* matches the reference taken when the event was created */
	fsnotify_put_event(ignored_event);

	/* remove this mark from the idr */
	inotify_remove_from_idr(group, i_mark);

	atomic_dec(&group->inotify_data.user->inotify_watches);
}

/* ding dong the mark is dead */
static void inotify_free_mark(struct fsnotify_mark *fsn_mark)
{
	struct inotify_inode_mark *i_mark;

	i_mark = container_of(fsn_mark, struct inotify_inode_mark, fsn_mark);

	kmem_cache_free(inotify_inode_mark_cachep, i_mark);
}

static int inotify_update_existing_watch(struct fsnotify_group *group,
					 struct inode *inode,
					 u32 arg)
{
	struct fsnotify_mark *fsn_mark;
	struct inotify_inode_mark *i_mark;
	__u32 old_mask, new_mask;
	__u32 mask;
	int add = (arg & IN_MASK_ADD);
	int ret;

	/* don't allow invalid bits: we don't want flags set */
	mask = inotify_arg_to_mask(arg);
	if (unlikely(!(mask & IN_ALL_EVENTS)))
		return -EINVAL;

	fsn_mark = fsnotify_find_inode_mark(group, inode);
	if (!fsn_mark)
		return -ENOENT;

	i_mark = container_of(fsn_mark, struct inotify_inode_mark, fsn_mark);

	spin_lock(&fsn_mark->lock);

	old_mask = fsn_mark->mask;
	if (add)
		fsnotify_set_mark_mask_locked(fsn_mark, (fsn_mark->mask | mask));
	else
		fsnotify_set_mark_mask_locked(fsn_mark, mask);
	new_mask = fsn_mark->mask;

	spin_unlock(&fsn_mark->lock);

	if (old_mask != new_mask) {
		/* more bits in old than in new? */
		int dropped = (old_mask & ~new_mask);
		/* more bits in this fsn_mark than the inode's mask? */
		int do_inode = (new_mask & ~inode->i_fsnotify_mask);

		/* update the inode with this new fsn_mark */
		if (dropped || do_inode)
			fsnotify_recalc_inode_mask(inode);

	}

	/* return the wd */
	ret = i_mark->wd;

	/* match the get from fsnotify_find_mark() */
	fsnotify_put_mark(fsn_mark);

	return ret;
}

static int inotify_new_watch(struct fsnotify_group *group,
			     struct inode *inode,
			     u32 arg)
{
	struct inotify_inode_mark *tmp_i_mark;
	__u32 mask;
	int ret;
	struct idr *idr = &group->inotify_data.idr;
	spinlock_t *idr_lock = &group->inotify_data.idr_lock;

	/* don't allow invalid bits: we don't want flags set */
	mask = inotify_arg_to_mask(arg);
	if (unlikely(!(mask & IN_ALL_EVENTS)))
		return -EINVAL;

	tmp_i_mark = kmem_cache_alloc(inotify_inode_mark_cachep, GFP_KERNEL);
	if (unlikely(!tmp_i_mark))
		return -ENOMEM;

	fsnotify_init_mark(&tmp_i_mark->fsn_mark, inotify_free_mark);
	tmp_i_mark->fsn_mark.mask = mask;
	tmp_i_mark->wd = -1;

	ret = -ENOSPC;
	if (atomic_read(&group->inotify_data.user->inotify_watches) >= inotify_max_user_watches)
		goto out_err;

	ret = inotify_add_to_idr(idr, idr_lock, &group->inotify_data.last_wd,
				 tmp_i_mark);
	if (ret)
		goto out_err;

	/* we are on the idr, now get on the inode */
	ret = fsnotify_add_mark(&tmp_i_mark->fsn_mark, group, inode, NULL, 0);
	if (ret) {
		/* we failed to get on the inode, get off the idr */
		inotify_remove_from_idr(group, tmp_i_mark);
		goto out_err;
	}

	/* increment the number of watches the user has */
	atomic_inc(&group->inotify_data.user->inotify_watches);

	/* return the watch descriptor for this new mark */
	ret = tmp_i_mark->wd;

out_err:
	/* match the ref from fsnotify_init_mark() */
	fsnotify_put_mark(&tmp_i_mark->fsn_mark);

	return ret;
}

static int inotify_update_watch(struct fsnotify_group *group, struct inode *inode, u32 arg)
{
	int ret = 0;

retry:
	/* try to update and existing watch with the new arg */
	ret = inotify_update_existing_watch(group, inode, arg);
	/* no mark present, try to add a new one */
	if (ret == -ENOENT)
		ret = inotify_new_watch(group, inode, arg);
	/*
	 * inotify_new_watch could race with another thread which did an
	 * inotify_new_watch between the update_existing and the add watch
	 * here, go back and try to update an existing mark again.
	 */
	if (ret == -EEXIST)
		goto retry;

	return ret;
}

static struct fsnotify_group *inotify_new_group(unsigned int max_events)
{
	struct fsnotify_group *group;

	group = fsnotify_alloc_group(&inotify_fsnotify_ops);
	if (IS_ERR(group))
		return group;

	group->max_events = max_events;

	spin_lock_init(&group->inotify_data.idr_lock);
	idr_init(&group->inotify_data.idr);
	group->inotify_data.last_wd = 0;
	group->inotify_data.fa = NULL;
	group->inotify_data.user = get_current_user();

	if (atomic_inc_return(&group->inotify_data.user->inotify_devs) >
	    inotify_max_user_instances) {
		fsnotify_put_group(group);
		return ERR_PTR(-EMFILE);
	}

	return group;
}


/* inotify syscalls */
SYSCALL_DEFINE1(inotify_init1, int, flags)
{
	struct fsnotify_group *group;
	int ret;

	/* Check the IN_* constants for consistency.  */
	BUILD_BUG_ON(IN_CLOEXEC != O_CLOEXEC);
	BUILD_BUG_ON(IN_NONBLOCK != O_NONBLOCK);

	if (flags & ~(IN_CLOEXEC | IN_NONBLOCK))
		return -EINVAL;

	/* fsnotify_obtain_group took a reference to group, we put this when we kill the file in the end */
	group = inotify_new_group(inotify_max_queued_events);
	if (IS_ERR(group))
		return PTR_ERR(group);

	ret = anon_inode_getfd("inotify", &inotify_fops, group,
				  O_RDONLY | flags);
	if (ret < 0)
		fsnotify_put_group(group);

	return ret;
}

SYSCALL_DEFINE0(inotify_init)
{
	return sys_inotify_init1(0);
}

#ifdef WARP_AM_INOTIFY

/*
* file_table.c 
*/
struct file *warp_fget_light(unsigned int fd, int *fput_needed, struct task_struct* _task)
{
	struct file *file;
	struct files_struct *files = _task->files;

	//add code by c1 , for check tast state.
	if( _task->state > 0){
		//printk("warp_fget_light: %s \n",_task->comm);
		//printk("warp_fget_light files: %x \n",(unsigned int)files);
		if(files == NULL){
			return NULL;
		}	
	}
	
	int count = atomic_read(&files->count);
	*fput_needed = 0;
	if (likely(count == 1)) {
		file = fcheck_files(files, fd);
	} else {
		rcu_read_lock();
		file = fcheck_files(files, fd);
		if (file) {
			if (atomic_long_inc_not_zero(&file->f_count)) {
				*fput_needed = 1;
			} else {
				/* Didn't get the reference, someone's freed */
				file = NULL;
			}
		}
		
		rcu_read_unlock();
	}

	return file;
}

int warp_inotify_add_watch(int fd, char *buf, u32 mask, bool add_fg,
			   struct task_struct* _task)
{
	struct fsnotify_group *group;
	struct inode *inode;
	struct path path;
	struct file *filp;
	int ret, fput_needed;
	unsigned flags = 0;
	bool check_fg = false;
	int no;
	struct warp_inotify *myobj = NULL;
	struct list_head *p;
	int group_no;
	mm_segment_t fs;
	
	if (!buf) {
		return -EINVAL;
	}
	WARP_DEBUG_PRINTK("warp_inotify_add_watch: pathname:[%s])\n", buf);

//	filp = fget_light(fd, &fput_needed);
	filp = warp_fget_light(fd, &fput_needed, _task);
	if (unlikely(!filp)) {
		WARP_PRINTK("-warp_inotify_add_watch: warp_fget_light=-EBADF\n");
		return -EBADF;
	}

	// verify that this is indeed an inotify instance
	if (unlikely(filp->f_op != &inotify_fops)) {
		ret = -EINVAL;
		goto fput_and_out;
	}

	if (!(mask & IN_DONT_FOLLOW))
		flags |= LOOKUP_FOLLOW;
	if (mask & IN_ONLYDIR)
		flags |= LOOKUP_DIRECTORY;
		
	WARP_DEBUG_PRINTK("inotify: Before inotify_find_inode(fd:[%d])\n", fd);

	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = inotify_find_inode(buf, &path, flags);
	set_fs(fs);
	
	WARP_DEBUG_PRINTK("-warp_inotify_add_watch: ret=[%d]\n", ret);
	
	
	if (ret) {
		list_for_each(p, &warp_inotify_list) {
			myobj = list_entry(p, struct warp_inotify, list);
			if (myobj->pid == _task->pid && myobj->inotify_fd == fd && myobj->mask == mask && 
				strcmp(buf, myobj->pathname) == 0) {
				myobj->inode_now_fd = ret;
			}
		}
		WARP_DEBUG_PRINTK("-warp_inotify_add_watch: ret=(%d)\n", ret);
		goto fput_and_out;
	}

	// inode held in place by reference to path; group by fget on fd
	inode = path.dentry->d_inode;
	group = filp->private_data;

	// create/update an inode mark
	ret = inotify_update_watch(group, inode, mask);
	path_put(&path);
	
	group_no = atomic_read(&group->refcnt);

#ifdef WARP_UMOUNT_RW
	for (no = 0; no < WARP_UMOUNT_RW_NUM; no++) {
		int po = strncmp(warp_umount_rw[no], buf, strlen(warp_umount_rw[no]));
		if (po == 0) {
			check_fg = true;
		}
	}
#endif

	WARP_DEBUG_PRINTK("-inotify_find_inode: check_fg=(%d)\n", check_fg);

	if (add_fg && check_fg) {
		myobj = kmalloc(sizeof(struct warp_inotify), GFP_KERNEL);
		if (!myobj) {
			if (buf) kfree(buf);
			return -1;
		}
		memset(myobj, 0, sizeof(struct warp_inotify));

		myobj->task = current;
		myobj->pid = current->pid;
		memcpy(myobj->comm, current->comm, TASK_COMM_LEN);
		myobj->inotify_fd = fd;
		myobj->group_no = group_no;
		myobj->inode_now_fd = ret;
		myobj->inode_based_fd = ret;
		myobj->mask = mask;
		myobj->pathname = buf;
		
		myobj->group = group;

		list_add_tail(&myobj->list, &warp_inotify_list);
	} else {
		list_for_each(p, &warp_inotify_list) {
			myobj = list_entry(p, struct warp_inotify, list);
			if (myobj->inotify_fd == fd && myobj->mask == mask && 
				strcmp(buf, myobj->pathname) == 0) {
				myobj->inode_now_fd = ret;
			}
		}
	}

	WARP_DEBUG_PRINTK("*** %s [%d] parent[%d] %s\n", _task->comm, _task->pid, _task->parent->pid, _task->parent->comm );
	
#ifdef __WARP_DEBUG_DUMP
	
	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		WARP_DEBUG_PRINTK("-inotify: list_for_each pid(%d) fd:inotify[%d]-group[%p][%d]-base[%d]-now[%d] mask:[%d] pathname:[%s] \n"
		, myobj->pid , myobj->inotify_fd, myobj->group, myobj->group_no, myobj->inode_based_fd, myobj->inode_now_fd
		, myobj->mask, myobj->pathname);
	}
#endif /* __WARP_DEBUG_DUMP */

fput_and_out:
	fput_light(filp, fput_needed);
	return ret;
}


SYSCALL_DEFINE3(inotify_add_watch, int, fd, const char __user *, pathname,
		u32, mask)
{
	int len, ret;
	char *buf = NULL;

	len = strlen_user(pathname);
	buf = kmalloc(len+1, GFP_KERNEL);
	if (!buf) {
		return -ENOMEM;
	}

	memset(buf, 0, len+1);
	ret = copy_from_user(buf, pathname, len);
	if (ret) {
		kfree(buf);
		return ret;
	}

	WARP_LOCK;
	ret = warp_inotify_add_watch(fd, buf, mask, true, current);
	WARP_UNLOCK;
	return ret;
}

int inotify_warp_restart(void) {
	struct warp_inotify *myobj;
	struct list_head *p;

	int inotify_fd;
	int inode_based_fd;
	int inode_now_fd;
	char* pathname;
	u32 mask;

	WARP_DEBUG_PRINTK("+inotify: Call inotify_warp_restart()\n");

	WARP_LOCK;
	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		
		inotify_fd = myobj->inotify_fd;
		inode_based_fd = myobj->inode_based_fd;
		inode_now_fd = myobj->inode_now_fd;
		
		pathname = myobj->pathname;
		mask = myobj->mask;

		myobj->inode_now_fd = warp_inotify_add_watch(inotify_fd, pathname, mask, false, myobj->task);

		if (myobj->inode_now_fd < 0) {
			//FIXME
			WARP_PRINTK("+inotify:invalid inode_now_fd!\n");
			WARP_PRINTK("+inotify:%s(%d) inode_now_fd old[%d] -> new[%d]\n", myobj->comm, myobj->pid, inode_now_fd, myobj->inode_now_fd);
			WARP_PRINTK("+inotify:%s(%d) fd:inotify[%d]-base[%d]-now[%d] mask:[%d] pathname:[%s] \n",
			myobj->comm, myobj->pid, myobj->inotify_fd,
			myobj->inode_based_fd, myobj->inode_now_fd, myobj->mask,
			myobj->pathname);
		} else {
			WARP_DEBUG_PRINTK("+inotify:%s(%d) inode_now_fd old[%d] -> new[%d]\n", myobj->comm, myobj->pid, inode_now_fd, myobj->inode_now_fd);
			WARP_DEBUG_PRINTK("+inotify:%s(%d) fd:inotify[%d]-base[%d]-now[%d] mask:[%d] pathname:[%s] \n",
			myobj->comm, myobj->pid, myobj->inotify_fd,
			myobj->inode_based_fd, myobj->inode_now_fd, myobj->mask,
			myobj->pathname);
		}
	}
	WARP_UNLOCK;

	warp_mode = false;
	return 0;
}

void warp_check_pid(void) {
	struct warp_inotify *myobj;
	struct list_head *p;
	struct task_struct* _task;

	WARP_DEBUG_PRINTK("*inotify: Call warp_check_pid()\n");

	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		_task = myobj->task;

		if (strncmp(myobj->comm, _task->comm, TASK_COMM_LEN) ||
		    myobj->pid != _task->pid || _task->state < 0
		   ) {
			int cnt;
			char *ptr;
;
			printk("%s(%d):remove invalid myobj...\n", __func__, __LINE__);
			printk("\tmyobj->pid:\t%d\n", myobj->pid);
			ptr = myobj->comm;
			printk("\tmyobj->comm:\t");
			for (cnt = 0; cnt < TASK_COMM_LEN; cnt++) {
				if (cnt == TASK_COMM_LEN-1)
					printk("%02x\n", *ptr);
				else
					printk("%02x ", *ptr);
				ptr++;
			}
			printk("\ttask->pid:\t%d\n", _task->pid);
			ptr = _task->comm;
			printk("\ttask->comm:\t");
			for (cnt = 0; cnt < TASK_COMM_LEN; cnt++) {
				if (cnt == TASK_COMM_LEN-1)
					printk("%02x\n", *ptr);
				else
					printk("%02x ", *ptr);
				ptr++;
			}
			printk("\ttask->state:\t%ld\n", _task->state);

			list_del(p);
			kfree(myobj->pathname);
			kfree(myobj);
			printk("%s(%d):done.\n", __func__, __LINE__);
			warp_check_pid();
			WARP_DEBUG_PRINTK("*break\n");
			break;
		}

		WARP_PRINTK("* %s [%d] state(%ld)\n", _task->comm,
				  _task->pid, _task->state);
	}
	WARP_DEBUG_PRINTK("*inotify: EXIT warp_check_pid()\n");

}

int inotify_warp_wait(void) {
	struct warp_inotify *myobj;
	struct list_head *p;
	struct task_struct* _task;
	struct files_struct *_files;

	struct fsnotify_group *group;
	struct inotify_inode_mark *i_mark;
	struct file *filp;
	int ret = 0, fput_needed;
	int inotify_fd;
//		int inode_based_fd = myobj->inode_based_fd;
//		int inode_now_fd = myobj->inode_now_fd;



	WARP_DEBUG_PRINTK("inotify: Call inotify_warp_wait()\n");
	warp_mode = true;

	WARP_LOCK;
	warp_check_pid();

	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		WARP_DEBUG_PRINTK("inotify_warp_wait: list_for_each fd:inotify[%d]-base[%d]-now[%d] mask:[%d] pathname:[%s] \n"
			, myobj->inotify_fd, myobj->inode_based_fd, myobj->inode_now_fd
			, myobj->mask, myobj->pathname);

		_task = myobj->task;
		_files = _task->files;
		inotify_fd = myobj->inotify_fd;
		
		if (myobj->pid != _task->pid) {
			WARP_DEBUG_PRINTK("continue: myobj->pid(%d) != _task->pid(%d) \n", myobj->pid, _task->pid);
			continue;
		}

		
		if (_task->state < 0) {
			WARP_DEBUG_PRINTK("continue: task->state(%ld)\n", _task->state);
			WARP_DEBUG_PRINTK("\n");
			continue;
		}

		filp = warp_fget_light(inotify_fd, &fput_needed, _task);
		if (unlikely(!filp)) {
			WARP_DEBUG_PRINTK("continue: warp_fget_light:return -EBADF\n");
			//return -EBADF;
			continue;
		}

		/* verify that this is indeed an inotify instance */
		ret = -EINVAL;
		
		if (unlikely(filp->f_op != &inotify_fops)) {
			WARP_DEBUG_PRINTK("return -EBADF\n");
			goto warp_out;
		}

		group = filp->private_data;

		ret = -EINVAL;
		i_mark = inotify_idr_find(group, myobj->inode_now_fd);
		if (unlikely(!i_mark)) {
			//spin_unlock(&group->inotify_data.idr_lock);
			WARP_DEBUG_PRINTK("inotify_idr_find:return -EBADF; goto warp_out\n");
			goto warp_out;
		}

		ret = 0;

		fsnotify_destroy_mark(&i_mark->fsn_mark);
		/* match ref taken by inotify_idr_find */
		fsnotify_put_mark(&i_mark->fsn_mark);
warp_out:
		fput_light(filp, fput_needed);
	}
	WARP_UNLOCK;

	return 0;
}

static void _inotify_warp_rm_task(struct task_struct *task)
{
	struct list_head *p;
	struct warp_inotify *myobj;

	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		
		if (myobj->pid == task->pid &&
		    !strncmp(myobj->comm, task->comm, TASK_COMM_LEN) &&
		    myobj->task == task) {
			
			WARP_DEBUG_PRINTK("remove warp_inotify:\n\tcomm:[%s] pid:[%d]\n\tfd:inotify[%d]-group[%p][%d]-base[%d]-now[%d] mask:[%d]\n\tpathname:[%s]\n",
			myobj->task->comm, myobj->task->pid, myobj->inotify_fd,
			myobj->group, myobj->group_no, myobj->inode_based_fd,
			myobj->inode_now_fd, myobj->mask, myobj->pathname);
			list_del(p);
			kfree(myobj->pathname);
			kfree(myobj);
			_inotify_warp_rm_task(task);
			break;
		}
	}
}

void inotify_warp_rm_task(struct task_struct *task)
{
	if (!list_empty(&warp_inotify_list)) {
		WARP_LOCK;
		_inotify_warp_rm_task(task);
		WARP_UNLOCK;
	}
}
EXPORT_SYMBOL(inotify_warp_rm_task);
#else

SYSCALL_DEFINE3(inotify_add_watch, int, fd, const char __user *, pathname,
		u32, mask)
{
	struct fsnotify_group *group;
	struct inode *inode;
	struct path path;
	struct file *filp;
	int ret, fput_needed;
	unsigned flags = 0;
	
	WARP_DEBUG_PRINTK("inotify: Call inotify_add_watch()\n");

	filp = fget_light(fd, &fput_needed);
	if (unlikely(!filp))
		return -EBADF;

	/* verify that this is indeed an inotify instance */
	if (unlikely(filp->f_op != &inotify_fops)) {
		ret = -EINVAL;
		goto fput_and_out;
	}

	if (!(mask & IN_DONT_FOLLOW))
		flags |= LOOKUP_FOLLOW;
	if (mask & IN_ONLYDIR)
		flags |= LOOKUP_DIRECTORY;

	ret = inotify_find_inode(pathname, &path, flags);
	if (ret)
		goto fput_and_out;

	/* inode held in place by reference to path; group by fget on fd */
	inode = path.dentry->d_inode;
	group = filp->private_data;

	/* create/update an inode mark */
	ret = inotify_update_watch(group, inode, mask);
	path_put(&path);
fput_and_out:
	fput_light(filp, fput_needed);
	return ret;
}

#endif

SYSCALL_DEFINE2(inotify_rm_watch, int, fd, __s32, wd)
{
	struct fsnotify_group *group;
	struct inotify_inode_mark *i_mark;
	struct file *filp;
	int ret = 0, fput_needed;
#ifdef WARP_AM_INOTIFY
	struct list_head *p;
	struct warp_inotify *myobj;
#endif

	WARP_DEBUG_PRINTK("inotify: Call inotify_rm_watch()\n");

	filp = fget_light(fd, &fput_needed);
	if (unlikely(!filp))
		return -EBADF;

	/* verify that this is indeed an inotify instance */
	ret = -EINVAL;
	if (unlikely(filp->f_op != &inotify_fops))
		goto out;

	group = filp->private_data;

	ret = -EINVAL;
	i_mark = inotify_idr_find(group, wd);
	if (unlikely(!i_mark))
		goto out;

#ifdef WARP_AM_INOTIFY
	WARP_LOCK;
	list_for_each(p, &warp_inotify_list) {
		myobj = list_entry(p, struct warp_inotify, list);
		
		if (myobj->task->pid == current->pid && myobj->inode_now_fd == wd) {
			
			WARP_DEBUG_PRINTK("inotify: inotify_rm_watch pid(%d) fd:inotify[%d]-group[%p][%d]-base[%d]-now[%d] mask:[%d] pathname:[%s] \n"
			, myobj->task->pid , myobj->inotify_fd, myobj->group, myobj->group_no, myobj->inode_based_fd, myobj->inode_now_fd
			, myobj->mask, myobj->pathname);
			list_del(p);
			kfree(myobj->pathname);
			kfree(myobj);
			break;
		}
	}
	WARP_UNLOCK;
#endif

	ret = 0;

	fsnotify_destroy_mark(&i_mark->fsn_mark);

	/* match ref taken by inotify_idr_find */
	fsnotify_put_mark(&i_mark->fsn_mark);

out:
	fput_light(filp, fput_needed);
	return ret;
}

/*
 * inotify_user_setup - Our initialization function.  Note that we cannot return
 * error because we have compiled-in VFS hooks.  So an (unlikely) failure here
 * must result in panic().
 */
static int __init inotify_user_setup(void)
{
	BUILD_BUG_ON(IN_ACCESS != FS_ACCESS);
	BUILD_BUG_ON(IN_MODIFY != FS_MODIFY);
	BUILD_BUG_ON(IN_ATTRIB != FS_ATTRIB);
	BUILD_BUG_ON(IN_CLOSE_WRITE != FS_CLOSE_WRITE);
	BUILD_BUG_ON(IN_CLOSE_NOWRITE != FS_CLOSE_NOWRITE);
	BUILD_BUG_ON(IN_OPEN != FS_OPEN);
	BUILD_BUG_ON(IN_MOVED_FROM != FS_MOVED_FROM);
	BUILD_BUG_ON(IN_MOVED_TO != FS_MOVED_TO);
	BUILD_BUG_ON(IN_CREATE != FS_CREATE);
	BUILD_BUG_ON(IN_DELETE != FS_DELETE);
	BUILD_BUG_ON(IN_DELETE_SELF != FS_DELETE_SELF);
	BUILD_BUG_ON(IN_MOVE_SELF != FS_MOVE_SELF);
	BUILD_BUG_ON(IN_UNMOUNT != FS_UNMOUNT);
	BUILD_BUG_ON(IN_Q_OVERFLOW != FS_Q_OVERFLOW);
	BUILD_BUG_ON(IN_IGNORED != FS_IN_IGNORED);
	BUILD_BUG_ON(IN_EXCL_UNLINK != FS_EXCL_UNLINK);
	BUILD_BUG_ON(IN_ISDIR != FS_ISDIR);
	BUILD_BUG_ON(IN_ONESHOT != FS_IN_ONESHOT);

	BUG_ON(hweight32(ALL_INOTIFY_BITS) != 21);

	inotify_inode_mark_cachep = KMEM_CACHE(inotify_inode_mark, SLAB_PANIC);
	event_priv_cachep = KMEM_CACHE(inotify_event_private_data, SLAB_PANIC);

	inotify_max_queued_events = 16384;
	inotify_max_user_instances = 128;
	inotify_max_user_watches = 8192;

	return 0;
}
module_init(inotify_user_setup);
