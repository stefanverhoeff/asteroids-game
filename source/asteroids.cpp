///////////////////////////////////////////////////////////
// PAINT1.CPP: Implementation file for the CPaintApp1,
//             class, which represents the application
//             object.
///////////////////////////////////////////////////////////
#include <afxwin.h>
#include "asteroids.h"
#include "mainfrm.h"
#include "shared.h"
#include ".\asteroids.h"

// Global application object.
CAsteroidsApp AsteroidsApp;

///////////////////////////////////////////////////////////
// Construction/Destruction.
///////////////////////////////////////////////////////////
CAsteroidsApp::CAsteroidsApp()
{
	m_Game = new CGame();
	game = m_Game;
}

CAsteroidsApp::~CAsteroidsApp()
{
	if (m_Game != NULL)
		delete m_Game;
}

///////////////////////////////////////////////////////////
// Overrides
///////////////////////////////////////////////////////////
BOOL CAsteroidsApp::InitInstance()
{
	// Initialise random seed
	srand( (unsigned)time( NULL ) );

    m_pMainWnd = new CMainFrame(m_Game);
	//HICON hIcon
	//m_pMainWnd->SetIcon( hIcon, true );
    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

BOOL CAsteroidsApp::OnIdle(LONG lCount)
{
	CWinApp::OnIdle(lCount);
	update();

	return TRUE;
}

// Override to implement gameloop
int CAsteroidsApp::Run()
{
	ASSERT_VALID(this);
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	// acquire and dispatch messages until a WM_QUIT message is received.
	for (;;)
	{
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; // assume "no idle" state
		}

		// phase2: pump messages while available
		do
		{
			// pump message, but quit on WM_QUIT
			if (!PumpMessage())
				return ExitInstance();

			// reset "no idle" state after pumping "normal" message
			//if (IsIdleMessage(&m_msgCur))
			if (IsIdleMessage(&(pState->m_msgCur)))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

			update();
		} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
	}
}

void CAsteroidsApp::update()
{
	// Do one iteration of the gameloop
	if (m_Game != NULL)
		m_Game->doOneLoop();
}
