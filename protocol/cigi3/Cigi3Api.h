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

const ApiInfo &GetApiInfo();
bool IsLegacyApiLinked();
}

#endif // CIGI3_API_H
