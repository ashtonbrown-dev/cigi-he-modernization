#pragma once

#include "resource.h"

// CPacketDlgShortSymbolControl dialog

class CPacketDlgShortSymbolControl : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgShortSymbolControl)

public:
    CPacketDlgShortSymbolControl(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgShortSymbolControl();

    // Dialog Data
    enum { IDD = IDD_PACKET_SHORT_SYMBOL_CONTROL };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
    virtual void OnOK();
    virtual void OnCancel();

private:
    unsigned int m_SymbolID;
    int m_SymbolState;
    int m_AttachState;
    int m_ResetFlash;
    int m_InheritColor;
    CComboBox m_ComboType1;
    CComboBox m_ComboType2;
    CEdit m_EditDatum1;
    CEdit m_EditDatum2;

    unsigned int GetValueAsWord(const CEdit &edit, unsigned int type);
public:
    virtual BOOL OnInitDialog();
};
