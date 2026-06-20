/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

#include "StdAfx.h"
#include "cigi4types.h"
#include "Entity.h"
#include "globals.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

// Utility function to create and send a Weather Control packet.
static void SendWeatherControlPacket(CWeather *weather)
{
    unsigned char buffer[sizeof(CIGI_WEATHER_CONTROL)] = {0};

    weather->CreateCigiPacket(buffer);

    SendImmedCigiMessage(buffer);
}

long ScriptDoWeatherHumidity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long humidity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &humidity);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetHumidity((unsigned char)humidity);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetWeatherEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherTopScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetTopScudEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherBottomScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetBottomScudEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherRandomWindEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetRandomWindEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherLightning(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &enable);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetRandomLightningEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherCloudType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long cloud_type = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &cloud_type);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetCloudType((unsigned char)cloud_type);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherSeverity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long severity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &severity);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetSeverity((unsigned char)severity);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}


long ScriptDoWeatherAirTemp(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float temp = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &temp);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetTemperature(temp);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherVisibility(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float visibility = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &visibility);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetVisibility(visibility);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherTopScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &freq);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetTopScudFreq((unsigned char)freq);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherBottomScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &freq);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetBottomScudFreq((unsigned char)freq);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherCoverage(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    unsigned long coverage = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &entity_id, &coverage);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetCoverage((unsigned char)coverage);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherTopTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &height);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetTopTransitionBand(height);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherBottomTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &height);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetBottomTransitionBand(height);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherWind(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float horz = 0.0;
    float vert = 0.0;
    float direction = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &entity_id, &horz, &vert, &direction);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetHorzWindspeed(horz);
    weather->SetVertWindspeed(vert);
    weather->SetWindDirection(direction);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherBaroPressure(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float pressure = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &pressure);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetBaroPressure(pressure);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoWeatherAerosolConc(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long entity_id = -1;
    float conc = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &entity_id, &conc);

    CEntity *entity = g_DataManager.GetEntity(entity_id);
    if (!entity) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather entity specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    CWeather *weather = entity->GetWeatherAttributes();
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Specified entity is not a weather entity. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Update the environment data.
    weather->SetAerosolConcentration(conc);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

// TODO:  The regional weather commands are not hooked up, nor are they
//        documented.
long ScriptDoRLayerHumidity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long humidity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &humidity);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetHumidity((unsigned char)humidity);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetWeatherEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerTopScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetTopScudEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerBottomScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetBottomScudEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerRandomWindEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetRandomWindEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerLightning(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetRandomLightningEnable((unsigned char)enable);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerCloudType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long cloud_type = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &cloud_type);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetCloudType((unsigned char)cloud_type);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerSeverity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long severity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &severity);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetSeverity((unsigned char)severity);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}


long ScriptDoRLayerAirTemp(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float temp = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &temp);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetTemperature(temp);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerVisibility(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float visibility = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &visibility);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetVisibility(visibility);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerTopScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &freq);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetTopScudFreq((unsigned char)freq);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerBottomScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &freq);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetBottomScudFreq((unsigned char)freq);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerCoverage(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long coverage = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &coverage);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetCoverage((unsigned char)coverage);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerTopTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &height);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetTopTransitionBand(height);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerBottomTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &height);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetBottomTransitionBand(height);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerWind(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float horz = 0.0;
    float vert = 0.0;
    float direction = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &layer_id, &horz, &vert, &direction);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetHorzWindspeed(horz);
    weather->SetVertWindspeed(vert);
    weather->SetWindDirection(direction);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerBaroPressure(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float pressure = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &pressure);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetBaroPressure(pressure);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerAerosolConc(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float conc = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &conc);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetAerosolConcentration(conc);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerThickness(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float thickness = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &thickness);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetThickness(thickness);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoRLayerElevation(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float elev = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &elev);

    CWeather *weather = g_DataManager.GetWeatherLayer(layer_id);
    if (!weather) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    weather->SetBaseElevation(elev);
    SendWeatherControlPacket(weather);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}


long ScriptDoGLayerHumidity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long humidity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &humidity);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetHumidity((unsigned char)humidity);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetWeatherEnable((unsigned char)enable);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerTopScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetTopScudEnable((unsigned char)enable);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerBottomScudEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetBottomScudEnable((unsigned char)enable);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerRandomWindEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetRandomWindEnable((unsigned char)enable);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerLightning(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &enable);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetRandomLightningEnable((unsigned char)enable);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerCloudType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long cloud_type = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &cloud_type);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetCloudType((unsigned char)cloud_type);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerSeverity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long severity = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &severity);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetSeverity((unsigned char)severity);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerAirTemp(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float temp = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &temp);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetTemperature(temp);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerVisibility(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float visibility = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &visibility);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetVisibility(visibility);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerTopScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &freq);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetTopScudFreq((unsigned char)freq);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerBottomScudFreq(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long freq = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &freq);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetBottomScudFreq((unsigned char)freq);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerCoverage(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    unsigned long coverage = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &layer_id, &coverage);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetCoverage((unsigned char)coverage);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerElevation(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float elev = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &elev);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetBaseElevation(elev);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerThickness(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float thickness = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &thickness);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetThickness(thickness);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerTopTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &height);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetTopTransitionBand(height);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerBottomTransitionBand(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float height = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &height);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetBottomTransitionBand(height);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerWind(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float horz = 0.0;
    float vert = 0.0;
    float direction = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &layer_id, &horz, &vert, &direction);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetHorzWindspeed(horz);
    layer->SetVertWindspeed(vert);
    layer->SetWindDirection(direction);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerBaroPressure(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float pressure = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &pressure);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetBaroPressure(pressure);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGLayerAerosolConc(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long layer_id = -1;
    float conc = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &layer_id, &conc);

    CWeather *layer = g_DataManager.GetWeatherLayer(layer_id);
    if (!layer) {
        CString errmsg;
        errmsg.Format("Error: Invalid weather layer specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Update the environment data.
    layer->SetAerosolConcentration(conc);
    SendWeatherControlPacket(layer);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}
