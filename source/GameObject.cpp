#include ".\gameobject.h"
#include <windows.h>

CGameObject::CGameObject(DWORD colourIn)
:	m_Rotation(0.0f, 0.0f, 0.0f)
,	m_Origin(0.0f, 0.0f, 0.0f)
,	m_Size(0.2f, 0.2f, 0.2f)
,	m_DeltaRotation(0.0f, 0.0f, 0.0f)
,	m_DeltaOrigin(0.0f, 0.0f, 0.0f)
,	m_DeltaSize(0.0f, 0.0f, 0.0f)
,	m_DecayRotation(1.0f)
,	m_DecayOrigin(1.0f)
,	m_DecaySize(1.0f)
,	m_Colour(colourIn)
,	m_NumVertices(8)
,	m_NumPolygons(12)
,	m_DeathTime(0)
,	m_InvulnerableTime(0)
,	m_Type(OBJECT_UNKNOWN)
{
	init();
}

CGameObject::CGameObject(LPCTSTR fileName)
{
	init();

	// Model file loading stuff here
}

CGameObject::CGameObject(D3DXVECTOR3 rotationIn, D3DXVECTOR3 originIn, D3DXVECTOR3 sizeIn, DWORD colourIn)
:	m_Rotation(rotationIn)
,	m_Origin(originIn)
,	m_Size(sizeIn)
,	m_DeltaRotation(0.0f, 0.0f, 0.0f)
,	m_DeltaOrigin(0.0f, 0.0f, 0.0f)
,	m_DeltaSize(0.0f, 0.0f, 0.0f)
,	m_DecayRotation(1.0f)
,	m_DecayOrigin(1.0f)
,	m_DecaySize(1.0f)
,	m_Colour(colourIn)
,	m_NumVertices(8)
,	m_NumPolygons(12)
,	m_DeathTime(0)
,	m_InvulnerableTime(0)
,	m_Type(OBJECT_UNKNOWN)
{
	init();
}

CGameObject::~CGameObject(void)
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void
CGameObject::init()
{
	initialiseIndexBuffer();
	initialiseVertexBuffer();
}

void
CGameObject::update(void)
{
	// Round delta vectors
	if (m_DeltaOrigin.x > 0.5)	m_DeltaOrigin.x = 0.5;
	if (m_DeltaOrigin.x < -0.5)	m_DeltaOrigin.x = -0.5;
	if (m_DeltaOrigin.y > 0.5)	m_DeltaOrigin.y = 0.5;
	if (m_DeltaOrigin.y < -0.5)	m_DeltaOrigin.y = -0.5;
	if (m_DeltaOrigin.z > 0.5)	m_DeltaOrigin.z = 0.5;
	if (m_DeltaOrigin.z < -0.5)	m_DeltaOrigin.z = -0.5;

	if (m_DeltaRotation.z > 0.2)	m_DeltaRotation.z = 0.2f;
	if (m_DeltaRotation.z < -0.2)	m_DeltaRotation.z = -0.2f;

	// Apply delta movement
	m_Rotation	+= m_DeltaRotation;
	m_Origin	+= m_DeltaOrigin;
	m_Size		+= m_DeltaSize;

	// Apply decay to delta vectors
	m_DeltaRotation	*= m_DecayRotation;
	m_DeltaOrigin	*= m_DecayOrigin;
	m_DeltaSize		*= m_DecaySize;

	// Round vectors
	if (m_Rotation.x > 2*D3DX_PI || m_Rotation.x < -2*D3DX_PI) m_Rotation.x -= 2*D3DX_PI;
	if (m_Rotation.y > 2*D3DX_PI || m_Rotation.y < -2*D3DX_PI) m_Rotation.y -= 2*D3DX_PI;
	if (m_Rotation.z > 2*D3DX_PI || m_Rotation.z < -2*D3DX_PI) m_Rotation.z -= 2*D3DX_PI;

	// Flip objects across the screen when they cross the border
	if (m_Origin.x > 4.2f)	m_Origin.x = -4.2f;
	if (m_Origin.x < -4.2f)	m_Origin.x = 4.2f;
	if (m_Origin.y > 4.2f)	m_Origin.y = -4.2f;
	if (m_Origin.y < -4.2f)	m_Origin.y = 4.2f;
	if (m_Origin.z > 4.2f)	m_Origin.z = -4.2f;
	if (m_Origin.z < -4.2f)	m_Origin.z = 4.2f;
	/*
	if (m_Origin.x > 4.2 || m_Origin.x < -4.2 ||
		m_Origin.y > 4.2 || m_Origin.y < -4.2 ||
		m_Origin.z > 4.2 || m_Origin.z < -4.2)
	{
		m_Origin.x = -m_Origin.x;
		m_Origin.y = -m_Origin.y;
		m_Origin.z = -m_Origin.z;

		if (m_DeltaOrigin.x > -0.001f && m_DeltaOrigin.x < 0.001f) m_DeltaOrigin.x *= 10;
		if (m_DeltaOrigin.y > -0.001f && m_DeltaOrigin.y < 0.001f) m_DeltaOrigin.y *= 10;
		if (m_DeltaOrigin.z > -0.001f && m_DeltaOrigin.z < 0.001f) m_DeltaOrigin.z *= 10;
	}
	*/

	// Insert values into matrix
	setupScaling();
	setupRotation();
	setupTranslation();
}

