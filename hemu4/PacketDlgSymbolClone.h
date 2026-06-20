#pragma once

#include "resource.h"

// CPacketDlgSymbolClone dialog

class CPacketDlgSymbolClone : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolClone)

public:
    CPacketDlgSymbolClone(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolClone();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_CLONE };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
private:
    int m_SymbolID;
    int m_SourceID;
    int m_SourceType;
protected:
    virtual void OnOK();
    virtual void OnCancel();
};
