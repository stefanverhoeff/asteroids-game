#pragma once

#include "Scene.h"

#include <map>
#include <string>

class CGame
{
public:
	CGame(void);
	~CGame(void);

	typedef std::map<int, std::string> highscore_map;

	bool loadHighScores();

	void doOneLoop();
	void loseLive();
	void addPoints(int points);
	void nextLevel();

	void stopRunning()				{ m_Running = false;}
	bool isRunning()				{ return m_Running;}
	CScene *getScene()				{ return m_Scene; }
	highscore_map getHighScores()	{ return m_HighScores;}

	void reset();

	int getScore()		{ return m_Score;}
	int getLives()		{ return m_Lives;}
	int getLevel()		{ return m_Level;}

protected:
	highscore_map m_HighScores;

	int m_Score;
	int m_Lives;
	int m_Level;
	CScene *m_Scene;
	bool m_Paused;
	bool m_Running;

	DWORD m_NextShip;
	bool m_NewShip;
	DWORD m_NextLevel;
	bool m_NewLevel;
};
