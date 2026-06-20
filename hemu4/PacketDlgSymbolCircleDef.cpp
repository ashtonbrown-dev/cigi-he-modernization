// PacketDlgSymbolCircleDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolCircleDef.h"

// CPacketDlgSymbolCircleDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolCircleDef, CDialog)
CPacketDlgSymbolCircleDef::CPacketDlgSymbolCircleDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolCircleDef::IDD, pParent)
    , m_PacketSize(CIGI_SYMBOL_CIRCLE_DEF_BASE_SIZE)
    , m_SymbolID(0)
    , m_DrawingStyle(0)
    , m_LineWidth(0)
    , m_StipplePatternLength(0)
{
}

CPacketDlgSymbolCircleDef::~CPacketDlgSymbolCircleDef()
{
}

void CPacketDlgSymbolCircleDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_STIPPLE_PATTERN, m_EditStipplePattern);
    DDX_Text(pDX, IDC_PACKET_SIZE, m_PacketSize);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_LineWidth);
    DDX_Text(pDX, IDC_EDIT_STIPPLE_LENGTH, m_StipplePatternLength);
    DDX_Radio(pDX, IDC_RADIO_LINE, m_DrawingStyle);
    DDX_Control(pDX, IDC_BUTTON1, m_ButtonAdd);
    DDX_Control(pDX, IDC_BUTTON2, m_ButtonDelete);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolCircleDef, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnAddCircle)
    ON_BN_CLICKED(IDC_BUTTON2, OnRemoveCircle)
END_MESSAGE_MAP()

// CPacketDlgSymbolCircleDef message handlers

void CPacketDlgSymbolCircleDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_CIRCLE_DEF sd = {0};
    sd.packet_id = CIGI_SYMBOL_CIRCLE_DEF_OPCODE;
    sd.packet_size = m_PacketSize;                  // already calculated
    sd.symbol_id = m_SymbolID;
    sd.drawing_style = m_DrawingStyle;
    sd.line_width = m_LineWidth;
    sd.stipple_pattern_length = m_StipplePatternLength;

    CString hex;
    m_EditStipplePattern.GetWindowText(hex);
    unsigned int stipple = 0;
    sscanf((LPCTSTR)hex, "%x", &stipple);
    sd.stipple_pattern = stipple;

    // Add each circle to the packet.
    int rowCount = m_Grid.GetNumberRows();
    for (int i = 0; i < rowCount; ++i) {
        FetchCircleFromGrid(i, &sd.circle[i]);
    }

    SendImmedCigiMessage(&sd);
}

void CPacketDlgSymbolCircleDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolCircleDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Grid.AttachGrid(this, IDC_GRID);

    m_EditStipplePattern.SetLimitText(4);    // Room for 16-bit hex value
    m_EditStipplePattern.SetWindowText("0");

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgSymbolCircleDef::OnAddCircle()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() < CSymbolCircleGrid::MaxRows) {
        m_PacketSize = 24 + 24 * m_Grid.AddNewRow();
        UpdateData(FALSE);
    }
}

void CPacketDlgSymbolCircleDef::OnRemoveCircle()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() > 0) {
        m_PacketSize = 24 + 24 * m_Grid.DeleteSelectedRow();
        UpdateData(FALSE);
    }
}

bool CPacketDlgSymbolCircleDef::FetchCircleFromGrid(int row, CIGI_CIRCLE_DEF_ELEMENT *pCircle)
{
    CString string;

    m_Grid.QuickGetText(CSymbolCircleGrid::ColCenterU, row, &string);
    pCircle->center_u = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolCircleGrid::ColCenterV, row, &string);
    pCircle->center_v = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolCircleGrid::ColRadius, row, &string);
    pCircle->radius = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolCircleGrid::ColInnerRadius, row, &string);
    pCircle->inner_radius = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolCircleGrid::ColStartAngle, row, &string);
    pCircle->start_angle = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolCircleGrid::ColEndAngle, row, &string);
    pCircle->end_angle = (float)atof((LPCTSTR)string);

    return true;
}
