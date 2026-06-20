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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__725B73A6_58AD_44DC_A297_22378C963565__INCLUDED_)
#define AFX_MAINFRM_H__725B73A6_58AD_44DC_A297_22378C963565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBDialogBar.h"
#include "ConfigWnd.h"
#include "FlyDlg.h"
#include "TimingHistoryDlg.h"
#include "RecPlaybackDlg.h"
#include "ScriptDlg.h"
#include "PacketDlgIGControl.h"
#include "PacketDlgEntityPosition.h"
#include "PacketDlgCCEntityPosition.h"
#include "PacketDlgComponentControl.h"
#include "PacketDlgShortComponentControl.h"
#include "PacketDlgArtpartControl.h"
#include "PacketDlgShortArtpartControl.h"
#include "PacketDlgVelocityControl.h"
#include "PacketDlgCelestialSphereControl.h"
#include "PacketDlgAtmosphereControl.h"
#include "PacketDlgEnvRegionControl.h"
#include "PacketDlgWeatherControl.h"
#include "PacketDlgMaritimeCondControl.h"
#include "PacketDlgWaveControl.h"
#include "PacketDlgTerrestrialCondControl.h"
#include "PacketDlgViewControl.h"
#include "PacketDlgSensorControl.h"
#include "PacketDlgMotionTrackerControl.h"
#include "PacketDlgERMDef.h"
#include "PacketDlgAccelerationControl.h"
#include "PacketDlgViewDef.h"
#include "PacketDlgCdSegmentDef.h"
#include "PacketDlgCdVolumeDef.h"
#include "PacketDlgHatHotReq.h"
#include "PacketDlgLosSegmentReq.h"
#include "PacketDlgLosVectorReq.h"
#include "PacketDlgPositionReq.h"
#include "PacketDlgEnvConditionsReq.h"
#include "PacketDlgSymbolSurfaceDef.h"
#include "PacketDlgSymbolTextDef.h"
#include "PacketDlgSymbolCircleDef.h"
#include "PacketDlgSymbolPolygonDef.h"
#include "PacketDlgSymbolClone.h"
#include "PacketDlgSymbolControl.h"
#include "PacketDlgShortSymbolControl.h"
#include "PacketDlgEntityControl.h"
#include "PacketDlgAnimationControl.h"
#include "PacketDlgSymbolTexturedCircleDef.h"
#include "PacketDlgSymbolTexturedPolygonDef.h"
 

class CBaseStateView;
class CEntity;
class CEntityStateView;
class CEntityTreeView;
class CEnvTreeView;
class CMessageView;
class CObjectStateView;
class CObjectTreeView;
class CPacketWatchView;
class CSituationDispCtrl;
class CSkyStateView;
class CTerrainStateView;
class CTreeCtrl;
class CViewGroupStateView;
class CViewStateView;
class CViewTreeView;
class CWeatherStateView;
class HEMU_MESSAGE;
class TEMPL_ENTITY;

class CMainFrame : public CFrameWnd
{
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext &dc) const;
#endif

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
public:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext);
    virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
    //}}AFX_VIRTUAL

    CObjectTreeView &GetObjectTreeView(void);
    CObjectStateView &GetObjectStateView(void);
    CEntityTreeView &GetEntityTreeView(void);
    CEnvTreeView &GetEnvTreeView(void);
    CViewTreeView &GetViewTreeView(void);
    CMessageView &GetMessageView(void);
    CPacketWatchView &GetWatchView(void);
    CSituationDispCtrl &GetSituationCtrl(void);
    CTreeCtrl &GetEntityTree(void);
    CTreeCtrl &GetEnvTree(void);
    CTreeCtrl &GetViewTree(void);
    CEntityStateView &GetEntityStateView(void);
    CViewStateView &GetViewStateView(void);
    CViewGroupStateView &GetViewGroupStateView(void);
    CWeatherStateView &GetWeatherStateView(void);
    CSkyStateView &GetSkyStateView(void);
    CTerrainStateView &GetTerrainStateView(void);
    CFlyDlg &GetFlyDlg(void);
    void ClearHistory(void);
    CTimingHistoryDlg &GetHistoryDlg(void);
    CRecPlaybackDlg &GetRecPlaybackDlg(void);
    CScriptDlg &GetScriptDlg(void);
    CEntity *AddNewEntity(const int id, TEMPL_ENTITY *templ);
    void AddEntityToGUI(CEntity *entity);
    void RemoveEntity(CEntity *entity);
    void RemoveEntityFromGUI(const int id, const HTREEITEM hEntity,
                             const HTREEITEM hEnv, const POSITION hSituation);
    HTREEITEM AddItemToEntityTree(LPCTSTR name, const int image,
                                  const DWORD data = 0, const HTREEITEM parent = TVI_ROOT,
                                  const BOOL maketop = FALSE);
    void RemoveItemFromEntityTree(const HTREEITEM htree);
    HTREEITEM AddItemToEnvTree(LPCTSTR name, const int image,
                               const DWORD data = 0, const HTREEITEM parent = TVI_ROOT);
    void RemoveItemFromEnvTree(const HTREEITEM htree);
    HTREEITEM AddItemToViewTree(LPCTSTR name, const int image,
                                const DWORD data = 0, const HTREEITEM parent = TVI_ROOT);
    int SetActiveStateView(const int view);
    int GetActiveStateViewNum(void);
    CBaseStateView *GetActiveStateView(void);
    BOOL SelectSituationViewEntity(const int id);
    void UpdateSituationView(void);
    int StartTimer(const int rate);
    void KillTimer(void);
    BOOL IsWatchingThisFrame(void);
    void WatchThisFrame(void);
    void SkipThisFrame(void);
    void SetIGModeStatusText(LPCTSTR text);
    void PopulateDatabaseCombo(void);
    void ClearDatabaseCombo(void);
    void UpdateDatabaseCombo(void);
    void SelectFirstDatabaseComboItem(void);
    void EnforceCigiVersion(void);

