#ifndef CIGI_PROTOCOL_ADAPTER_H
#define CIGI_PROTOCOL_ADAPTER_H

#include "CigiProtocolVersion.h"

typedef long (*CigiProtocolCallback)(const int sessionid, void *packet);

struct CigiHostCallbacks
{
    CigiProtocolCallback startOfFrame;
    CigiProtocolCallback hatHotResponse;
    CigiProtocolCallback hatHotExtResponse;
    CigiProtocolCallback losResponse;
    CigiProtocolCallback losExtResponse;
    CigiProtocolCallback sensorResponse;
    CigiProtocolCallback sensorExtResponse;
    CigiProtocolCallback positionResponse;
    CigiProtocolCallback weatherResponse;
    CigiProtocolCallback aerosolResponse;
    CigiProtocolCallback maritimeSurfaceResponse;
    CigiProtocolCallback terrestrialSurfaceResponse;
    CigiProtocolCallback collisionSegmentNotification;
    CigiProtocolCallback collisionVolumeNotification;
    CigiProtocolCallback animationStopNotification;
    CigiProtocolCallback eventNotification;
    CigiProtocolCallback igMessage;
};

struct CigiParserCallbacks
{
    CigiProtocolCallback igControl;
    CigiProtocolCallback entityPosition;
    CigiProtocolCallback ccEntityPosition;
    CigiProtocolCallback componentControl;
    CigiProtocolCallback shortComponentControl;
    CigiProtocolCallback artPartControl;
    CigiProtocolCallback shortArtPartControl;
    CigiProtocolCallback velocityControl;
    CigiProtocolCallback celestialSphereControl;
    CigiProtocolCallback atmosphereControl;
    CigiProtocolCallback envRegionControl;
    CigiProtocolCallback weatherControl;
    CigiProtocolCallback maritimeSurfaceControl;
    CigiProtocolCallback waveControl;
    CigiProtocolCallback terrestrialSurfaceControl;
    CigiProtocolCallback viewControl;
    CigiProtocolCallback sensorControl;
    CigiProtocolCallback motionTrackerControl;
    CigiProtocolCallback ermDef;
    CigiProtocolCallback accelerationControlDef;
    CigiProtocolCallback viewDef;
    CigiProtocolCallback collisionSegmentDef;
    CigiProtocolCallback collisionVolumeDef;
    CigiProtocolCallback hatHotRequest;
    CigiProtocolCallback losSegmentRequest;
    CigiProtocolCallback losVectorRequest;
    CigiProtocolCallback positionRequest;
    CigiProtocolCallback envConditionsRequest;
    CigiProtocolCallback symbolSurfaceDef;
    CigiProtocolCallback symbolTextDef;
    CigiProtocolCallback symbolCircleDef;
    CigiProtocolCallback symbolPolygonDef;
    CigiProtocolCallback symbolClone;
    CigiProtocolCallback symbolControl;
    CigiProtocolCallback shortSymbolControl;
    CigiProtocolCallback entityControl;
    CigiProtocolCallback animationControl;

    CigiProtocolCallback skippedFrame;
    CigiHostCallbacks igResponses;
};

struct CigiParserSessions
{
    int hostSession;
    int igSession;
};

enum CigiProtocolCapabilityStatus
{
    CIGI_PROTOCOL_CAPABILITY_UNSUPPORTED = 0,
    CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED = 1,
    CIGI_PROTOCOL_CAPABILITY_SUPPORTED = 2
};

struct CigiProtocolCapability
{
    CigiProtocolCapabilityStatus status;
    const char *description;
};

struct CigiProtocolCapabilities
{
    CigiProtocolVersion version;
    CigiProtocolCapability sessionLifecycle;
    CigiProtocolCapability parserSessions;
    CigiProtocolCapability packetSend;
    CigiProtocolCapability packetReceiveWatch;
    CigiProtocolCapability entityControl;
    CigiProtocolCapability viewControl;
    CigiProtocolCapability weatherEnvironment;
    CigiProtocolCapability articulatedPartComponent;
};

struct CigiEntityDestructionData
{
    unsigned short entityId;
    int entityType;
    bool extendedType;
    unsigned char kind;
    unsigned char domain;
    unsigned short country;
    unsigned char category;
    unsigned char subcategory;
    unsigned char specific;
    unsigned char extra;
};

class ICigiProtocolAdapter
{
public:
    virtual ~ICigiProtocolAdapter() {}

    virtual CigiProtocolVersion GetActiveVersion() const = 0;
    virtual CigiProtocolCapabilities GetCapabilities() const = 0;
    virtual bool IsPacketIoSupported() const = 0;
    virtual const char *GetPacketIoUnsupportedReason() const = 0;
    virtual bool Configure(void *igControlPacket) = 0;

    virtual int InitializeHostSession(const CigiHostCallbacks *callbacks,
                                      int maxSessions,
                                      int numBuffers,
                                      int bufferSize) = 0;
    virtual CigiParserSessions InitializeParserSessions(
        const CigiParserCallbacks *callbacks,
        int maxSessions,
        int numBuffers,
        int bufferSize) = 0;
    virtual void StartMessage(int session) = 0;
    virtual void AddIGControlPacket(int session, void *igControlPacket) = 0;
    virtual bool AddEntityDestroyedPacket(
        int session, const CigiEntityDestructionData &entity) = 0;
    virtual bool AddEntitySelectionViewControlPacket(
        int session, unsigned short viewId, unsigned short entityId) = 0;
    virtual void EndMessage(int session) = 0;
    virtual void SetIncomingMessageBuffer(int session, unsigned char *buffer,
                                          int size) = 0;
    virtual void SyncFrameCounter(int session) = 0;
    virtual void ProcessIncomingMessage(int session) = 0;
    virtual void GetOutgoingMessageBuffer(int session, unsigned char **buffer,
                                          int *size) = 0;
    virtual void SwapOutgoingMessageBuffer(char *buffer, int size) = 0;
    virtual int GetPacketId(const unsigned char *buffer, int size) const = 0;
    virtual bool IsFrameBoundaryPacket(const unsigned char *buffer,
                                       int size) const = 0;
    virtual bool AddLegacyQueuedPacket(int session,
                                       const unsigned char *buffer,
                                       int size) = 0;
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
