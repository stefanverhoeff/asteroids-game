///////////////////////////////////////////////////////////
// MAINFRM.CPP: Implementation file for the CMainFrame
//              class, which represents the application's
//              main window.
///////////////////////////////////////////////////////////
#include <afxwin.h>
#include "mainfrm.h"
#include "resource.h"
#include "game.h"
#include "scoresdlg.h"

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_GAME_NEW, OnGameNew)
	ON_COMMAND(ID_GAME_EXIT, OnGameExit)
	ON_COMMAND(ID_INFO_ABOUT, OnInfoAbout)
	ON_COMMAND(ID_INFO_HELP, OnInfoHelp)
	ON_COMMAND(ID_GAME_HIGHSCORES, OnGameHighscores)
	ON_WM_CREATE()
	ON_WM_KEYUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////
// CMainFrame: Construction and destruction.
///////////////////////////////////////////////////////////
CMainFrame::CMainFrame(CGame *game)
:	m_Game(game)
{
	Create(NULL, "Asteroids: Attack  Of The Clone", WS_OVERLAPPED | WS_SYSMENU, rectDefault, NULL, MAKEINTRESOURCE(IDR_MENU));
}

CMainFrame::~CMainFrame()
{
}

///////////////////////////////////////////////////////////
// Overrides.
///////////////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // Set size of the main window.
    cs.cx = WINDOW_WIDTH;
    cs.cy = WINDOW_HEIGHT;

    // Call the base class's version.
    return CFrameWnd::PreCreateWindow(cs);
}

afx_msg void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	m_Game->getScene()->OnKeyDown(nChar);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	m_Game->getScene()->OnKeyUp(nChar);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CMainFrame::OnGameNew()
{
	// Reset the game
	m_Game->reset();
	
	// Get the new scene and restart everything
	m_Game->getScene()->setHwnd( this->GetSafeHwnd() );
	if ( m_Game->getScene()->initDirectX() == false )
		MessageBox( "Failed to initialise Direct3D!", "Error", MB_ICONWARNING | MB_OK );
	if ( m_Game->getScene()->initObjects() == false )
		MessageBox( "Failed to initialise game objects!", "Error", MB_ICONWARNING | MB_OK );

}

void CMainFrame::OnGameExit()
{
	exit(0);
}

void CMainFrame::OnInfoAbout()
{
	CDialog dlg(IDD_ABOUT, this);
    dlg.DoModal();
}

void CMainFrame::OnInfoHelp()
{
	CDialog dlg(IDD_HELP, this);
    dlg.DoModal();
}

void CMainFrame::OnGameHighscores()
{
	CScoresDlg dlg(this);
	CGame::highscore_map scores = game->getHighScores();

	// Construct a string out of the map
	CString text = "";
	int place = 1;
	for ( CGame::highscore_map::iterator it = scores.begin(); it != scores.end(); it++ ) {
		std::pair<int, std::string> pair = *it;
		// Convert ints so we can concat
		char bufPlace[100], bufScore[100];
		int score = pair.first;
		itoa(place, bufPlace, 100);
		itoa(score, bufScore, 100);
		CString strPlace;
		CString strScore;
		CString strText;
		strPlace.Format( "%d", place );
		strScore.Format( "%d", score );
		strText.Format( "%2d. %-25s %-6d\r\n", place, pair.second.c_str(), score );

		text = text + strText;
		place++;
	}

	dlg.m_scores = text;
    dlg.DoModal();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Load bitmap
	// Try to load bitmap from resource
	m_HBitmap = (HBITMAP) LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_INTRO), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
	if ( m_HBitmap == NULL ) {
		// Failed so try a file
		m_HBitmap = (HBITMAP) LoadImage( NULL, "back_intro.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	}

	// FIXME: doesn't load the correct icon
	// but a warning icon which is quite cool too
	HICON hIcon;
	hIcon = (HICON) LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_A), IMAGE_ICON, 0, 0, 0 );
	this->SetIcon( hIcon, true );

	return 0;
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_HBitmap == NULL)
		return;

	// Back background
	dc.FillRect(new CRect(CPoint(0,0), CPoint(500,500)), new CBrush((COLORREF)0x00) );

	// Create a memory DC that's compatible
    // with the window's DC.
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    // Display the bitmap in the window.
    memDC.SelectObject(m_HBitmap);
    dc.BitBlt(0, 0, 500, 500, &memDC, 0, 0, SRCCOPY);
}