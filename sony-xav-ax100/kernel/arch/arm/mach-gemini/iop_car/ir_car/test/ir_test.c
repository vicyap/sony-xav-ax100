
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>

#define IR_PATH "/dev/input/event0"

struct input_event ev;

/*
struct input_event {
struct timeval time; // event happen time
__u16 type; // EV_SYN=0x00, EV_KEY=0x01, EV_REL=0x02, ...
__u16 code; // KEY_ESC=1, KEY_1=2, ..., KEY_POWER=116, ...
__s32 value; //0 for EV_KEY for release, 1 for keypress and 2 for autorepeat. 
};
*/

int main(void)
{
	int fd;
	int readbytes;
	
	fd = open(IR_PATH, O_RDONLY);
	if(fd < 0)
	{
		printf("[ir_test] ERROR: can not open %s !\n", IR_PATH);
		exit(1);
	}
	
	while(1)
	{
		readbytes = read(fd, &ev, sizeof(struct input_event));
		if(readbytes <= 0)
		{
			printf("[ir_test] ERROR: can not read %s !\n", IR_PATH);
			exit(1);
		}

		printf("[ir_test] input_event.type = %d, input_event.code = %d, input_event.value = %d.\n", ev.type, ev.code, ev.value);

		if((ev.type == 1) && (ev.code == 116) && (ev.value == 0))
			break;
	}

	close(IR_PATH);
	return 0;
}

