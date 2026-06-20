/** <pre>
 *  Copyright 2004 The Boeing Company
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
 *
 *  FILENAME:   ComboBoxVariant.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface Host Emulator (CIGI HE)
 *
 *  PROGRAM DESCRIPTION:
 *  The Host Emulator (HE) acts as a surrogate in the absence of a simulation host.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE
 *
 *  11/11/2004 Lance Durham
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  Version 3.1.1
 */

#include <afxwin.h>         // MFC core and standard components
#include "ComboBoxVariant.h"

const int CComboBoxVariant::DataTypeInt32   = 0;
const int CComboBoxVariant::DataTypeUInt32  = 1;
const int CComboBoxVariant::DataTypeFloat   = 2;
const int CComboBoxVariant::DataTypeInt64   = 3;
const int CComboBoxVariant::DataTypeUInt64  = 4;
const int CComboBoxVariant::DataTypeDouble  = 5;

const char CComboBoxVariant::m_Label[6][18] = { {"Int32 (signed):"},
    {"Int32 (unsigned):"},
    {"Float (single):"},
    {"Int64 (signed):"},
    {"Int64 (unsigned):"},
    {"Float (double):"}
};

// CBV_EDIT

IMPLEMENT_DYNAMIC(CBV_EDIT, CEdit)
CBV_EDIT::CBV_EDIT()
{
    m_AllowDecimal = FALSE;
    m_AllowMinus = TRUE;
}

CBV_EDIT::~CBV_EDIT()
{
}

BEGIN_MESSAGE_MAP(CBV_EDIT, CEdit)
    ON_WM_DRAWITEM()
    ON_WM_CHAR()
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CBV_EDIT message handlers

void CBV_EDIT::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CString Holder = "0123456789";

    if (m_AllowDecimal)
        Holder += ".";

    if (m_AllowMinus)
        Holder += "-";

    // Allow only numeric values.
    if (Holder.Find(nChar) == -1)
        if (nChar != 8) // If not Backspace key, return
            return;

    CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CBV_EDIT::OnPaint()
{
    COLORREF textcolor;
    int BkColorIndex = COLOR_BTNFACE;
    int TextColorIndex = COLOR_GRAYTEXT;

    CPaintDC dc(this); // device context for painting

    // Do not call CEdit::OnPaint() for painting messages

    CRect rect;
    GetClientRect(&rect);

    if (IsWindowEnabled()) {
        BkColorIndex = COLOR_WINDOW;
        TextColorIndex = COLOR_WINDOWTEXT;
    }

    // Erase the window.
    CPen pen(PS_NULL, 0, RGB(255, 255, 255));
    CPen *pOldPen = dc.SelectObject(&pen);
    CBrush *pOldBrush = NULL;
    pOldBrush = dc.SelectObject(CBrush::FromHandle(::GetSysColorBrush(BkColorIndex)));
    dc.Rectangle(&rect);
    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);

    // Draw the datatype label.
    dc.SetBkColor(::GetSysColor(BkColorIndex));
    dc.SelectObject(GetParent()->GetFont());
    textcolor = dc.SetTextColor((IsWindowEnabled()) ? m_LabelColor :
                                ::GetSysColor(COLOR_GRAYTEXT));
    dc.TextOut(rect.left + 3, rect.top, (LPCTSTR)m_LabelText);

    // Finally, draw the window text.
    CString str;
    GetWindowText(str);
    int MarginLeft = LOWORD(GetMargins());
    dc.SetTextColor(::GetSysColor(TextColorIndex));
    dc.TextOut(MarginLeft, rect.top, (LPCTSTR)str);
    dc.SetTextColor(textcolor);
}

// CComboBoxVariant

IMPLEMENT_DYNAMIC(CComboBoxVariant, CComboBox)
CComboBoxVariant::CComboBoxVariant()
{
    m_Data.Data.idata = 0;
    m_Data.Data.fdata = 0.0f;
    m_Data.DataType = DataTypeInt32;
    m_MarginLeft = 103;     // Default is 3, add 100
    m_MarginRight = 3;
    m_LabelWidth = 100;
    m_CalledCreate = FALSE;

    SetLabelColor(DataTypeInt32, RGB(0, 0, 255));
    SetLabelColor(DataTypeUInt32, RGB(0, 191, 255));
    SetLabelColor(DataTypeFloat, RGB(63, 191, 0));
    SetLabelColor(DataTypeInt64, RGB(255, 0, 0));
    SetLabelColor(DataTypeUInt64, RGB(255, 191, 0));
    SetLabelColor(DataTypeDouble, RGB(0, 63, 0));
}

