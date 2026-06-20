// PacketDlgSymbolLineDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolTexturedPolygonDef.h"

// CPacketDlgSymbolTexturedPolygonDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolTexturedPolygonDef, CDialog)
CPacketDlgSymbolTexturedPolygonDef::CPacketDlgSymbolTexturedPolygonDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolTexturedPolygonDef::IDD, pParent)
    , m_PacketSize(CIGI_SYMBOL_TEXTURED_POLYGON_DEF_BASE_SIZE)
    , m_SymbolID(0)
    , m_TextureID(0)
	, m_FilterMode( 0 )
	, m_RepeatClamp( 0 )
{
}

CPacketDlgSymbolTexturedPolygonDef::~CPacketDlgSymbolTexturedPolygonDef()
{
}

void CPacketDlgSymbolTexturedPolygonDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PACKET_SIZE, m_PacketSize);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_TEXTURE_ID, m_TextureID);
    DDX_Radio(pDX, IDC_NEAREST, m_FilterMode);
    DDX_Radio(pDX, IDC_REPEAT2, m_RepeatClamp);
    DDX_Control(pDX, IDC_BUTTON1, m_ButtonAdd);
    DDX_Control(pDX, IDC_BUTTON2, m_ButtonDelete);
    DDX_Control(pDX, IDC_COMBO_PRIMITIVE, m_ComboPrimitive);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolTexturedPolygonDef, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnAddLine)
    ON_BN_CLICKED(IDC_BUTTON2, OnRemoveLine)
END_MESSAGE_MAP()


// CPacketDlgSymbolTexturedPolygonDef message handlers

void CPacketDlgSymbolTexturedPolygonDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_TEXTURED_POLYGON_DEF sd = {0};
    sd.packet_id = CIGI_SYMBOL_TEXTURED_POLYGON_DEF_OPCODE;
    sd.packet_size = m_PacketSize;                  // already calculated
    sd.symbol_id = m_SymbolID;
	sd.texture_id = m_TextureID;
	sd.filter_mode = m_FilterMode;
	sd.repeat_clamp = m_RepeatClamp;
    sd.primitive_type = m_ComboPrimitive.GetCurSel();

    // Add each line to the packet.
    int rowCount = m_Grid.GetNumberRows();
    for (int i = 0; i < rowCount; ++i) {
        FetchLineFromGrid(i, &sd.line[i]);
    }

    SendImmedCigiMessage(&sd);
}

void CPacketDlgSymbolTexturedPolygonDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolTexturedPolygonDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Grid.AttachGrid(this, IDC_GRID);

    m_ComboPrimitive.SetCurSel(0);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgSymbolTexturedPolygonDef::OnAddLine()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() < CSymbolTexturedPolygonGrid::MaxRows) {
        m_PacketSize = CIGI_SYMBOL_TEXTURED_POLYGON_DEF_BASE_SIZE + CIGI_SYMBOL_TEXTURED_POLYGON_ELEMENT_SIZE * m_Grid.AddNewRow();
        UpdateData(FALSE);
    }
}

void CPacketDlgSymbolTexturedPolygonDef::OnRemoveLine()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() > 0) {
        m_PacketSize = CIGI_SYMBOL_TEXTURED_POLYGON_DEF_BASE_SIZE + CIGI_SYMBOL_TEXTURED_POLYGON_ELEMENT_SIZE * m_Grid.DeleteSelectedRow();
        UpdateData(FALSE);
    }
}

bool CPacketDlgSymbolTexturedPolygonDef::FetchLineFromGrid(int row, CIGI_TEXTURED_POLYGON_DEF_ELEMENT *pTexturedPolygon)
{
    CString string;

    m_Grid.QuickGetText(CSymbolTexturedPolygonGrid::ColVertexU, row, &string);
    pTexturedPolygon->vertex_u = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedPolygonGrid::ColVertexV, row, &string);
    pTexturedPolygon->vertex_v = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedPolygonGrid::ColTextureS, row, &string);
    pTexturedPolygon->texture_coord_s = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedPolygonGrid::ColTextureT, row, &string);
    pTexturedPolygon->texture_coord_t = (float)atof((LPCTSTR)string);

    return true;
}
