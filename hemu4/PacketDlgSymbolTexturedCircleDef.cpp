// PacketDlgSymbolCircleDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolTexturedCircleDef.h"

// CPacketDlgSymbolTexturedCircleDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolTexturedCircleDef, CDialog)
CPacketDlgSymbolTexturedCircleDef::CPacketDlgSymbolTexturedCircleDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolTexturedCircleDef::IDD, pParent)
    , m_PacketSize(CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_BASE_SIZE)
    , m_SymbolID(0)
    , m_TextureID(0)
	, m_FilterMode( 0 )
	, m_RepeatClamp( 0 )
{
}

CPacketDlgSymbolTexturedCircleDef::~CPacketDlgSymbolTexturedCircleDef()
{
}

void CPacketDlgSymbolTexturedCircleDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PACKET_SIZE, m_PacketSize);
    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_TEXTURE_ID, m_TextureID);
    DDX_Radio(pDX, IDC_NEAREST, m_FilterMode);
    DDX_Radio(pDX, IDC_REPEAT, m_RepeatClamp);
    DDX_Control(pDX, IDC_BUTTON1, m_ButtonAdd);
    DDX_Control(pDX, IDC_BUTTON2, m_ButtonDelete);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolTexturedCircleDef, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnAddCircle)
    ON_BN_CLICKED(IDC_BUTTON2, OnRemoveCircle)
END_MESSAGE_MAP()

// CPacketDlgSymbolTexturedCircleDef message handlers

void CPacketDlgSymbolTexturedCircleDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_TEXTURED_CIRCLE_DEF sd = {0};
    sd.packet_id = CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_OPCODE;
    sd.packet_size = m_PacketSize;                  // already calculated
    sd.symbol_id = m_SymbolID;
    sd.texture_id = m_TextureID;
	sd.filter_mode = m_FilterMode;
	sd.repeat_clamp = m_RepeatClamp;

    // Add each circle to the packet.
    int rowCount = m_Grid.GetNumberRows();
    for (int i = 0; i < rowCount; ++i) {
        FetchCircleFromGrid(i, &sd.circle[i]);
    }

    SendImmedCigiMessage(&sd);
}

void CPacketDlgSymbolTexturedCircleDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolTexturedCircleDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_Grid.AttachGrid(this, IDC_GRID);

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}

void CPacketDlgSymbolTexturedCircleDef::OnAddCircle()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() < CSymbolTexturedCircleGrid::MaxRows) {
        m_PacketSize = CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_BASE_SIZE + CIGI_SYMBOL_TEXTURED_CIRCLE_ELEMENT_SIZE * m_Grid.AddNewRow();
        UpdateData(FALSE);
    }
}

void CPacketDlgSymbolTexturedCircleDef::OnRemoveCircle()
{
    UpdateData(TRUE);

    if (m_Grid.GetNumberRows() > 0) {
        m_PacketSize = CIGI_SYMBOL_TEXTURED_CIRCLE_DEF_BASE_SIZE + CIGI_SYMBOL_TEXTURED_CIRCLE_ELEMENT_SIZE * m_Grid.DeleteSelectedRow();
        UpdateData(FALSE);
    }
}


bool CPacketDlgSymbolTexturedCircleDef::FetchCircleFromGrid(int row, CIGI_TEXTURED_CIRCLE_DEF_ELEMENT *pCircle)
{
    CString string;

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColCenterU, row, &string);
    pCircle->center_u = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColCenterV, row, &string);
    pCircle->center_v = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColRadius, row, &string);
    pCircle->radius = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColInnerRadius, row, &string);
    pCircle->inner_radius = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColStartAngle, row, &string);
    pCircle->start_angle = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColEndAngle, row, &string);
    pCircle->end_angle = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColTextureS, row, &string);
    pCircle->texture_coord_s = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColTextureT, row, &string);
    pCircle->texture_coord_t = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColTextureRadius, row, &string);
    pCircle->texture_mapping_radius = (float)atof((LPCTSTR)string);

    m_Grid.QuickGetText(CSymbolTexturedCircleGrid::ColTextureRotation, row, &string);
    pCircle->texture_mapping_rotation = (float)atof((LPCTSTR)string);
	

    return true;
}
