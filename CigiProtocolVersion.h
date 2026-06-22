#ifndef CIGI_PROTOCOL_VERSION_H
#define CIGI_PROTOCOL_VERSION_H

enum CigiProtocolVersionId
{
    CIGI_3_0_3_1 = 0x00030001,
    CIGI_3_2 = 0x00030002,
    CIGI_3_3 = 0x00030003,
    CIGI_4_0 = 0x00040000
};

class CigiProtocolVersion
{
public:
    CigiProtocolVersion();
    CigiProtocolVersion(int majorVersion, int minorVersion);

    static CigiProtocolVersion FromId(CigiProtocolVersionId id);
    static CigiProtocolVersion Current();
    static bool TryCreate(int majorVersion, int minorVersion,
                          CigiProtocolVersion *version);

    int GetMajorVersion() const;
    int GetMinorVersion() const;
    CigiProtocolVersionId GetId() const;
    bool IsCigi3() const;
    bool IsCigi4() const;
    bool IsPacketIoImplemented() const;
    bool IsSameSelection(const CigiProtocolVersion &other) const;

private:
    int m_MajorVersion;
    int m_MinorVersion;
};

class CigiProtocolVersionCatalog
{
public:
    static int GetVersionCount();
    static CigiProtocolVersion GetVersion(int index);
};

#endif // CIGI_PROTOCOL_VERSION_H
