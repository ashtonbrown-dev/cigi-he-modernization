#pragma once

#include "resource.h"
#include "SymbolTexturedCircleGrid.h"

struct CIGI_TEXTURED_CIRCLE_DEF_ELEMENT;

// CPacketDlgSymbolTexturedCircleDef dialog

class CPacketDlgSymbolTexturedCircleDef : public CDialog
{
    DECLARE_DYNAMIC(CPacketDlgSymbolTexturedCircleDef)

public:
    CPacketDlgSymbolTexturedCircleDef(CWnd *pParent = NULL);   // standard constructor
    virtual ~CPacketDlgSymbolTexturedCircleDef();

    // Dialog Data
    enum { IDD = IDD_PACKET_SYMBOL_TEXTURED_CIRCLE_DEF };

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
    unsigned int   m_TextureID;
    int            m_FilterMode;
    int            m_RepeatClamp;
    CButton        m_ButtonAdd;
    CButton        m_ButtonDelete;
    CSymbolTexturedCircleGrid m_Grid;

    bool FetchCircleFromGrid(int row, CIGI_TEXTURED_CIRCLE_DEF_ELEMENT *pCircle);
};