CComboBoxVariant::~CComboBoxVariant()
{
}

BEGIN_MESSAGE_MAP(CComboBoxVariant, CComboBox)
    ON_WM_DRAWITEM()
    ON_WM_KEYDOWN()
    ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnCbnEditchange)
    ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CComboBoxVariant message handlers

void CComboBoxVariant::Initialize(void)
{
    for (int i = 0; i < 6; i++)
        InsertString(i, "");

    SetCurSel(0);
}

void CComboBoxVariant::SetValueInt32(const __int32 value)
{
    SetValue<__int32, __int64>(DataTypeInt32, value, FALSE, TRUE);
}

void CComboBoxVariant::SetValueUInt32(const unsigned __int32 value)
{
    SetValue<unsigned __int32, unsigned __int64>(DataTypeUInt32, value, FALSE, FALSE);
}

void CComboBoxVariant::SetValueInt64(const __int64 value)
{
    SetValue<__int64, __int64>(DataTypeInt64, value, FALSE, TRUE);
}

void CComboBoxVariant::SetValueUInt64(const unsigned __int64 value)
{
    SetValue<unsigned __int64, __int64>(DataTypeUInt64, value, FALSE, FALSE);
}

void CComboBoxVariant::SetValueFloat(const float value)
{
    SetValue<float, __int64>(DataTypeFloat, value, TRUE, TRUE);
}

void CComboBoxVariant::SetValueDouble(const double value)
{
    SetValue<double, __int64>(DataTypeDouble, value, TRUE, TRUE);
}

CString CComboBoxVariant::CreateItemText(const int id)
{
    CString text;

    switch (id) {
    case DataTypeInt32:
        text.Format("%d", GetValueInt32());
        break;

    case DataTypeUInt32:
        text.Format("%u", GetValueUInt32());
        break;

    case DataTypeInt64:
        text.Format("%I64d", GetValueInt64());
        break;

    case DataTypeUInt64:
        text.Format("%I64u", GetValueUInt64());
        break;

    case DataTypeFloat:
        text.Format("%.7f", GetValueFloat());
        text = text.Left(8);    // 7 digits of precision, plus dot
        break;

    case DataTypeDouble:
        text.Format("%.15lf", GetValueDouble());
        text = text.Left(16);   // 15 digits of precision, plus dot
        break;

    default:
        break;
    }

    return text;
}

BOOL CComboBoxVariant::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
    m_CalledCreate = TRUE;
    BOOL retval = CComboBox::Create(dwStyle | CBS_OWNERDRAWFIXED | CBS_DROPDOWN,
                                    rect, pParentWnd, nID);

    if (retval)
        Initialize();

    return retval;
}

void CComboBoxVariant::PreSubclassWindow()
{
    LONG style = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
    style |= CBS_OWNERDRAWFIXED;
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

    CComboBox::PreSubclassWindow();

    if (!m_CalledCreate)
        Initialize();
}

void CComboBoxVariant::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight -= 2;
}

void CComboBoxVariant::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    UINT id = lpDrawItemStruct->itemID;
    UINT action = lpDrawItemStruct->itemAction;
    UINT state = lpDrawItemStruct->itemState;
    CRect rect = lpDrawItemStruct->rcItem;
    COLORREF textcolor;
    COLORREF bkcolor;

    // If the item is selected, highlight it and invert the text color.
    if (id == (UINT)m_Data.DataType) {
        pDC->FillRect(rect, CBrush::FromHandle(::GetSysColorBrush(COLOR_HIGHLIGHT)));
        bkcolor = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
        textcolor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
    } else {
        textcolor = ::GetSysColor(COLOR_WINDOWTEXT);
    }

    // Draw the datatype labels in their respective colors.
    pDC->SetTextColor(m_LabelClr[id]);
    pDC->TextOut(rect.left + 5, rect.top + 1, m_Label[id]);
    pDC->SetTextColor(textcolor);

    // Draw the numerical value.
    CSize size = pDC->GetTextExtent(m_Label[3]);    // This is the longest string.
    rect.left += size.cx + 20;
    pDC->TextOut(rect.left, rect.top + 1, CreateItemText(id));

    // Restore the background color.
    if (id == (UINT)m_Data.DataType) {
        pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
        pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    }
}

int CComboBoxVariant::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
    // Just do a simple string compare.
    LPCTSTR lpszText1 = (LPCTSTR)lpCompareItemStruct->itemData1;
    LPCTSTR lpszText2 = (LPCTSTR)lpCompareItemStruct->itemData2;

    return strcmp(lpszText2, lpszText1);
}

