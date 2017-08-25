/**
 * @file AudioCtrl.h
 * @author xun.lu
 * @date 2015-12-12
 * @version 
 * @copyright SunMedia Technology (Chengdu) co. Ltd.
 * @brief declaration of AudioCtrl
 */
#ifndef AUDIO_CTRL_H
#define AUDIO_CTRL_H

#include "AudioTypes.h"
#include "AudioError.h"

/**
 * @class AudioCtrl
 * @brief API of server setting
 * @details  request exit
 * @todo more API will be add to this class.
 */
class AudioCtrl
{
public:
    /**
        * @fn AudioCtrl()()
        * @brief Creates a ClientCtrl object and registers it with AudioService.
        */
    AudioCtrl(){}

    /**
        * @fn ~AudioCtrl()()
        * @brief Terminates the ClientCtrl and unregisters it from AudioService.
     * Also destroys all resources associated with the ClientCtrl.
     */
    virtual ~AudioCtrl(){}

    /**
        * @fn status_t requestServerExit()
        * @brief request server exit
        */
    virtual void requestServerExit() = 0;
};
#endif //AUDIO_CTRL_H