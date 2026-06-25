#ifndef CIGI3_API_H
#define CIGI3_API_H

namespace cigi3
{
struct ApiInfo
{
    int majorVersion;
    int maxPacketSize;
    int maxSessions;
    int hostSessionType;
    int igSessionType;
};

enum SessionType
{
    HostSession,
    IgSession
};

const ApiInfo &GetApiInfo();
bool IsLegacyApiLinked();
int Initialize(int maxSessions);
int Shutdown();
void SetMinorVersion(int minorVersion);
int CreateSession(SessionType type, int numBuffers, int bufferSize);
}

#endif // CIGI3_API_H
