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
#include "CigiView.h"
#include "globals.h"
#include "hemumsg.h"
#include "ViewGroup.h"

extern void ScriptPostTextMessage(const char *text);

long ScriptDoAttachView(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long entity_id = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &entity_id);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.entity_id = entity_id;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialEntityID(entity_id);

    return 0;
}

long ScriptDoAttachViewgroup(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    unsigned long entity_id = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &group_id, &entity_id);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA grpdata = group->GetAndLockCigi();
    grpdata.entity_id = entity_id;
    group->SetLockedCigi(grpdata);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialEntityID(entity_id);

    // Iterate through all the views in this group and set the entity to match.
    CViewMap &map = g_DataManager.GetViewMap();
    CCigiView *view = NULL;
    int view_id = -1;
    POSITION pos = map.GetStartPosition();
    while (pos) {
        map.GetNextAssoc(pos, view_id, view);
        VIEW viewdata = view->GetAndLock();

        if (view && ((unsigned long)viewdata.viewdef.group_id == group_id)) {
            viewdata.viewctrl.entity_id = entity_id;
            view->SetLocked(viewdata);

            // Set the initial value so the user doesn't have to re-run the entire
            // script to return to this state if he wants to go into free-flight.
            view->SetInitialEntityID(entity_id);
        }

        view->Unlock();
    }

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewPitch(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float pitch = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &pitch);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.pitch = pitch;
    data.pitch_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialPitch(pitch);

    return 0;
}

long ScriptDoViewRoll(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &roll);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.roll = roll;
    data.roll_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialRoll(roll);

    return 0;
}

long ScriptDoViewType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long type = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &type);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.view_type = type;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialViewType(type);

    return 0;
}

long ScriptDoViewX(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float x = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &x);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.offset_x = x;
    data.offset_x_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialXOffset(x);

    return 0;
}

long ScriptDoViewXYZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &view_id, &x, &y, &z);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.offset_x = x;
    data.offset_x_enable = 1;
    data.offset_y = y;
    data.offset_y_enable = 1;
    data.offset_z = z;
    data.offset_z_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial values so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialXOffset(x);
    view->SetInitialYOffset(y);
    view->SetInitialZOffset(z);

    return 0;
}

long ScriptDoViewY(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float y = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &y);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.offset_y = y;
    data.offset_y_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialYOffset(y);

    return 0;
}

long ScriptDoViewYaw(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float yaw = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &yaw);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.yaw = yaw;
    data.yaw_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialYaw(yaw);

    return 0;
}

long ScriptDoViewYPR(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float yaw = 0.0;
    float pitch = 0.0;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &view_id, &yaw, &pitch, &roll);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.yaw = yaw;
    data.yaw_enable = 1;
    data.pitch = pitch;
    data.pitch_enable = 1;
    data.roll = roll;
    data.roll_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialYaw(yaw);
    view->SetInitialPitch(pitch);
    view->SetInitialRoll(roll);

    return 0;
}

long ScriptDoViewZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float z = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &z);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_CONTROL_DATA data = view->GetAndLockViewCtrl();
    data.offset_z = z;
    data.offset_z_enable = 1;
    view->SetLockedViewCtrl(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialZOffset(z);

    return 0;
}

long ScriptDoViewGroup(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long group_id = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &group_id);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.group_id = group_id;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialGroupID(group_id);

    // Now notify the main thread that it should update the view.
    MESSAGE_SCRIPT_SET_VIEWGROUP_IN_GUI msg;
    msg.view_id = view_id;
    msg.group_id = group_id;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewMirrorMode(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long mode = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &mode);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.mirror_mode = mode;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialMirrorMode(mode);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewPixelRepMode(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long mode = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &mode);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.replication = mode;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialReplicationMode(mode);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewProjectionType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long type = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &type);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.projection = type;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialProjectionType(type);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewBringToTop(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    unsigned long reorder = 0;

    sscanf(buffer, "%s %lu %lu", keyword, &view_id, &reorder);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.reorder = reorder;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialReorderFlag(reorder);

    return 0;
}

long ScriptDoViewNearPlane(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float fov_near = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &fov_near);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_near = fov_near;
    data.fov_near_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovNear(fov_near);

    return 0;
}

long ScriptDoViewFarPlane(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float fov_far = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &fov_far);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_far = fov_far;
    data.fov_far_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovFar(fov_far);

    return 0;
}

long ScriptDoViewFOVLeft(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float left = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &left);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_left = left;
    data.fov_left_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovLeft(left);

    return 0;
}

long ScriptDoViewFOVRight(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float right = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &right);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_right = right;
    data.fov_right_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovRight(right);

    return 0;
}

long ScriptDoViewFOVTop(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float top = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &top);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_top = top;
    data.fov_top_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovTop(top);

    return 0;
}

long ScriptDoViewFOVBottom(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long view_id = -1;
    float bottom = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &view_id, &bottom);

    CCigiView *view = g_DataManager.GetView(view_id);
    if (!view) {
        CString errmsg;
        errmsg.Format("Error: Invalid view specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEW_DEF_DATA data = view->GetAndLockViewDef();
    data.fov_bottom = bottom;
    data.fov_bottom_enable = 1;
    view->SetLockedViewDef(data);
    view->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    view->SetInitialFovBottom(bottom);

    return 0;
}

long ScriptDoViewgroupPitch(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float pitch = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &pitch);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the view properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.pitch = pitch;
    data.pitch_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialPitch(pitch);

    return 0;
}

long ScriptDoViewgroupRoll(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &roll);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.roll = roll;
    data.roll_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialRoll(roll);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupX(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float x = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &x);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.offset_x = x;
    data.offset_x_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialXOffset(x);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupXYZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &group_id, &x, &y, &z);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.offset_x = x;
    data.offset_x_enable = 1;
    data.offset_y = y;
    data.offset_y_enable = 1;
    data.offset_z = z;
    data.offset_z_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial values so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialXOffset(x);
    group->SetInitialYOffset(y);
    group->SetInitialZOffset(z);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupY(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float y = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &y);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.offset_y = y;
    data.offset_y_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialYOffset(y);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupYaw(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float yaw = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &yaw);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.yaw = yaw;
    data.yaw_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialYaw(yaw);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupYPR(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float yaw = 0.0;
    float pitch = 0.0;
    float roll = 0.0;

    sscanf(buffer, "%s %lu %f %f %f", keyword, &group_id, &yaw, &pitch, &roll);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.yaw = yaw;
    data.yaw_enable = 1;
    data.pitch = pitch;
    data.pitch_enable = 1;
    data.roll = roll;
    data.roll_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialYaw(yaw);
    group->SetInitialPitch(pitch);
    group->SetInitialRoll(roll);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}

long ScriptDoViewgroupZ(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    unsigned long group_id = -1;
    float z = 0.0;

    sscanf(buffer, "%s %lu %f", keyword, &group_id, &z);

    CViewGroup *group = g_DataManager.GetViewGroup((unsigned char)group_id);
    if (!group) {
        CString errmsg;
        errmsg.Format("Error: Invalid view group specified. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Set the group properties and initial state.
    VIEWGROUP_CIGI_DATA data = group->GetAndLockCigi();
    data.offset_z = z;
    data.offset_z_enable = 1;
    group->SetLockedCigi(data);
    group->Unlock();

    // Set the initial value so the user doesn't have to re-run the entire
    // script to return to this state if he wants to go into free-flight.
    group->SetInitialZOffset(z);

    MESSAGE_UPDATE_VIEWS_VIEW msg;
    PostGUIMsg(msg);

    return 0;
}
