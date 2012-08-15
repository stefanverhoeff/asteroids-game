#include ".\bullet.h"

CBullet::CBullet(float initSpeed)
:	CGameObject()
{
	init();
}

CBullet::~CBullet(void)
{
}

void
CBullet::init()
{
	m_Type = OBJECT_BULLET;

	m_Colour = D3DCOLOR_XRGB(255, 255, 0);
	m_Size *= 0.2f;

	// Call parent init
	__super::init();
}