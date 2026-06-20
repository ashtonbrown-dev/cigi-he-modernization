// PacketDlgSymbolControl.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolControl.h"

// CPacketDlgSymbolControl dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolControl, CDialog)
CPacketDlgSymbolControl::CPacketDlgSymbolControl(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolControl::IDD, pParent)
    , m_SymbolID(0)
    , m_SurfaceID(0)
    , m_Layer(0)
    , m_AttachState(0)
    , m_ParentID(0)
    , m_SymbolState(0)
    , m_FlashPeriod(0)
    , m_FlashDutyCycle(0)
    , m_ResetFlash(0)
    , m_Red(0)
    , m_Green(0)
    , m_Blue(0)
    , m_Alpha(0)
    , m_InheritColor(0)
    , m_PositionU(0.0)
    , m_PositionV(0.0)
    , m_Rotation(0.0)
    , m_ScaleU(0.0)
    , m_ScaleV(0.0)
{
}

CPacketDlgSymbolControl::~CPacketDlgSymbolControl()
{
}

void CPacketDlgSymbolControl::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PARENT, m_EditParentSymbolID);

    DDX_Text(pDX, IDC_EDIT_SYMBOL_ID, m_SymbolID);
    DDX_Text(pDX, IDC_EDIT_SURFACE_ID, m_SurfaceID);
    DDX_Text(pDX, IDC_EDIT_LAYER, m_Layer);
    DDX_Text(pDX, IDC_EDIT_PARENT, m_ParentID);
    DDX_Text(pDX, IDC_EDIT_FLASH_PERIOD, m_FlashPeriod);
    DDX_Text(pDX, IDC_EDIT_FLASH_DUTY_CYCLE, m_FlashDutyCycle);
    DDX_Text(pDX, IDC_EDIT_RED, m_Red);
    DDX_Text(pDX, IDC_EDIT_GREEN, m_Green);
    DDX_Text(pDX, IDC_EDIT_BLUE, m_Blue);
    DDX_Text(pDX, IDC_EDIT_ALPHA, m_Alpha);
    DDX_Text(pDX, IDC_EDIT_POS_U, m_PositionU);
    DDX_Text(pDX, IDC_EDIT_POS_V, m_PositionV);
    DDX_Text(pDX, IDC_EDIT_ROTATION, m_Rotation);
    DDX_Text(pDX, IDC_EDIT_SCALE_U, m_ScaleU);
    DDX_Text(pDX, IDC_EDIT_SCALE_V, m_ScaleV);
    DDX_Check(pDX, IDC_CHECK_ATTACH, m_AttachState);
    DDX_Check(pDX, IDC_CHECK_RESET_FLASH, m_ResetFlash);
    DDX_Check(pDX, IDC_CHECK_INHERIT_COLOR, m_InheritColor);
    DDX_Radio(pDX, IDC_RADIO1, m_SymbolState);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolControl, CDialog)
    ON_BN_CLICKED(IDC_CHECK_ATTACH, OnClickAttach)
END_MESSAGE_MAP()

// CPacketDlgSymbolControl message handlers

void CPacketDlgSymbolControl::OnClickAttach()
{
}

void CPacketDlgSymbolControl::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_CONTROL sc = {0};
    sc.packet_id = CIGI_SYMBOL_CONTROL_OPCODE;
    sc.packet_size = CIGI_SYMBOL_CONTROL_SIZE;
    sc.symbol_id = m_SymbolID;
    sc.symbol_state = m_SymbolState;
    sc.attach_state = m_AttachState;
    sc.reset_flash = m_ResetFlash;
    sc.inherit_color = m_InheritColor;
    sc.parent_id = m_ParentID;
    sc.surface_id = m_SurfaceID;
    sc.layer = m_Layer;
    sc.flash_duty_cycle = m_FlashDutyCycle;
    sc.flash_period = m_FlashPeriod;
    sc.pos_u = m_PositionU;
    sc.pos_v = m_PositionV;
    sc.rotation = m_Rotation;
    sc.red = m_Red;
    sc.green = m_Green;
    sc.blue = m_Blue;
    sc.alpha = m_Alpha;
    sc.scale_u = m_ScaleU;
    sc.scale_v = m_ScaleV;

    SendImmedCigiMessage(&sc);
}

void CPacketDlgSymbolControl::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

BOOL CPacketDlgSymbolControl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}
