#pragma once
#include "afxwin.h"
#include "resource.h"

class CScoresDlg :
	public CDialog
{
public:
	CScoresDlg(CWnd* pParent);

	void setHighScores(CString scores);

// Data members for controls
public:
	CString m_scores;

// Overrides
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
};
