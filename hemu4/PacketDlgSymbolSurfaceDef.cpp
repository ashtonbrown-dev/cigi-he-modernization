// PacketDlgSymbolSurfaceDef.cpp : implementation file
//

#include "stdafx.h"
#include "cigi4types.h"
#include "globals.h"
#include "PacketDlgSymbolSurfaceDef.h"

// CPacketDlgSymbolSurfaceDef dialog

IMPLEMENT_DYNAMIC(CPacketDlgSymbolSurfaceDef, CDialog)
CPacketDlgSymbolSurfaceDef::CPacketDlgSymbolSurfaceDef(CWnd *pParent /*=NULL*/)
    : CDialog(CPacketDlgSymbolSurfaceDef::IDD, pParent)
    , m_SurfaceID(0)
    , m_EntityViewID(0)
    , m_SurfaceState(0)
    , m_AttachSelect(0)
    , m_Billboard(0)
    , m_PerspectiveGrowth(0)
    , m_X_Left(0)
    , m_Y_Right(0)
    , m_Z_Top(0)
    , m_Yaw_Bottom(0)
    , m_Pitch(0)
    , m_Roll(0)
    , m_Width(0)
    , m_Height(0)
    , m_MinU(0)
    , m_MaxU(0)
    , m_MinV(0)
    , m_MaxV(0)
{
}

CPacketDlgSymbolSurfaceDef::~CPacketDlgSymbolSurfaceDef()
{
}

void CPacketDlgSymbolSurfaceDef::DoDataExchange(CDataExchange *pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK3, m_Billboard);
    DDX_Check(pDX, IDC_CHECK4, m_PerspectiveGrowth);
    DDX_Text(pDX, IDC_EDIT1, m_SurfaceID);
    //DDV_MinMaxInt(pDX, m_SurfaceID, 0, 65535);
    DDX_Text(pDX, IDC_EDIT2, m_EntityViewID);
    //DDV_MinMaxInt(pDX, m_EntityViewID, 0, 65535);
    DDX_Text(pDX, IDC_EDIT3, m_X_Left);
    DDX_Text(pDX, IDC_EDIT4, m_Y_Right);
    DDX_Text(pDX, IDC_EDIT5, m_Z_Top);
    DDX_Text(pDX, IDC_EDIT6, m_Yaw_Bottom);
    DDX_Text(pDX, IDC_EDIT7, m_Pitch);
    DDX_Text(pDX, IDC_EDIT8, m_Roll);
    DDX_Text(pDX, IDC_EDIT10, m_Width);
    DDX_Text(pDX, IDC_EDIT11, m_Height);
    DDX_Text(pDX, IDC_EDIT9, m_MinU);
    DDX_Text(pDX, IDC_EDIT20, m_MaxU);
    DDX_Text(pDX, IDC_EDIT22, m_MinV);
    DDX_Text(pDX, IDC_EDIT21, m_MaxV);
    DDX_Radio(pDX, IDC_RADIO1, m_SurfaceState);
    DDX_Radio(pDX, IDC_RADIO15, m_AttachSelect);
    DDX_Control(pDX, IDC_EDIT6, m_EditYaw);
    DDX_Control(pDX, IDC_EDIT7, m_EditPitch);
    DDX_Control(pDX, IDC_EDIT8, m_EditRoll);
    DDX_Control(pDX, IDC_EDIT10, m_EditWidth);
    DDX_Control(pDX, IDC_EDIT11, m_EditHeight);
    DDX_Control(pDX, IDC_LABEL_X_LEFT, m_LabelXLeft);
    DDX_Control(pDX, IDC_LABEL_Y_RIGHT, m_LabelYRight);
    DDX_Control(pDX, IDC_LABEL_Z_TOP, m_LabelZTop);
    DDX_Control(pDX, IDC_LABEL_YAW_BOTTOM, m_LabelYawBottom);
    DDX_Control(pDX, IDC__LABEL_ORIENTATION, m_LabelOrientation);
    DDX_Control(pDX, IDC_CHECK4, m_CheckPerspectiveGrowth);
    DDX_Control(pDX, IDC_LABEL_PITCH, m_LabelPitch);
    DDX_Control(pDX, IDC_LABEL_ROLL, m_LabelRoll);
    DDX_Control(pDX, IDC_LABEL_WIDTH, m_LabelWidth);
    DDX_Control(pDX, IDC_LABEL_HEIGHT, m_LabelHeight);
}

