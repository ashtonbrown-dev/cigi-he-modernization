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

extern void ScriptPostTextMessage(const char *text);

// Symbol Surface Definition

bool in_symbol_surface = false;

CIGI_SYMBOL_SURFACE_DEF SymbolSurfaceDef_begin = {
    CIGI_SYMBOL_SURFACE_DEF_OPCODE,
    CIGI_SYMBOL_SURFACE_DEF_SIZE,
    0, 0, 0, 0, 0, 0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

CIGI_SYMBOL_SURFACE_DEF SymbolSurfaceDef;

long ScriptDoSymbolSurfaceDefBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolSurfaceDefEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_surface = true;
    SymbolSurfaceDef = SymbolSurfaceDef_begin;

    return 0;
}

long ScriptDoSymbolSurfaceDefEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Surface Definition packet.
    SendImmedCigiMessage(&SymbolSurfaceDef);

    in_symbol_surface = false;

    return 0;
}

long ScriptDoSurfaceID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Surface_id = 0;

    sscanf(buffer, "%s %d", keyword, &Surface_id);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.surface_id = Surface_id;

    return 0;
}

long ScriptDoSurfaceState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char state[32] = "";

    sscanf(buffer, "%s %s", keyword, &state);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(state, "ACTIVE") == 0) {
        SymbolSurfaceDef.surface_state = 0;
    } else if (strcmp(state, "DISTROYED") == 0) {
        SymbolSurfaceDef.surface_state = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Surface State. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSurfaceAttachType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char type[32] = "";

    sscanf(buffer, "%s %s", keyword, &type);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(type, "ENTITY") == 0) {
        SymbolSurfaceDef.attach_type = 0;
    } else if (strcmp(type, "VIEW") == 0) {
        SymbolSurfaceDef.attach_type = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Attach Type. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSurfaceIsNotBillboard(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.billboard = 0;

    return 0;
}

long ScriptDoSurfaceIsBillboard(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.billboard = 1;

    return 0;
}

long ScriptDoSurfacePerspectiveGrowth(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char stat[32] = "";

    sscanf(buffer, "%s %s", keyword, &stat);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(stat, "DISABLE") == 0) {
        SymbolSurfaceDef.persp_growth_enable = 0;
    } else if (strcmp(stat, "ENABLE") == 0) {
        SymbolSurfaceDef.persp_growth_enable = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Perspective Growth. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSurfaceEntityID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Entity_id = 0;

    sscanf(buffer, "%s %d", keyword, &Entity_id);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.entity_or_view_id = Entity_id;
    SymbolSurfaceDef.attach_type = 0;

    return 0;
}

long ScriptDoSurfaceViewID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int View_id = 0;

    sscanf(buffer, "%s %d", keyword, &View_id);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolSurfaceDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.entity_or_view_id = View_id;
    SymbolSurfaceDef.attach_type = 1;

    return 0;
}

long ScriptDoSurfaceOffsetPosition(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    sscanf(buffer, "%s %f %f %f", keyword, &x, &y, &z);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoHatHotReqBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.x_left = x;
    SymbolSurfaceDef.y_right = y;
    SymbolSurfaceDef.z_top = z;

    return 0;
}

