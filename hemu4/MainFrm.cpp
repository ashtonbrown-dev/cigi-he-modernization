/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

// MainFrm.cpp : implementation of the CMainFrame class

#include "stdafx.h"
#include "hemumsg.h"
#include "DebugTrace.h"
#include "ObjectTreeView.h"
#include "EntityTreeView.h"
#include "EnvTreeView.h"
#include "ViewTreeView.h"
#include "ObjectStateView.h"
#include "OutputView.h"
#include "SituationView.h"
#include "Hemu4.h"
#include "PacketHandlers.h"
#include "MainFrm.h"
#include "FilterPacketsDlg.h"
#include "ObjectTreeView.h"
#include "ObjectStateView.h"
#include "OutputView.h"
#include "SituationView.h"
#include "Entity.h"
#include "AddEntityDlg.h"
#include "SetupDlg.h"
#include "DirDialog.h"
#include "TerrainDB.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_ID_REFRESH    1

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_CONFIGURATION, OnViewConfiguration)
    ON_WM_CLOSE()
    ON_COMMAND(ID_TREE_ADD, OnAddNode)
    ON_WM_TIMER()
    ON_COMMAND(ID_TREE_REMOVE, OnRemoveNode)
    ON_COMMAND(ID_FILE_SETUP, OnSetup)
    ON_COMMAND(ID_EXERCISE_PAUSE, OnExercisePause)
    ON_UPDATE_COMMAND_UI(ID_EXERCISE_PAUSE, OnUpdateExercisePause)
    ON_COMMAND(ID_EXERCISE_RUN, OnExerciseRun)
    ON_UPDATE_COMMAND_UI(ID_EXERCISE_RUN, OnUpdateExerciseRun)
    ON_COMMAND(ID_EXERCISE_RESET_SCENARIO, OnExerciseResetScenario)
    ON_UPDATE_COMMAND_UI(ID_EXERCISE_RESET_SCENARIO, OnUpdateExerciseResetScenario)
    ON_UPDATE_COMMAND_UI(ID_EXERCISE_RESET_HOST, OnUpdateExerciseResetHost)
    ON_UPDATE_COMMAND_UI(ID_EXERCISE_RESET_IG, OnUpdateExerciseResetIg)
    ON_COMMAND(ID_TOOLS_FLY, OnFly)
    ON_COMMAND(ID_TOOLS_JOYSTICK_ENABLE, OnToggleJoystick)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_JOYSTICK_ENABLE, OnUpdateToggleJoystick)
    ON_MESSAGE(WM_HEMU_JOYSTICK_INPUT, OnJoystickInput)
    ON_COMMAND(ID_VIEW_CLEAR_MESSAGES, OnClearMessages)
    ON_COMMAND(ID_FILE_LOAD_CONFIG, OnLoadConfig)
    ON_COMMAND(ID_WATCH_CAPTURE, OnWatchCapture)
    ON_UPDATE_COMMAND_UI(ID_WATCH_CAPTURE, OnUpdateWatchCapture)
    ON_COMMAND(ID_WATCH_CLEAR, OnWatchClear)
    ON_COMMAND(ID_WATCH_FILTER, OnWatchFilter)
    ON_UPDATE_COMMAND_UI(ID_TREE_ADD, OnUpdateTreeAdd)
    ON_UPDATE_COMMAND_UI(ID_TREE_REMOVE, OnUpdateTreeRemove)
    ON_COMMAND(ID_VIEW_HISTORY, OnViewHistory)
    ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORY, OnUpdateViewHistory)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_FLY, OnUpdateFly)
    ON_COMMAND(ID_TOOLS_RECORDPLAYBACK, OnToolsRecordplayback)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_RECORDPLAYBACK, OnUpdateToolsRecordplayback)
    ON_COMMAND(ID_TOOLS_SCRIPT, OnToolsScript)
    ON_UPDATE_COMMAND_UI(ID_TOOLS_SCRIPT, OnUpdateToolsScript)
    ON_COMMAND(ID_SEND_PACKET, OnSendPacket)
    ON_COMMAND(ID_PACKET_IG_CONTROL, OnPacketIGControl)
    ON_COMMAND(ID_PACKET_ENTITY_POSITION, OnPacketEntityPosition)
    ON_COMMAND(ID_PACKET_CLAMPED_ENTITY_POSITION, OnPacketClampedEntityPosition)
    ON_COMMAND(ID_PACKET_COMPONENT, OnPacketComponentControl)
    ON_COMMAND(ID_PACKET_SHORT_COMPONENT, OnPacketShortComponentControl)
    ON_COMMAND(ID_PACKET_ARTPART, OnPacketArtpartControl)
    ON_COMMAND(ID_PACKET_SHORT_ARTPART, OnPacketShortArtpartControl)
    ON_COMMAND(ID_PACKET_VELOCITY, OnPacketVelocityControl)
    ON_COMMAND(ID_PACKET_CELSPHERE, OnPacketCelestialSphereControl)
    ON_COMMAND(ID_PACKET_ATMOSPHERE, OnPacketAtmosphereControl)
    ON_COMMAND(ID_PACKET_ENV_REGION, OnPacketEnvRegionControl)
    ON_COMMAND(ID_PACKET_WEATHER, OnPacketWeatherControl)
    ON_COMMAND(ID_PACKET_MARITIME, OnPacketMaritimeSurfaceControl)
    ON_COMMAND(ID_PACKET_WAVE, OnPacketWaveControl)
    ON_COMMAND(ID_PACKET_TERRESTRIAL, OnPacketTerrestrialSurfaceControl)
    ON_COMMAND(ID_PACKET_VIEW_CONTROL, OnPacketViewControl)
    ON_COMMAND(ID_PACKET_SENSOR, OnPacketSensorControl)
    ON_COMMAND(ID_PACKET_MOTION_TRACKER, OnPacketMotionTrackerControl)
    ON_COMMAND(ID_PACKET_ERM, OnPacketErmDef)
    ON_COMMAND(ID_PACKET_ACCELERATION, OnPacketAccelerationControl)
    ON_COMMAND(ID_PACKET_VIEW_DEF, OnPacketViewDef)
    ON_COMMAND(ID_PACKET_CD_SEGMENT, OnPacketCdSegmentDef)
    ON_COMMAND(ID_PACKET_CD_VOLUME, OnPacketCdVolumeDef)
    ON_COMMAND(ID_PACKET_HAT, OnPacketHatReq)
    ON_COMMAND(ID_PACKET_LOS_SEGMENT, OnPacketLosSegmentReq)
    ON_COMMAND(ID_PACKET_LOS_VECTOR, OnPacketLosVectorReq)
    ON_COMMAND(ID_PACKET_POSITION, OnPacketPositionReq)
    ON_COMMAND(ID_PACKET_ENV_COND, OnPacketEnvCondReq)
    ON_COMMAND(ID_PACKET_SYMBOLSURFACEDEFINITION, OnPacketSymbolSurfaceDefinition)
    ON_COMMAND(ID_PACKET_SYMBOLTEXTDEFINITION, OnPacketSymbolTextDefinition)
    ON_COMMAND(ID_PACKET_SYMBOLPOLYGONDEFINITION, OnPacketSymbolPolygonDefinition)
    ON_COMMAND(ID_PACKET_SYMBOLCIRCLEDEFINITION, OnPacketSymbolCircleDefinition)
    ON_COMMAND(ID_PACKET_SYMBOLCLONE, OnPacketSymbolClone)
    ON_COMMAND(ID_PACKET_SYMBOLCONTROL, OnPacketSymbolControl)
    ON_COMMAND(ID_PACKET_SHORTSYMBOLCONTROL, OnPacketShortSymbolControl)
    ON_COMMAND(ID_PACKET_SYMBOLTEXTUREDPOLYGONDEFINITION, OnPacketSymbolTexturedPolygonDefinition)
    ON_COMMAND(ID_PACKET_SYMBOLTEXTUREDCIRCLEDEFINITION, OnPacketSymbolTexturedCircleDefinition)
    ON_COMMAND(ID_PACKET_ENTITY, OnPacketEntityControl)
    ON_COMMAND(ID_PACKET_ANIMATIONCONTROL, OnPacketAnimationControl)

    ON_COMMAND(ID_ANIM_PAUSE, OnAnimPause)
    ON_UPDATE_COMMAND_UI(ID_ANIM_PAUSE, OnUpdateAnimPause)
    ON_COMMAND(ID_ANIM_PLAY, OnAnimPlay)
    ON_UPDATE_COMMAND_UI(ID_ANIM_PLAY, OnUpdateAnimPlay)
    ON_COMMAND(ID_ANIM_STOP, OnAnimStop)
    ON_UPDATE_COMMAND_UI(ID_ANIM_STOP, OnUpdateAnimStop)
    ON_COMMAND(ID_TREE_COLLAPSE, OnTreeCollapse)
    ON_COMMAND(ID_TREE_EXPAND, OnTreeExpand)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLSURFACEDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLTEXTDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLPOLYGONDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLCIRCLEDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLCLONE, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLCONTROL, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SHORTSYMBOLCONTROL, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLTEXTUREDCIRCLEDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    ON_UPDATE_COMMAND_UI(ID_PACKET_SYMBOLTEXTUREDPOLYGONDEFINITION, &CMainFrame::OnUpdatePacketSymbol)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR,           // status line indicator
    IDS_INDICATOR_FR_STATUS,
    IDS_INDICATOR_IGMODE,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() :
    m_FlyDlg(this),
    m_HistoryDlg(this),
    m_RecPlaybackDlg(this),
    m_ScriptDlg(this),
    m_PacketDlgIGControl(this),
    m_PacketDlgEntityPosition(this),
    m_PacketDlgCCEntityPosition(this),
    m_PacketDlgComponentControl(this),
    m_PacketDlgShortComponentControl(this),
    m_PacketDlgArtpartControl(this),
    m_PacketDlgShortArtpartControl(this),
    m_PacketDlgVelocityControl(this),
    m_PacketDlgCelestialSphereControl(this),
    m_PacketDlgAtmosphereControl(this),
    m_PacketDlgEnvRegionControl(this),
    m_PacketDlgWeatherControl(this),
    m_PacketDlgMaritimeCondControl(this),
    m_PacketDlgWaveControl(this),
    m_PacketDlgTerrestrialCondControl(this),
    m_PacketDlgViewControl(this),
    m_PacketDlgSensorControl(this),
    m_PacketDlgMotionTrackerControl(this),
    m_PacketDlgERMDef(this),
    m_PacketDlgAccelerationControl(this),
    m_PacketDlgViewDef(this),
    m_PacketDlgCdSegmentDef(this),
    m_PacketDlgCdVolumeDef(this),
    m_PacketDlgHatHotReq(this),
    m_PacketDlgLosSegmentReq(this),
    m_PacketDlgLosVectorReq(this),
    m_PacketDlgPositionReq(this),
    m_PacketDlgEnvConditionsReq(this),
    m_PacketDlgSymbolSurfaceDef(this),
    m_PacketDlgSymbolTextDef(this),
    m_PacketDlgSymbolCircleDef(this),
    m_PacketDlgSymbolPolygonDef(this),
    m_PacketDlgSymbolClone(this),
    m_PacketDlgSymbolControl(this),
    m_PacketDlgShortSymbolControl(this),
	m_PacketDlgEntityControl(this),
	m_PacketDlgAnimationControl(this)
{
    m_WatchFrame = FALSE;
    m_bHoldAnimPlay = FALSE;
    m_bHoldAnimPause = FALSE;
    m_bHoldAnimStop = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CDebugTrace trace("CMainFrame::OnCreate(LPCREATESTRUCT)");

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_wndMainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE
                              | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY
                              | CBRS_SIZE_FIXED);
    m_wndMainToolBar.LoadToolBar(IDR_MAINFRAME);

    m_wndDlgBar.Create(IDD_DIALOGBAR, this);
    m_wndReBar.Create(this);
    m_wndReBar.AddBar(&m_wndMainToolBar);
    m_wndReBar.AddBar(&m_wndDlgBar);

    m_wndStatusBar.Create(this);
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

    UINT frameRatePaneId = 0;
    UINT frameRatePaneStyle = 0;
    int frameRatePaneWidth = 0;
    m_wndStatusBar.GetPaneInfo(1, frameRatePaneId, frameRatePaneStyle,
                               frameRatePaneWidth);

    CClientDC statusDC(&m_wndStatusBar);
    CFont *statusFont = m_wndStatusBar.GetFont();
    CFont *oldStatusFont = statusFont ? statusDC.SelectObject(statusFont) : NULL;
    const int connectedTextWidth =
        statusDC.GetTextExtent("Connected at 100.0 Hz").cx + 12;
    if (oldStatusFont)
        statusDC.SelectObject(oldStatusFont);

    if (connectedTextWidth > frameRatePaneWidth) {
        m_wndStatusBar.SetPaneInfo(1, frameRatePaneId, frameRatePaneStyle,
                                   connectedTextWidth);
    }

    // Load the default configuration.
    LoadEntityConfig("config\\default\\Entities.def");
    LoadViewConfig("config\\default\\Views.def");
    LoadTerrainConfig("config\\default\\Terrain.def");
    LoadEnvConfig("config\\default\\Environment.def");
    LoadSystemConfig("config\\default\\System.def");

    // The 2nd pane should have no text by default. The frame-rate pane was
    // explicitly sized above for its longest expected connection status.
    m_wndStatusBar.SetPaneText(2, "");

    return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
                                CCreateContext *pContext)
{
    CDebugTrace trace("CMainFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext *)");

    if (!m_wndSplitter.CreateStatic(this, 2, 1)) {
        return FALSE;
    }

    if (!m_wndTopSplitter.CreateStatic(&m_wndSplitter, 1, 2,
                                       WS_CHILD | WS_VISIBLE | WS_BORDER,
                                       m_wndSplitter.IdFromRowCol(0, 0))) {
        return FALSE;
    }

    if (!m_wndBottomSplitter.CreateStatic(&m_wndSplitter, 1, 2,
                                          WS_CHILD | WS_VISIBLE | WS_BORDER,
                                          m_wndSplitter.IdFromRowCol(1, 0))) {
        return FALSE;
    }

    // Resize the views.
    CRect mainrect, dlgrect;
    CDialog dlg;
    GetClientRect(&mainrect);
    dlg.Create(IDD_ENTITY_STATE_VIEW, this);
    dlg.GetWindowRect(&dlgrect);
    dlg.DestroyWindow();
    CSize pane00(mainrect.Width() - dlgrect.Width() - 11, dlgrect.Height());
    CSize pane10(mainrect.Height() - dlgrect.Height() - 40,
                 mainrect.Height() - dlgrect.Height());

    m_wndSplitter.SetRowInfo(0, dlgrect.Height(), 10);

    m_wndTopSplitter.CreateView(0, 0, RUNTIME_CLASS(CObjectTreeView), pane00, pContext);
    m_wndTopSplitter.CreateView(0, 1, RUNTIME_CLASS(CObjectStateView), CSize(0, 0), pContext);
    m_wndBottomSplitter.CreateView(0, 0, RUNTIME_CLASS(CSituationView), pane10, pContext);
    m_wndBottomSplitter.CreateView(0, 1, RUNTIME_CLASS(COutputView), CSize(0, 0), pContext);

    // Configure the situational view.
    GetSituationCtrl().SetBkgndColor(RGB(0, 0, 0));
    GetSituationCtrl().SetGamingAreaSize(1000, TRUE);

    return TRUE;
}

