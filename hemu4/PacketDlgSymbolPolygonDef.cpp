// PacketDlgSymbolLineDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolPolygonDef.h"

// CPacketDlgSymbolPolygonDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolPolygonDef, CDialog)
CPacketDlgSymbolPolygonDef::CPacketDlgSymbolPolygonDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolPolygonDef::IDD, pParent)
    , m_PacketSize(CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE)
    , m_SymbolID(0)
    , m_LineWidth(0)
    , m_StipplePatternLength(0)
{
}

CPacketDlgSymbolPolygonDef::~CPacketDlgSymbolPolygonDef()
{
}

void CPacketDlgSymbolPolygonDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_STIPPLE_PATTERN, m_EditStipplePattern);
    DDX_Text(pDX, IDC_PACKET_SIZE, m_PacketSize);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_LineWidth);
    DDX_Text(pDX, IDC_EDIT_STIPPLE_LENGTH, m_StipplePatternLength);
    DDX_Control(pDX, IDC_BUTTON1, m_ButtonAdd);
    DDX_Control(pDX, IDC_BUTTON2, m_ButtonDelete);
    DDX_Control(pDX, IDC_COMBO_PRIMITIVE, m_ComboPrimitive);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolPolygonDef, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnAddLine)
    ON_BN_CLICKED(IDC_BUTTON2, OnRemoveLine)
END_MESSAGE_MAP()


// CPacketDlgSymbolPolygonDef message handlers

void CPacketDlgSymbolPolygonDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_POLYGON_DEF sd = {0};
    sd.packet_id = CIGI_SYMBOL_POLYGON_DEF_OPCODE;
    sd.packet_size = m_PacketSize;                  // already calculated
    sd.symbol_id = m_SymbolID;
    sd.primitive_type = m_ComboPrimitive.GetCurSel();
    sd.line_width = m_LineWidth;
    sd.stipple_pattern_length = m_StipplePatternLength;

    CString hex;
    m_EditStipplePattern.GetWindowText(hex);
    unsigned int stipple = 0;
    sscanf((LPCTSTR)hex, "%x", &stipple);
    sd.stipple_pattern = stipple;

    // Add each line to the packet.
    int rowCount = m_Grid.GetNumberRows();
    for (int i = 0; i < rowCount; ++i) {
        FetchLineFromGrid(i, &sd.line[i]);
    }

    SendImmedCigiMessage(&sd);
}

void CPacketDlgSymbolPolygonDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolPolygonDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Grid.AttachGrid(this, IDC_GRID);

    m_ComboPrimitive.SetCurSel(0);

    m_EditStipplePattern.SetLimitText(4);    // Room for 16-bit hex value
    m_EditStipplePattern.SetWindowText("0");

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgSymbolPolygonDef::OnAddLine()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() < CSymbolPolygonGrid::MaxRows) {
        m_PacketSize = CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE + CIGI_SYMBOL_VERTEX_SIZE * m_Grid.AddNewRow();
        UpdateData(FALSE);
    }
}

void CPacketDlgSymbolPolygonDef::OnRemoveLine()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() > 0) {
        m_PacketSize = CIGI_SYMBOL_POLYGON_DEF_BASE_SIZE + CIGI_SYMBOL_VERTEX_SIZE * m_Grid.DeleteSelectedRow();
        UpdateData(FALSE);
    }
}

bool CPacketDlgSymbolPolygonDef::FetchLineFromGrid(int row, CIGI_POLYGON_DEF_ELEMENT *pPolygon)
{
    CString string;

    m_Grid.QuickGetText(CSymbolPolygonGrid::ColVertexU, row, &string);
    pPolygon->vertex_u = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolPolygonGrid::ColVertexV, row, &string);
    pPolygon->vertex_v = (float)atof((LPCTSTR)string);

    return true;
}
