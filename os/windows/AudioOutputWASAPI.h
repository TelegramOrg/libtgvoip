//
// libtgvoip is free and unencumbered public domain software.
// For more information, see http://unlicense.org or the UNLICENSE file
// you should have received with this source code distribution.
//

#ifndef LIBTGVOIP_AUDIOOUTPUTWASAPI_H
#define LIBTGVOIP_AUDIOOUTPUTWASAPI_H

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#define TGVOIP_WINDOWS_PHONE
#endif
#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#define TGVOIP_WINDOWS_DESKTOP
#endif

#include <string>
#include <vector>
#include <windows.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#ifndef TGVOIP_WP_SILVERLIGHT
#include <mmdeviceapi.h>
#endif
#ifdef TGVOIP_WINDOWS_DESKTOP
#include <audiopolicy.h>
#include <functiondiscoverykeys.h>
#else
#include "WindowsSandboxUtils.h"
#include <audioclient.h>
#endif
#pragma warning(pop)
#include "../../audio/AudioOutput.h"

namespace tgvoip
{
namespace audio
{

#ifdef TGVOIP_WINDOWS_DESKTOP
    class AudioOutputWASAPI : public AudioOutput, IMMNotificationClient, IAudioSessionEvents
    {
#else
    class AudioOutputWASAPI : public AudioOutput
    {
#endif
    public:
        AudioOutputWASAPI(std::string deviceID);
        virtual ~AudioOutputWASAPI();
        virtual void Start();
        virtual void Stop();
        virtual bool IsPlaying();
        virtual void SetCurrentDevice(std::string deviceID);
        static void EnumerateDevices(std::vector<AudioOutputDevice>& devs);
#ifdef TGVOIP_WINDOWS_DESKTOP
        STDMETHOD_(ULONG, AddRef)
        ();
        STDMETHOD_(ULONG, Release)
        ();
#endif

    private:
        void ActuallySetCurrentDevice(std::string deviceID);
        static DWORD WINAPI StartThread(void* arg);
        void RunThread();
        WAVEFORMATEX format;
        bool isPlaying;
        HANDLE shutdownEvent;
        HANDLE audioSamplesReadyEvent;
        HANDLE streamSwitchEvent;
        HANDLE thread;
        IAudioClient* audioClient = NULL;
        IAudioRenderClient* renderClient = NULL;
#ifdef TGVOIP_WINDOWS_DESKTOP
        IMMDeviceEnumerator* enumerator;
        IAudioSessionControl* audioSessionControl;
        IMMDevice* device;
#endif
        std::uint8_t remainingData[10240];
        std::size_t remainingDataLen;
        bool isDefaultDevice;
        ULONG refCount;
        std::string streamChangeToDevice;

#ifdef TGVOIP_WINDOWS_DESKTOP
        STDMETHOD(OnDisplayNameChanged)
        (LPCWSTR /*NewDisplayName*/, LPCGUID /*EventContext*/)
        {
            return S_OK;
        };
        STDMETHOD(OnIconPathChanged)
        (LPCWSTR /*NewIconPath*/, LPCGUID /*EventContext*/) { return S_OK; };
        STDMETHOD(OnSimpleVolumeChanged)
        (float /*NewSimpleVolume*/, BOOL /*NewMute*/, LPCGUID /*EventContext*/) { return S_OK; }
        STDMETHOD(OnChannelVolumeChanged)
        (DWORD /*ChannelCount*/, float /*NewChannelVolumes*/[], DWORD /*ChangedChannel*/, LPCGUID /*EventContext*/) { return S_OK; };
        STDMETHOD(OnGroupingParamChanged)
        (LPCGUID /*NewGroupingParam*/, LPCGUID /*EventContext*/) { return S_OK; };
        STDMETHOD(OnStateChanged)
        (AudioSessionState /*NewState*/) { return S_OK; };
        STDMETHOD(OnSessionDisconnected)
        (AudioSessionDisconnectReason DisconnectReason);
        STDMETHOD(OnDeviceStateChanged)
        (LPCWSTR /*DeviceId*/, DWORD /*NewState*/) { return S_OK; }
        STDMETHOD(OnDeviceAdded)
        (LPCWSTR /*DeviceId*/) { return S_OK; };
        STDMETHOD(OnDeviceRemoved)
        (LPCWSTR /*DeviceId(*/) { return S_OK; };
        STDMETHOD(OnDefaultDeviceChanged)
        (EDataFlow Flow, ERole Role, LPCWSTR NewDefaultDeviceId);
        STDMETHOD(OnPropertyValueChanged)
        (LPCWSTR /*DeviceId*/, const PROPERTYKEY /*Key*/) { return S_OK; };

        //
        //  IUnknown
        //
        STDMETHOD(QueryInterface)
        (REFIID iid, void** pvObject);
#endif
    };

}
}

#endif //LIBTGVOIP_AUDIOOUTPUTWASAPI_H
