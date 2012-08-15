#include "shared.h"
#include <windows.h>

#pragma once

typedef enum {
	OBJECT_SHIP,
	OBJECT_ROCK,
	OBJECT_BULLET,
	OBJECT_UNKNOWN
} GAME_OBJECT_TYPE;

class CGameObject
{
public:
	CGameObject(LPCTSTR fileName);
	CGameObject(DWORD colourIn = D3DCOLOR_XRGB(255, 0, 0));
	CGameObject(D3DXVECTOR3 rotationIn, D3DXVECTOR3 originIn, D3DXVECTOR3 sizeIn, DWORD colourIn = D3DCOLOR_XRGB(255, 0, 0));
	~CGameObject(void);

	// Basic object manipulation
	virtual void init();
	virtual void update();
	virtual bool paint();
	virtual bool collission(CGameObject *obj);
	
	void setDeathTime(DWORD time);
	bool isDead();

	// Object property accessors
	void setRotation(D3DXVECTOR3 &val)			{ m_Rotation = val;}
	void setOrigin(D3DXVECTOR3 &val)			{ m_Origin = val;}
	void setSize(D3DXVECTOR3 &val)				{ m_Size = val;}

	void setDeltaRotation(D3DXVECTOR3 &delta)	{ m_DeltaRotation = delta;}
	void setDeltaOrigin(D3DXVECTOR3 &delta)		{ m_DeltaOrigin = delta;}
	void setDeltaSize(D3DXVECTOR3 &delta)		{ m_DeltaSize = delta;}

	void setDecayRotation(float decay)			{ m_DecayRotation = decay;}
	void setDecayOrigin(float decay)			{ m_DecayOrigin = decay;}
	void setDecaySize(float decay)				{ m_DecaySize = decay;}

	D3DXVECTOR3& getRotation()					{ return m_Rotation;}
	D3DXVECTOR3& getOrigin()					{ return m_Origin;}
	D3DXVECTOR3& getSize()						{ return m_Size;}

	D3DXVECTOR3& getDeltaRotation()				{ return m_DeltaRotation;}
	D3DXVECTOR3& getDeltaOrigin()				{ return m_DeltaOrigin;}
	D3DXVECTOR3& getDeltaSize()					{ return m_DeltaSize;}

	GAME_OBJECT_TYPE getType()					{ return m_Type;}

	bool isInvulnerable();
	void setInvulnerable(int miliSecs);

protected:
	// Internal Direct3D methods
	virtual bool initialiseVertexBuffer();
	virtual bool initialiseIndexBuffer();

	void setupRotation();
	void setupTranslation();
	void setupScaling();

	// Direct3D buffers
	LPDIRECT3DVERTEXBUFFER8 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER8 m_pIndexBuffer;

	int m_NumVertices;
	int m_NumPolygons;

	// Custom object world matrix
	D3DXMATRIX m_MatWorld;

	// Location, orientation properties
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Origin;
	D3DXVECTOR3 m_Size;

	// Delta movement vectors
	D3DXVECTOR3 m_DeltaRotation;
	D3DXVECTOR3 m_DeltaOrigin;
	D3DXVECTOR3 m_DeltaSize;

	// Decay factors
	float m_DecayRotation;
	float m_DecayOrigin;
	float m_DecaySize;

	DWORD m_Colour;
	DWORD m_DeathTime;	// Time until removed from the scene in ms
	DWORD m_InvulnerableTime;

	GAME_OBJECT_TYPE m_Type;
};