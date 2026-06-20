#pragma once

// CDialog compatibility wrapper for Visual Studio-generated DLGINIT records.
// MFC handles ordinary combo boxes, but not the 0x1234 marker emitted for
// ComboBoxEx controls.
class OldDialog : public CDialog
{
public:
    OldDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

protected:
    virtual BOOL OnInitDialog();

private:
    BOOL ExecuteExtendedDlgInit();
};
