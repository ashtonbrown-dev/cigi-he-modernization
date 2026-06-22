#include "CigiProtocolVersion.h"

namespace
{
const CigiProtocolVersionId KnownVersions[] = {
    CIGI_3_0_3_1,
    CIGI_3_2,
    CIGI_3_3,
    CIGI_4_0
};
}

CigiProtocolVersion::CigiProtocolVersion()
    : m_MajorVersion(4), m_MinorVersion(0)
{
}

CigiProtocolVersion::CigiProtocolVersion(int majorVersion, int minorVersion)
    : m_MajorVersion(majorVersion), m_MinorVersion(minorVersion)
{
}

CigiProtocolVersion CigiProtocolVersion::FromId(CigiProtocolVersionId id)
{
    switch (id) {
    case CIGI_3_0_3_1:
        // CIGI 3.0 and 3.1 share one compatibility selection. Store 3.1 as
        // the canonical value while accepting either minor version on load.
        return CigiProtocolVersion(3, 1);
    case CIGI_3_2:
        return CigiProtocolVersion(3, 2);
    case CIGI_3_3:
        return CigiProtocolVersion(3, 3);
    default:
        CigiProtocolVersion version;
        const int value = (int)id;
        if (TryCreate((value >> 16) & 0xffff, value & 0xffff, &version))
            return version;
        return Current();
    }
}

CigiProtocolVersion CigiProtocolVersion::Current()
{
    return CigiProtocolVersion(4, 0);
}

bool CigiProtocolVersion::TryCreate(int majorVersion, int minorVersion,
                                    CigiProtocolVersion *version)
{
    if (!version || minorVersion < 0 || minorVersion > 255)
        return false;

    if (majorVersion == 3 && minorVersion <= 3) {
        *version = CigiProtocolVersion(majorVersion, minorVersion);
        return true;
    }

    // Any CIGI 4 minor is a valid value. A future release only needs to add
    // its version to the catalog when it should become a standard UI choice.
    if (majorVersion == 4) {
        *version = CigiProtocolVersion(majorVersion, minorVersion);
        return true;
    }

    return false;
}

int CigiProtocolVersion::GetMajorVersion() const
{
    return m_MajorVersion;
}

int CigiProtocolVersion::GetMinorVersion() const
{
    return m_MinorVersion;
}

CigiProtocolVersionId CigiProtocolVersion::GetId() const
{
    if (m_MajorVersion == 3) {
        if (m_MinorVersion <= 1)
            return CIGI_3_0_3_1;
        if (m_MinorVersion == 2)
            return CIGI_3_2;
        return CIGI_3_3;
    }

    // The packed ID lets combo-box item data represent any future CIGI 4
    // minor without adding switch logic or changing persisted values.
    return (CigiProtocolVersionId)((m_MajorVersion << 16) | m_MinorVersion);
}

bool CigiProtocolVersion::IsCigi3() const
{
    return m_MajorVersion == 3;
}

bool CigiProtocolVersion::IsCigi4() const
{
    return m_MajorVersion == 4;
}

bool CigiProtocolVersion::IsPacketIoImplemented() const
{
    return IsCigi4();
}

bool CigiProtocolVersion::IsSameSelection(const CigiProtocolVersion &other) const
{
    if (m_MajorVersion != other.m_MajorVersion)
        return false;

    if (m_MajorVersion == 3 && m_MinorVersion <= 1 && other.m_MinorVersion <= 1)
        return true;

    return m_MinorVersion == other.m_MinorVersion;
}

int CigiProtocolVersionCatalog::GetVersionCount()
{
    return sizeof(KnownVersions) / sizeof(KnownVersions[0]);
}

CigiProtocolVersion CigiProtocolVersionCatalog::GetVersion(int index)
{
    if (index < 0 || index >= GetVersionCount())
        return CigiProtocolVersion::Current();

    return CigiProtocolVersion::FromId(KnownVersions[index]);
}
