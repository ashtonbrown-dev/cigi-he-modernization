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
#include "globals.h"
#include "hemumsg.h"
#include "MainFrm.h"

extern long ScriptExecuteFile(const char *fname);   // defined in script.cpp

extern void ScriptPostTextMessage(const char *text);

long ScriptDoNothing(const char *buffer, const unsigned int linenum)
{
    return 0;
}

long ScriptDoExecute(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = {0};
    char fname[256] = {0};
    char *ptr = (char *)buffer;
    int idx = 0;

    // Get a pointer to the first character after the EXECUTE keyword.
    sscanf(buffer, "%s", keyword);
    ptr += strlen(keyword) + 1;

    // Find the opening quote.
    while ((*ptr == ' ') || (*ptr == '\t'))
        ++ptr;

    if (*ptr != '\"') {
        CString errmsg;
        errmsg.Format("Error: Invalid EXECUTE command.  Quotation marks (\"\") expected. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Get the next character after the opening quote.
    ++ptr;

    // Copy the buffer.
    while ((*ptr != '\"') && (*ptr != '\n')) {
        fname[idx++] = *ptr;
        ptr++;
    }

    // Add the terminating NULL.
    fname[idx] = '\0';

    ScriptExecuteFile(fname);

    return 0;
}

long ScriptDoSpawnApp(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char *ptr = (char *)buffer;
    int idx = 0;

    // Get a pointer to the first character in the filename.
    sscanf(buffer, "%s", keyword);
    ptr += strlen(keyword) + 1;

    // Post a message to the main thread to spawn the application.
    MESSAGE_SPAWN_APP msg;
    strcpy(msg.command, ptr);
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoRun(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    double t;
    unsigned long millisecs = 0;
    unsigned long wait_ct = 0;

    sscanf(buffer, "%s %lf", keyword, &t);

    if (t < 0) {
        CString errmsg;
        errmsg.Format("Error: Invalid duration. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    } else if (t > 0.00000001) { // assume this is zero
        MESSAGE_SCRIPT_BEGIN_WAIT_TIMER begin_msg;
        begin_msg.secs = (int)t;
        PostGUIMsg(begin_msg);

        millisecs = GetTickCount() + unsigned long(t * 1000.0f);
    }

    // Set the script state to auto pause.
    g_ScriptParsePause = TRUE;

    // Run the exercise.  This is the same code as in CMainFrame::OnExerciseRun().
    ::SetFreezeFlag(FALSE);
    ::SetResetIGFlag(FALSE);
    MESSAGE_RUN_EXERCISE msg;
    PostDriverMsg(msg);

    if (t > 0.00000001) {       // assume this is zero
        DWORD count = GetTickCount();
        DWORD prev_count = count;   // makes the logic work the first time through

        while (count < millisecs) {
            if (WaitForSingleObject(g_ShutdownEventHdl, 0) != WAIT_TIMEOUT)
                _endthread();

            // Check to see if the user has stopped execution.
            if (g_ScriptState == SCRIPT_STATE_STOP)
                return 0;

            Sleep(50);

            // If the script is paused, increment the target time so
            // the while loop doesn't exit yet.
            if (g_ScriptState == SCRIPT_STATE_PAUSE)
                millisecs += count - prev_count;
            else {
                // Send a message to increment the wait timer display.
                if (wait_ct++ % 200) {
                    MESSAGE_SCRIPT_INC_WAIT_TIMER inc_msg;
                    inc_msg.secs = (millisecs - GetTickCount()) / 1000;
                    PostGUIMsg(inc_msg);
                }
            }

            prev_count = count;
            count = GetTickCount();
        }

        MESSAGE_SCRIPT_END_WAIT_TIMER end_msg;
        PostGUIMsg(end_msg);

        // Freeze the exercise.
        MESSAGE_PAUSE_EXERCISE pause_msg;
        PostGUIMsg(pause_msg);

        g_ScriptParsePause = FALSE;
    } else {
        ScriptPostTextMessage("Simulation running.  Click Play to resume script.");

        // Pause, but reenable parsing.
        g_ScriptState = SCRIPT_STATE_PAUSE;
        g_ScriptParsePause = FALSE;
    }

    return 0;
}

long ScriptDoFrameWait(const char *buffer, const unsigned int linenum)
{
    // Send a SOF to the driver.  The driver will know
    // that if it receives this, it is a signal to wait.
    CIGI_START_OF_FRAME sof = {  sizeof(CIGI_START_OF_FRAME),
CIGI_START_OF_FRAME_OPCODE, 4, 0, 1, 0, 0, 0, 0, 0
                                 };

    char keyword[64] = "";
    unsigned long count;

    sscanf(buffer, "%s %lu", keyword, &count);

    // Queue the desired number of packets.
    for (unsigned long i = 0; i < count; i++) {
        // If the shutdown event is signaled, terminate the thread.
        if (WaitForSingleObject(g_ShutdownEventHdl, 0) != WAIT_TIMEOUT)
            _endthread();

        // Send a FRAME to the driver.
        g_SendImmedCIGIMsgQueue.Push((char *)&sof, sizeof(sof));

        // Now wait until the driver has had time to process the FRAME.
        if (WaitForSingleObject(g_FrameWaitEventHdl, 1000) != WAIT_OBJECT_0)
            PrintMessageText("Warning: Driver did not respond to FRAME command.");
    }

    Sleep(16);

    // Since we might be waiting a noticeable amount of time, increment
    // the progress meter in case the script is very short.
    MESSAGE_SCRIPT_PROGRESS msg_progress;
    msg_progress.linenum = linenum;
    PostGUIMsg(msg_progress);

    return 0;
}

long ScriptDoFrame(const char *buffer, const unsigned int linenum)
{
    if (g_CigiMinorVersion == 0) {
        // Send a SOF to the driver.  The driver will know
        // that if it receives this, it is a signal to wait.
    CIGI_START_OF_FRAME sof = {  sizeof(CIGI_START_OF_FRAME),
CIGI_START_OF_FRAME_OPCODE, 4, 0, 1, 0, 0, 0, 0, 0
                                     };

        // Queue the packet.
        g_SendImmedCIGIMsgQueue.Push((char *)&sof, sizeof(sof));
    }

    // Now wait until the driver has had time to process the FRAME.
    if (WaitForSingleObject(g_FrameWaitEventHdl, 1000) != WAIT_OBJECT_0)
        PrintMessageText("Warning: Driver did not respond to FRAME command.");

    return 0;
}

long ScriptDoClearMessages(const char *buffer, const unsigned int linenum)
{
    MESSAGE_CLEAR_MSG_TEXT msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoMessage(const char *buffer, const unsigned int linenum)
{
    char keyword[64], text[256];
    char *ptr = (char *)buffer;
    int idx = 0;

    // Get a pointer to the first character after the MESSAGE keyword.
    sscanf(buffer, "%s", keyword);
    ptr += strlen(keyword) + 1;

    // Find the opening quote.
    while ((*ptr == ' ') || (*ptr == '\t'))
        ++ptr;

    if (*ptr != '\"') {
        CString errmsg;
        errmsg.Format("Error: Invalid MESSAGE command.  Quotation marks (\"\") expected. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Get the next character after the opening quote.
    ++ptr;

    // Copy the buffer.
    while ((*ptr != '\"') && (*ptr != '\n') && (idx < 255)) {
        text[idx++] = *ptr;
        ptr++;
    }

    // Add the terminating NULL.
    text[idx] = '\0';

    ScriptPostTextMessage(text);

    return 0;
}

long ScriptDoPause(const char *buffer, const unsigned int linenum)
{
    // Set the script state to pause.
    g_ScriptState = SCRIPT_STATE_PAUSE;

    // Stop parsing.
    g_ScriptParsePause = TRUE;

    ScriptPostTextMessage("Paused.  Click Play to resume.");
    MESSAGE_SCRIPT_SET_PAUSE msg;
    PostGUIMsg(msg);

    // Since we might be waiting a noticeable amount of time, increment
    // the progress meter in case the script is very short.
    MESSAGE_SCRIPT_PROGRESS msg_progress;
    msg_progress.linenum = linenum;
    PostGUIMsg(msg_progress);

    return 0;
}

long ScriptDoPauseMessage(const char *buffer, const unsigned int linenum)
{
    char keyword[64], text[256];
    char *ptr = (char *)buffer;
    int idx = 0;

    // Get a pointer to the first character after the open quotes.
    sscanf(buffer, "%s", keyword);
    ptr += strlen(keyword) + 2;

    // Copy the buffer.
    while ((*ptr != '\"') && (*ptr != '\n') && (idx < 255)) {
        text[idx++] = *ptr;
        ptr++;
    }

    // Add the terminating NULL.
    text[idx] = '\0';

    // Set the script state to pause.
    g_ScriptState = SCRIPT_STATE_PAUSE;

    // Stop parsing.
    g_ScriptParsePause = TRUE;

    ScriptPostTextMessage(text);
    MESSAGE_SCRIPT_SET_PAUSE msg;
    PostGUIMsg(msg);

    // Since we might be waiting a noticeable amount of time, increment
    // the progress meter in case the script is very short.
    MESSAGE_SCRIPT_PROGRESS msg_progress;
    msg_progress.linenum = linenum;
    PostGUIMsg(msg_progress);

    return 0;
}

long ScriptDoWait(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    double t;
    unsigned long millisecs = 0;
    unsigned long wait_ct = 0;

    sscanf(buffer, "%s %lf", keyword, &t);

    MESSAGE_SCRIPT_BEGIN_WAIT_TIMER begin_msg;
    begin_msg.secs = (int)t;
    PostGUIMsg(begin_msg);

    millisecs = GetTickCount() + unsigned long(t * 1000.0f);

    g_ScriptParsePause = TRUE;

    DWORD count = GetTickCount();
    DWORD prev_count = count;   // makes the logic work the first time through

    while (count < millisecs) {
        if (WaitForSingleObject(g_ShutdownEventHdl, 0) != WAIT_TIMEOUT)
            _endthread();

        // Check to see if the user has stopped execution.
        if (g_ScriptState == SCRIPT_STATE_STOP)
            return 0;

        Sleep(50);

        // If the script is paused, increment the target time so
        // the while loop doesn't exit yet.
        if (g_ScriptState == SCRIPT_STATE_PAUSE)
            millisecs += count - prev_count;
        else {
            // Send a message to increment the wait timer display.
            if (wait_ct++ % 200) {
                MESSAGE_SCRIPT_INC_WAIT_TIMER inc_msg;
                inc_msg.secs = (millisecs - GetTickCount()) / 1000;
                PostGUIMsg(inc_msg);
            }
        }

        prev_count = count;
        count = GetTickCount();
    }

    MESSAGE_SCRIPT_END_WAIT_TIMER end_msg;
    PostGUIMsg(end_msg);

    g_ScriptParsePause = FALSE;

    // Since we might be waiting a noticeable amount of time, increment
    // the progress meter in case the script is very short.
    MESSAGE_SCRIPT_PROGRESS msg_progress;
    msg_progress.linenum = linenum;
    PostGUIMsg(msg_progress);

    return 0;
}

long ScriptDoCapturePlay(const char *buffer, const unsigned int linenum)
{
    char keyword[64];
    char filename[256];
    char *ptr = (char *)buffer;
    int idx = 0;

    // Get a pointer to the first character after the open quotes.
    sscanf(buffer, "%s", keyword);
    ptr += strlen(keyword) + 2;

    // Copy the buffer.
    while ((*ptr != '\"') && (*ptr != '\n')) {
        filename[idx++] = *ptr;
        ptr++;
    }

    // Add the terminating NULL.
    filename[idx] = '\0';

    // Touch the file to see if it exists.
    HANDLE hfile = CreateFile(filename, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hfile == NULL) {
        CString errmsg;
        errmsg.Format("Error: Invalid filename specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        CloseHandle(hfile);

        return -1;
    }
    CloseHandle(hfile);

    // Run the exercise.
    ((CMainFrame *)AfxGetMainWnd())->OnExerciseRun();

    g_RecPlaybackState = RECPLAYBACK_STATE_PLAY;

    // Post a message so the main thread will open and play the file.
    MESSAGE_BEGIN_PLAYBACK msg;
    char **fname = NULL;
    GetFullPathName(filename, sizeof(msg.filename), msg.filename, fname);
    if (msg.filename[0] != '\0')
        PostDriverMsg(msg);

    // Reuse the filename buffer to hold the text message.
    sprintf(filename, "Playing %s", msg.filename);
    ScriptPostTextMessage(filename);

    while (g_RecPlaybackState == RECPLAYBACK_STATE_PLAY) {
        if (WaitForSingleObject(g_ShutdownEventHdl, 0) != WAIT_TIMEOUT)
            _endthread();

        Sleep(1);

        // Check to see if the user has stopped execution.
        if (g_ScriptState == SCRIPT_STATE_STOP) {
            MESSAGE_END_PLAYBACK msg;
            PostDriverMsg(msg);

            g_RecPlaybackState = RECPLAYBACK_STATE_STOP;

            return 0;
        }
    }

    // Freeze the exercise.
    ((CMainFrame *)AfxGetMainWnd())->OnExercisePause();

    return 0;
}

// Undocumented:
////////////////////////////////////////////////////////////////////////////////

long ScriptDoTranspDelayAddPoint(const char *buffer, const unsigned int linenum)
{
    char keyword[64];
    long point_id = 0;
    DOF dof = { 0.0 };

    sscanf(buffer, "%s %ld %lf %lf %lf %lf %lf %lf", keyword, &point_id, &dof.latitude,
           &dof.longitude, &dof.altitude, &dof.yaw, &dof.pitch, &dof.roll);

    g_DataManager.SetTranspDelayTestPoint(point_id, &dof);

    return 0;
}

long ScriptDoTranspDelayClearPoints(const char *buffer, const unsigned int linenum)
{
    g_DataManager.ClearTranspDelayTestPoints();
    return 0;
}

long ScriptDoTranspDelayTest(const char *buffer, const unsigned int linenum)
{
    char keyword[64];
    int point1_id = 0;
    int point2_id = 0;
    int count1 = 0;
    int count2 = 0;
    DOF dof1 = { 0.0 };
    DOF dof2 = { 0.0 };
    MESSAGE_TDTEST_START msg;

    // Check for the existence of the ownship.
    if (g_DataManager.GetEntity(0) == NULL) {
        CString errmsg;
        errmsg.Format("Error: Cannot execute transport delay test. Ownship not created. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    sscanf(buffer, "%s %d %d %d %d", keyword, &point1_id, &count1, &point2_id, &count2);

    if (g_DataManager.GetTranspDelayTestPoint(point1_id, &dof1) == FALSE) {
        CString errmsg;
        errmsg.Format("Error: Invalid test point specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    if (g_DataManager.GetTranspDelayTestPoint(point2_id, &dof2) == FALSE) {
        CString errmsg;
        errmsg.Format("Error: Invalid test point specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    // Build and send the message.
    msg.count1 = count1;
    msg.count2 = count2;
    msg.lat1 = dof1.latitude;
    msg.lon1 = dof1.longitude;
    msg.alt1 = dof1.altitude;
    msg.heading1 = dof1.yaw;
    msg.pitch1 = dof1.pitch;
    msg.roll1 = dof1.roll;
    msg.lat2 = dof2.latitude;
    msg.lon2 = dof2.longitude;
    msg.alt2 = dof2.altitude;
    msg.heading2 = dof2.yaw;
    msg.pitch2 = dof2.pitch;
    msg.roll2 = dof2.roll;
    PostDriverMsg(msg);

    g_TestScriptTranspDelayRunning = 1;

    ScriptPostTextMessage("Transport Delay Test running. Click Stop to end.");

    return 0;
}
