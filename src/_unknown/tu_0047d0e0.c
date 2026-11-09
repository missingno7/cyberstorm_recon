#include <windows.h>
#include <mmsystem.h>
struct MciState {
    MciState(); ~MciState();
    unsigned int device;
    unsigned int pending;
    unsigned int openType;
    unsigned int element;
    unsigned int state;
    
    int Open(int,int); void Close(); int PlayTimeRange(int, int, unsigned int, int);
};
MciState gShellResources;
MciState::MciState()
{
    device=0; pending=0; state=0;
}
MciState::~MciState()
{
    register MciState *deviceState = this;
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(deviceState->device, MCI_CLOSE, 0, 0);
        deviceState->device = 0;
    }
}
int MciState::Open(int openType, int element)
{
    this->openType = openType;
    this->element = element;
    if (this->device == 0) {
        MCI_OPEN_PARMSA open;
        open.wDeviceID = this->device;
        open.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_CD_AUDIO;
        if (mciSendCommandA(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, (unsigned long)&open) != 0)
            return 0;
        this->device = open.wDeviceID;
    }
    if (this->pending == 0) {
        MCI_STATUS_PARMS status;
        status.dwItem = 5;
        mciSendCommandA(this->device, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (unsigned long)&status);
        if (status.dwReturn == 0)
            return 0;
    }
    return 1;
}
void MciState::Close()
{
    if (device) {
        register MciState *statusState = this;
        if (device)
            mciSendCommandA(device, MCI_STOP, 0, 0);
        statusState->state = 0;
        mciSendCommandA(device, MCI_CLOSE, 0, 0);
        device = 0;
    }
}
extern "C" int MciStateNotPlaying(MciState *);
#pragma aux MciStateNotPlaying "f_d270";
extern "C" int MciStateNotPlaying(MciState *s)
{
    if (!s->device)
        return 0;
    MCI_STATUS_PARMS status;
    status.dwItem = 4;
    mciSendCommandA(s->device, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (unsigned long)&status);
    if (status.dwReturn == 0x20e)
        return 0;
    return 1;
}
extern "C" void StopMciState(MciState *);
#pragma aux StopMciState "f_d2c0";
extern "C" void StopMciState(MciState *s)
{
    if (s->device)
        mciSendCommandA(s->device, MCI_STOP, 0, 0);
    s->state = 0;
}
extern "C" void PlayNextMciTrack(MciState *, int);
#pragma aux PlayNextMciTrack "f_d2f0";
extern "C" void PlayNextMciTrack(MciState *s, int start)
{
    s->PlayTimeRange(start, 0, start + 1, 0);
}
int MciState::PlayTimeRange(int from, int fromSeconds, unsigned int to, int toSeconds)
{
    register MciState *state = this;
    struct LocalParameters {
        MCI_PLAY_PARMS play;
        MCI_SET_PARMS setTime;
    } local;
    local.setTime.dwTimeFormat = 10;
    mciSendCommandA(state->device, MCI_SET, MCI_SET_TIME_FORMAT, (unsigned long)&local.setTime);
    DWORD flags;
    if (to)
        flags = MCI_FROM | MCI_TO;
    else
        flags = MCI_FROM;
    state->state = 1;
    DWORD packedFrom = (DWORD)(unsigned char)from | ((fromSeconds / 60 & 0xffff) << 8);
    local.play.dwFrom = packedFrom | ((unsigned char)(fromSeconds % 60) << 16);
    local.play.dwTo = (to & 0xff) | ((toSeconds / 60 & 0xffff) << 8) | ((unsigned char)(toSeconds % 60) << 16);
    return mciSendCommandA(state->device, MCI_PLAY, flags, (unsigned long)&local.play) == 0;
}
