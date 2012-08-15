#include ".\rock.h"

CRock::CRock(bool isBig)
:	CGameObject()
,	m_BigRock(isBig)
{
	init();
}

CRock::~CRock(void)
{
}

void
CRock::init()
{
	m_Type = OBJECT_ROCK;

	if (rand()%3 == 0)
		m_Colour = D3DCOLOR_XRGB(150 + rand()%50, 100 + rand()%50, 100+rand()%50);
	else if ((rand() & 1))
		m_Colour = D3DCOLOR_XRGB(100 + rand()%50, 150 + rand()%50, 100+rand()%50);
	else
		m_Colour = D3DCOLOR_XRGB(100 + rand()%50, 100 + rand()%50, 150+rand()%50);

	if (! m_BigRock)
		m_Size *= 0.5;

	// Call parent init
	__super::init();
}