#pragma once

#include "SymbolPolygonGrid.h"
#include "resource.h"

struct CIGI_POLYGON_DEF_ELEMENT;

// CPacketDlgSymbolPolygonDef dialog

class CPacketDlgSymbolPolygonDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolPolygonDef)

public:
    CPacketDlgSymbolPolygonDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolPolygonDef();

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
    float          m_LineWidth;
    float          m_StipplePatternLength;
    CEdit          m_EditStipplePattern;
    CButton        m_ButtonAdd;
    CButton        m_ButtonDelete;
    CSymbolPolygonGrid m_Grid;
    CComboBox      m_ComboPrimitive;

    bool FetchLineFromGrid(int row, CIGI_POLYGON_DEF_ELEMENT *pPolygon);
};
