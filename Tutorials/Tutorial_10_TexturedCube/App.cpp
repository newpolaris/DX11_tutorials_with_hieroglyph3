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

	auto actor = new GeometryActor();
	actor->DrawBox(Vector3f(0.0f, 3.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f));
	actor->UseTexturedMaterial( m_pRenderer11->LoadTexture( L"EyeOfHorus_128.png" ) );
	m_pScene->AddActor(actor);
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