int CMainFrame::StartTimer(const int rate)
{
    CDebugTrace trace("CMainFrame::StartTimer(int)");

    // Set the timer.
    m_nTimer = SetTimer(TIMER_ID_REFRESH, int(1000 / (double)rate), 0);
    return m_nTimer;
}

void CMainFrame::KillTimer(void)
{
    CDebugTrace trace("CMainFrame::KillTimer()");

    CWnd::KillTimer(m_nTimer);
    m_nTimer = 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
    return CFrameWnd::PreCreateWindow(cs);
}

HTREEITEM CMainFrame::AddItemToEntityTree(LPCTSTR name, const int image,
                                          const DWORD data, const HTREEITEM parent,
                                          const BOOL maketop)
{
    CDebugTrace trace("CMainFrame::AddItemToEntityTree(LPCTSTR, int, DWORD, HTREEITEM, BOOL)");

    HTREEITEM hitem = GetEntityTree().InsertItem(name, image, image, parent,
                                                 maketop ? TVI_FIRST : TVI_LAST);
    GetEntityTree().SetItemData(hitem, data);
    return hitem;
}

void CMainFrame::RemoveItemFromEntityTree(const HTREEITEM htree)
{
    CDebugTrace trace("CMainFrame::RemoveItemFromEntityTree(HTREEITEM)");

    GetEntityTree().DeleteItem(htree);
}

