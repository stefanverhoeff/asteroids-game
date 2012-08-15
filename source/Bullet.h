#pragma once
#include "gameobject.h"

class CBullet :
	public CGameObject
{
public:
	CBullet(float initSpeed = 10.0f);
	virtual ~CBullet(void);

	virtual void init();
};
