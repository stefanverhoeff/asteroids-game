#include "shared.h"
#include "Game.h"

// Direct3D objects
LPDIRECT3D8 g_pD3D = NULL;
LPDIRECT3DDEVICE8 g_pD3DDevice = NULL;

// Matrices
D3DXMATRIX matWorld;	// Rotation
D3DXMATRIX matView;		// Camera
D3DXMATRIX matProj;		// Perspective

// Global game object
CGame *game = NULL;