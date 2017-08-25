/**
 * @file ClientCtrl.h
 * @author mm.li
 * @date 2015-11-13
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declarates of ClientCtrl
 */
#ifndef AUDIO_CLIENT_CTRL_H
#define AUDIO_CLIENT_CTRL_H

#include "AudioCtrl.h"

class RemoteCall;

class ClientCtrl : public AudioCtrl
{
public:

    /* Creates a ClientCtrl object and registers it with AudioService.*/
    ClientCtrl();

    /* Terminates the ClientCtrl and unregisters it from AudioService.
     * Also destroys all resources associated with the ClientCtrl.
     */
    ~ClientCtrl();

    status_t initCheck();

    void requestServerExit();
private:
    RemoteCall* mRc;
    status_t mInitStatus;
};
#endif //AUDIO_CLIENT_CTRL_H