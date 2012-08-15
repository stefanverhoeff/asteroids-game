// Common header for asteroids game
#pragma once

#define WINDOW_WIDTH	500
#define WINDOW_HEIGHT	500
#define USE_SOUND		0

#include <d3dx8.h>
//#include "Game.h"

struct CUSTOMVERTEX
{
    FLOAT x, y, z;
    DWORD colour;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#define SafeRelease(pObject) if(pObject != NULL) {pObject->Release(); pObject=NULL;}

// Direct3D objects
extern LPDIRECT3D8 g_pD3D;
extern LPDIRECT3DDEVICE8 g_pD3DDevice;

// Matrices
extern D3DXMATRIX matWorld;		// Rotation
extern D3DXMATRIX matView;		// Camera
extern D3DXMATRIX matProj;		// Perspective

// Global game object
class CGame;
extern CGame *game;