void CMainFrame::RemoveItemFromEnvTree(const HTREEITEM htree)
{
    CDebugTrace trace("CMainFrame::RemoveItemFromEnvTree(HTREEITEM)");

    GetEnvTree().DeleteItem(htree);
}

HTREEITEM CMainFrame::AddItemToEnvTree(LPCTSTR name, const int image,
                                       const DWORD data, const HTREEITEM parent)
{
    CDebugTrace trace("CMainFrame::AddItemToEnvTree(LPCTSTR, int, DWORD, HTREEITEM)");

    HTREEITEM hitem = GetEnvTree().InsertItem(name, image, image, parent);
    GetEnvTree().SetItemData(hitem, data);
    return hitem;
}

HTREEITEM CMainFrame::AddItemToViewTree(LPCTSTR name, const int image,
                                        const DWORD data, const HTREEITEM parent)
{
    CDebugTrace trace("CMainFrame::AddItemToViewTree(LPCTSTR, int, DWORD, HTREEITEM)");

    HTREEITEM hitem = GetViewTree().InsertItem(name, image, image, parent);
    GetViewTree().SetItemData(hitem, data);
    return hitem;
}

BOOL CMainFrame::SelectSituationViewEntity(const int id)
{
    CDebugTrace trace("CMainFrame::SelectSituationViewEntity(int)");

    CEntity *entity = g_DataManager.GetEntity(id);
    if (entity && (entity->GetParent() == -1)) {
        POSITION hdl = entity->GetSituationHandle();
        theApp.GetMainFrame().GetSituationCtrl().SelectEntity(hdl);

        return TRUE;
    }

    return FALSE;
}

