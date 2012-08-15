#include ".\spaceship.h"

CSpaceShip::CSpaceShip()
:	CGameObject()
{
	init();
}

CSpaceShip::~CSpaceShip(void)
{
}

void
CSpaceShip::init()
{
	m_Type = OBJECT_SHIP;

	m_Colour = D3DCOLOR_XRGB(0, 0, 255);
	m_DecayOrigin   = 0.97f;
	m_DecayRotation = 0.9f;

	m_NumVertices = 3;
	m_NumPolygons = 1;

	// Call parent init
	__super::init();
}

void
CSpaceShip::setAccelerate(bool state)
{
	if (state) {
		m_DeltaOrigin.x += cos(m_Rotation.z) / 15.0f;
		m_DeltaOrigin.y += sin(m_Rotation.z) / 15.0f;
		flyState = FLY_ACCELERATE;
	} else {
		flyState = FLY_NEUTRAL;
	}
}

void
CSpaceShip::setBrake(bool state)
{
	if (state) {
		m_DeltaOrigin.x *= 0.5f;
		m_DeltaOrigin.y *= 0.5f;
		flyState = FLY_BRAKE;
	}
	else {
		flyState = FLY_NEUTRAL;
	}
}

void
CSpaceShip::setTurnLeft(bool state)
{
	if (state) {
		m_DeltaRotation.z += 0.05f;
		turnState = TURN_LEFT;
	} else {
		turnState = TURN_NEUTRAL;
	}
}

void
CSpaceShip::setTurnRight(bool state)
{
	if (state) {
		m_DeltaRotation.z -= 0.05f;
		turnState = TURN_RIGHT;
	} else {
		turnState = TURN_NEUTRAL;
	}
}

bool
CSpaceShip::initialiseVertexBuffer()
{
	if (!g_pD3DDevice)
		return false;

    VOID* pVertices;
    
    //Store each point of the cube together with it's colour
    //Make sure that the points of a polygon are specified in a clockwise direction,
    //this is because anti-clockwise faces will be culled
    //We will use a three triangle strips to render these polygons (Top, Sides, Bottom).
    CUSTOMVERTEX cvVertices[] =
    {
		{-m_Size.x, -m_Size.y, 0.0, m_Colour},
		{-m_Size.x, m_Size.y, 0.0, m_Colour},
		{m_Size.x, 0.0, 0.0, m_Colour}
    };

    //Create the vertex buffer from our device.
    if(FAILED(g_pD3DDevice->CreateVertexBuffer(m_NumVertices * sizeof(CUSTOMVERTEX),
                                               0, D3DFVF_CUSTOMVERTEX,
                                               D3DPOOL_DEFAULT, &m_pVertexBuffer)))
    {
        return false;
    }

    //Get a pointer to the vertex buffer vertices and lock the vertex buffer
    if(FAILED(m_pVertexBuffer->Lock(0, m_NumVertices * sizeof(CUSTOMVERTEX), (BYTE**)&pVertices, 0)))
    {
        return false;
    }

    //Copy our stored vertices values into the vertex buffer
    memcpy(pVertices, cvVertices, m_NumVertices * sizeof(CUSTOMVERTEX));

    //Unlock the vertex buffer
    m_pVertexBuffer->Unlock();

    return true;
}

bool
CSpaceShip::initialiseIndexBuffer()
{
	if (!g_pD3DDevice)
		return false;

    VOID* pIndexes;
    
    //Store each point of the cube together with it's colour
    //Make sure that the points of a polygon are specified in a clockwise direction,
    //this is because anti-clockwise faces will be culled
    //We will use a three triangle strips to render these polygons (Top, Sides, Bottom).
    WORD cvIndexes[] =
    {
		0,1,2
	};

	// Create the index buffer from our device.
    if(FAILED(g_pD3DDevice->CreateIndexBuffer(m_NumVertices*m_NumPolygons * sizeof(WORD),
                                           0, D3DFMT_INDEX16,
                                           D3DPOOL_MANAGED, &m_pIndexBuffer)))
    {
        return false;
    }

    // Get a pointer to the index buffer indexes and lock the index buffer
    if(FAILED(m_pIndexBuffer->Lock(0, m_NumVertices*m_NumPolygons * sizeof(WORD), (BYTE**)&pIndexes, 0)))
    {
        return false;
    }

    // Copy our stored indexed values into the index buffer
    memcpy(pIndexes, cvIndexes, m_NumVertices*m_NumPolygons * sizeof(WORD));

    // Unlock the index buffer
    m_pIndexBuffer->Unlock();

    return true;
}