long ScriptDoSurfaceOffsetAttitude(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float yaw = 0.0;
    float pitch = 0.0;
    float roll = 0.0;

    sscanf(buffer, "%s %f %f %f", keyword, &yaw, &pitch, &roll);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoHatHotReqBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if ((yaw < 0.0f) || (yaw > 360.0)) {
        CString errmsg;
        errmsg.Format("Error: Yaw is not valid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if ((pitch < -90.0f) || (pitch > 90.0f)) {
        CString errmsg;
        errmsg.Format("Error: Pitch is not valid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if ((roll < -180.0f) || (roll > 180.0f)) {
        CString errmsg;
        errmsg.Format("Error: Roll is not valid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else {
        SymbolSurfaceDef.yaw_bottom = yaw;
        SymbolSurfaceDef.pitch = pitch;
        SymbolSurfaceDef.roll = roll;
    }

    return 0;
}

long ScriptDoSurfaceSize(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float width = 0.0;
    float height = 0.0;

    sscanf(buffer, "%s %f %f", keyword, &width, &height);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoHatHotReqBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolSurfaceDef.width = width;
    SymbolSurfaceDef.height = height;

    return 0;
}

long ScriptDoSurfaceExtents(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float left = 0.0;
    float right = 0.0;
    float bottom = 0.0;
    float top = 0.0;

    sscanf(buffer, "%s %f %f %f %f", keyword, &left, &right, &bottom, &top);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoHatHotReqBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if ((left < 0.0f) || (left > 1.0f)) {
        CString errmsg;
        errmsg.Format("Error: Left is invalid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if ((right < 0.0f) || (right > 1.0f)) {
        CString errmsg;
        errmsg.Format("Error: Right is invalid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if ((bottom < 0.0f) || (bottom > 1.0f)) {
        CString errmsg;
        errmsg.Format("Error: Bottom is invalid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if ((top < 0.0f) || (top > 1.0f)) {
        CString errmsg;
        errmsg.Format("Error: Top is invalid. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if (left >= right) {
        CString errmsg;
        errmsg.Format("Error: Right is not greater than left. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if (top <= bottom) {
        CString errmsg;
        errmsg.Format("Error: Top is not greater than bottom. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else {
        SymbolSurfaceDef.x_left = left;
        SymbolSurfaceDef.y_right = right;
        SymbolSurfaceDef.z_top = top;
        SymbolSurfaceDef.yaw_bottom = bottom;
    }

    return 0;
}

long ScriptDoSurfaceUVBoundaries(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float min_u = 0.0;
    float max_u = 0.0;
    float min_v = 0.0;
    float max_v = 0.0;

    sscanf(buffer, "%s %f %f %f %f", keyword, &min_u, &max_u, &min_v, &max_v);
    if (!in_symbol_surface) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoHatHotReqBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (min_u >= max_u) {
        CString errmsg;
        errmsg.Format("Error: Maximum U is not greater than minimum U. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else if (min_v >= max_v) {
        CString errmsg;
        errmsg.Format("Error: Maximum V is not greater than minimum V. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    } else {
        SymbolSurfaceDef.min_u = min_u;
        SymbolSurfaceDef.max_u = max_u;
        SymbolSurfaceDef.min_v = min_v;
        SymbolSurfaceDef.max_v = max_v;
    }

    return 0;
}

// Text Definition

bool in_symbol_text_def = false;

CIGI_SYMBOL_TEXT_DEF SymbolTextDef_begin = {
    CIGI_SYMBOL_TEXT_DEF_OPCODE,
    16, 0, 0, 0, 0, 0.0f
};

CIGI_SYMBOL_TEXT_DEF SymbolTextDef;

long ScriptDoSymbolTextDefBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolTextDefEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_text_def = true;
    SymbolTextDef = SymbolTextDef_begin;
    memset(SymbolTextDef.text, 0, CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE);

    return 0;
}

long ScriptDoSymbolTextDefEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Text Definition packet.
    SendImmedCigiMessage(&SymbolTextDef);

    in_symbol_text_def = false;

    return 0;
}

long ScriptDoSymbolTextDefSymbolID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Symbol_id = 0;

    sscanf(buffer, "%s %d", keyword, &Symbol_id);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolTextDef.symbol_id = Symbol_id;

    return 0;
}

long ScriptDoSymbolTextDefAlignment(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char alignment[32] = "";

    sscanf(buffer, "%s %s", keyword, alignment);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(alignment, "TOPLEFT") == 0) {
        SymbolTextDef.alignment = 0;
    } else if (strcmp(alignment, "TOPCENTER") == 0) {
        SymbolTextDef.alignment = 1;
    } else if (strcmp(alignment, "TOPRIGHT") == 0) {
        SymbolTextDef.alignment = 2;
    } else if (strcmp(alignment, "CENTERLEFT") == 0) {
        SymbolTextDef.alignment = 3;
    } else if (strcmp(alignment, "CENTER") == 0) {
        SymbolTextDef.alignment = 4;
    } else if (strcmp(alignment, "CENTERRIGHT") == 0) {
        SymbolTextDef.alignment = 5;
    } else if (strcmp(alignment, "BOTTOMLEFT") == 0) {
        SymbolTextDef.alignment = 6;
    } else if (strcmp(alignment, "BOTTOMCENTER") == 0) {
        SymbolTextDef.alignment = 7;
    } else if (strcmp(alignment, "BOTTOMRIGHT") == 0) {
        SymbolTextDef.alignment = 8;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Text Alignment. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolTextDefOrientation(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char orientation[32] = "";

    sscanf(buffer, "%s %s", keyword, orientation);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(orientation, "LEFTTORIGHT") == 0) {
        SymbolTextDef.orientation = 0;
    } else if (strcmp(orientation, "TOPTOBOTTOM") == 0) {
        SymbolTextDef.orientation = 1;
    } else if (strcmp(orientation, "RIGHTTOLEFT") == 0) {
        SymbolTextDef.orientation = 2;
    } else if (strcmp(orientation, "BOTTOMTOTOP") == 0) {
        SymbolTextDef.orientation = 3;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Text Orientation. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolTextDefFontID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Font_id = 0;

    sscanf(buffer, "%s %d", keyword, &Font_id);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolTextDef.font_id = Font_id;

    return 0;
}

long ScriptDoSymbolTextDefFontSize(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float FontSz = 0.0;

    sscanf(buffer, "%s %f", keyword, &FontSz);
    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolTextDef.font_size = FontSz;

    return 0;
}

long ScriptDoSymbolTextDefText(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    memset(SymbolTextDef.text, 0, CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE);

    if (!in_symbol_text_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolTextDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    int ndx;
    for (ndx = 0; ((buffer[ndx] != ' ') && (ndx < 63)); ndx++);

    if (ndx >= 63) {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Text Keyword. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    memcpy(keyword, buffer, ndx);
    keyword[ndx] = 0;

    strncpy(SymbolTextDef.text, &buffer[ndx + 1], CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE);
    SymbolTextDef.text[(CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE - 1)] = 0;

    int len = strlen(SymbolTextDef.text);
    int size = len + 12;
    int orphan = size % 8;
    int pad = 8 - orphan;

    SymbolTextDef.packet_size = len + pad;

    return 0;
}

// Circle Definition

bool in_symbol_circle_def = false;

CIGI_SYMBOL_CIRCLE_DEF SymbolCircleDef_begin = {
    CIGI_SYMBOL_CIRCLE_DEF_BASE_SIZE,
    CIGI_SYMBOL_CIRCLE_DEF_OPCODE,
    0, 0, 0xff, 0xffff, 0.0f, 0.0f
};

CIGI_SYMBOL_CIRCLE_DEF SymbolCircleDef;

long ScriptDoSymbolCircleDefBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolCircleDefEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_circle_def = true;
    SymbolCircleDef = SymbolCircleDef_begin;
    memset( &SymbolCircleDef.circle, 0, (CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT * CIGI_SYMBOL_CIRCLE_ELEMENT_SIZE));

    return 0;
}

long ScriptDoSymbolCircleDefEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Circle Definition packet.
    SendImmedCigiMessage(&SymbolCircleDef);

    in_symbol_circle_def = false;

    return 0;
}

long ScriptDoSymbolCircleDefSymbolID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Symbol_id = 0;

    sscanf(buffer, "%s %d", keyword, &Symbol_id);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleDef.symbol_id = Symbol_id;

    return 0;
}

long ScriptDoSymbolCircleDefDrawingStyle(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char DrawStyle[32] = "";

    sscanf(buffer, "%s %s", keyword, &DrawStyle);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(DrawStyle, "LINE") == 0) {
        SymbolCircleDef.drawing_style = 0;
    } else if (strcmp(DrawStyle, "FILL") == 0) {
        SymbolCircleDef.drawing_style = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Circle Drawing Style. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolCircleDefStipple(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Stipple = 0;

    sscanf(buffer, "%s %d", keyword, &Stipple);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleDef.stipple_pattern = (unsigned short)Stipple;

    return 0;
}

long ScriptDoSymbolCircleDefLineWidth(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float LineWidth = 0;

    sscanf(buffer, "%s %f", keyword, &LineWidth);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleDef.line_width = LineWidth;

    return 0;
}

long ScriptDoSymbolCircleDefStippleLength(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float StippleLength = 0;

    sscanf(buffer, "%s %f", keyword, &StippleLength);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleDef.stipple_pattern_length = StippleLength;

    return 0;
}

// Circle Definition Element

bool in_symbol_circle_elem = false;

CIGI_CIRCLE_DEF_ELEMENT SymbolCircle_begin =
{   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

CIGI_CIRCLE_DEF_ELEMENT SymbolCircleElem;

long ScriptDoSymbolAddCircleBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_circle_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCircleDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolAddCircleEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_circle_elem = true;
    SymbolCircleElem = SymbolCircle_begin;

    return 0;
}

long ScriptDoSymbolAddCircleEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Add the circle
    int Length = SymbolCircleDef.packet_size;
    int CircleCnt = (Length - CIGI_SYMBOL_CIRCLE_DEF_BASE_SIZE) /
                    CIGI_SYMBOL_CIRCLE_ELEMENT_SIZE;
    if (CircleCnt < CIGI_SYMBOL_CIRCLE_DEF_MAX_ELEMENT_CNT) {
        SymbolCircleDef.circle[CircleCnt] = SymbolCircleElem;
        SymbolCircleDef.packet_size += CIGI_SYMBOL_CIRCLE_ELEMENT_SIZE;
    }

    in_symbol_circle_elem = false;

    return 0;
}

long ScriptDoSymbolCircleCenter(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float U = 0;
    float V = 0;

    sscanf(buffer, "%s %f %f", keyword, &U, &V);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleElem.center_u = U;
    SymbolCircleElem.center_v = V;

    return 0;
}

long ScriptDoSymbolCircleRadius(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float Radius = 0;

    sscanf(buffer, "%s %f", keyword, &Radius);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleElem.radius = Radius;

    return 0;
}

long ScriptDoSymbolCircleInnerRadius(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float InnerRadius = 0;

    sscanf(buffer, "%s %f", keyword, &InnerRadius);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleElem.inner_radius = InnerRadius;

    return 0;
}

long ScriptDoSymbolCircleStartAngle(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float StartAngle = 0;

    sscanf(buffer, "%s %f", keyword, &StartAngle);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleElem.start_angle = StartAngle;

    return 0;
}

long ScriptDoSymbolCircleEndAngle(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float EndAngle = 0;

    sscanf(buffer, "%s %f", keyword, &EndAngle);
    if (!in_symbol_circle_elem) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolAddCircleBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolCircleElem.end_angle = EndAngle;

    return 0;
}

// Polygon Definition

bool in_symbol_polygon_def = false;

CIGI_SYMBOL_POLYGON_DEF SymbolPolygon_begin = {
    CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE,
    CIGI_SYMBOL_POLYGON_DEF_OPCODE,
    0, 0, 0xff,0xffff, 0.0f, 0.0f
};

CIGI_SYMBOL_POLYGON_DEF SymbolPolygonDef;

long ScriptDoSymbolPolygonDefBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolLineDefEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_polygon_def = true;
    SymbolPolygonDef = SymbolPolygon_begin;
    memset(&SymbolPolygonDef.line, 0, (CIGI_SYMBOL_POLYGON_DEF_MAX_VERTEX_CNT * CIGI_SYMBOL_VERTEX_SIZE));

    return 0;
}

long ScriptDoSymbolPolygonDefEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Line Def packet.
    SendImmedCigiMessage(&SymbolPolygonDef);

    in_symbol_polygon_def = false;

    return 0;
}

long ScriptDoSymbolPolygonDefSymbolID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int SymbolID = 0;

    sscanf(buffer, "%s %d", keyword, &SymbolID);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolPolygonDef.symbol_id = (unsigned short)SymbolID;

    return 0;
}

long ScriptDoSymbolPolygonDefPrimitive(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char Primitive[32] = "";

    sscanf(buffer, "%s %s", keyword, &Primitive);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(Primitive, "POINTS") == 0) {
        SymbolPolygonDef.primitive_type = 0;
    } else if (strcmp(Primitive, "LINES") == 0) {
        SymbolPolygonDef.primitive_type = 1;
    } else if (strcmp(Primitive, "LINESTRIP") == 0) {
        SymbolPolygonDef.primitive_type = 2;
    } else if (strcmp(Primitive, "LINELOOP") == 0) {
        SymbolPolygonDef.primitive_type = 3;
    } else if (strcmp(Primitive, "TRIANGLES") == 0) {
        SymbolPolygonDef.primitive_type = 4;
    } else if (strcmp(Primitive, "TRIANGLESTRIP") == 0) {
        SymbolPolygonDef.primitive_type = 5;
    } else if (strcmp(Primitive, "TRIANGLEFAN") == 0) {
        SymbolPolygonDef.primitive_type = 6;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Line Primitive. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolPolygonDefStipple(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Stipple = 0;

    sscanf(buffer, "%s %d", keyword, &Stipple);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolPolygonDef.stipple_pattern = (unsigned short)Stipple;

    return 0;
}

long ScriptDoSymbolPolygonDefLineWidth(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float LineWidth = 0;

    sscanf(buffer, "%s %f", keyword, &LineWidth);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolPolygonDef.line_width = LineWidth;

    return 0;
}

long ScriptDoSymbolPolygonDefStippleLength(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float StippleLength = 0;

    sscanf(buffer, "%s %f", keyword, &StippleLength);
    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolPolygonDef.stipple_pattern_length = StippleLength;

    return 0;
}

// Line Definition element (Vertex)

CIGI_POLYGON_DEF_ELEMENT SymbolVertex_begin =
{   0.0f, 0.0f };

CIGI_POLYGON_DEF_ELEMENT SymbolVertex;

long ScriptDoSymbolVertex(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float U = 0;
    float V = 0;

    sscanf(buffer, "%s %f %f", keyword, &U, &V);

    if (!in_symbol_polygon_def) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolLineDefBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolVertex = SymbolVertex_begin;

    SymbolVertex.vertex_u = U;
    SymbolVertex.vertex_v = V;

    // Add the primitive
    int Length = SymbolPolygonDef.packet_size;
    int VertexCnt = (Length - CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE) /
                    CIGI_SYMBOL_VERTEX_SIZE;
    if (VertexCnt < CIGI_SYMBOL_POLYGON_DEF_MAX_VERTEX_CNT) {
        SymbolPolygonDef.line[VertexCnt] = SymbolVertex;
        SymbolPolygonDef.packet_size += CIGI_SYMBOL_VERTEX_SIZE;
    }

    return 0;

}

// Symbol Clone

bool in_symbol_clone = false;

CIGI_SYMBOL_CLONE SymbolClone_begin = {
    CIGI_SYMBOL_CLONE_OPCODE,
    CIGI_SYMBOL_CLONE_SIZE,
    0, 0, 0
};

CIGI_SYMBOL_CLONE SymbolClone;

long ScriptDoSymbolCloneBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_clone) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolCloneEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_clone = true;
    SymbolClone = SymbolClone_begin;

    return 0;
}

long ScriptDoSymbolCloneEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_clone) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCloneBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Line Def packet.
    SendImmedCigiMessage(&SymbolClone);

    in_symbol_clone = false;

    return 0;
}

long ScriptDoSymbolCloneSymbolID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int SymbolID = 0;

    sscanf(buffer, "%s %d", keyword, &SymbolID);
    if (!in_symbol_clone) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCloneBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolClone.symbol_id = (unsigned short)SymbolID;

    return 0;
}

long ScriptDoSymbolCloneSourceType(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char SymbolType[32] = "";

    sscanf(buffer, "%s %s", keyword, &SymbolType);
    if (!in_symbol_clone) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCloneBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(SymbolType, "SYMBOL") == 0) {
        SymbolClone.source_type = 0;
    } else if (strcmp(SymbolType, "SYMBOLTEMPLATE") == 0) {
        SymbolClone.source_type = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol Clone Source Type. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolCloneSourceID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int SourceID = 0;

    sscanf(buffer, "%s %d", keyword, &SourceID);
    if (!in_symbol_clone) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolCloneBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolClone.source_id = (unsigned short)SourceID;

    return 0;
}

// Symbol Control

bool in_symbol_control = false;

CIGI_SYMBOL_CONTROL SymbolControl_begin = {
    CIGI_SYMBOL_CONTROL_OPCODE,
    CIGI_SYMBOL_CONTROL_SIZE,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.0f, 0.0f, 0.0f, 0.0f,
    0, 0, 0, 0,
    0.0f, 0.0f
};

CIGI_SYMBOL_CONTROL SymbolControl;

long ScriptDoSymbolControlBegin(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: Must call ScriptDoSymbolControlEnd before starting a new symbol surface definition. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    in_symbol_control = true;
    SymbolControl = SymbolControl_begin;

    return 0;
}

long ScriptDoSymbolControlEnd(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    // Send the Symbol Line Def packet.
    SendImmedCigiMessage(&SymbolControl);

    in_symbol_control = false;

    return 0;
}

long ScriptDoSymbolControlSymbolID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int SymbolID = 0;

    sscanf(buffer, "%s %d", keyword, &SymbolID);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.symbol_id = (unsigned short)SymbolID;

    return 0;
}

