#pragma once

#include "resource.h"

// CPacketDlgSymbolControl dialog

class CPacketDlgSymbolControl : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolControl)

public:
    CPacketDlgSymbolControl(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolControl();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_CONTROL };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    afx_msg void OnClickAttach();
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

private:
    unsigned int m_SymbolID;
    unsigned int m_SurfaceID;
    unsigned char m_Layer;
    int m_AttachState;
    unsigned int m_ParentID;
    int m_SymbolState;
    float m_FlashPeriod;
    unsigned char m_FlashDutyCycle;
    int m_ResetFlash;
    unsigned char m_Red;
    unsigned char m_Green;
    unsigned char m_Blue;
    unsigned char m_Alpha;
    int m_InheritColor;
    float m_PositionU;
    float m_PositionV;
    float m_Rotation;
    float m_ScaleU;
    float m_ScaleV;
    CEdit m_EditParentSymbolID;
};
