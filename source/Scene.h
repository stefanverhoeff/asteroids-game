#pragma once

#include <vector>
#include <windows.h>

#include "GameObject.h"
#include "SpaceShip.h"

class CScene
{
public:
	CScene(void);
	~CScene(void);

	typedef std::vector<CGameObject*> objects_vec;

	bool initDirectX();
	bool initObjects();
	void removeObjects();

	void paint();
	void update();

	void OnKeyDown(UINT nChar);
	void OnKeyUp(UINT nChar);

	void setHwnd(HWND hWndIn) { hWnd = hWndIn;}

	void insertObject(CGameObject *obj);
	void removeObject(CGameObject *obj);

	void insertSpaceShip();

	// Matrix setting methods
	void setupRotation();
	void setupCamera();
	void setupPerspective();

protected:
	objects_vec				m_Objects;
	CSpaceShip			*	m_Ship;
	int						m_NumRocks;

	bool					dxInit;
	HWND					hWnd;
};