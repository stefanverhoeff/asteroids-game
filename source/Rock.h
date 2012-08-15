#pragma once
#include "gameobject.h"

class CRock :
	public CGameObject
{
public:
	CRock(bool isBig = false);
	virtual ~CRock();

	virtual void init();

	bool isBig()	{ return m_BigRock;}
private:
	bool m_BigRock;
};
