#pragma once

#include "resource.h"

// CPacketDlgSymbolSurfaceDef dialog

class CPacketDlgSymbolSurfaceDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolSurfaceDef)

public:
    CPacketDlgSymbolSurfaceDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolSurfaceDef();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_SURFACE_DEF };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnClickViewAttached();
    afx_msg void OnClickEntityAttached();
    afx_msg void OnClickBillboard();

private:
    int m_SurfaceID;
    int m_EntityViewID;
    int m_SurfaceState;
    int m_AttachSelect;
    int m_Billboard;
    int m_PerspectiveGrowth;
    float m_X_Left;
    float m_Y_Right;
    float m_Z_Top;
    float m_Yaw_Bottom;
    float m_Pitch;
    float m_Roll;
    float m_Width;
    float m_Height;
    float m_MinU;
    float m_MaxU;
    float m_MinV;
    float m_MaxV;
    CEdit m_EditYaw;
    CEdit m_EditPitch;
    CEdit m_EditRoll;
    CEdit m_EditWidth;
    CEdit m_EditHeight;
    CStatic m_LabelXLeft;
    CStatic m_LabelYRight;
    CStatic m_LabelZTop;
    CStatic m_LabelYawBottom;
    CStatic m_LabelOrientation;
    CButton m_CheckPerspectiveGrowth;
    CStatic m_LabelPitch;
    CStatic m_LabelRoll;
    CStatic m_LabelWidth;
    CStatic m_LabelHeight;
public:
    virtual BOOL OnInitDialog();
};