BEGIN_MESSAGE_MAP(CPacketDlgSymbolSurfaceDef, CDialog)
    ON_BN_CLICKED(IDC_RADIO15, OnClickViewAttached)
    ON_BN_CLICKED(IDC_RADIO16, OnClickEntityAttached)
    ON_BN_CLICKED(IDC_CHECK3, OnClickBillboard)
END_MESSAGE_MAP()

// CPacketDlgSymbolSurfaceDef message handlers

void CPacketDlgSymbolSurfaceDef::OnOK()
{
    // Build and queue a packet.
    UpdateData(TRUE);

    CIGI_SYMBOL_SURFACE_DEF ssd = {0};
    ssd.packet_id = CIGI_SYMBOL_SURFACE_DEF_OPCODE;
    ssd.packet_size = sizeof(CIGI_SYMBOL_SURFACE_DEF);
    ssd.surface_id = m_SurfaceID;
    ssd.surface_state = m_SurfaceState;
    ssd.attach_type = (m_AttachSelect) ? 0 : 1;
    ssd.billboard = m_Billboard;
    ssd.persp_growth_enable = m_PerspectiveGrowth;
    ssd.entity_or_view_id = m_EntityViewID;
    ssd.x_left = m_X_Left;
    ssd.y_right = m_Y_Right;
    ssd.z_top = m_Z_Top;
    ssd.yaw_bottom = m_Yaw_Bottom;
    ssd.pitch = m_Pitch;
    ssd.roll = m_Roll;
    ssd.width = m_Width;
    ssd.height = m_Height;
    ssd.min_u = m_MinU;
    ssd.max_u = m_MaxU;
    ssd.min_v = m_MinV;
    ssd.max_v = m_MaxV;

    SendImmedCigiMessage(&ssd);
}

void CPacketDlgSymbolSurfaceDef::OnCancel()
{
    CDialog::OnCancel();

    // Destroy the window so GetSafeHwnd() will return NULL.
    DestroyWindow();
}

void CPacketDlgSymbolSurfaceDef::OnClickViewAttached()
{
    m_LabelXLeft.SetWindowText("Left:");
    m_LabelYRight.SetWindowText("Right:");
    m_LabelZTop.SetWindowText("Top:");
    m_LabelYawBottom.SetWindowText("Bottom:");
    m_EditPitch.EnableWindow(0);
    m_EditRoll.EnableWindow(0);
    m_EditWidth.EnableWindow(0);
    m_EditHeight.EnableWindow(0);
    m_LabelPitch.EnableWindow(0);
    m_LabelRoll.EnableWindow(0);
    m_LabelWidth.EnableWindow(0);
    m_LabelHeight.EnableWindow(0);
    m_LabelOrientation.ShowWindow(SW_HIDE);
}

void CPacketDlgSymbolSurfaceDef::OnClickEntityAttached()
{
    m_LabelXLeft.SetWindowText("X Offset:");
    m_LabelYRight.SetWindowText("Y Offset:");
    m_LabelZTop.SetWindowText("Z Offset:");
    m_LabelYawBottom.SetWindowText("Yaw:");
    m_EditPitch.EnableWindow(1);
    m_EditRoll.EnableWindow(1);
    m_EditWidth.EnableWindow(1);
    m_EditHeight.EnableWindow(1);
    m_LabelPitch.EnableWindow(1);
    m_LabelRoll.EnableWindow(1);
    m_LabelWidth.EnableWindow(1);
    m_LabelHeight.EnableWindow(1);
    m_LabelOrientation.ShowWindow(SW_SHOW);
}

void CPacketDlgSymbolSurfaceDef::OnClickBillboard()
{
    UpdateData(1);

    m_CheckPerspectiveGrowth.EnableWindow(m_Billboard ? 1 : 0);
}

BOOL CPacketDlgSymbolSurfaceDef::OnInitDialog()
{
    CDialog::OnInitDialog();

    OnClickViewAttached();
    OnClickBillboard();

    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
    return TRUE;
}
