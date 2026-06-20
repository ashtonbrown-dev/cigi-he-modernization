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

#pragma once

// CBV_EDIT

class CBV_EDIT : public CEdit
{
    DECLARE_DYNAMIC(CBV_EDIT)

public:
    CBV_EDIT();
    virtual ~CBV_EDIT();
    COLORREF GetLabelColor(void);
    void SetLabelColor(const COLORREF color);
    CString GetLabelText(void);
    void SetLabelText(LPCTSTR str);
    void Initialize(void);
    void AllowDecimal(const BOOL allow = TRUE);
    BOOL AllowsDecimal(void);
    void AllowMinus(const BOOL allow = TRUE);
    BOOL AllowsMinus(void);

protected:
    BOOL m_AllowDecimal;
    BOOL m_AllowMinus;
    COLORREF m_LabelColor;
    CString m_LabelText;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnPaint();
};

inline COLORREF CBV_EDIT::GetLabelColor(void)
{
    return m_LabelColor;
}

inline void CBV_EDIT::SetLabelColor(const COLORREF color)
{
    m_LabelColor = color;
}

inline CString CBV_EDIT::GetLabelText(void)
{
    return m_LabelText;
}

inline void CBV_EDIT::SetLabelText(LPCTSTR str)
{
    m_LabelText = str;
}

inline void CBV_EDIT::AllowDecimal(const BOOL allow)
{
    m_AllowDecimal = allow;
}

inline BOOL CBV_EDIT::AllowsDecimal(void)
{
    return m_AllowDecimal;
}

inline void CBV_EDIT::AllowMinus(const BOOL allow)
{
    m_AllowMinus = allow;
}

inline BOOL CBV_EDIT::AllowsMinus(void)
{
    return m_AllowMinus;
}

// CComboBoxVariant

#define CBV_EDITCHANGE      (WM_APP + 8)
#define CBV_SELCHANGE       (WM_APP + 9)

class CComboBoxVariant : public CComboBox
{
    DECLARE_DYNAMIC(CComboBoxVariant)

public:
    static const int DataTypeInt32;
    static const int DataTypeUInt32;
    static const int DataTypeInt64;
    static const int DataTypeUInt64;
    static const int DataTypeFloat;
    static const int DataTypeDouble;

    CComboBoxVariant();
    virtual ~CComboBoxVariant();
    void SetLabelColor(const int type, const COLORREF crColor);
    COLORREF GetLabelColor(const int type);
    LPCTSTR GetLabelText(void);
    void SetValueInt32(const __int32 value);
    void SetValueUInt32(const unsigned __int32 value);
    void SetValueInt64(const __int64 value);
    void SetValueUInt64(const unsigned __int64 value);
    void SetValueFloat(const float value);
    void SetValueDouble(const double value);
    __int32 GetValueInt32(void);
    unsigned __int32 GetValueUInt32(void);
    __int64 GetValueInt64(void);
    unsigned __int64 GetValueUInt64(void);
    float GetValueFloat(void);
    double GetValueDouble(void);
    int GetDataType(void);
    unsigned int GetTextMargin();

protected:
    static const char m_Label[6][18];
    struct _VARIANT {
        int DataType;
        struct {
            __int64 idata;      // preserve the sign
            double  fdata;
        } Data;
    };

    _VARIANT m_Data;
    BOOL m_CalledCreate;
    int m_LabelWidth;
    UINT m_MarginLeft;
    UINT m_MarginRight;
    COLORREF m_LabelClr[6];
    CBV_EDIT m_Edit;

    void Initialize(void);
    CString CreateItemText(const int id);
    void UpdateControl(const int datatype);

    DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow();

private:
    template <class ParamType, class IDataType>
    void SetValue(int dataType,
                  const ParamType value,
                  BOOL allowDecimal,
                  BOOL allowMinus) {
        m_Data.DataType = dataType;
        m_Data.Data.idata = (IDataType)value;
        m_Data.Data.fdata = (double)value;
        m_Edit.AllowDecimal(allowDecimal);
        m_Edit.AllowMinus(allowMinus);

        // Save the selection and set the window text.
        int start = 0, end = 0;
        m_Edit.GetSel(start, end);
        SetWindowText(CreateItemText(dataType));
        m_Edit.SetSel(start, end);

        UpdateControl(dataType);
    }

    virtual BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnCbnEditchange();
    afx_msg void OnCbnSelchange();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
};

inline void CComboBoxVariant::SetLabelColor(const int type, const COLORREF crColor)
{
    if ((unsigned int)type < 6)
        m_LabelClr[type] = crColor;
}

inline COLORREF CComboBoxVariant::GetLabelColor(const int type)
{
    if ((unsigned int)type < 6)
        return m_LabelClr[type];

    return RGB(0, 0, 0);
}

inline __int32 CComboBoxVariant::GetValueInt32(void)
{
    return (__int32)m_Data.Data.idata;
}

inline unsigned __int32 CComboBoxVariant::GetValueUInt32(void)
{
    return (unsigned __int32)m_Data.Data.idata;
}

inline __int64 CComboBoxVariant::GetValueInt64(void)
{
    return m_Data.Data.idata;
}

inline unsigned __int64 CComboBoxVariant::GetValueUInt64(void)
{
    return (unsigned __int64)m_Data.Data.idata;
}

inline float CComboBoxVariant::GetValueFloat(void)
{
    return (float)m_Data.Data.fdata;
}

inline double CComboBoxVariant::GetValueDouble(void)
{
    return m_Data.Data.fdata;
}

inline int CComboBoxVariant::GetDataType(void)
{
    return m_Data.DataType;
}

inline LPCTSTR CComboBoxVariant::GetLabelText(void)
{
    return (LPCTSTR)m_Label[m_Data.DataType];
}

inline unsigned int CComboBoxVariant::GetTextMargin()
{
    return m_MarginLeft;
}
