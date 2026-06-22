#ifndef CIGI_PROTOCOL_ADAPTER_H
#define CIGI_PROTOCOL_ADAPTER_H

#include "CigiProtocolVersion.h"

class ICigiProtocolAdapter
{
public:
    virtual ~ICigiProtocolAdapter() {}

    virtual CigiProtocolVersion GetActiveVersion() const = 0;
    virtual bool Configure(void *igControlPacket) = 0;

    virtual void StartMessage(int session) = 0;
    virtual void AddIGControlPacket(int session, void *igControlPacket) = 0;
    virtual void EndMessage(int session) = 0;
    virtual void SetIncomingMessageBuffer(int session, unsigned char *buffer,
                                          int size) = 0;
    virtual void SyncFrameCounter(int session) = 0;
    virtual void ProcessIncomingMessage(int session) = 0;
    virtual void GetOutgoingMessageBuffer(int session, unsigned char **buffer,
                                          int *size) = 0;
    virtual void SwapOutgoingMessageBuffer(char *buffer, int size) = 0;
};

struct CigiProtocolAdapterSelection
{
    ICigiProtocolAdapter *adapter;
    bool exactMatch;
};

class CigiProtocolAdapterFactory
{
public:
    static CigiProtocolAdapterSelection Select(
        const CigiProtocolVersion &requestedVersion);
};

#endif // CIGI_PROTOCOL_ADAPTER_H
