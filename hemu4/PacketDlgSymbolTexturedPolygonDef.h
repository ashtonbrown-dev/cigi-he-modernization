#pragma once

#include "SymbolTexturedPolygonGrid.h"
#include "resource.h"

struct CIGI_TEXTURED_POLYGON_DEF_ELEMENT;

// CPacketDlgSymbolTexturedPolygonDef dialog

class CPacketDlgSymbolTexturedPolygonDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolTexturedPolygonDef)

public:
    CPacketDlgSymbolTexturedPolygonDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolTexturedPolygonDef();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_POLYGON_DEF };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnAddLine();
    afx_msg void OnRemoveLine();

    DECLARE_MESSAGE_MAP()

private:
    unsigned int   m_PacketSize;
    unsigned int   m_SymbolID;
    unsigned int   m_TextureID;
    int            m_FilterMode;
    int            m_RepeatClamp;
    CButton        m_ButtonAdd;
    CButton        m_ButtonDelete;
    CComboBox      m_ComboPrimitive;
    CSymbolTexturedPolygonGrid m_Grid;

    bool FetchLineFromGrid(int row, CIGI_TEXTURED_POLYGON_DEF_ELEMENT *pTexturedPolygon);
};
