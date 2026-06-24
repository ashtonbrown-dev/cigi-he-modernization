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
