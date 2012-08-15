#pragma once
#include "gameobject.h"

typedef enum {
	FLY_ACCELERATE,
	FLY_NEUTRAL,
	FLY_BRAKE
} SHIP_FLY_STATE;

typedef enum {
	TURN_LEFT,
	TURN_NEUTRAL,
	TURN_RIGHT
} SHIP_TURN_STATE;

class CSpaceShip :
	public CGameObject
{
public:
	CSpaceShip();
	virtual ~CSpaceShip(void);

	virtual void init();

	void setAccelerate(bool state = true);
	void setBrake(bool state = true);
	void setTurnLeft(bool state = true);
	void setTurnRight(bool state = true);

protected:
	virtual bool initialiseVertexBuffer();
	virtual bool initialiseIndexBuffer();

	SHIP_FLY_STATE	flyState;
	SHIP_TURN_STATE turnState;
};