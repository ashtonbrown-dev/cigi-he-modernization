#include "CigiProtocolAdapter.h"

#include "cigi4.h"
#include "cigi4types.h"

namespace
{
class Cigi4ProtocolAdapter : public ICigiProtocolAdapter
{
public:
    Cigi4ProtocolAdapter()
        : m_Version(CigiProtocolVersion::Current())
    {
    }

    void SetVersion(const CigiProtocolVersion &version)
    {
        m_Version = version;
    }

    virtual CigiProtocolVersion GetActiveVersion() const
    {
        return m_Version;
    }

    virtual bool Configure(void *igControlPacket)
    {
        if (!igControlPacket)
            return false;

        CIGI_IG_CONTROL *igControl = (CIGI_IG_CONTROL *)igControlPacket;
        igControl->cigi_version = (unsigned char)m_Version.GetMajorVersion();
        igControl->minor_version = (unsigned char)m_Version.GetMinorVersion();
        igControl->packet_size = sizeof(CIGI_IG_CONTROL);
        CigiSetMinorVersion(m_Version.GetMinorVersion());
        return true;
    }

    virtual void StartMessage(int session)
    {
        CigiStartMessage(session);
    }

    virtual void AddIGControlPacket(int session, void *igControlPacket)
    {
        CigiAddPacketIGCtrl(session, (CIGI_IG_CONTROL *)igControlPacket);
    }

    virtual void EndMessage(int session)
    {
        CigiEndMessage(session);
    }

    virtual void SetIncomingMessageBuffer(int session, unsigned char *buffer,
                                          int size)
    {
        CigiSetIncomingMsgBuffer(session, buffer, size);
    }

    virtual void SyncFrameCounter(int session)
    {
        CigiSyncFrameCounter(session);
    }

    virtual void ProcessIncomingMessage(int session)
    {
        CigiProcessIncomingMsgBuffer(session);
    }

    virtual void GetOutgoingMessageBuffer(int session, unsigned char **buffer,
                                          int *size)
    {
        CigiGetOutgoingMsgBuffer(session, buffer, size);
    }

    virtual void SwapOutgoingMessageBuffer(char *buffer, int size)
    {
        CigiSwapOutgoingMsgBuffer(buffer, size);
    }

private:
    CigiProtocolVersion m_Version;
};

Cigi4ProtocolAdapter Cigi4Adapter;
}

CigiProtocolAdapterSelection CigiProtocolAdapterFactory::Select(
    const CigiProtocolVersion &requestedVersion)
{
    CigiProtocolAdapterSelection selection;

    if (requestedVersion.IsPacketIoImplemented()) {
        Cigi4Adapter.SetVersion(requestedVersion);
        selection.exactMatch = true;
    } else {
        // TODO(CIGI-3): return a CIGI 3 adapter once packet definitions,
        // callbacks, and translation from the shared scenario model exist.
        // Until then, retain the known-good CIGI 4.0 wire behavior.
        Cigi4Adapter.SetVersion(CigiProtocolVersion::Current());
        selection.exactMatch = false;
    }

    selection.adapter = &Cigi4Adapter;
    return selection;
}
