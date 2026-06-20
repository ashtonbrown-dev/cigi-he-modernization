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
#include "globals.h"
#include "hemumsg.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoGlobalHumidity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long humidity = 0;

    sscanf(buffer, "%s %lu", keyword, &humidity);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetHumidity(humidity);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGlobalAirTemp(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float temp = 0.0;

    sscanf(buffer, "%s %f", keyword, &temp);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetTemperature(temp);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGlobalVisibility(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float range = 0.0;

    sscanf(buffer, "%s %f", keyword, &range);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetVisibility(range);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGlobalWind(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float horz = 0.0;
    float vert = 0.0;
    float direction = 0.0;

    sscanf(buffer, "%s %f %f %f", keyword, &horz, &vert, &direction);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetHorzWindspeed(horz);
    g_DataManager.GetGlobalEnv()->SetVertWindspeed(vert);
    g_DataManager.GetGlobalEnv()->SetWindDirection(direction);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoGlobalBaroPressure(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float pressure = 0.0;

    sscanf(buffer, "%s %f", keyword, &pressure);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetBaroPressure(pressure);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}
