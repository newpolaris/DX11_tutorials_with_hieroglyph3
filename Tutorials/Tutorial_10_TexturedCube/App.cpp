#include <DirectXMath.h>

#include "RenderApplication.h"
#include "Win32RenderWindow.h"
#include "RendererDX11.h"
#include "EvtFrameStart.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "GeometryGeneratorDX11.h"
#include "GeometryActor.h"
#include "PointLight.h"
#include "RotationController.h"

using namespace Glyph3;

class App : public RenderApplication
{
public:
	App();

public:
	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();
	virtual std::wstring GetName();

protected:
	PointLight*					m_pLight;
	GeometryActor*				m_pIndexedActor;
	GeometryActor*				m_pGeometryActor;
	TextActor*					m_pTextActor;
	Actor*						m_pMeshActor;
	GeometryActor*				m_pTextureActor;
};

//--------------------------------------------------------------------------------
App AppInstance; // Provides an instance of the application
//--------------------------------------------------------------------------------

App::App()
{
}
bool App::ConfigureEngineComponents()
{
	if (!ConfigureRenderingEngineComponents(640, 480, D3D_FEATURE_LEVEL_11_1, D3D_DRIVER_TYPE_HARDWARE))
		return false;
	if (!ConfigureRenderingSetup())
		return false;
	return true;
}

void App::ShutdownEngineComponents()
{
	ShutdownRenderingSetup();
	ShutdownRenderingEngineComponents();
}

std::wstring App::GetName()
{
	return( std::wstring( L"TexturedCube" ) );
}

void App::Initialize()
{
	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.

	m_pCamera->Spatial().SetRotation( Vector3f( 0.5f, 0.3f, 0.0f ) );
	m_pCamera->Spatial().SetTranslation( Vector3f( -3.0f, 12.0f, -15.0f ) );
	m_pRenderView->SetColorClearValue( Vector4f( 0.2f, 0.2f, 0.4f, 0.0f ) );

	m_pGeometryActor = new GeometryActor();
	m_pScene->AddActor( m_pGeometryActor );
	m_pGeometryActor->GetNode()->Transform.Position() = Vector3f( 0.0f, 2.5f, 0.0f );

	m_pGeometryActor->UseTransparentMaterial();
	m_pGeometryActor->SetDiffuse( Vector4f( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_pGeometryActor->SetColor( Vector4f( 1.0f, 0.0f, 0.0f, 0.5f ) );
	m_pGeometryActor->UseTexturedMaterial( m_pRenderer11->LoadTexture( L"EyeOfHorus.png" ) );
	m_pGeometryActor->DrawBox( Vector3f( 0.0f, 0.0f, 0.0f ), Vector3f( 3.0f, 3.0f, 3.0f ) );

	RotationController<Node3D>* pGeometryRotController = new RotationController<Node3D>( Vector3f( 0.0f, 1.0f, 0.0f ), 0.4f );
	m_pGeometryActor->GetNode()->Controllers.Attach( pGeometryRotController );

	// Add a single point light to the scene.
	m_pLight = new PointLight();
	m_pScene->AddLight( m_pLight );

	m_pLight->GetNode()->Controllers.Attach( new RotationController<Node3D>( Vector3f( 0.0f, 1.0f, 0.0f ), -1.0f ) );
	m_pLight->GetNode()->Transform.Position() = Vector3f( 0.0f, 50.0f, 0.0f );
	m_pLight->GetBody()->Transform.Position() = Vector3f( 50.0f, 0.0f, 0.0f );
}

void App::Update()
{
	m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr(new EvtFrameStart(m_pTimer->Elapsed())));

	// Update the scene, and then render all cameras within the scene.

	m_pScene->Update( m_pTimer->Elapsed() );
	m_pScene->Render( m_pRenderer11 );

	// Perform the rendering and presentation for the window.
	m_pRenderer11->Present( m_pWindow->GetHandle(), m_pWindow->GetSwapChain() );
}

void App::Shutdown()
{
}