void CMainFrame::UpdateSituationView()
{
    int id = -1;
    double x, y;
    CEntity *entity = NULL;
    CEntityMap &map = g_DataManager.GetEntityMap();
    POSITION pos = map.GetStartPosition();

    while (pos) {
        map.GetNextAssoc(pos, id, entity);
        if (entity) {
            ENTITY state = entity->GetAndLock();
            entity->Unlock();

            GDCtoTM(state.cigi.dofs.latitude, state.cigi.dofs.longitude, &y, &x);

            BOOL visible = ((state.cigi.parent_id == -1) && (state.cigi.active)
                            && (state.cigi.alpha > 0)) ? TRUE : FALSE;

            POSITION hdl = entity->GetSituationHandle();

            if (hdl)
                GetSituationCtrl().SetEntity(hdl, x, y, state.cigi.dofs.yaw, visible);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext &dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnViewConfiguration()
{
    if (m_ConfigWindow.IsWindowVisible())
        m_ConfigWindow.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    else
        m_ConfigWindow.ShowWindow(SW_SHOW);
}

CEntity *CMainFrame::AddNewEntity(const int id, TEMPL_ENTITY *templ)
{
    CDebugTrace trace("CMainFrame::AddNewEntity(int, TEMPL_ENTITY *)");

    CEntity *entity = g_DataManager.CreateEntity(id, templ);
    if (entity) {
        AddEntityToGUI(entity);
        GetEntityTree().SelectItem(entity->GetHtree());

        // We need to manually refresh the view.
        GetObjectStateView().RefreshActiveView();
        UpdateSituationView();
    }

    return entity;
}

void CMainFrame::AddEntityToGUI(CEntity *entity)
{
    CDebugTrace trace("CMainFrame::AddEntityToGUI(CEntity *)");

    // Add the entity to the tree control.
    HTREEITEM htree = AddItemToEntityTree(entity->GetName(), entity->GetClass(),
                                          MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY, entity->GetClass()),
                                          TVI_ROOT, ((entity->GetID()) == 0 ? TRUE : FALSE));

    entity->SetHtree(htree);

    // If the entity is a weather entity, also add it to the Environment tree.
    if (entity->GetClass() == ENTITY_CLASS_CLOUD) {
        CString temp_name = "Entity " + entity->GetName();
        htree = AddItemToEnvTree((LPCTSTR)temp_name, IMG_WEATHER_ENTITY,
                                 MAKE_TREE_DATA(entity->GetID(), OBJECT_TYPE_ENTITY, entity->GetClass()),
                                 g_htreeSky);

        entity->SetHtreeEnv(htree);
    }

    // Add the entity to the situational view.
    double x, y;
    DOF dof = entity->GetDofs();
    GDCtoTM(dof.latitude, dof.longitude, &y, &x);
    CSituationDispCtrl &sitctrl = ((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl();
    POSITION shdl = sitctrl.AddEntity(x, y, 0, EntityClassColor[entity->GetClass()],
                                      (LPCTSTR)(entity->GetName()));

    entity->SetSituationHandle(shdl);

    // Don't select the entity here: GetEntityTree().SelectItem(entity->GetHtree()).
    // It prevents us from creating a large number of entities from a script.
    // Instead, we'll have to do this from AddNewEntity().

    // Add the entity to the situation view.
    UpdateSituationView();
}

void CMainFrame::OnAddNode()
{
    CDebugTrace trace("CMainFrame::OnAddNode()");

    CAddEntityDlg dlg;
    if (dlg.DoModal() == IDCANCEL)
        return;

    CEntity *entity = NULL;
    TEMPL_ENTITY *templ = GetEntityTempl(dlg.GetType());
    if (templ) {
        AddNewEntity(dlg.GetID(), templ);
    } else {
        // If we are creating an entity of Type 0 but haven't
        // configured this type, create a default template.
        if (dlg.GetType() == 0) {
            TEMPL_ENTITY default_type;
            default_type.Type = 0;
            default_type.Name = "None";
            default_type.Class = ENTITY_CLASS_FIXEDWING;

            AddNewEntity(dlg.GetID(), &default_type);
        } else {
            AfxMessageBox("Invalid entity type specified.", MB_ICONEXCLAMATION);
        }
    }

    return;
}

void CMainFrame::OnUpdateTreeAdd(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((GetObjectTreeView().GetSelectedTabNum() == 0) ? TRUE : FALSE);
}

void CMainFrame::OnRemoveNode()
{
    CDebugTrace trace("CMainFrame::OnRemoveNode()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (entity)
        RemoveEntity(entity);
}

void CMainFrame::RemoveEntity(CEntity *entity)
{
    CDebugTrace trace("CMainFrame::RemoveEntity(CEntity *)");

    if (entity) {
        RemoveEntityFromGUI(entity->GetID(), entity->GetHtree(), entity->GetHtreeEnv(),
                            entity->GetSituationHandle());

        // Delete the entity object.
        g_DataManager.DeleteEntity(entity->GetID());
    }
}

void CMainFrame::RemoveEntityFromGUI(const int id, const HTREEITEM hEntity,
                                     const HTREEITEM hEnv,
                                     const POSITION hSituation)
{
    CDebugTrace trace("CMainFrame::RemoveEntityFromGUI(id, HTREEITEM, HTREEITEM, POSITION)");

    // Remove the entity from the Entity tree control.
    HTREEITEM prev = GetEntityTree().GetPrevVisibleItem(hEntity);
    HTREEITEM first = GetEntityTree().GetFirstVisibleItem();
    if (prev)
        GetEntityTree().SelectItem(prev);
    else if (first)
        GetEntityTree().SelectItem(first);
    RemoveItemFromEntityTree(hEntity);

    // Remove the entity from the Environment tree control if necessary.
    if (hEnv) {
        prev = GetEnvTree().GetPrevVisibleItem(hEnv);
        first = GetEnvTree().GetFirstVisibleItem();
        if (prev)
            GetEnvTree().SelectItem(prev);
        else if (first)
            GetEnvTree().SelectItem(first);
        RemoveItemFromEnvTree(hEnv);
    }

    // Remove the entity from the situational display.
    if (hSituation)
        (((CMainFrame *)AfxGetMainWnd())->GetSituationCtrl()).RemoveEntity(hSituation);
}

void CMainFrame::OnUpdateTreeRemove(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(GetObjectTreeView().GetSelectedTabNum() == 0 ? TRUE : FALSE);
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
    CString t_str;

    if (!::GetFreezeFlag()) {
        GetObjectStateView().RefreshActiveView();
        GetEntityStateView().GetCollDetPropPage().ClearCollisionData();
        UpdateSituationView();
    }

    CheckMessageQueue();
    ProcessOutgoingCigiPacketQueue();
    ProcessIncomingCigiPacketQueue();

    m_bHoldAnimPlay = FALSE;
    m_bHoldAnimPause = FALSE;
    m_bHoldAnimStop = FALSE;

    CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::CheckMessageQueue(void)
{
    HEMU_MESSAGE *msg = NULL;

    msg = GetGUIMsg();
    while (msg) {
        DoMessage(msg);
        msg = GetGUIMsg();
    }
}

void CMainFrame::DoMessage(HEMU_MESSAGE *msg)
{
    if (!msg)
        return;

    switch (msg->msg) {
    case MSG_PLAYBACK_EOF: {
        if (GetRecPlaybackDlg().GetSafeHwnd()) {
            GetRecPlaybackDlg().StopPlayback();

            if (g_bRecPlaybackLoop)
                GetRecPlaybackDlg().BeginPlayback();
        }

        break;
    }

    case MSG_FILE_ERROR:
        break;

    case MSG_FRAME_RATE: {
        static unsigned int prev_avgfr = 0;

        CString statustxt;
        g_FrameRate = ((MESSAGE_FRAME_RATE *)msg)->framerate;
        if (g_FrameRate == 10000000)
            m_wndStatusBar.SetPaneText(1, "Connecting...");
        else {
            // Keep the smoothed value for history/capture calculations, but
            // display the current full-window sample without another lag.
            ::CalcNewAvgFrameRate();
            statustxt.Format("Connected at %d.0 Hz", g_FrameRate);
            m_wndStatusBar.SetPaneText(1, (LPCTSTR)statustxt);
        }

        if (fabs((double)(g_AvgFrameRate - prev_avgfr)) > 5.0f) {
            // Set the median of the heartbeat window.
            if (m_HistoryDlg.GetSafeHwnd()) {
                m_HistoryDlg.RecalcLimits();
            }

            // Set the number of seconds in the capture file.
            if (m_RecPlaybackDlg.GetSafeHwnd())
                m_RecPlaybackDlg.IncrementCaptureProgress();
        }

        prev_avgfr = g_AvgFrameRate;

        break;
    }

    case MSG_NO_CONNECT: {
        g_FrameRate = 0;
        ::CalcNewAvgFrameRate();
        m_wndStatusBar.SetPaneText(1, "Not connected");
        m_wndStatusBar.SetPaneText(2, "");
        break;
    }

    case MSG_NOTIFY_ANIM_STOP: {
        CEntity *anim = g_DataManager.GetEntity(((MESSAGE_NOTIFY_ANIM_STOP *)msg)->entity_id);
        if (anim) {
            anim->SetAnimState(ENTITY_ANIM_STATE_STOP);

            // We'll assume the IG has played the final frame of the
            // animation, so we will hide the geometry.
            anim->SetActiveState(0);
        }

        break;
    }

    case MSG_MISSILE_HIT: {
        CEntity *missile = g_DataManager.GetEntity(((MESSAGE_MISSILE_HIT *)msg)->missile_id);
        if (missile) {
            PlayDetonationAnims(missile);
            StopTerminatingLaunchAnims(missile);
            // Note: Do not StopPersistentLaunchAnims(missile), and
            //       do not missile->SetAlpha(0) (It's done in the driver.)
            GetSituationCtrl().SetEntityVisible(missile->GetSituationHandle(), FALSE);

            CEntity *target = g_DataManager.GetEntity(missile->GetTarget());
            if (target) {
                // Hide the target here instead of the driver, AFTER we
                // start playing the animations.
                target->SetActiveState(0);

                GetSituationCtrl().SetEntityVisible(target->GetSituationHandle(), FALSE);
            }
        }

        break;
    }

    case MSG_NOTIFY_RECPLAYBACK_FRAME:
        if (GetRecPlaybackDlg().GetSafeHwnd())
            GetRecPlaybackDlg().IncrementFrame();

        break;

    case MSG_WAYPOINT_REACHED: {
        int entity_id = ((MESSAGE_WAYPOINT_REACHED *)msg)->entity_id;
        int waypoint_id = ((MESSAGE_WAYPOINT_REACHED *)msg)->waypoint_id;
        CString msg;

        CEntity *entity = g_DataManager.GetEntity(entity_id);
        if (entity) {
            msg.Format("Entity %d reached waypoint %d.", entity_id, waypoint_id);
            PrintMessageText((LPCTSTR)msg);

            entity->AcquireNextWaypoint();
        }

        break;
    }

    case MSG_SCRIPT_EOF:
        // Note: Do not do CloseHandle(g_ScriptThreadAddrHdl) with _endthread().
        //       Although we would want to if we used _endthreadex().

        g_ScriptThreadAddrHdl = 0;

        if (GetScriptDlg().GetSafeHwnd())
            GetScriptDlg().StopScript();

        break;

    case MSG_SCRIPT_PROGRESS:
        if (GetScriptDlg().GetSafeHwnd())
            GetScriptDlg().IncrementScriptProgress(((MESSAGE_SCRIPT_PROGRESS *)msg)->linenum);

        break;

    case MSG_SCRIPT_BEGIN_WAIT_TIMER:
        if (GetScriptDlg().GetSafeHwnd())
            GetScriptDlg().SetWaitTime(((MESSAGE_SCRIPT_BEGIN_WAIT_TIMER *)msg)->secs);

        break;

    case MSG_SCRIPT_INC_WAIT_TIMER:
        if (GetScriptDlg().GetSafeHwnd())
            GetScriptDlg().SetWaitTime(((MESSAGE_SCRIPT_INC_WAIT_TIMER *)msg)->secs);

        break;

    case MSG_SCRIPT_END_WAIT_TIMER:
        if (GetScriptDlg().GetSafeHwnd())
            GetScriptDlg().SetWaitTime(-1);

        break;

    case MSG_DISPLAY_MSG_TEXT: {
        CMessageView &msgview = theApp.GetMainFrame().GetMessageView();
        if (msgview.GetSafeHwnd())
            msgview.PrintMessage(((MESSAGE_DISPLAY_MSG_TEXT *)msg)->text);

        break;
    }

    case MSG_UPDATE_ENV_VIEW:
        GetWeatherStateView().RefreshView();
        GetSkyStateView().RefreshView();
        GetTerrainStateView().RefreshView();
        break;

    case MSG_UPDATE_ENTITIES_VIEW:
        GetEntityStateView().RefreshView();
        break;

    case MSG_UPDATE_VIEWS_VIEW:
        GetViewStateView().RefreshView();
        GetViewGroupStateView().RefreshView();
        break;

    case MSG_UPDATE_ENV_TAB_PAGES:
        GetWeatherStateView().RefreshCompPage();
        GetSkyStateView().RefreshCompPages();
        GetTerrainStateView().RefreshCompPage();
        break;

    case MSG_UPDATE_ENTITY_TAB_PAGES:
        GetEntityStateView().RefreshCompPage();
        GetEntityStateView().RefreshArtPartPage();
        break;

    case MSG_UPDATE_VIEW_TAB_PAGES:
        GetViewStateView().RefreshCompPage();
        GetViewGroupStateView().RefreshCompPage();
        break;

    case MSG_SCRIPT_LOAD_SCENARIO:
        break;

    case MSG_SCRIPT_SAVE_SCENARIO:
        break;

    case MSG_SCRIPT_RELOAD_SCENARIO:
        break;

    case MSG_SCRIPT_CAPTURE_PLAY:
        break;

    case MSG_CLEAR_MSG_TEXT: {
        CMessageView &msgview = theApp.GetMainFrame().GetMessageView();
        if (msgview.GetSafeHwnd())
            msgview.ClearMessageText();

        break;
    }

    case MSG_UPDATE_DATABASE:
        break;

    case MSG_SPAWN_APP:
        if (SpawnApp(((MESSAGE_SPAWN_APP *)msg)->command) < 0) {
            CString syserrmsg;
            syserrmsg.Format("Cannot execute the following command: %s",
                             ((MESSAGE_SPAWN_APP *)msg)->command);
            PrintMessageText((LPCTSTR)syserrmsg);
        }

        break;

    case MSG_SCRIPT_ADD_ENTITY_TO_GUI: {
        int entity_id = ((MESSAGE_SCRIPT_ADD_ENTITY_TO_GUI *)msg)->entity_id;
        CEntity *entity = g_DataManager.GetEntity(entity_id);
        if (entity)
            AddEntityToGUI(entity);

        break;
    }

    case MSG_SCRIPT_DEL_ENTITY_FROM_GUI: {
        int entity_id = ((MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI *)msg)->entity_id;
        HTREEITEM htree = (HTREEITEM)((MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI *)msg)->htree;
        HTREEITEM htree_env = (HTREEITEM)((MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI *)msg)->htree_env;
        POSITION situation_hdl = (POSITION)((MESSAGE_SCRIPT_DEL_ENTITY_FROM_GUI *)msg)->situation_hdl;

        RemoveEntityFromGUI(entity_id, htree, htree_env, situation_hdl);

        break;
    }

    case MSG_SCRIPT_SET_PARENT_IN_GUI: {
        int entity_id = ((MESSAGE_SCRIPT_SET_PARENT_IN_GUI *)msg)->entity_id;
        int parent_id = ((MESSAGE_SCRIPT_SET_PARENT_IN_GUI *)msg)->parent_id;

        GetEntityTreeView().MoveEntityToParent(g_DataManager.GetEntity(entity_id),
                                               parent_id);

        break;
    }

    case MSG_SCRIPT_SET_VIEWGROUP_IN_GUI: {
        int view_id = ((MESSAGE_SCRIPT_SET_VIEWGROUP_IN_GUI *)msg)->view_id;
        int group_id = ((MESSAGE_SCRIPT_SET_VIEWGROUP_IN_GUI *)msg)->group_id;

        GetViewTreeView().MoveViewToGroup(g_DataManager.GetView(view_id),
                                          group_id);

        break;
    }

    case MSG_SCRIPT_SET_PAUSE:
        if (g_ScriptState != SCRIPT_STATE_STOP)
            GetScriptDlg().CheckPauseButton(TRUE);

        break;

    case MSG_SCRIPT_FIRE_MISSILE: {
        FireMissile(((MESSAGE_SCRIPT_FIRE_MISSILE *)msg)->pMissile);

        // Update the Fly window if it exists.
        if (theApp.GetMainFrame().GetFlyDlg()) {
            theApp.GetMainFrame().GetFlyDlg().UpdateMissileCombo();
        }

        break;
    }

    case MSG_SCRIPT_UPDATE_MISSILE_TARGET_COMBO:
        if (theApp.GetMainFrame().GetFlyDlg().GetSafeHwnd())
            theApp.GetMainFrame().GetFlyDlg().UpdateTargetCombo();

        break;

    default:
        break;
    }
}

void CMainFrame::OnSetup()
{
    theApp.ShowSetupDlg();
}

void CMainFrame::OnExercisePause()
{
    // Set the flag so we know whether to enable or disable the toolbar button.
    ::SetFreezeFlag(TRUE);

    // Send a message to the RT driver.
    MESSAGE_PAUSE_EXERCISE msg;
    PostDriverMsg(msg);

    // The driver will update the entities' positions one more time before
    // the MESSAGE_PAUSE_EXERCISE message is processed, so we need to
    // refresh after the driver is finished.
    GetObjectStateView().RefreshActiveView();
    UpdateSituationView();
}

void CMainFrame::OnUpdateExercisePause(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((BOOL)!::GetFreezeFlag());
}

void CMainFrame::OnExerciseRun()
{
    ::SetFreezeFlag(FALSE);
    ::SetResetIGFlag(FALSE);

    // Send a message to the RT driver.
    MESSAGE_RUN_EXERCISE msg;
    PostDriverMsg(msg);
}

void CMainFrame::OnUpdateExerciseRun(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((BOOL)::GetFreezeFlag());
}

void CMainFrame::OnExerciseResetScenario()
{
    g_DataManager.ResetScenario();
}

void CMainFrame::OnUpdateExerciseResetScenario(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((BOOL)::GetFreezeFlag());
}

void CMainFrame::OnUpdateExerciseResetHost(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((BOOL)::GetFreezeFlag());
}

void CMainFrame::OnUpdateExerciseResetIg(CCmdUI *pCmdUI)
{
    pCmdUI->Enable((BOOL)::GetFreezeFlag());
}

void CMainFrame::ShowDlg(UINT resourceId, CDialog &dlgRef, int nCmdShow)
{
    if (!dlgRef.GetSafeHwnd()) {
        dlgRef.Create(resourceId, NULL);
        dlgRef.CenterWindow();
    }

    dlgRef.ShowWindow(nCmdShow);

    // Move the window to the top.
    dlgRef.SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CMainFrame::ToggleShowDlg(UINT resourceId, CDialog &dlgRef)
{
    const bool showWindow = (!dlgRef.GetSafeHwnd() || !dlgRef.IsWindowVisible());
    const int  nCmdShow   = (showWindow ? SW_SHOW : SW_HIDE);

    ShowDlg(resourceId, dlgRef, nCmdShow);
}

void CMainFrame::OnFly()
{
    CDebugTrace trace("CMainFrame::OnFly()");

    ToggleShowDlg(IDD_DIALOG_FLY, m_FlyDlg);
}

void CMainFrame::OnToggleJoystick()
{
    CJoystickInput &joystick = theApp.GetJoystickInput();
    const BOOL enable = joystick.IsEnabled() ? FALSE : TRUE;
    joystick.SetEnabled(enable);

    if (!enable)
        m_FlyDlg.DisableHardwareJoystick();
}

void CMainFrame::OnUpdateToggleJoystick(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(theApp.GetJoystickInput().IsEnabled());
}

LRESULT CMainFrame::OnJoystickInput(WPARAM wParam, LPARAM lParam)
{
    HEMU_JOYSTICK_STATE state;
    if (theApp.GetJoystickInput().ConsumeLatestState(&state))
        m_FlyDlg.ProcessHardwareJoystickState(state);

    return 0;
}

void CMainFrame::OnClearMessages()
{
    ClearMessageText();
}

void CMainFrame::OnWatchCapture()
{
    // When watching packets, skip the first frame to make things look neater.
    m_WatchFrame = FALSE;

    SetPacketWatchFlag(!GetPacketWatchFlag());
}

void CMainFrame::OnUpdateWatchCapture(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(GetPacketWatchFlag());
}

void CMainFrame::OnWatchClear()
{
    GetWatchView().DeleteAllItems();
}

void CMainFrame::OnWatchFilter()
{
    CFilterPacketsDlg dlg;
    if (dlg.DoModal() == IDCANCEL)
        return;
}

void CMainFrame::OnLoadConfig()
{
    CDebugTrace trace("CMainFrame::OnLoadConfig()");

    CDirDialog dlg(".\\config\\default", "Definition Files (*.def)|*.def|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK) {
        CString strDir = dlg.GetPath();

        CString path = strDir + "Entities.def";
        ClearEntityConfig();
        LoadEntityConfig(path);

        path = strDir + "Views.def";
        ClearViewConfig();
        LoadViewConfig(path);
        g_DataManager.ApplyViewConfiguration();

        path = strDir + "Terrain.def";
        ClearTerrainConfig();
        LoadTerrainConfig(path);
        g_DataManager.ApplyTerrainConfiguration();

        path = strDir + "Environment.def";
        ClearEnvConfig();
        LoadEnvConfig(path);
        g_DataManager.ApplyEnvConfiguration();

        path = strDir + "System.def";
        ClearSystemConfig();
        LoadSystemConfig(path);
    }
}

void CMainFrame::ClearEntityConfig(void)
{
    CDebugTrace trace("CMainFrame::ClearEntityConfig()");

    g_DataManager.RemoveAllEntityTemplates();
}

int CMainFrame::LoadEntityConfig(LPCTSTR filename)
{
    CDebugTrace trace("CMainFrame::LoadEntityConfig()");

    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    CHemuDoc *doc = ::GetDocument();
    g_DataManager.LoadEntityTemplates(file);

    file.Close();

    return 1;
}

void CMainFrame::ClearViewConfig(void)
{
    CDebugTrace trace("CMainFrame::ClearViewConfig()");

    g_DataManager.RemoveAllViewTemplates();
    g_DataManager.RemoveAllViewGroupTemplates();

    GetViewTreeView().ClearItems();
}

int CMainFrame::LoadViewConfig(LPCTSTR filename)
{
    CDebugTrace trace("CMainFrame::LoadViewConfig()");

    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    CHemuDoc *doc = ::GetDocument();
    g_DataManager.LoadViewTemplates(file);
    g_DataManager.LoadViewGroupTemplates(file);

    file.Close();

    return 1;
}

void CMainFrame::ClearTerrainConfig(void)
{
    CDebugTrace trace("CMainFrame::ClearTerrainConfig()");

    g_DataManager.RemoveAllDatabaseTemplates();
    g_DataManager.GetGlobalEnv()->ClearTerrainComponents();
}

int CMainFrame::LoadTerrainConfig(LPCTSTR filename)
{
    CDebugTrace trace("CMainFrame::LoadTerrainConfig()");

    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    CHemuDoc *doc = ::GetDocument();
    g_DataManager.LoadDatabaseTemplates(file);

    file.Close();

    return 1;
}

void CMainFrame::ClearEnvConfig(void)
{
    CDebugTrace trace("CMainFrame::ClearEnvConfig()");

    g_DataManager.RemoveAllDatabaseTemplates();
}

int CMainFrame::LoadEnvConfig(LPCTSTR filename)
{
    CDebugTrace trace("CMainFrame::LoadEnvConfig()");

    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    CHemuDoc *doc = ::GetDocument();
    g_DataManager.LoadEnvTemplates(file);

    file.Close();

    return 1;
}

void CMainFrame::ClearSystemConfig(void)
{
    CDebugTrace trace("CMainFrame::ClearSystemConfig()");

    g_DataManager.ClearSystemComponents();
}

int CMainFrame::LoadSystemConfig(LPCTSTR filename)
{
    CDebugTrace trace("CMainFrame::LoadSystemConfig()");

    CStdioFile file;
    if (!file.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText)) {
        CString errmsg;
        errmsg.Format("Cannot open the file \"%s\"", (LPCTSTR)filename);
        MessageBox((LPCTSTR)errmsg, "File Error", MB_ICONEXCLAMATION);
        return 0;
    }

    CHemuDoc *doc = ::GetDocument();
    g_DataManager.LoadSystemTemplates(file);

    file.Close();

    return 1;
}

void CMainFrame::OnViewHistory()
{
    CDebugTrace trace("CMainFrame::OnViewHistory()");

    ToggleShowDlg(IDD_DIALOG_HISTORY, m_HistoryDlg);
}

void CMainFrame::OnUpdateViewHistory(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_HistoryDlg.GetSafeHwnd() && m_HistoryDlg.IsWindowVisible());
}

void CMainFrame::OnUpdateFly(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_FlyDlg.GetSafeHwnd() && m_FlyDlg.IsWindowVisible());
}

void CMainFrame::UpdateDatabaseCombo(void)
{
    CDebugTrace trace("CMainFrame::UpdateDatabaseCombo()");

    CTerrainDB *db = g_DataManager.GetSelectedDatabase();

    if (db)
        m_wndDlgBar.SelectDatabaseItem(db->GetID());
}

void CMainFrame::SelectFirstDatabaseComboItem(void)
{
    CDebugTrace trace("CMainFrame::SelectFirstDatabaseComboItem()");

    m_wndDlgBar.m_ComboDatabase.SetCurSel(0);
    m_wndDlgBar.OnSelchangeDatabase();
}

void CMainFrame::OnToolsRecordplayback()
{
    CDebugTrace trace("CMainFrame::OnToolsRecordplayback()");

    ToggleShowDlg(IDD_DIALOG_RECPLAYBACK, m_RecPlaybackDlg);
}

void CMainFrame::OnUpdateToolsRecordplayback(CCmdUI *pCmdUI)
{
    if (m_ScriptDlg.GetSafeHwnd() && m_ScriptDlg.IsWindowVisible())
        pCmdUI->Enable(FALSE);
    else if (m_RecPlaybackDlg.GetSafeHwnd()) {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(m_RecPlaybackDlg.IsWindowVisible());
    }
}

void CMainFrame::OnToolsScript()
{
    CDebugTrace trace("CMainFrame::OnToolsScript()");

    ToggleShowDlg(IDD_DIALOG_SCRIPT, m_ScriptDlg);
}

void CMainFrame::OnUpdateToolsScript(CCmdUI *pCmdUI)
{
    if (m_RecPlaybackDlg.GetSafeHwnd() && m_RecPlaybackDlg.IsWindowVisible())
        pCmdUI->Enable(FALSE);
    else if (m_ScriptDlg.GetSafeHwnd()) {
        pCmdUI->Enable(TRUE);
        pCmdUI->SetCheck(m_ScriptDlg.IsWindowVisible());
    }
}

void CMainFrame::OnSendPacket()
{
    CDebugTrace trace("CMainFrame::OnSendPacket()");

    const CMenu *mainmenu   = GetMenu();
    const CMenu *actions    = mainmenu->GetSubMenu(2);
    const CMenu *sendpacket = actions->GetSubMenu(0);
    CMenu popup;

    popup.CreatePopupMenu();

    // Build the menu.
    for (int i = 0; i < sendpacket->GetMenuItemCount(); i++) {
        CString string;

        sendpacket->GetMenuString(i, string, MF_BYPOSITION);
        popup.AppendMenu(MF_ENABLED | MF_STRING, sendpacket->GetMenuItemID(i), (LPCTSTR)string);
    }

    // Display the menu below the button.
    CRect rectbutton;
    CRect rectmenu;
    CSize size;

    m_wndMainToolBar.GetToolBarCtrl().GetRect(ID_SEND_PACKET, &rectbutton);
    m_wndMainToolBar.GetToolBarCtrl().GetWindowRect(&rectmenu);

    size.cx = rectbutton.left;
    size.cy = rectbutton.top;
    rectmenu += size;

    popup.TrackPopupMenu(TPM_LEFTALIGN, rectmenu.left, rectmenu.bottom, this, NULL);
}

void CMainFrame::OnPacketIGControl()
{
    ShowDlg(IDD_PACKET_IG_CONTROL, m_PacketDlgIGControl);
}

void CMainFrame::OnPacketEntityPosition()
{
    ShowDlg(IDD_PACKET_ENTITY_POSITION, m_PacketDlgEntityPosition);
}

void CMainFrame::OnPacketClampedEntityPosition()
{
    ShowDlg(IDD_PACKET_CLAMPED_ENTITY_POSITION, m_PacketDlgCCEntityPosition);
}

void CMainFrame::OnPacketComponentControl()
{
    ShowDlg(IDD_PACKET_COMPONENT_CONTROL, m_PacketDlgComponentControl);
}

void CMainFrame::OnPacketShortComponentControl()
{
    ShowDlg(IDD_PACKET_SHORT_COMPONENT_CONTROL, m_PacketDlgShortComponentControl);
}

void CMainFrame::OnPacketArtpartControl()
{
    ShowDlg(IDD_PACKET_ARTPART_CONTROL, m_PacketDlgArtpartControl);
}

void CMainFrame::OnPacketShortArtpartControl()
{
    ShowDlg(IDD_PACKET_SHORT_ARTPART_CONTROL, m_PacketDlgShortArtpartControl);
}

void CMainFrame::OnPacketVelocityControl()
{
    ShowDlg(IDD_PACKET_VELOCITY_CONTROL, m_PacketDlgVelocityControl);
}

void CMainFrame::OnPacketCelestialSphereControl()
{
    ShowDlg(IDD_PACKET_CELESTIAL_SPHERE_CONTROL, m_PacketDlgCelestialSphereControl);
}

void CMainFrame::OnPacketAtmosphereControl()
{
    ShowDlg(IDD_PACKET_ATMOSPHERE_CONTROL, m_PacketDlgAtmosphereControl);
}

void CMainFrame::OnPacketEnvRegionControl()
{
    ShowDlg(IDD_PACKET_ENV_REGION_CONTROL, m_PacketDlgEnvRegionControl);
}

void CMainFrame::OnPacketWeatherControl()
{
    ShowDlg(IDD_PACKET_WEATHER_CONTROL, m_PacketDlgWeatherControl);
}

void CMainFrame::OnPacketMaritimeSurfaceControl()
{
    ShowDlg(IDD_PACKET_MARITIME_CONTROL, m_PacketDlgMaritimeCondControl);
}

void CMainFrame::OnPacketWaveControl()
{
    ShowDlg(IDD_PACKET_WAVE_CONTROL, m_PacketDlgWaveControl);
}

void CMainFrame::OnPacketTerrestrialSurfaceControl()
{
    ShowDlg(IDD_PACKET_TERRESTRIAL_CONTROL, m_PacketDlgTerrestrialCondControl);
}

void CMainFrame::OnPacketViewControl()
{
    ShowDlg(IDD_PACKET_VIEW_CONTROL, m_PacketDlgViewControl);
}

void CMainFrame::OnPacketSensorControl()
{
    ShowDlg(IDD_PACKET_SENSOR_CONTROL, m_PacketDlgSensorControl);
}

void CMainFrame::OnPacketMotionTrackerControl()
{
    ShowDlg(IDD_PACKET_MOTION_TRACKER_CONTROL, m_PacketDlgMotionTrackerControl);
}

void CMainFrame::OnPacketErmDef()
{
    ShowDlg(IDD_PACKET_ERM_DEF, m_PacketDlgERMDef);
}

void CMainFrame::OnPacketAccelerationControl()
{
    ShowDlg(IDD_PACKET_ACCELERATION_CONTROL, m_PacketDlgAccelerationControl);
}

void CMainFrame::OnPacketViewDef()
{
    ShowDlg(IDD_PACKET_VIEW_DEF, m_PacketDlgViewDef);
}

void CMainFrame::OnPacketCdSegmentDef()
{
    ShowDlg(IDD_PACKET_CD_SEGMENT_DEF, m_PacketDlgCdSegmentDef);
}

void CMainFrame::OnPacketCdVolumeDef()
{
    ShowDlg(IDD_PACKET_CD_VOLUME_DEF, m_PacketDlgCdVolumeDef);
}

void CMainFrame::OnPacketHatReq()
{
    ShowDlg(IDD_PACKET_HAT_HOT_REQ, m_PacketDlgHatHotReq);
}

void CMainFrame::OnPacketLosSegmentReq()
{
    ShowDlg(IDD_PACKET_LOS_SEGMENT_REQ, m_PacketDlgLosSegmentReq);
}

void CMainFrame::OnPacketLosVectorReq()
{
    ShowDlg(IDD_PACKET_LOS_VECTOR_REQ, m_PacketDlgLosVectorReq);
}

void CMainFrame::OnPacketPositionReq()
{
    ShowDlg(IDD_PACKET_POSITION_REQ, m_PacketDlgPositionReq);
}

void CMainFrame::OnPacketEnvCondReq()
{
    ShowDlg(IDD_PACKET_ENV_CONDITIONS_REQ, m_PacketDlgEnvConditionsReq);
}

void CMainFrame::OnPacketSymbolSurfaceDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_SURFACE_DEF, m_PacketDlgSymbolSurfaceDef);
}

void CMainFrame::OnPacketSymbolTextDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_TEXT_DEF, m_PacketDlgSymbolTextDef);
}

void CMainFrame::OnPacketSymbolCircleDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_CIRCLE_DEF, m_PacketDlgSymbolCircleDef);
}

void CMainFrame::OnPacketSymbolPolygonDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_POLYGON_DEF, m_PacketDlgSymbolPolygonDef);
}

void CMainFrame::OnPacketSymbolClone()
{
    ShowDlg(IDD_PACKET_SYMBOL_CLONE, m_PacketDlgSymbolClone);
}

void CMainFrame::OnPacketSymbolControl()
{
    ShowDlg(IDD_PACKET_SYMBOL_CONTROL, m_PacketDlgSymbolControl);
}

void CMainFrame::OnPacketShortSymbolControl()
{
    ShowDlg(IDD_PACKET_SHORT_SYMBOL_CONTROL, m_PacketDlgShortSymbolControl);
}

void CMainFrame::OnPacketEntityControl()
{
    ShowDlg(IDD_PACKET_ENTITY_CONTROL, m_PacketDlgEntityControl);
}

void CMainFrame::OnPacketAnimationControl()
{
    ShowDlg(IDD_PACKET_ANIMATION_CONTROL, m_PacketDlgAnimationControl);
}

void CMainFrame::OnPacketSymbolTexturedCircleDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_TEXTURED_CIRCLE_DEF, m_PacketDlgSymbolTexturedCircleDef);
}