bool
CGameObject::paint()
{
	if (g_pD3DDevice == NULL)
		return false;

	// Set the transform to local matrix
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatWorld);

	g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, sizeof(CUSTOMVERTEX));
    g_pD3DDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);
	g_pD3DDevice->SetIndices(m_pIndexBuffer, 0);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, m_NumVertices, 0, m_NumPolygons);

	// Set orientation matrix back to original
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	return true;
}

bool
CGameObject::collission(CGameObject *obj)
{
	if (	obj->getOrigin().x + obj->getSize().x > m_Origin.x - m_Size.x
		&&	obj->getOrigin().x - obj->getSize().x < m_Origin.x + m_Size.x
		&&	obj->getOrigin().y + obj->getSize().y > m_Origin.y - m_Size.y
		&&	obj->getOrigin().y - obj->getSize().y < m_Origin.y + m_Size.y )
		return true;
	else
		return false;
}


void
CGameObject::setupRotation()
{
	D3DXMATRIX matWorldX, matWorldY, matWorldZ;

    // Rotate to global orientation
	D3DXMatrixRotationX(&matWorldX, m_Rotation.x);
	D3DXMatrixRotationY(&matWorldY, m_Rotation.y);
	D3DXMatrixRotationZ(&matWorldZ, m_Rotation.z);

	// Combine the transformations by multiplying them together
	D3DXMatrixMultiply(&m_MatWorld, &matWorld, &matWorldX);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matWorldY);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matWorldZ);
}

void
CGameObject::setupTranslation()
{
	D3DXMATRIX matMove;

	D3DXMatrixTranslation(&matMove, m_Origin.x, m_Origin.y, m_Origin.z);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matMove);
}

void
CGameObject::setupScaling()
{
	D3DXMATRIX matScale;

	D3DXMatrixScaling(&matScale, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_MatWorld, &m_MatWorld, &matScale);
}

bool
CGameObject::initialiseVertexBuffer()
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
		{-m_Size.x, -m_Size.y, -m_Size.z, m_Colour},
		{-m_Size.x, m_Size.y, -m_Size.z, m_Colour},
		{m_Size.x, -m_Size.y, -m_Size.z, m_Colour},
		{m_Size.x, m_Size.y, -m_Size.z, m_Colour},
		{m_Size.x, -m_Size.y, m_Size.z, m_Colour},
		{m_Size.x, m_Size.y, m_Size.z, m_Colour},
		{-m_Size.x, -m_Size.y, m_Size.z, m_Colour},
		{-m_Size.x, m_Size.y, m_Size.z, m_Colour}
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
CGameObject::initialiseIndexBuffer()
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
		,2,1,3
		,2,3,4
		,4,3,5
		,4,5,6
		,6,5,7
		,6,7,0
		,0,7,1
		,1,7,3
		,3,7,5
		,0,2,6
		,2,4,6
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

void
CGameObject::setDeathTime(DWORD time)
{
	m_DeathTime = timeGetTime() + time;
}

bool
CGameObject::isDead()
{
	return m_DeathTime != 0 && m_DeathTime < timeGetTime();
}

bool
CGameObject::isInvulnerable()
{
	return m_InvulnerableTime > timeGetTime(); 
}

void
CGameObject::setInvulnerable(int miliSecs)
{
	m_InvulnerableTime = timeGetTime() + miliSecs;
}