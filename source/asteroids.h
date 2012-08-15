///////////////////////////////////////////////////////////
// PAINT1.H: Header file for the CPaintApp1 class, which
//           represents the application object.
///////////////////////////////////////////////////////////

#pragma once

#include "game.h"

class CAsteroidsApp : public CWinApp
{
public:
    CAsteroidsApp();
	~CAsteroidsApp();

    // Virtual function overrides.
    BOOL InitInstance();
	BOOL OnIdle(LONG lCount);
	int Run();

	void update();
protected:
	CGame *m_Game;
};