void CMainFrame::OnPacketSymbolTexturedPolygonDefinition()
{
    ShowDlg(IDD_PACKET_SYMBOL_TEXTURED_POLYGON_DEF, m_PacketDlgSymbolTexturedPolygonDef);
}


void CMainFrame::OnAnimPause()
{
    CDebugTrace trace("CMainFrame::OnAnimPause()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    int animstate = entity->GetAnimState();
    switch (animstate) {
    case ENTITY_ANIM_STATE_STOP:
        break;

    case ENTITY_ANIM_STATE_PAUSE:
        entity->SetAnimState(ENTITY_ANIM_STATE_CONTINUE);
        break;

    case ENTITY_ANIM_STATE_PLAY:
        entity->SetAnimState(ENTITY_ANIM_STATE_PAUSE);
        break;

    case ENTITY_ANIM_STATE_CONTINUE:
        entity->SetAnimState(ENTITY_ANIM_STATE_PAUSE);
        break;

    default:
        break;
    }

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldAnimPause = TRUE;
}

void CMainFrame::OnUpdateAnimPause(CCmdUI *pCmdUI)
{
    // We don't want to update the play button state the frame
    // immediately following user action because the 1-frame
    // lag from the driver may cause flutter.
    if (m_bHoldAnimPause)
        pCmdUI->SetCheck(TRUE);
    else {
        CEntity *entity = g_DataManager.GetSelectedEntity();

        if (entity && (entity->GetClass() == ENTITY_CLASS_ANIMATION)) {
            int animstate = entity->GetAnimState();

            if (animstate != ENTITY_ANIM_STATE_STOP)
                pCmdUI->Enable(TRUE);

            if (animstate == ENTITY_ANIM_STATE_PAUSE)
                pCmdUI->SetCheck(TRUE);
            else
                pCmdUI->SetCheck(FALSE);
        } else {
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
}

void CMainFrame::OnAnimPlay()
{
    CDebugTrace trace("CMainFrame::OnAnimPlay()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    int animstate = entity->GetAnimState();
    switch (animstate) {
    case ENTITY_ANIM_STATE_STOP:
        entity->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        break;

    case ENTITY_ANIM_STATE_PAUSE:
        entity->SetAnimState(ENTITY_ANIM_STATE_CONTINUE);
        break;

    case ENTITY_ANIM_STATE_PLAY:
        entity->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        break;

    case ENTITY_ANIM_STATE_CONTINUE:
        entity->SetAnimState(ENTITY_ANIM_STATE_PLAY);
        break;

    default:
        break;
    }

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldAnimPlay = TRUE;
}

void CMainFrame::OnUpdateAnimPlay(CCmdUI *pCmdUI)
{
    // We don't want to update the play button state the frame
    // immediately following user action because the 1-frame
    // lag from the driver may cause flutter.
    if (m_bHoldAnimPlay)
        pCmdUI->SetCheck(TRUE);
    else {
        CEntity *entity = g_DataManager.GetSelectedEntity();

        if (entity && (entity->GetClass() == ENTITY_CLASS_ANIMATION)) {
            pCmdUI->Enable(TRUE);

            int animstate = entity->GetAnimState();

            if (animstate == ENTITY_ANIM_STATE_STOP)
                pCmdUI->SetCheck(FALSE);
            else
                pCmdUI->SetCheck(TRUE);
        } else {
            pCmdUI->Enable(FALSE);
            pCmdUI->SetCheck(FALSE);
        }
    }
}

void CMainFrame::OnAnimStop()
{
    CDebugTrace trace("CMainFrame::OnAnimStop()");

    CEntity *entity = g_DataManager.GetSelectedEntity();
    if (!entity)
        return;

    entity->SetAnimState(ENTITY_ANIM_STATE_STOP);

    // Set the hold flag so we don't get "flutter" from the
    // 1-frame latency from the driver.
    m_bHoldAnimStop = TRUE;
}

void CMainFrame::OnUpdateAnimStop(CCmdUI *pCmdUI)
{
    // We don't want to update the play button state the frame
    // immediately following user action because the 1-frame
    // lag from the driver may cause flutter.
    if (m_bHoldAnimStop)
        pCmdUI->SetCheck(TRUE);
    else {
        pCmdUI->SetCheck(FALSE);

        CEntity *entity = g_DataManager.GetSelectedEntity();

        if (entity && (entity->GetClass() == ENTITY_CLASS_ANIMATION))
            pCmdUI->Enable(TRUE);
        else
            pCmdUI->Enable(FALSE);
    }
}

void CMainFrame::OnTreeCollapse()
{
    CDebugTrace trace("CMainFrame::OnTreeCollapse()");

    CTreeCtrl *tree = NULL;

    switch (GetObjectTreeView().GetSelectedTabNum()) {
    case 0:     // Entities
        tree = &GetEntityTree();
        break;

    case 1:     // Environment
        tree = &GetEnvTree();
        break;

    case 2:
        tree = &GetViewTree();
        break;

    default:
        break;
    }

    if (tree) {
        HTREEITEM item = tree->GetNextItem(TVI_ROOT, TVGN_CHILD);
        while (item) {
            tree->Expand(item, TVE_COLLAPSE);
            item = tree->GetNextItem(item, TVGN_NEXT);
        }
    }
}

void CMainFrame::OnTreeExpand()
{
    CDebugTrace trace("CMainFrame::OnTreeExpand()");

    CTreeCtrl *tree = NULL;

    switch (GetObjectTreeView().GetSelectedTabNum()) {
    case 0:     // Entities
        tree = &GetEntityTree();
        break;

    case 1:     // Environment
        tree = &GetEnvTree();
        break;

    case 2:
        tree = &GetViewTree();
        break;

    default:
        break;
    }

    if (tree) {
        HTREEITEM item = tree->GetNextItem(TVI_ROOT, TVGN_CHILD);
        while (item) {
            tree->Expand(item, TVE_EXPAND);
            item = tree->GetNextItem(item, TVGN_NEXTVISIBLE);
        }
    }
}

/// Enables or disables (grays out) symbology-related menu items, depending on
/// the CIGI version selected by the user in the Setup menu.
void CMainFrame::OnUpdatePacketSymbol(CCmdUI *pCmdUI)
{
// chas     const bool enableSymbology = (GetCigiMinorVersion() >= 3);

// chas     pCmdUI->Enable(enableSymbology);
     pCmdUI->Enable( true );
}

/// If the user changes the CIGI version in the Setup window, this function
/// will close any windows that are used to activate functionality not allowed
/// by the newly selected CIGI version. (E.g. if the user selects a version
/// below CIGI 3.3, all symbology-related packet windows will be closed.)
void CMainFrame::EnforceCigiVersion(void)
{
#if 0 // chas
const bool enableSymbology = (GetCigiMinorVersion() >= 3);

   if (!enableSymbology) {
        if (m_PacketDlgSymbolSurfaceDef.GetSafeHwnd())
            m_PacketDlgSymbolSurfaceDef.ShowWindow(SW_HIDE);
        if (m_PacketDlgSymbolTextDef.GetSafeHwnd())
            m_PacketDlgSymbolTextDef.ShowWindow(SW_HIDE);
        if (m_PacketDlgSymbolCircleDef.GetSafeHwnd())
            m_PacketDlgSymbolCircleDef.ShowWindow(SW_HIDE);
        if (m_PacketDlgSymbolLineDef.GetSafeHwnd())
            m_PacketDlgSymbolLineDef.ShowWindow(SW_HIDE);
        if (m_PacketDlgSymbolClone.GetSafeHwnd())
            m_PacketDlgSymbolClone.ShowWindow(SW_HIDE);
        if (m_PacketDlgSymbolControl.GetSafeHwnd())
            m_PacketDlgSymbolControl.ShowWindow(SW_HIDE);
        if (m_PacketDlgShortSymbolControl.GetSafeHwnd())
            m_PacketDlgShortSymbolControl.ShowWindow(SW_HIDE);
  }
#endif
}

CObjectTreeView &CMainFrame::GetObjectTreeView(void)
{
    return *((CObjectTreeView *)(m_wndTopSplitter.GetPane(0, 0)));
}

CObjectStateView &CMainFrame::GetObjectStateView(void)
{
    return *((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)));
}

CEntityTreeView &CMainFrame::GetEntityTreeView(void)
{
    return GetObjectTreeView().GetEntityTreeView();
}

CEnvTreeView &CMainFrame::GetEnvTreeView(void)
{
    return GetObjectTreeView().GetEnvTreeView();
}

CViewTreeView &CMainFrame::GetViewTreeView(void)
{
    return GetObjectTreeView().GetViewTreeView();
}

CMessageView &CMainFrame::GetMessageView(void)
{
    return (*((COutputView *)(m_wndBottomSplitter.GetPane(0, 1)))).GetMessageView();
}

CPacketWatchView &CMainFrame::GetWatchView(void)
{
    return (*((COutputView *)(m_wndBottomSplitter.GetPane(0, 1)))).GetWatchView();
}

CSituationDispCtrl &CMainFrame::GetSituationCtrl(void)
{
    return (*((CSituationView *)(m_wndBottomSplitter.GetPane(0, 0)))).m_SituationCtrl;
}

CTreeCtrl &CMainFrame::GetEntityTree(void)
{
    return GetObjectTreeView().GetEntityTree();
}

CTreeCtrl &CMainFrame::GetEnvTree(void)
{
    return GetObjectTreeView().GetEnvTree();
}

CTreeCtrl &CMainFrame::GetViewTree(void)
{
    return GetObjectTreeView().GetViewTree();
}

CFlyDlg &CMainFrame::GetFlyDlg(void)
{
    return m_FlyDlg;
}

void CMainFrame::ClearHistory(void)
{
    m_HistoryDlg.m_Graph.Clear();
}

CTimingHistoryDlg &CMainFrame::GetHistoryDlg(void)
{
    return m_HistoryDlg;
}

CRecPlaybackDlg &CMainFrame::GetRecPlaybackDlg(void)
{
    return m_RecPlaybackDlg;
}

CScriptDlg &CMainFrame::GetScriptDlg(void)
{
    return m_ScriptDlg;
}

int CMainFrame::SetActiveStateView(const int view)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).SetActiveView(view);
}
int CMainFrame::GetActiveStateViewNum(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetActiveViewNum();
}

CBaseStateView *CMainFrame::GetActiveStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetActiveView();
}

