#include ".\scoresdlg.h"

CScoresDlg::CScoresDlg(CWnd* pParent)
:	CDialog(IDD_HIGHSCORES, pParent)
{
	m_scores = "Hiscores go here";
}

void
CScoresDlg::setHighScores(CString scores)
{
	;
}

void
CScoresDlg::DoDataExchange(CDataExchange* pDX)
{
    // Call the base class's version.
    CDialog::DoDataExchange(pDX);
    // Associate the data transfer variables with
    // the ID's of the controls.
    DDX_Text(pDX, IDC_EDIT_SCORES, m_scores);
}