protected:
    BOOL m_WatchFrame;
    BOOL m_bHoldAnimPlay;
    BOOL m_bHoldAnimPause;
    BOOL m_bHoldAnimStop;
    UINT m_nTimer;
    CStatusBar  m_wndStatusBar;
    CToolBar    m_wndMainToolBar;
    CReBar      m_wndReBar;
    CDBDialogBar m_wndDlgBar;
    CSplitterWnd m_wndSplitter;
    CSplitterWnd m_wndTopSplitter;
    CSplitterWnd m_wndBottomSplitter;
    CConfigWnd m_ConfigWindow;
    CFlyDlg m_FlyDlg;
    CTimingHistoryDlg m_HistoryDlg;
    CRecPlaybackDlg m_RecPlaybackDlg;
    CScriptDlg m_ScriptDlg;
    CPacketDlgIGControl m_PacketDlgIGControl;
    CPacketDlgEntityPosition m_PacketDlgEntityPosition;
    CPacketDlgCCEntityPosition m_PacketDlgCCEntityPosition;
    CPacketDlgComponentControl m_PacketDlgComponentControl;
    CPacketDlgShortComponentControl m_PacketDlgShortComponentControl;
    CPacketDlgArtpartControl m_PacketDlgArtpartControl;
    CPacketDlgShortArtpartControl m_PacketDlgShortArtpartControl;
    CPacketDlgVelocityControl m_PacketDlgVelocityControl;
    CPacketDlgCelestialSphereControl m_PacketDlgCelestialSphereControl;
    CPacketDlgAtmosphereControl m_PacketDlgAtmosphereControl;
    CPacketDlgEnvRegionControl m_PacketDlgEnvRegionControl;
    CPacketDlgWeatherControl m_PacketDlgWeatherControl;
    CPacketDlgMaritimeCondControl m_PacketDlgMaritimeCondControl;
    CPacketDlgWaveControl m_PacketDlgWaveControl;
    CPacketDlgTerrestrialCondControl m_PacketDlgTerrestrialCondControl;
    CPacketDlgViewControl m_PacketDlgViewControl;
    CPacketDlgSensorControl m_PacketDlgSensorControl;
    CPacketDlgMotionTrackerControl m_PacketDlgMotionTrackerControl;
    CPacketDlgERMDef m_PacketDlgERMDef;
    CPacketDlgAccelerationControl m_PacketDlgAccelerationControl;
    CPacketDlgViewDef m_PacketDlgViewDef;
    CPacketDlgCdSegmentDef m_PacketDlgCdSegmentDef;
    CPacketDlgCdVolumeDef m_PacketDlgCdVolumeDef;
    CPacketDlgHatHotReq m_PacketDlgHatHotReq;
    CPacketDlgLosSegmentReq m_PacketDlgLosSegmentReq;
    CPacketDlgLosVectorReq m_PacketDlgLosVectorReq;
    CPacketDlgPositionReq m_PacketDlgPositionReq;
    CPacketDlgEnvConditionsReq m_PacketDlgEnvConditionsReq;
    CPacketDlgSymbolSurfaceDef m_PacketDlgSymbolSurfaceDef;
    CPacketDlgSymbolTextDef m_PacketDlgSymbolTextDef;
    CPacketDlgSymbolCircleDef m_PacketDlgSymbolCircleDef;
    CPacketDlgSymbolPolygonDef m_PacketDlgSymbolPolygonDef;
    CPacketDlgSymbolClone m_PacketDlgSymbolClone;
    CPacketDlgSymbolControl m_PacketDlgSymbolControl;
    CPacketDlgShortSymbolControl m_PacketDlgShortSymbolControl;
    CPacketDlgSymbolTexturedCircleDef m_PacketDlgSymbolTexturedCircleDef;
    CPacketDlgSymbolTexturedPolygonDef m_PacketDlgSymbolTexturedPolygonDef;
    CPacketDlgEntityControl m_PacketDlgEntityControl;
    CPacketDlgAnimationControl m_PacketDlgAnimationControl;

    void CheckMessageQueue(void);
    void DoMessage(HEMU_MESSAGE *msg);

    void ClearEntityConfig(void);
    int LoadEntityConfig(LPCTSTR filename);

    void ClearViewConfig(void);
    int LoadViewConfig(LPCTSTR filename);

    void ClearTerrainConfig(void);
    int LoadTerrainConfig(LPCTSTR filename);

    void ClearEnvConfig(void);
    int LoadEnvConfig(LPCTSTR filename);

    void ClearSystemConfig(void);
    int LoadSystemConfig(LPCTSTR filename);

    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    void ShowDlg(UINT resourceId, CDialog &dlgRef, int nCmdShow = SW_SHOW);
    void ToggleShowDlg(UINT resourceId, CDialog &dlgRef);

    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewConfiguration();
    afx_msg void OnAddNode();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnRemoveNode();
    afx_msg void OnSetup();
    afx_msg void OnExercisePause();
    afx_msg void OnUpdateExercisePause(CCmdUI *pCmdUI);
    afx_msg void OnExerciseRun();
    afx_msg void OnUpdateExerciseRun(CCmdUI *pCmdUI);
    afx_msg void OnExerciseResetScenario();
    afx_msg void OnUpdateExerciseResetScenario(CCmdUI *pCmdUI);
    afx_msg void OnUpdateExerciseResetHost(CCmdUI *pCmdUI);
    afx_msg void OnUpdateExerciseResetIg(CCmdUI *pCmdUI);
    afx_msg void OnFly();
    afx_msg void OnClearMessages();
    afx_msg void OnLoadConfig();
    afx_msg void OnWatchCapture();
    afx_msg void OnUpdateWatchCapture(CCmdUI *pCmdUI);
    afx_msg void OnWatchClear();
    afx_msg void OnWatchFilter();
    afx_msg void OnUpdateTreeAdd(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTreeRemove(CCmdUI *pCmdUI);
    afx_msg void OnViewHistory();
    afx_msg void OnUpdateViewHistory(CCmdUI *pCmdUI);
    afx_msg void OnUpdateFly(CCmdUI *pCmdUI);
    afx_msg void OnToolsRecordplayback();
    afx_msg void OnUpdateToolsRecordplayback(CCmdUI *pCmdUI);
    afx_msg void OnToolsScript();
    afx_msg void OnUpdateToolsScript(CCmdUI *pCmdUI);
    afx_msg void OnSendPacket();
    afx_msg void OnPacketIGControl();
    afx_msg void OnPacketEntityPosition();
    afx_msg void OnPacketClampedEntityPosition();
    afx_msg void OnPacketComponentControl();
    afx_msg void OnPacketShortComponentControl();
    afx_msg void OnPacketArtpartControl();
    afx_msg void OnPacketShortArtpartControl();
    afx_msg void OnPacketVelocityControl();
    afx_msg void OnPacketCelestialSphereControl();
    afx_msg void OnPacketAtmosphereControl();
    afx_msg void OnPacketEnvRegionControl();
    afx_msg void OnPacketWeatherControl();
    afx_msg void OnPacketMaritimeSurfaceControl();
    afx_msg void OnPacketWaveControl();
    afx_msg void OnPacketTerrestrialSurfaceControl();
    afx_msg void OnPacketViewControl();
    afx_msg void OnPacketSensorControl();
    afx_msg void OnPacketMotionTrackerControl();
    afx_msg void OnPacketErmDef();
    afx_msg void OnPacketAccelerationControl();
    afx_msg void OnPacketViewDef();
    afx_msg void OnPacketCdSegmentDef();
    afx_msg void OnPacketCdVolumeDef();
    afx_msg void OnPacketHatReq();
    afx_msg void OnPacketLosSegmentReq();
    afx_msg void OnPacketLosVectorReq();
    afx_msg void OnPacketPositionReq();
    afx_msg void OnPacketEnvCondReq();
    afx_msg void OnPacketSymbolSurfaceDefinition();
    afx_msg void OnPacketSymbolTextDefinition();
    afx_msg void OnPacketSymbolPolygonDefinition();
    afx_msg void OnPacketSymbolCircleDefinition();
    afx_msg void OnPacketSymbolClone();
    afx_msg void OnPacketSymbolControl();
    afx_msg void OnPacketShortSymbolControl();
    afx_msg void OnPacketEntityControl();
    afx_msg void OnPacketAnimationControl();
	afx_msg void OnPacketSymbolTexturedCircleDefinition();
	afx_msg void OnPacketSymbolTexturedPolygonDefinition();
    afx_msg void OnAnimPause();
    afx_msg void OnUpdateAnimPause(CCmdUI *pCmdUI);
    afx_msg void OnAnimPlay();
    afx_msg void OnUpdateAnimPlay(CCmdUI *pCmdUI);
    afx_msg void OnAnimStop();
    afx_msg void OnUpdateAnimStop(CCmdUI *pCmdUI);
    afx_msg void OnTreeCollapse();
    afx_msg void OnTreeExpand();
    afx_msg void OnUpdatePacketSymbol(CCmdUI *pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__725B73A6_58AD_44DC_A297_22378C963565__INCLUDED_)
