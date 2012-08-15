#include "shared.h"
#include ".\scene.h"

#include "Game.h"
#include "gameobject.h"
#include "spaceship.h"
#include "rock.h"
#include "bullet.h"

CScene::CScene(void)
:	dxInit(false)
,	m_Ship(NULL)
,	m_NumRocks(0)
{
}

CScene::~CScene(void)
{
	// Clean up DirectX stuff
	SafeRelease(g_pD3D);
	SafeRelease(g_pD3DDevice);

	// Clean up game objects
	removeObjects();
}

void
CScene::paint(void)
{
	if (!dxInit)
		return;

    if( g_pD3DDevice == NULL )
    {
        return;
    }

    //Clear the backbuffer to black
    g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    //Begin the scene
    g_pD3DDevice->BeginScene();

    //Setup the rotation, camera, and perspective matrices
    setupRotation();
    setupCamera();
    setupPerspective();

	// Call paint on objects
	for (objects_vec::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it) {
		CGameObject *obj = *it;
		if (obj->isDead())
			continue;
		obj->paint();
	}

    //End the scene
    g_pD3DDevice->EndScene();

    //Filp the back and front buffers so that whatever has been rendered on the back buffer
    //will now be visible on screen (front buffer).
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void
CScene::update(void)
{
	objects_vec testObjects(m_Objects);	// make a local copy of vector
	bool explodeRock, explodeShip;

	// Crappy collission detection for bullet-rock and ship-rock
	while ( testObjects.size() > 0 ) {
		explodeRock = false;
		explodeShip = false;

		CGameObject *obj = testObjects[0];

		if ((obj->getType() == OBJECT_BULLET || obj == m_Ship) && obj->isDead() == false && obj->isInvulnerable() == false ) {
			for (objects_vec::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it) {
				CGameObject *obj2 = *it;

				if ( obj2->isDead() )
					continue;
				if (obj2->getType() != OBJECT_ROCK)
					continue;
				if ( obj->collission(obj2) ) {
 					CRock *rock = (CRock *) obj2; // Cast should be okey
					if ( rock->isBig() ) {
#if USE_SOUND
						PlaySound( "detpack.wav", NULL, NULL );
#endif
						explodeRock = true;
						if (obj->getType() == OBJECT_BULLET)
							game->addPoints( 100 );
					} else {
						if (obj->getType() == OBJECT_BULLET)
							game->addPoints( 25 );
					}

					// Remove old rock and object from the scene
					rock->setDeathTime(-1);
					obj->setDeathTime(-1);
     				m_NumRocks--;

					if (obj->getType() == OBJECT_SHIP) {
 						game->loseLive();
						explodeShip = true;
					}
				}
			}

			// Create the small rocks left after the explosion
			if (explodeRock) {
				for (int i=0; i < 4 + (rand()&1); i++) {
					CRock *newRock = new CRock(false);
					newRock->setOrigin( obj->getOrigin() );
					newRock->setDeltaOrigin(D3DXVECTOR3((rand() % 100)/500.0f - 0.05f, (rand() % 100)/500.0f - 0.05f, 0.0f));
					newRock->setDeltaRotation(D3DXVECTOR3(0.0f, 0.0f, (rand() % 100)/500.0f - 0.05f));
					m_Objects.push_back( newRock );
					m_NumRocks++;
				}
			}
			if (explodeShip) {
				// Some shatters from the ship
				for (int i=0; i < 4 + (rand()%4); i++) {
					CSpaceShip *particle = new CSpaceShip();
					particle->setOrigin( m_Ship->getOrigin() );
					particle->setSize(D3DXVECTOR3(0.02f * (1+rand()%4) / 2.0f, 0.02f * (1+rand()%4) / 2.0f, 0.02f * (rand()%4) / 2.0f));
					particle->setDeltaOrigin(D3DXVECTOR3((1+rand() % 100)/500.0f - 0.1f, (1+rand() % 100)/500.0f - 0.1f, 0.0f));
					particle->setDeltaRotation(D3DXVECTOR3(0.0f, 0.0f, (1+rand() % 100)/500.0f - 0.025f));
					particle->setDecayRotation(1.0f);
					particle->setDeathTime( 1500 );
					m_Objects.push_back( particle );
				}

				// Some bullets for extra effect
				for (int i=0; i < 4 + (rand()%4); i++) {
					CBullet *particle = new CBullet();
					particle->setOrigin( m_Ship->getOrigin() );
					particle->setSize(D3DXVECTOR3(0.02f * (1+rand()%4) / 2.0f, 0.02f * (1+rand()%4) / 2.0f, 0.02f * (1+rand()%4) / 2.0f));
					particle->setDeltaOrigin(D3DXVECTOR3((1+rand() % 100)/250.0f - 0.02f, (1+rand() % 100)/250.0f - 0.02f, 0.0f));
					//particle->setDeltaRotation(D3DXVECTOR3(0.0f, 0.0f, (rand() % 100)/500.0f - 0.025f));
					//particle->setDecayRotation(1.0f);
					particle->setDeathTime( 500 );
					m_Objects.push_back( particle );
				}
			}
		}

		testObjects.erase( testObjects.begin() );
	}

	// Call update on objects
	for (objects_vec::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it) {
		CGameObject *obj = *it;

		if ( obj->isDead() ) {
			// FIXME:
			// Should remove from list, skip for now
			continue;
		}

		obj->update();
	}

	if (m_NumRocks == 0)
		game->nextLevel();
}

bool
CScene::initDirectX()
{
    //First of all, create the main D3D object. If it is created successfully we
    //should get a pointer to an IDirect3D8 interface.
    g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);
    if(g_pD3D == NULL)
    {
        return false;
    }

    //Get the current display mode
    D3DDISPLAYMODE d3ddm;
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return false;
    }

    //Create a structure to hold the settings for our device
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    //Fill the structure.
    //We want our program to be windowed, and set the back buffer to a format
    //that matches our current display mode
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
    d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;

    //Create a Direct3D device.
    if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
    {
        return false;
    }
    
    // Turn on back face culling. This is because we want to hide the back of our polygons
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    // Turn off lighting becuase we are specifying that our vertices have colour
    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on depth-buffering
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	dxInit = true;
	return true;
}

