#include ".\game.h"
#include "scene.h"

extern "C" {
#include <stdio.h>
#include <string.h>
}

CGame::CGame(void)
:	m_Score(0)
,	m_Lives(5)
,	m_Level(1)
,	m_Scene(NULL)
,	m_Paused(false)
,	m_Running(true)
,	m_NextShip(0)
,	m_NewShip(false)
,	m_NextLevel(0)
,	m_NewLevel(false)
{
	loadHighScores();
	//m_Scene = new CScene();
}

CGame::~CGame(void)
{
	if (m_Scene != NULL)
		delete m_Scene;
}

bool
CGame::loadHighScores()
{
	// C hacking is nice once in a while
	FILE *fp;
	char buffer[100];

	fp = fopen("highscores.txt", "r");
	if (fp == NULL)
		return false;

	while ( 1 ) {
		std::string name;
		char buf[100];
		int score;
		int index = 0;

		// Fill the buffer the old-fashioned way
		for ( int i = 0; i < 100; i++ ) {
			buffer[i] = fgetc(fp);
			if (buffer[i] == '\n' || buffer[i] == EOF)
				break;
		}

		if (buffer[i] == EOF)
			break;
		buffer[i] = '\0';

		char *c = strchr(buffer, ':');

		while (buffer[index] != ':') {
			buf[index] = buffer[index];
			index++;
		}
		buf[index] = '\0';

		// sneaky pointer tricking :-)
		score = atoi(c+1);
		// overloaded operator
		name = buf;

		std::pair<int, std::string> entry;
		entry.first = score;
		entry.second = name;
		m_HighScores.insert(entry);
	}

	fclose(fp);
	return true;
}

void
CGame::loseLive()
{
	m_Lives--;

	if (m_Lives > 0) {
		// Insert new SpaceShip in 2 seconds
		m_NextShip = timeGetTime() + 2000;
		m_NewShip = true;
	}
}

void
CGame::addPoints(int points)
{
	m_Score += points;
}

void
CGame::nextLevel()
{
	if (m_NewLevel)
		return;

	m_Level++;

	m_NextLevel = timeGetTime() + 2500;
	m_NewLevel = true;
}

void
CGame::reset()
{
	// Start a new game
	m_Score = 0;
	m_Lives = 5;
	m_Level = 1;
	m_Paused = false;
	m_Running = true;
	m_NextShip = 0;
	m_NewShip = false;
	m_NextLevel = 0;
	m_NewLevel = false;

	if (m_Scene != NULL)
		delete m_Scene;
	m_Scene = new CScene();
}

void
CGame::doOneLoop(void)
{
	static bool init = false;

	static LONGLONG cur_time;
	static DWORD time_count;
	static LONGLONG perf_cnt;
	static BOOL perf_flag = FALSE;

	static LONGLONG next_time = 0;
	static LONGLONG last_time = 0;
	static double time_elapsed;
	static double time_scale;

	if (!init) {
		// Check what timer to use
		if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)) {
			// Use a performance timer
			perf_flag = TRUE;
			time_count = (DWORD)( perf_cnt / 30 );
			QueryPerformanceCounter((LARGE_INTEGER *) &next_time);
			time_scale = 1.0/perf_cnt;
		} else {
			// Use default timer function
			next_time = timeGetTime();
			time_scale = 0.001;
			time_count = 33;
		}

		init = true;
	}

	// Check which timer to use
	if (perf_flag)
		QueryPerformanceCounter((LARGE_INTEGER *) &cur_time);
	else
		cur_time = timeGetTime();

	if (cur_time > next_time) {
		time_elapsed = (cur_time - last_time) * time_scale;
		last_time = cur_time;

		if (m_Scene != NULL) {
			if ( m_NewShip && timeGetTime() > m_NextShip ) {
				m_Scene->insertSpaceShip();
				m_NewShip = false;
			}
			if ( m_NewLevel && timeGetTime() > m_NextLevel ) {
				m_Scene->removeObjects();
				m_Scene->initObjects();
				m_NewLevel = false;
			}

			// Update positions of all objects
			m_Scene->update();

			// Draw the scene
			m_Scene->paint();
		}

		next_time = cur_time + time_count;
	}
}