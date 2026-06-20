#pragma once

#include "resource.h"
#include "SymbolCircleGrid.h"

struct CIGI_CIRCLE_DEF_ELEMENT;

// CPacketDlgSymbolCircleDef dialog

class CPacketDlgSymbolCircleDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolCircleDef)

public:
    CPacketDlgSymbolCircleDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolCircleDef();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_CIRCLE_DEF };

protected:
    virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnAddCircle();
    afx_msg void OnRemoveCircle();

    DECLARE_MESSAGE_MAP()

private:
    unsigned int   m_PacketSize;
    unsigned int   m_SymbolID;
    int            m_DrawingStyle;
    float          m_LineWidth;
    float          m_StipplePatternLength;
    CEdit          m_EditStipplePattern;
    CButton        m_ButtonAdd;
    CButton        m_ButtonDelete;
    CSymbolCircleGrid m_Grid;

    bool FetchCircleFromGrid(int row, CIGI_CIRCLE_DEF_ELEMENT *pCircle);
};