void
CScene::insertSpaceShip()
{
	m_Ship = new CSpaceShip();
	m_Ship->setInvulnerable(3000); // 3 secs time to move to a safe location

	// Insert the spaceship
	m_Objects.push_back( m_Ship );
}

void
CScene::removeObjects()
{
	for (objects_vec::iterator it = m_Objects.begin(); it != m_Objects.end(); ++it) {
		CGameObject *obj = *it;
		if (obj != NULL)
			delete obj;
	}

	m_Ship = NULL;
	m_Objects.clear();
}

bool
CScene::initObjects()
{
	insertSpaceShip();

	// Some random rocks
	for (int i=0; i < 3 + game->getLevel() + (rand()%4); i++) {
		CRock *rock = new CRock(rand()&1);

		// Random position and direction
		rock->setOrigin(D3DXVECTOR3((rand() % 100)/100.0f * 10.0f - 5.0f, (rand() % 100)/100.0f * 10.0f - 5.0f, 0.0f));
		rock->setDeltaOrigin(D3DXVECTOR3(((rand() % 100)/1000.0f - 0.05f) * game->getLevel(), ((rand() % 100)/1000.0f - 0.05f) * game->getLevel(), 0.0f));
		rock->setDeltaRotation(D3DXVECTOR3(0.0f, 0.0f, (rand() % 100)/1000.0f - 0.05f));

		m_Objects.push_back( rock );
		m_NumRocks++;
	}

	return true;
}

void
CScene::insertObject(CGameObject *obj)
{
	m_Objects.push_back( obj );
}

void
CScene::removeObject(CGameObject *obj)
{
	// TODO: implement this
//	m_Objects.erase( obj );
}

void
CScene::setupRotation()
{
    //Here we will rotate our world around the x, y and z axis.
    D3DXMATRIX matWorldX, matWorldY, matWorldZ;

    //Create the transformation matrices
    D3DXMatrixRotationX(&matWorldX, 0.0f);
    D3DXMatrixRotationY(&matWorldY, 0.0f);
    D3DXMatrixRotationZ(&matWorldZ, 0.0f);

    //Combine the transformations by multiplying them together
    D3DXMatrixMultiply(&matWorld, &matWorldX, &matWorldY);
    D3DXMatrixMultiply(&matWorld, &matWorld, &matWorldZ);

    //Apply the tansformation
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void
CScene::setupCamera()
{
    //Here we will setup the camera.
    //The camera has three settings: "Camera Position", "Look at Position" and "Up Direction"
    //We have set the following:
    //Camera Position: (0, 0, -30)
    //Look at Position: (0, 0, 0)
    //Up direction: Y-Axis.
    D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0.0f, 0.0f,-10.0f), //Camera Position
                                 &D3DXVECTOR3(0.0f, 0.0f, 0.0f), //Look At Position
                                 &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); //Up Direction
    g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void
CScene::setupPerspective()
{
    //Here we specify the field of view, aspect ration and near and far clipping planes.
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 500.0f);
    g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void
CScene::OnKeyDown(UINT nChar)
{
	static DWORD nextShot = 0;

	switch (nChar) {
		case VK_UP:
			m_Ship->setAccelerate(true);
			break;
		case VK_DOWN:
			m_Ship->setBrake(true);
			break;
		case VK_RIGHT:
			m_Ship->setTurnRight(true);
			break;
		case VK_LEFT:
			m_Ship->setTurnLeft(true);
			break;
		case VK_SPACE:
			if (m_Ship->isDead() || nextShot > timeGetTime())
				break;

			// Fire bullet from spaceship
			CBullet *bullet = new CBullet();
			D3DXVECTOR3 deltaOrigin;

			deltaOrigin.x = cos( m_Ship->getRotation().z ) / 5.0f;
			deltaOrigin.y = sin( m_Ship->getRotation().z ) / 5.0f;
			deltaOrigin.z = 0.0f;

			bullet->setOrigin( m_Ship->getOrigin() );
			bullet->setDeltaOrigin( deltaOrigin );	// Bullet speed = ship speed + acceleration
			bullet->setDeathTime( 1000 );
			m_Objects.push_back( bullet );
#if USE_SOUND
			PlaySound( "sniper.wav", NULL, NULL );
#endif

			nextShot = timeGetTime() + 200;
			break;
	}
}

void
CScene::OnKeyUp(UINT nChar)
{
	switch (nChar) {
		case VK_UP:
			m_Ship->setAccelerate(false);
			break;
		case VK_DOWN:
			m_Ship->setBrake(false);
			break;
		case VK_RIGHT:
			m_Ship->setTurnRight(false);
			break;
		case VK_LEFT:
			m_Ship->setTurnLeft(false);
			break;
	}
}