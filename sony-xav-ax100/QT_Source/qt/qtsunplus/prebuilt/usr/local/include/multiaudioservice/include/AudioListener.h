/**
 * @file AudioListener.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of AudioListener
 */
#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H


typedef enum
{
    AL_SUCCESS,
    AL_FAIL,
    AL_BAD_CONNECTION,
    AL_DISCONNECTED,
    AL_BAD_SEQ,
    AL_BAD_PARAM
}al_status_t;

typedef enum
{
    SKT_MSG_CONNECTED,
    SKT_MSG_DISCONNECTED,
    SKT_MSG_RECV,
    SKT_MSG_SEND
}am_cmd_t;

typedef void (*listener_cbk_func_t)(void* opaque, void* client, int msg, const unsigned char* data, int size);

class AudioListener
{
public:
    AudioListener(const char* name, int port);
    ~AudioListener(void);
    void setMsgCbk(listener_cbk_func_t cbk, void* opaque);
    al_status_t run(void);
    al_status_t stall(void);
    al_status_t response(void* client, const unsigned char* data, int size);
    al_status_t notify(void* client, unsigned int msg, unsigned int value);
    al_status_t closeClient(void* client);
private:
    const char* m_name;
    const int m_port;
    void* m_priv;
    listener_cbk_func_t m_cbk;
    void* m_cbk_opaque;
};

#endif //AUDIO_LISTENER_H