void CComboBoxVariant::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // If not a tab, an arrow, or a numerical key, do nothing.
    Invalidate();

    CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboBoxVariant::OnCbnEditchange()
{
    int selstart = 0, selend = 0;
    m_Edit.GetSel(selstart, selend);

    CString str;
    GetWindowText(str);
    switch (m_Data.DataType) {
    case DataTypeInt32:
        SetValueInt32(atol((LPCTSTR)str));
        break;

    case DataTypeUInt32:
        SetValueUInt32((unsigned __int32)_atoi64((LPCTSTR)str));
        break;

    case DataTypeInt64:
        SetValueInt64(_atoi64((LPCTSTR)str));
        break;

    case DataTypeUInt64:
        SetValueUInt64((unsigned __int64)_atoi64((LPCTSTR)str));
        break;

    case DataTypeFloat:
        SetValueFloat((float)atof((LPCTSTR)str));
        break;

    case DataTypeDouble:
        SetValueDouble(atof((LPCTSTR)str));
        break;

    default:
        break;
    }

    // If the user has deleted all the text, SetValue...() will have put
    // in a zero.  Go ahead and delete the text so the user sees what he/she
    // expects, but the value will actually be zero.
    if (str == "")
        SetWindowText("");

    m_Edit.SetSel(selstart, selend);
    m_Edit.SetLabelText(m_Label[m_Data.DataType]);
    m_Edit.SetLabelColor(m_LabelClr[m_Data.DataType]);
    m_Edit.Invalidate();

    // This causes a stack overflow:
    //     GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_EDITCHANGE), (LPARAM)GetSafeHwnd());
    // Instead, send a custom message.
    GetParent()->SendMessage(CBV_EDITCHANGE, (WPARAM)m_Data.DataType, LPARAM(this));
}

void CComboBoxVariant::OnCbnSelchange()
{
    m_Data.DataType = GetCurSel();

    UpdateControl(m_Data.DataType);

    // This causes a stack overflow:
    //     GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)GetSafeHwnd());
    // Instead, send a custom message.
    GetParent()->SendMessage(CBV_SELCHANGE, (WPARAM)m_Data.DataType,
                             LPARAM(this));

    // TODO: This doesn't work -- find out why.
    //m_Edit.SetSel(0, -1);
}

void CComboBoxVariant::UpdateControl(const int datatype)
{
    m_Edit.SetWindowText(CreateItemText(datatype));
    m_Edit.SetLabelText(m_Label[datatype]);
    m_Edit.SetLabelColor(m_LabelClr[datatype]);

    switch (datatype) {
    case DataTypeInt32:
        m_Edit.AllowDecimal(FALSE);
        m_Edit.AllowMinus(TRUE);
        break;

    case DataTypeUInt32:
        m_Edit.AllowDecimal(FALSE);
        m_Edit.AllowMinus(FALSE);
        break;

    case DataTypeInt64:
        m_Edit.AllowDecimal(FALSE);
        m_Edit.AllowMinus(TRUE);
        break;

    case DataTypeUInt64:
        m_Edit.AllowDecimal(FALSE);
        m_Edit.AllowMinus(FALSE);
        break;

    case DataTypeFloat:
        m_Edit.AllowDecimal(TRUE);
        m_Edit.AllowMinus(TRUE);
        break;

    case DataTypeDouble:
        m_Edit.AllowDecimal(TRUE);
        m_Edit.AllowMinus(TRUE);
        break;

    default:
        break;
    }
}

void CComboBoxVariant::OnDestroy()
{
    CComboBox::OnDestroy();

    if (m_Edit.GetSafeHwnd() != NULL)
        m_Edit.UnsubclassWindow();
}

HBRUSH CComboBoxVariant::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    // Subclass and initialize the edit control.
    if (m_Edit.GetSafeHwnd() == NULL) {
        m_Edit.SubclassWindow(pWnd->GetSafeHwnd());

        // Set the text margins.
        DWORD margin = m_Edit.GetMargins();
        m_MarginLeft = LOWORD(margin) + 100;
        m_MarginRight = HIWORD(margin);
        m_Edit.SetMargins(m_MarginLeft, m_MarginRight);

        // Set the default label text and color.
        m_Edit.SetLabelText(m_Label[m_Data.DataType]);
        m_Edit.SetLabelColor(m_LabelClr[m_Data.DataType]);
        m_Edit.SetWindowText(CreateItemText(m_Data.DataType));
    }

    HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}