CEntityStateView &CMainFrame::GetEntityStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetEntityStateView();
}

CViewStateView &CMainFrame::GetViewStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetViewStateView();
}

CViewGroupStateView &CMainFrame::GetViewGroupStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetViewGroupStateView();
}

CWeatherStateView &CMainFrame::GetWeatherStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetWeatherStateView();
}

CSkyStateView &CMainFrame::GetSkyStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetSkyStateView();
}

CTerrainStateView &CMainFrame::GetTerrainStateView(void)
{
    return (*((CObjectStateView *)(m_wndTopSplitter.GetPane(0, 1)))).GetTerrainStateView();
}

BOOL CMainFrame::IsWatchingThisFrame(void)
{
    return m_WatchFrame;
}

void CMainFrame::WatchThisFrame(void)
{
    m_WatchFrame = TRUE;
}

void CMainFrame::SkipThisFrame(void)
{
    m_WatchFrame = FALSE;
}

void CMainFrame::SetIGModeStatusText(LPCTSTR text)
{
    m_wndStatusBar.SetPaneText(2, text);
}

void CMainFrame::PopulateDatabaseCombo(void)
{
    CDebugTrace trace("CMainFrame::PopulateDatabaseCombo()");

    m_wndDlgBar.PopulateDatabaseCombo();
}

void CMainFrame::ClearDatabaseCombo(void)
{
    CDebugTrace trace("CMainFrame::ClearDatabaseCombo()");

    m_wndDlgBar.ClearDatabaseCombo();
}
