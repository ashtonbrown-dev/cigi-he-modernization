#if !defined(AFX_PACKETDLGSENSORCONTROL_H__C93BB059_0D8F_4431_B996_6BAAC6D50E53__INCLUDED_)
#define AFX_PACKETDLGSENSORCONTROL_H__C93BB059_0D8F_4431_B996_6BAAC6D50E53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketDlgSensorControl.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPacketDlgSensorControl dialog

class CPacketDlgSensorControl : public CDialog
{
    // Construction
public:
    CPacketDlgSensorControl(CWnd *pParent = NULL);   // standard constructor
    void PopulateViewIDCombo(void);

    // Dialog Data
    //{{AFX_DATA(CPacketDlgSensorControl)
    enum { IDD = IDD_PACKET_SENSOR_CONTROL };
    CComboBoxEx m_ComboViewID;
    BOOL    m_AutoGain;
    BOOL    m_Dropout;
    BOOL    m_SensorOn;
    int     m_TrackMode;
    float   m_ACCoupling;
    float   m_Gain;
    float   m_Level;
    float   m_Noise;
    int     m_SensorID;
    int     m_Polarity;
    int     m_RespType;
    int     m_TrackType;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPacketDlgSensorControl)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int GetViewIDFromCombo(void);
    int FindComboInsertPoint(CComboBoxEx &combo, const int id);

    // Generated message map functions
    //{{AFX_MSG(CPacketDlgSensorControl)
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETDLGSENSORCONTROL_H__C93BB059_0D8F_4431_B996_6BAAC6D50E53__INCLUDED_)
