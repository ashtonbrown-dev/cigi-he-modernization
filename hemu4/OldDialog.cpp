#include "stdafx.h"
#include "OldDialog.h"

namespace
{
const WORD WIN16_LB_ADDSTRING = 0x0401;
const WORD WIN16_CB_ADDSTRING = 0x0403;
const WORD WIN32_CBEM_INSERTITEM = 0x1234;
}

OldDialog::OldDialog(UINT nIDTemplate, CWnd* pParentWnd)
    : CDialog(nIDTemplate, pParentWnd)
{
}

BOOL OldDialog::OnInitDialog()
{
    if (!ExecuteExtendedDlgInit())
    {
        TRACE("Warning: extended dialog initialization failed.\n");
        EndDialog(-1);
        return FALSE;
    }

    LoadDynamicLayoutResource(m_lpszTemplateName);

    if (!UpdateData(FALSE))
    {
        TRACE("Warning: UpdateData failed during dialog initialization.\n");
        EndDialog(-1);
        return FALSE;
    }

    return TRUE;
}

BOOL OldDialog::ExecuteExtendedDlgInit()
{
    HINSTANCE hInstance = AfxFindResourceHandle(m_lpszTemplateName, RT_DLGINIT);
    HRSRC hResource = ::FindResource(hInstance, m_lpszTemplateName, RT_DLGINIT);
    if (hResource == NULL)
        return TRUE;

    HGLOBAL hData = ::LoadResource(hInstance, hResource);
    if (hData == NULL)
        return FALSE;

    UNALIGNED WORD* pRecord = static_cast<UNALIGNED WORD*>(::LockResource(hData));
    if (pRecord == NULL)
        return FALSE;

    while (*pRecord != 0)
    {
        const WORD controlId = *pRecord++;
        const WORD message = *pRecord++;
        const DWORD dataSize = *reinterpret_cast<UNALIGNED DWORD*&>(pRecord)++;
        const char* text = reinterpret_cast<const char*>(pRecord);
        LRESULT result = 0;

        if (message == WIN16_LB_ADDSTRING)
        {
            result = ::SendDlgItemMessageA(m_hWnd, controlId, LB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(text));
        }
        else if (message == WIN16_CB_ADDSTRING)
        {
            result = ::SendDlgItemMessageA(m_hWnd, controlId, CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(text));
        }
        else if (message == WIN32_CBEM_INSERTITEM)
        {
            COMBOBOXEXITEMA item = {};
            item.mask = CBEIF_TEXT;
            item.iItem = -1;
            item.pszText = const_cast<LPSTR>(text);
            result = ::SendDlgItemMessageA(m_hWnd, controlId, CBEM_INSERTITEMA, 0,
                reinterpret_cast<LPARAM>(&item));
        }
        else
        {
            TRACE("Unsupported DLGINIT message 0x%04X for control %u.\n", message, controlId);
            return FALSE;
        }

        if (result == -1)
            return FALSE;

        pRecord = reinterpret_cast<WORD*>(
            reinterpret_cast<BYTE*>(pRecord) + dataSize);
    }

    return TRUE;
}
