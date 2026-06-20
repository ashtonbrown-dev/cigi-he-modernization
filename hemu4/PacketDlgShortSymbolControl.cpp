// PacketDlgShortSymbolControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgShortSymbolControl.h"

// CPacketDlgShortSymbolControl dialog

IMPLEMENT_DYNAMIC(CPacketDlgShortSymbolControl, CDialog)
CPacketDlgShortSymbolControl::CPacketDlgShortSymbolControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgShortSymbolControl::IDD, pParent)
    , m_SymbolID(0)
    , m_SymbolState(0)
    , m_AttachState(0)
    , m_ResetFlash(0)
    , m_InheritColor(0)
{
}

CPacketDlgShortSymbolControl::~CPacketDlgShortSymbolControl()
{
}

void CPacketDlgShortSymbolControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_ComboType1);
    DDX_Control(pDX, IDC_COMBO2, m_ComboType2);
    DDX_Control(pDX, IDC_EDIT_DATUM1, m_EditDatum1);
    DDX_Control(pDX, IDC_EDIT_DATUM2, m_EditDatum2);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Check(pDX, IDC_CHECK_ATTACH, m_AttachState);
    DDX_Check(pDX, IDC_CHECK_RESET_FLASH, m_ResetFlash);
    DDX_Check(pDX, IDC_CHECK_INHERIT_COLOR, m_InheritColor);
    DDX_Radio(pDX, IDC_RADIO1, m_SymbolState);
}

BEGIN_MESSAGE_MAP(CPacketDlgShortSymbolControl, CDialog)
END_MESSAGE_MAP()

// CPacketDlgShortSymbolControl message handlers

BOOL CPacketDlgShortSymbolControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_ComboType1.SetCurSel(0);
    m_ComboType2.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgShortSymbolControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_CONTROL_S sc = {0};
    sc.packet_id = CIGI_SYMBOL_CONTROL_S_OPCODE;
    sc.packet_size = CIGI_SYMBOL_CONTROL_S_SIZE;
    sc.symbol_id = m_SymbolID;
    sc.symbol_state = m_SymbolState;
    sc.attach_state = m_AttachState;
    sc.reset_flash = m_ResetFlash;
    sc.inherit_color = m_InheritColor;
    sc.attribute1 = m_ComboType1.GetCurSel();
    sc.attribute2 = m_ComboType2.GetCurSel();
    sc.attribute1_value = GetValueAsWord(m_EditDatum1, sc.attribute1);
    sc.attribute2_value = GetValueAsWord(m_EditDatum2, sc.attribute2);

    SendImmedCigiMessage(&sc);
}

void CPacketDlgShortSymbolControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

unsigned int CPacketDlgShortSymbolControl::GetValueAsWord(const CEdit &edit, unsigned int type)
{
    unsigned int retval = 0;
    float tempFloat = 0.0f;
    CString text;
    edit.GetWindowText(text);

    switch (type) {
    case 0:     // none
        retval = 0;
        break;

        // unsigned char
    case 3:     // layer
    case 4:     // flash duty cycle
        sscanf((LPCTSTR)text, "%u", &retval);
        retval = (unsigned char)retval;
        break;

        // unsigned short
    case 1:     // surface ID
    case 2:     // parent ID
        sscanf((LPCTSTR)text, "%u", &retval);
        retval = (unsigned short)retval;
        break;

        // unsigned long (hex)
    case 9:     // color
        sscanf((LPCTSTR)text, "%x", &retval);
        break;

        // float
    case 5:     // flash period
    case 6:     // position u
    case 7:     // position v
    case 8:     // rotation
    case 10:    // scale u
    case 11:    // scale v
        sscanf((LPCTSTR)text, "%f", &tempFloat);
        retval = *((unsigned int *)(&tempFloat));
        break;

    default:
        break;
    }

    return retval;
}
