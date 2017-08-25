/**
 * @file AudioConnection.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief ddeclarates of AudioConnection
 */
#ifndef AUDIO_CONNECTION_H
#define AUDIO_CONNECTION_H

typedef enum
{
    AC_SUCCESS,
    AC_FAIL,
    AC_BAD_CONNECTION,
    AC_DISCONNECTED
}ac_status_t;

typedef void (*notify_cbk_func_t)(void* opaque, unsigned int msg, unsigned int value);

class AudioConnection
{
public:
    AudioConnection(const char* name, int port, notify_cbk_func_t cbk, void* opaque);
    ac_status_t send(const unsigned char* buf, int size);
    ac_status_t recv(unsigned char* buf, int size);
    ~AudioConnection(void);
private:
    ac_status_t connect();
    ac_status_t disconnect();
    const char* m_name;
    const int m_port;
    void* m_priv;
    notify_cbk_func_t m_notify_cbk_func;
    void* m_opaque;
};

#endif //AUDIO_CONNECTION_H