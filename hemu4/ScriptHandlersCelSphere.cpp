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

long ScriptDoDate(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long m = 0, d = 0, y = 0;

    sscanf(buffer, "%s %lu/%lu/%lu", keyword, &m, &d, &y);

    // Update the environment data.
    CTime datetime = g_DataManager.GetGlobalEnv()->GetDateTime();
    CTime newtime(y, m, d, datetime.GetHour(), datetime.GetMinute(), 0);
    g_DataManager.GetGlobalEnv()->SetDateTime(newtime);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoTime(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long h = 0, m = 0;

    sscanf(buffer, "%s %lu:%lu", keyword, &h, &m);

    // Update the environment data.
    CTime datetime = g_DataManager.GetGlobalEnv()->GetDateTime();
    CTime newtime(datetime.GetYear(), datetime.GetMonth(), datetime.GetDay(), h, m, 0);
    g_DataManager.GetGlobalEnv()->SetDateTime(newtime);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoEphemerisMode(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long mode = 0;

    sscanf(buffer, "%s %lu", keyword, &mode);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetEphemerisEnable(mode);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoSunEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu", keyword, &enable);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetSunEnable(enable);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoMoonEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu", keyword, &enable);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetMoonEnable(enable);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoStarfieldEnable(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long enable = 0;

    sscanf(buffer, "%s %lu", keyword, &enable);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetStarfieldEnable(enable);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}

long ScriptDoStarfieldIntensity(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float intensity = 0;

    sscanf(buffer, "%s %f", keyword, &intensity);

    // Update the environment data.
    g_DataManager.GetGlobalEnv()->SetStarfieldIntensity(intensity);

    MESSAGE_SYNC_UPDATE_ENV_VIEW env_msg;
    PostDriverMsg(env_msg);

    return 0;
}
