#pragma once

#include "resource.h"

// CPacketDlgSymbolTextDef dialog

class CPacketDlgSymbolTextDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolTextDef)

public:
    enum { IDD = IDD_PACKET_SYMBOL_TEXT_DEF };

    CPacketDlgSymbolTextDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolTextDef();

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    afx_msg void OnEditString();
    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();

private:
    unsigned int   m_PacketSize;
    unsigned int   m_SymbolID;
    unsigned int   m_FontID;
    int            m_Alignment;
    int            m_Orientation;
    float          m_FontSize;
    CString        m_String;
    CEdit          m_EditString;
};
