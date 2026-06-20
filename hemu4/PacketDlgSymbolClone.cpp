// PacketDlgSymbolClone.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolClone.h"

// CPacketDlgSymbolClone dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolClone, CDialog)
CPacketDlgSymbolClone::CPacketDlgSymbolClone(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolClone::IDD, pParent)
    , m_SymbolID(0)
    , m_SourceID(0)
    , m_SourceType(0)
{
}

CPacketDlgSymbolClone::~CPacketDlgSymbolClone()
{
}

void CPacketDlgSymbolClone::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT2, m_SourceID);
    DDX_Radio(pDX, IDC_RADIO1, m_SourceType);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolClone, CDialog)
END_MESSAGE_MAP()

// CPacketDlgSymbolClone message handlers

void CPacketDlgSymbolClone::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_CLONE sc = {0};
    sc.packet_id = CIGI_SYMBOL_CLONE_OPCODE;
    sc.packet_size = CIGI_SYMBOL_CLONE_SIZE;
    sc.symbol_id = m_SymbolID;
    sc.source_id = m_SourceID;
    sc.source_type = m_SourceType;

    SendImmedCigiMessage(&sc);
}

void CPacketDlgSymbolClone::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}
