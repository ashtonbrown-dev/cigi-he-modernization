#if !defined(AFX_FILTERDIALOG_H__8C8867B6_D934_4DBB_8228_5D21E83E9EF5__INCLUDED_)
#define AFX_FILTERDIALOG_H__8C8867B6_D934_4DBB_8228_5D21E83E9EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterDialog dialog

class CFilterDialog : public CDialog
{
    // Construction
public:
    CFilterDialog(CWnd *pParent = NULL);

    // Dialog Data
    //{{AFX_DATA(CFilterDialog)
    enum { IDD = IDD_DIALOG_FILTER };
    BOOL    m_PrintArtPartControl;
    BOOL    m_PrintAtmosphereControl;
    BOOL    m_PrintCCEntityControl;
    BOOL    m_PrintCelestialSphereControl;
    BOOL    m_PrintCDVolumeDef;
    BOOL    m_PrintCDSegmentDef;
    BOOL    m_PrintComponentControl;
    BOOL    m_PrintEntityControl;
    BOOL    m_PrintEnvConditionsReq;
    BOOL    m_PrintEnvRegionControl;
    BOOL    m_PrintErmDef;
    BOOL    m_PrintHatReq;
    BOOL    m_PrintIGControl;
    BOOL    m_PrintLosSegmentReq;
    BOOL    m_PrintLosVectorReq;
    BOOL    m_PrintMaritimeCondControl;
    BOOL    m_PrintMotionTrackerControl;
    BOOL    m_PrintPositionReq;
    BOOL    m_PrintRateControl;
    BOOL    m_PrintSensorControl;
    BOOL    m_PrintShortArtPartControl;
    BOOL    m_PrintShortComponentControl;
    BOOL    m_PrintTerrestrialCondControl;
    BOOL    m_PrintTrajectoryDef;
    BOOL    m_PrintViewControl;
    BOOL    m_PrintViewDef;
    BOOL    m_PrintWaveControl;
    BOOL    m_PrintWeatherControl;
    BOOL    m_PrintFrameStart;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFilterDialog)
protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
    // Generated message map functions
    //{{AFX_MSG(CFilterDialog)
    afx_msg void OnButtonClear();
    afx_msg void OnCheck(UINT ctrlId);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDIALOG_H__8C8867B6_D934_4DBB_8228_5D21E83E9EF5__INCLUDED_)
