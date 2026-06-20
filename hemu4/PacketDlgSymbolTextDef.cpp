// PacketDlgSymbolTextDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolTextDef.h"

// CPacketDlgSymbolTextDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolTextDef, CDialog)
CPacketDlgSymbolTextDef::CPacketDlgSymbolTextDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolTextDef::IDD, pParent)
    , m_PacketSize(16)
    , m_SymbolID(0)
    , m_FontID(0)
    , m_FontSize(0.0)
    , m_Alignment(0)
    , m_Orientation(0)
{
}

CPacketDlgSymbolTextDef::~CPacketDlgSymbolTextDef()
{
}

void CPacketDlgSymbolTextDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_STRING, m_EditString);
    DDX_Text(pDX, IDC_PACKET_SIZE, m_PacketSize);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_FONT_ID, m_FontID);
    DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_FontSize);
    DDX_Radio(pDX, IDC_RADIO_ALIGN0, m_Alignment);
    DDX_Radio(pDX, IDC_RADIO_ORIENT0, m_Orientation);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolTextDef, CDialog)
    ON_EN_CHANGE(IDC_EDIT_STRING, OnEditString)
END_MESSAGE_MAP()

// CPacketDlgSymbolTextDef message handlers

void CPacketDlgSymbolTextDef::OnEditString()
{
    UpdateData(TRUE);

    // Calculate the packet size.
    m_EditString.GetWindowText(m_String);
    m_PacketSize = CIGI_SYMBOL_TEXT_DEF_BASE_SIZE + (m_String.GetLength() + 8 );
    m_PacketSize -= m_PacketSize % 8;

    ASSERT((m_PacketSize <= CIGI_SYMBOL_TEXT_DEF_SIZE) && (m_PacketSize % 8 == 0));

    UpdateData(FALSE);
}

void CPacketDlgSymbolTextDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_TEXT_DEF sd = {0};
    sd.packet_id = CIGI_SYMBOL_TEXT_DEF_OPCODE;
    sd.packet_size = m_PacketSize;
    sd.symbol_id = m_SymbolID;
    sd.alignment = m_Alignment;
    sd.orientation = m_Orientation;
    sd.font_id = m_FontID;
    sd.font_size = m_FontSize;

    memset(sd.text, 0, CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE);
    m_EditString.GetWindowText( sd.text, m_EditString.LineLength( 0 ) + 1 );

    SendImmedCigiMessage(&sd);
}

void CPacketDlgSymbolTextDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolTextDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Leave a byte for NULL
    m_EditString.SetLimitText(CIGI_SYMBOL_TEXT_MAX_TEXT_SIZE - 1);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}