long ScriptDoSymbolControlSymbolState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char State[32] = "";

    sscanf(buffer, "%s %s", keyword, &State);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(State, "HIDDEN") == 0) {
        SymbolControl.symbol_state = 0;
    } else if (strcmp(State, "VISIBLE") == 0) {
        SymbolControl.symbol_state = 1;
    } else if (strcmp(State, "DESTROYED") == 0) {
        SymbolControl.symbol_state = 2;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Symbol State. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolControlAttachState(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    char State[32] = "";

    sscanf(buffer, "%s %s", keyword, &State);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    if (strcmp(State, "DETACH") == 0) {
        SymbolControl.attach_state = 0;
    } else if (strcmp(State, "ATTACH") == 0) {
        SymbolControl.attach_state = 1;
    } else {
        CString errmsg;
        errmsg.Format("Error: Invalid Attach State. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -2;
    }

    return 0;
}

long ScriptDoSymbolControlResetFlash(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.reset_flash = 1;

    return 0;
}

long ScriptDoSymbolControlInheritColor(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";

    sscanf(buffer, "%s", keyword);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.inherit_color = 1;

    return 0;
}

long ScriptDoSymbolControlParentID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int ParentID = 0;

    sscanf(buffer, "%s %d", keyword, &ParentID);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.parent_id = (unsigned short)ParentID;

    return 0;
}

long ScriptDoSymbolControlSurfaceID(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int SurfaceID = 0;

    sscanf(buffer, "%s %d", keyword, &SurfaceID);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.surface_id = (unsigned short)SurfaceID;

    return 0;
}

long ScriptDoSymbolControlLayer(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Layer = 0;

    sscanf(buffer, "%s %d", keyword, &Layer);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.layer = (unsigned char)Layer;

    return 0;
}

long ScriptDoSymbolControlFlashDutyCycle(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int FlashDutyCycle = 0;

    sscanf(buffer, "%s %d", keyword, &FlashDutyCycle);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.flash_duty_cycle = (unsigned char)FlashDutyCycle;

    return 0;
}

long ScriptDoSymbolControlFlashPeriod(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float FlashPeriod = 0;

    sscanf(buffer, "%s %f", keyword, &FlashPeriod);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.flash_period = FlashPeriod;

    return 0;
}

long ScriptDoSymbolControlPosition(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float PositionU = 0;
    float PositionV = 0;

    sscanf(buffer, "%s %f %f", keyword, &PositionU, &PositionV);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.pos_u = PositionU;
    SymbolControl.pos_v = PositionV;

    return 0;
}

long ScriptDoSymbolControlRotation(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float Rotation = 0;

    sscanf(buffer, "%s %f", keyword, &Rotation);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.rotation = Rotation;

    return 0;
}

long ScriptDoSymbolControlRGBA(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    int Red = 0;
    int Green = 0;
    int Blue = 0;
    int Alpha = 0;

    sscanf(buffer, "%s %d %d %d %d", keyword, &Red, &Green, &Blue, &Alpha);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.red = (unsigned char)Red;
    SymbolControl.green = (unsigned char)Green;
    SymbolControl.blue = (unsigned char)Blue;
    SymbolControl.alpha = (unsigned char)Alpha;

    return 0;
}

long ScriptDoSymbolControlScale(const char *buffer, const unsigned int linenum)
{
    char keyword[64] = "";
    float ScaleU = 0;
    float ScaleV = 0;

    sscanf(buffer, "%s %f %f", keyword, &ScaleU, &ScaleV);
    if (!in_symbol_control) {
        CString errmsg;
        errmsg.Format("Error: ScriptDoSymbolControlBegin was not called. (line %lu)", linenum);
        ScriptPostTextMessage((LPCTSTR)errmsg);
        return -1;
    }

    SymbolControl.scale_u = ScaleU;
    SymbolControl.scale_v = ScaleV;

    return 0;
}
