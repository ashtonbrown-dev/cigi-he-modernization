#include "CigiProtocolAdapter.h"

#include "cigi4.h"
#include "cigi4types.h"
#include "protocol/cigi3/Cigi3Api.h"

#include <stdio.h>

namespace
{
CigiProtocolCapability MakeCapability(CigiProtocolCapabilityStatus status,
                                      const char *description)
{
    CigiProtocolCapability capability;
    capability.status = status;
    capability.description = description;
    return capability;
}

CigiProtocolCapabilities MakeCigi4Capabilities(
    const CigiProtocolVersion &version)
{
    CigiProtocolCapabilities capabilities;
    capabilities.version = version;
    capabilities.sessionLifecycle = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 host and IG sessions use the existing known-good runtime path");
    capabilities.parserSessions = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 parser sessions and callback registration are implemented");
    capabilities.packetSend = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 outgoing packet construction and send buffering are implemented");
    capabilities.packetReceiveWatch = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 incoming packet processing and packet watch are implemented");
    capabilities.entityControl = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 entity control packets use the existing known-good mapping");
    capabilities.viewControl = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 view control packets use the existing known-good mapping");
    capabilities.weatherEnvironment = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 weather and environment packets use the existing known-good mapping");
    capabilities.articulatedPartComponent = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 4 articulated-part and component packets use the existing known-good mapping");
    return capabilities;
}

CigiProtocolCapabilities MakeCigi3Capabilities(
    const CigiProtocolVersion &version)
{
    CigiProtocolCapabilities capabilities;
    capabilities.version = version;
    capabilities.sessionLifecycle = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 3 host session lifecycle is initialized through the isolated CIGI 3 facade");
    capabilities.parserSessions = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_SUPPORTED,
        "CIGI 3 host/IG parser sessions can be allocated; decode callbacks are not implemented");
    capabilities.packetSend = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 outgoing packet construction and send buffering are not implemented");
    capabilities.packetReceiveWatch = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 incoming packet processing and packet watch are not implemented");
    capabilities.entityControl = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 entity control mapping is not implemented");
    capabilities.viewControl = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 view control mapping is not implemented");
    capabilities.weatherEnvironment = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 weather and environment mapping is not implemented");
    capabilities.articulatedPartComponent = MakeCapability(
        CIGI_PROTOCOL_CAPABILITY_NOT_IMPLEMENTED,
        "CIGI 3 articulated-part and component mapping is not implemented");
    return capabilities;
}

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

    virtual CigiProtocolCapabilities GetCapabilities() const
    {
        return MakeCigi4Capabilities(m_Version);
    }

    virtual bool IsPacketIoSupported() const
    {
        return true;
    }

    virtual const char *GetPacketIoUnsupportedReason() const
    {
        return "";
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

    virtual int InitializeHostSession(const CigiHostCallbacks *callbacks,
                                      int maxSessions,
                                      int numBuffers,
                                      int bufferSize)
    {
        CigiInit(maxSessions, CIGI_VERSION);
        const int session =
            CigiCreateSession(CIGI_HOST_SESSION, numBuffers, bufferSize);
        RegisterHostCallbacks(callbacks);
        return session;
    }

    virtual CigiParserSessions InitializeParserSessions(
        const CigiParserCallbacks *callbacks,
        int maxSessions,
        int numBuffers,
        int bufferSize)
    {
        CigiParserSessions sessions = {-1, -1};

        CigiInit(maxSessions, CIGI_VERSION);

        sessions.hostSession =
            CigiCreateSession(CIGI_HOST_SESSION, numBuffers, bufferSize);
        RegisterParserHostCallbacks(callbacks);

        sessions.igSession =
            CigiCreateSession(CIGI_IG_SESSION, numBuffers, bufferSize);
        RegisterParserIgCallbacks(callbacks);

        return sessions;
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

    virtual int GetPacketId(const unsigned char *buffer, int size) const
    {
        if (!buffer || size < (int)sizeof(CIGI_PACKET_HEADER))
            return -1;

        const CIGI_PACKET_HEADER *header =
            (const CIGI_PACKET_HEADER *)buffer;
        return header->packet_id;
    }

    virtual bool IsFrameBoundaryPacket(const unsigned char *buffer,
                                       int size) const
    {
        return GetPacketId(buffer, size) == CIGI_START_OF_FRAME_OPCODE;
    }

    virtual bool AddLegacyQueuedPacket(int session,
                                       const unsigned char *buffer,
                                       int size)
    {
        if (!buffer || size < (int)sizeof(CIGI_PACKET_HEADER))
            return false;

        switch (GetPacketId(buffer, size)) {
        case CIGI_IG_CONTROL_OPCODE:
            CigiAddPacketIGCtrl(session, (CIGI_IG_CONTROL *)buffer);
            return true;

        case CIGI_ENTITY_POSITION_OPCODE:
            CigiAddPacketReplaceEntityPosition(session,
                                               (CIGI_ENTITY_POSITION *)buffer);
            return true;

        case CIGI_ENTITY_POSITION_CC_OPCODE:
            CigiAddPacketReplaceCCEntityPosition(
                session, (CIGI_ENTITY_POSITION_CC *)buffer);
            return true;

        case CIGI_COMPONENT_CONTROL_OPCODE:
            CigiAddPacketReplaceComponentCtrl(session,
                                              (CIGI_COMPONENT_CONTROL *)buffer);
            return true;

        case CIGI_COMPONENT_CONTROL_S_OPCODE:
            CigiAddPacketReplaceShortComponentCtrl(
                session, (CIGI_COMPONENT_CONTROL_S *)buffer);
            return true;

        case CIGI_ARTPART_CONTROL_OPCODE:
            CigiAddPacketReplaceArtPartCtrl(session,
                                            (CIGI_ARTPART_CONTROL *)buffer);
            return true;

        case CIGI_ARTPART_CONTROL_S_OPCODE:
            CigiAddPacketReplaceShortArtPartCtrl(
                session, (CIGI_ARTPART_CONTROL_S *)buffer);
            return true;

        case CIGI_VELOCITY_CONTROL_OPCODE:
            CigiAddPacketReplaceVelocityCtrl(session,
                                             (CIGI_VELOCITY_CONTROL *)buffer);
            return true;

        case CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE:
            CigiAddPacketReplaceCelestialSphereCtrl(
                session, (CIGI_CELESTIAL_SPHERE_CONTROL *)buffer);
            return true;

        case CIGI_ATMOSPHERE_CONTROL_OPCODE:
            CigiAddPacketReplaceAtmosphereCtrl(
                session, (CIGI_ATMOSPHERE_CONTROL *)buffer);
            return true;

        case CIGI_ENV_REGION_CONTROL_OPCODE:
            CigiAddPacketReplaceEnvRegionCtrl(
                session, (CIGI_ENV_REGION_CONTROL *)buffer);
            return true;

        case CIGI_WEATHER_CONTROL_OPCODE:
            CigiAddPacketReplaceWeatherCtrl(session,
                                            (CIGI_WEATHER_CONTROL *)buffer);
            return true;

        case CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE:
            CigiAddPacketReplaceMaritimeCondCtrl(
                session, (CIGI_MARITIME_CONDITIONS_CONTROL *)buffer);
            return true;

        case CIGI_WAVE_CONTROL_OPCODE:
            CigiAddPacketReplaceWaveCtrl(session, (CIGI_WAVE_CONTROL *)buffer);
            return true;

        case CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE:
            CigiAddPacketReplaceTerrestrialCondCtrl(
                session, (CIGI_TERRESTRIAL_CONDITIONS_CONTROL *)buffer);
            return true;

        case CIGI_VIEW_CONTROL_OPCODE:
            CigiAddPacketReplaceViewCtrl(session, (CIGI_VIEW_CONTROL *)buffer);
            return true;

        case CIGI_SENSOR_CONTROL_OPCODE:
            CigiAddPacketReplaceSensorCtrl(session,
                                           (CIGI_SENSOR_CONTROL *)buffer);
            return true;

        case CIGI_MOTION_TRACKER_CONTROL_OPCODE:
            CigiAddPacketReplaceMotionTrackerCtrl(
                session, (CIGI_MOTION_TRACKER_CONTROL *)buffer);
            return true;

        case CIGI_ERM_DEF_OPCODE:
            CigiAddPacketReplaceErmDef(session, (CIGI_ERM_DEF *)buffer);
            return true;

        case CIGI_ACCELERATION_CONTROL_DEF_OPCODE:
            CigiAddPacketReplaceAccelerationDef(
                session, (CIGI_ACCELERATION_CONTROL_DEF *)buffer);
            return true;

        case CIGI_VIEW_DEF_OPCODE:
            CigiAddPacketReplaceViewDef(session, (CIGI_VIEW_DEF *)buffer);
            return true;

        case CIGI_COLL_SEGMENT_DEF_OPCODE:
            CigiAddPacketReplaceCollDetSegmentDef(
                session, (CIGI_COLL_SEGMENT_DEF *)buffer);
            return true;

        case CIGI_COLL_VOLUME_DEF_OPCODE:
            CigiAddPacketReplaceCollDetVolumeDef(
                session, (CIGI_COLL_VOLUME_DEF *)buffer);
            return true;

        case CIGI_HAT_HOT_REQUEST_OPCODE:
            CigiAddPacketReplaceHatHotReq(session,
                                          (CIGI_HAT_HOT_REQUEST *)buffer);
            return true;

        case CIGI_LOS_SEGMENT_REQUEST_OPCODE:
            CigiAddPacketReplaceLosSegmentReq(
                session, (CIGI_LOS_SEGMENT_REQUEST *)buffer);
            return true;

        case CIGI_LOS_VECTOR_REQUEST_OPCODE:
            CigiAddPacketReplaceLosVectorReq(
                session, (CIGI_LOS_VECTOR_REQUEST *)buffer);
            return true;

        case CIGI_POSITION_REQUEST_OPCODE:
            CigiAddPacketReplacePositionReq(session,
                                            (CIGI_POSITION_REQUEST *)buffer);
            return true;

        case CIGI_ENV_CONDITIONS_REQUEST_OPCODE:
            CigiAddPacketReplaceEnvConditionsReq(
                session, (CIGI_ENV_CONDITIONS_REQUEST *)buffer);
            return true;

        case CIGI_SYMBOL_SURFACE_DEF_OPCODE:
            CigiAddPacketReplaceSymbolSurfaceDef(
                session, (CIGI_SYMBOL_SURFACE_DEF *)buffer);
            return true;

        case CIGI_SYMBOL_TEXT_DEF_OPCODE:
            CigiAddPacketReplaceSymbolTextDef(
                session, (CIGI_SYMBOL_TEXT_DEF *)buffer);
            return true;

        case CIGI_SYMBOL_CIRCLE_DEF_OPCODE:
            CigiAddPacketReplaceSymbolCircleDef(
                session, (CIGI_SYMBOL_CIRCLE_DEF *)buffer);
            return true;

        case CIGI_SYMBOL_POLYGON_DEF_OPCODE:
            CigiAddPacketReplaceSymbolPolygonDef(
                session, (CIGI_SYMBOL_POLYGON_DEF *)buffer);
            return true;

        case CIGI_SYMBOL_CLONE_OPCODE:
            CigiAddPacketReplaceSymbolClone(session,
                                            (CIGI_SYMBOL_CLONE *)buffer);
            return true;

        case CIGI_SYMBOL_CONTROL_OPCODE:
            CigiAddPacketReplaceSymbolCtrl(session,
                                           (CIGI_SYMBOL_CONTROL *)buffer);
            return true;

        case CIGI_SYMBOL_CONTROL_S_OPCODE:
            CigiAddPacketReplaceShortSymbolCtrl(
                session, (CIGI_SYMBOL_CONTROL_S *)buffer);
            return true;

        case CIGI_ENTITY_CONTROL_OPCODE:
            CigiAddPacketReplaceEntityCtrl(session,
                                           (CIGI_ENTITY_CONTROL *)buffer);
            return true;

        case CIGI_ANIMATION_CONTROL_OPCODE:
            CigiAddPacketReplaceAnimationCtrl(session,
                                              (CIGI_ANIMATION_CONTROL *)buffer);
            return true;

        case CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_OPCODE:
            CigiAddPacketReplaceSymbolTexturedCircleDef(
                session, (CIGI_SYMBOL_TEXTURED_CIRCLE_DEF *)buffer);
            return true;

        case CIGI_SYMBOL_TEXTURED_POLYGON_DEF_OPCODE:
            CigiAddPacketReplaceSymbolTexturedPolygonDef(
                session, (CIGI_SYMBOL_TEXTURED_POLYGON_DEF *)buffer);
            return true;

        default:
            return false;
        }
    }

private:
    void RegisterParserHostCallbacks(const CigiParserCallbacks *callbacks)
    {
        if (!callbacks)
            return;

        CigiSetCallback(CIGI_IG_CONTROL_OPCODE,
                        callbacks->igControl);
        CigiSetCallback(CIGI_ENTITY_POSITION_OPCODE,
                        callbacks->entityPosition);
        CigiSetCallback(CIGI_ENTITY_POSITION_CC_OPCODE,
                        callbacks->ccEntityPosition);
        CigiSetCallback(CIGI_COMPONENT_CONTROL_OPCODE,
                        callbacks->componentControl);
        CigiSetCallback(CIGI_COMPONENT_CONTROL_S_OPCODE,
                        callbacks->shortComponentControl);
        CigiSetCallback(CIGI_ARTPART_CONTROL_OPCODE,
                        callbacks->artPartControl);
        CigiSetCallback(CIGI_ARTPART_CONTROL_S_OPCODE,
                        callbacks->shortArtPartControl);
        CigiSetCallback(CIGI_VELOCITY_CONTROL_OPCODE,
                        callbacks->velocityControl);
        CigiSetCallback(CIGI_CELESTIAL_SPHERE_CONTROL_OPCODE,
                        callbacks->celestialSphereControl);
        CigiSetCallback(CIGI_ATMOSPHERE_CONTROL_OPCODE,
                        callbacks->atmosphereControl);
        CigiSetCallback(CIGI_ENV_REGION_CONTROL_OPCODE,
                        callbacks->envRegionControl);
        CigiSetCallback(CIGI_WEATHER_CONTROL_OPCODE,
                        callbacks->weatherControl);
        CigiSetCallback(CIGI_MARITIME_CONDITIONS_CONTROL_OPCODE,
                        callbacks->maritimeSurfaceControl);
        CigiSetCallback(CIGI_WAVE_CONTROL_OPCODE,
                        callbacks->waveControl);
        CigiSetCallback(CIGI_TERRESTRIAL_CONDITIONS_CONTROL_OPCODE,
                        callbacks->terrestrialSurfaceControl);
        CigiSetCallback(CIGI_VIEW_CONTROL_OPCODE,
                        callbacks->viewControl);
        CigiSetCallback(CIGI_SENSOR_CONTROL_OPCODE,
                        callbacks->sensorControl);
        CigiSetCallback(CIGI_MOTION_TRACKER_CONTROL_OPCODE,
                        callbacks->motionTrackerControl);
        CigiSetCallback(CIGI_ERM_DEF_OPCODE,
                        callbacks->ermDef);
        CigiSetCallback(CIGI_ACCELERATION_CONTROL_DEF_OPCODE,
                        callbacks->accelerationControlDef);
        CigiSetCallback(CIGI_VIEW_DEF_OPCODE,
                        callbacks->viewDef);
        CigiSetCallback(CIGI_COLL_SEGMENT_DEF_OPCODE,
                        callbacks->collisionSegmentDef);
        CigiSetCallback(CIGI_COLL_VOLUME_DEF_OPCODE,
                        callbacks->collisionVolumeDef);
        CigiSetCallback(CIGI_HAT_HOT_REQUEST_OPCODE,
                        callbacks->hatHotRequest);
        CigiSetCallback(CIGI_LOS_SEGMENT_REQUEST_OPCODE,
                        callbacks->losSegmentRequest);
        CigiSetCallback(CIGI_LOS_VECTOR_REQUEST_OPCODE,
                        callbacks->losVectorRequest);
        CigiSetCallback(CIGI_POSITION_REQUEST_OPCODE,
                        callbacks->positionRequest);
        CigiSetCallback(CIGI_ENV_CONDITIONS_REQUEST_OPCODE,
                        callbacks->envConditionsRequest);
        CigiSetCallback(CIGI_SYMBOL_SURFACE_DEF_OPCODE,
                        callbacks->symbolSurfaceDef);
        CigiSetCallback(CIGI_SYMBOL_TEXT_DEF_OPCODE,
                        callbacks->symbolTextDef);
        CigiSetCallback(CIGI_SYMBOL_CIRCLE_DEF_OPCODE,
                        callbacks->symbolCircleDef);
        CigiSetCallback(CIGI_SYMBOL_POLYGON_DEF_OPCODE,
                        callbacks->symbolPolygonDef);
        CigiSetCallback(CIGI_SYMBOL_CLONE_OPCODE,
                        callbacks->symbolClone);
        CigiSetCallback(CIGI_SYMBOL_CONTROL_OPCODE,
                        callbacks->symbolControl);
        CigiSetCallback(CIGI_SYMBOL_CONTROL_S_OPCODE,
                        callbacks->shortSymbolControl);
        CigiSetCallback(CIGI_ENTITY_CONTROL_OPCODE,
                        callbacks->entityControl);
        CigiSetCallback(CIGI_ANIMATION_CONTROL_OPCODE,
                        callbacks->animationControl);
    }

    void RegisterParserIgCallbacks(const CigiParserCallbacks *callbacks)
    {
        if (!callbacks)
            return;

        CigiSetCallback(100, callbacks->skippedFrame);
        RegisterHostCallbacks(&callbacks->igResponses);
    }

    void RegisterHostCallbacks(const CigiHostCallbacks *callbacks)
    {
        if (!callbacks)
            return;

        CigiSetCallback(CIGI_START_OF_FRAME_OPCODE,
                        callbacks->startOfFrame);
        CigiSetCallback(CIGI_HAT_HOT_RESPONSE_OPCODE,
                        callbacks->hatHotResponse);
        CigiSetCallback(CIGI_HAT_HOT_EXT_RESPONSE_OPCODE,
                        callbacks->hatHotExtResponse);
        CigiSetCallback(CIGI_LOS_RESPONSE_OPCODE,
                        callbacks->losResponse);
        CigiSetCallback(CIGI_LOS_EXT_RESPONSE_OPCODE,
                        callbacks->losExtResponse);
        CigiSetCallback(CIGI_SENSOR_RESPONSE_OPCODE,
                        callbacks->sensorResponse);
        CigiSetCallback(CIGI_SENSOR_EXT_RESPONSE_OPCODE,
                        callbacks->sensorExtResponse);
        CigiSetCallback(CIGI_POSITION_RESPONSE_OPCODE,
                        callbacks->positionResponse);
        CigiSetCallback(CIGI_WEATHER_RESPONSE_OPCODE,
                        callbacks->weatherResponse);
        CigiSetCallback(CIGI_AEROSOL_RESPONSE_OPCODE,
                        callbacks->aerosolResponse);
        CigiSetCallback(CIGI_MARITIME_SURFACE_RESPONSE_OPCODE,
                        callbacks->maritimeSurfaceResponse);
        CigiSetCallback(CIGI_TERRESTRIAL_SURFACE_RESPONSE_OPCODE,
                        callbacks->terrestrialSurfaceResponse);
        CigiSetCallback(CIGI_COLL_SEGMENT_NOTIFICATION_OPCODE,
                        callbacks->collisionSegmentNotification);
        CigiSetCallback(CIGI_COLL_VOLUME_NOTIFICATION_OPCODE,
                        callbacks->collisionVolumeNotification);
        CigiSetCallback(CIGI_ANIMATION_STOP_NOTIFICATION_OPCODE,
                        callbacks->animationStopNotification);
        CigiSetCallback(CIGI_EVENT_NOTIFICATION_OPCODE,
                        callbacks->eventNotification);
        CigiSetCallback(CIGI_IG_MESSAGE_OPCODE,
                        callbacks->igMessage);
    }

    CigiProtocolVersion m_Version;
};

class Cigi3ProtocolAdapter : public ICigiProtocolAdapter
{
public:
    Cigi3ProtocolAdapter()
        : m_Version(CigiProtocolVersion::FromId(CIGI_3_0_3_1)),
          m_RuntimeInitialized(false),
          m_UnsupportedReported(false)
    {
    }

    virtual ~Cigi3ProtocolAdapter()
    {
        ShutdownRuntime();
    }

    void SetVersion(const CigiProtocolVersion &version)
    {
        m_Version = version;
        m_UnsupportedReported = false;
    }

    virtual CigiProtocolVersion GetActiveVersion() const
    {
        return m_Version;
    }

    virtual CigiProtocolCapabilities GetCapabilities() const
    {
        return MakeCigi3Capabilities(m_Version);
    }

    virtual bool IsPacketIoSupported() const
    {
        return false;
    }

    virtual const char *GetPacketIoUnsupportedReason() const
    {
        return "CIGI 3 packet I/O is scaffolded but not implemented";
    }

    virtual bool Configure(void *igControlPacket)
    {
        (void)igControlPacket;
        (void)cigi3::GetApiInfo();
        return false;
    }

    virtual int InitializeHostSession(const CigiHostCallbacks *callbacks,
                                      int maxSessions,
                                      int numBuffers,
                                      int bufferSize)
    {
        (void)callbacks;

        if (!InitializeRuntime(maxSessions))
            return -1;

        cigi3::SetMinorVersion(GetLegacySessionMinorVersion());

        const int session =
            cigi3::CreateSession(cigi3::HostSession, numBuffers, bufferSize);
        if (session < 0)
            ReportLifecycleError("Create host session", session);

        return session;
    }

    virtual CigiParserSessions InitializeParserSessions(
        const CigiParserCallbacks *callbacks,
        int maxSessions,
        int numBuffers,
        int bufferSize)
    {
        (void)callbacks;
        CigiParserSessions sessions = {-1, -1};

        if (!InitializeRuntime(maxSessions))
            return sessions;

        cigi3::SetMinorVersion(GetLegacySessionMinorVersion());

        sessions.hostSession =
            cigi3::CreateSession(cigi3::HostSession, numBuffers, bufferSize);
        if (sessions.hostSession < 0) {
            ReportLifecycleError("Create parser host session",
                                 sessions.hostSession);
            return sessions;
        }

        sessions.igSession =
            cigi3::CreateSession(cigi3::IgSession, numBuffers, bufferSize);
        if (sessions.igSession < 0)
            ReportLifecycleError("Create parser IG session",
                                 sessions.igSession);

        return sessions;
    }

    virtual void StartMessage(int session)
    {
        (void)session;
        ReportUnsupportedOperation("StartMessage");
    }

    virtual void AddIGControlPacket(int session, void *igControlPacket)
    {
        (void)session;
        (void)igControlPacket;
        ReportUnsupportedOperation("AddIGControlPacket");
    }

    virtual void EndMessage(int session)
    {
        (void)session;
        ReportUnsupportedOperation("EndMessage");
    }

    virtual void SetIncomingMessageBuffer(int session, unsigned char *buffer,
                                          int size)
    {
        (void)session;
        (void)buffer;
        (void)size;
        ReportUnsupportedOperation("SetIncomingMessageBuffer");
    }

    virtual void SyncFrameCounter(int session)
    {
        (void)session;
        ReportUnsupportedOperation("SyncFrameCounter");
    }

    virtual void ProcessIncomingMessage(int session)
    {
        (void)session;
        ReportUnsupportedOperation("ProcessIncomingMessage");
    }

    virtual void GetOutgoingMessageBuffer(int session, unsigned char **buffer,
                                          int *size)
    {
        (void)session;
        if (buffer)
            *buffer = NULL;
        if (size)
            *size = 0;
        ReportUnsupportedOperation("GetOutgoingMessageBuffer");
    }

    virtual void SwapOutgoingMessageBuffer(char *buffer, int size)
    {
        (void)buffer;
        (void)size;
        ReportUnsupportedOperation("SwapOutgoingMessageBuffer");
    }

    virtual int GetPacketId(const unsigned char *buffer, int size) const
    {
        (void)buffer;
        (void)size;
        return -1;
    }

    virtual bool IsFrameBoundaryPacket(const unsigned char *buffer,
                                       int size) const
    {
        (void)buffer;
        (void)size;
        return false;
    }

    virtual bool AddLegacyQueuedPacket(int session,
                                       const unsigned char *buffer,
                                       int size)
    {
        (void)session;
        (void)buffer;
        (void)size;
        ReportUnsupportedOperation("AddLegacyQueuedPacket");
        return false;
    }

private:
    bool InitializeRuntime(int maxSessions)
    {
        if (m_RuntimeInitialized)
            return true;

        if (!cigi3::IsLegacyApiLinked()) {
            ReportLifecycleError("Initialize runtime", -1);
            return false;
        }

        const int result = cigi3::Initialize(maxSessions);
        if (result != 0) {
            ReportLifecycleError("Initialize runtime", result);
            return false;
        }

        m_RuntimeInitialized = true;
        return true;
    }

    void ShutdownRuntime()
    {
        if (!m_RuntimeInitialized)
            return;

        cigi3::Shutdown();
        m_RuntimeInitialized = false;
    }

    int GetLegacySessionMinorVersion() const
    {
        const int minorVersion = m_Version.GetMinorVersion();

        if (minorVersion <= 1)
            return 0;

        // The isolated upstream CIGI 3 allocator has explicit startup buffer
        // layouts for 3.0/3.1 and 3.2. The 3.3 reference path creates sessions
        // with the 3.2 layout before applying packet-level 3.3 behavior.
        return 2;
    }

    void ReportLifecycleError(const char *operation, int result) const
    {
        fprintf(stderr, "CIGI 3 adapter: %s failed with result %d.\n",
                operation, result);
    }

    void ReportUnsupportedOperation(const char *operation) const
    {
        if (m_UnsupportedReported)
            return;

        m_UnsupportedReported = true;
        fprintf(stderr, "CIGI 3 adapter: %s is unsupported. %s.\n",
                operation,
                GetPacketIoUnsupportedReason());
    }

    CigiProtocolVersion m_Version;
    bool m_RuntimeInitialized;
    mutable bool m_UnsupportedReported;
};

Cigi4ProtocolAdapter Cigi4Adapter;
Cigi3ProtocolAdapter Cigi3Adapter;
}

CigiProtocolAdapterSelection CigiProtocolAdapterFactory::Select(
    const CigiProtocolVersion &requestedVersion)
{
    CigiProtocolAdapterSelection selection;

    if (requestedVersion.IsCigi3()) {
        Cigi3Adapter.SetVersion(requestedVersion);
        selection.adapter = &Cigi3Adapter;
        selection.exactMatch = true;
        return selection;
    }

    if (requestedVersion.IsPacketIoImplemented()) {
        Cigi4Adapter.SetVersion(requestedVersion);
        selection.exactMatch = true;
    } else {
        Cigi4Adapter.SetVersion(CigiProtocolVersion::Current());
        selection.exactMatch = false;
    }

    selection.adapter = &Cigi4Adapter;
    return selection;
}
