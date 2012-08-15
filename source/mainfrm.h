///////////////////////////////////////////////////////////
// MAINFRM.H: Header file for the CMainFrame class, which
//            represents the application's main window.
///////////////////////////////////////////////////////////
// Enumeration for GDI object to display.

#pragma once

#include "Game.h"

class CMainFrame : public CFrameWnd
{
// Protected data members.
protected:
	CGame *m_Game;
	HBITMAP m_HBitmap;

// Constructor and destructor.
public:
    CMainFrame(CGame *game);
    ~CMainFrame();
// Overrides.
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// Message map functions.
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  
// Protected member functions.
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnGameNew();
	afx_msg void OnGameExit();
	afx_msg void OnInfoAbout();
	afx_msg void OnInfoHelp();
	afx_msg void OnGameHighscores();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
};