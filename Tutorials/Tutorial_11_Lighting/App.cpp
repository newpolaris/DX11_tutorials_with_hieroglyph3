#include <DirectXMath.h>

#include "RenderApplication.h"
#include "Win32RenderWindow.h"
#include "RendererDX11.h"
#include "EvtFrameStart.h"
#include "EvtErrorMessage.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"
#include "GeometryGeneratorDX11.h"
#include "GeometryActor.h"
#include "FirstPersonCamera.h"
#include "LightView.h"
#include "RotationController.h"

#include "FileSystem.h"

using namespace Glyph3;

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
};

struct LightInfo
{
    Vector3f Strength = { 0.5f, 0.5f, 0.5f };
    FScalar FalloffStart = 1.0f;                // point/spot light only
    Vector3f Direction = { 0.0f, -1.0f, 0.0f }; // directional/spot light only
    FScalar FalloffEnd = 10.0f;                 // point/spot light only
    Vector3f Position = { 0.0f, 0.0f, 0.0f };   // point/spot light only
    FScalar SpotPower = 64.0f;                  // spot light only
};

class App : public RenderApplication
{
public:
	App();

public:
	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();
	virtual bool ConfigureRenderingSetup() override;
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();
	virtual std::wstring GetName();

protected:
	MaterialPtr				m_pMaterial;
	GeometryPtr				m_pGeometry;
	LightInfo				m_Lights[3];
};

//--------------------------------------------------------------------------------
App AppInstance; // Provides an instance of the application
//--------------------------------------------------------------------------------

App::App()
{
	m_Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	m_Lights[0].Strength = { 0.8f, 0.8f, 0.8f };
	m_Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	m_Lights[1].Strength = { 0.4f, 0.4f, 0.4f };
	m_Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	m_Lights[2].Strength = { 0.2f, 0.2f, 0.2f };
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

// Shader vertex, matrix data is stored in each thread slot (ex: GetValueID( threadID ))
// Each contant buffer parameters are can be updated properly in
// 1. before shader init (all thread slot updated)
// 2. In each thread execute (in each task's ExecuteTask function)
bool App::ConfigureRenderingSetup()
{
	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.
	auto pLightView = new LightView( *m_pRenderer11, m_BackBuffer );
	pLightView->SetColorClearValue( Vector4f( 0.6f, 0.6f, 0.9f, 1.0f ) );

	m_pRenderView = pLightView;

	m_pTextOverlayView = new ViewTextOverlay( *m_pRenderer11, m_BackBuffer );

	m_pCamera = new FirstPersonCamera();
	m_pCamera->SetEventManager( &CameraEventHub );
	m_pCamera->GetNode()->Transform.Rotation().Rotation( Vector3f( 0.0f, 0.0f, 0.0f ) );
	m_pCamera->GetNode()->Transform.Position() = Vector3f( 0.0f, 10.0f, -20.0f );
	m_pCamera->SetCameraView( m_pRenderView );
	m_pCamera->SetOverlayView( m_pTextOverlayView );
	m_pCamera->SetProjectionParams( 0.1f, 1000.0f, static_cast<float>(m_iWidth) / static_cast<float>(m_iHeight), static_cast<float>( GLYPH_PI ) / 4.0f );

	m_pScene->AddCamera( m_pCamera );

	return( true );
}

std::wstring App::GetName()
{
	return( std::wstring( L"TexturedCube" ) );
}

// Luna's intro 3d book's polygon file loader
GeometryPtr loadIntro3DPolygonFile( std::wstring filename )
{
	GeometryPtr MeshPtr = GeometryPtr(new GeometryDX11());
	if (MeshPtr == NULL) {
		EventManager::Get()->ProcessEvent(EvtErrorMessagePtr(new EvtErrorMessage(std::wstring(
			L"Attempted to create a triangle on null geometry object."))));
		return MeshPtr;
	}

	// Get the file path to the models
	FileSystem fs;
	filename = fs.GetModelsFolder() + filename;

	// Load the contents of the file
	std::ifstream fin;

	// Open the file and read the MS3D header data
	fin.open(filename.c_str(), std::ios::in);

	if (!fin.is_open())
	{
		// signal error - bad filename?
		throw new std::exception("Could not open file");
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;
	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	VertexElementDX11* vPos = new VertexElementDX11(3, vcount);		// three-components and three vertex
	vPos->m_SemanticName = "POSITION";
	vPos->m_uiSemanticIndex = 0;
	vPos->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vPos->m_uiInputSlot = 0;
	vPos->m_uiAlignedByteOffset = 0;
	vPos->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vPos->m_uiInstanceDataStepRate = 0;

	VertexElementDX11* vNorm = new VertexElementDX11(3, vcount);
	vNorm->m_SemanticName = "NORMAL";
	vNorm->m_uiSemanticIndex = 0;
	vNorm->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vNorm->m_uiInputSlot = 0;
	vNorm->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	vNorm->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	vNorm->m_uiInstanceDataStepRate = 0;

	for (UINT i = 0; i < vcount; ++i) {
		Vector3f* pPos = vPos->Get3f(i);
		Vector3f* pNor = vNorm->Get3f(i);
		fin >> pPos->x >> pPos->y >> pPos->z;
		fin >> pNor->x >> pNor->y >> pNor->z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	MeshPtr->AddElement(vPos);
	MeshPtr->AddElement(vNorm);

	UINT a, b, c;
	for (UINT i = 0; i < tcount; i++) {
		fin >> a >> b >> c;
		MeshPtr->AddFace({ a, b, c });
	}
	fin.close();

	// Return to caller
	return MeshPtr;
}

void App::Initialize()
{
	m_pRenderer11->MultiThreadingConfig.SetConfiguration(false);

	// Create the camera, and the render view that will produce an image of the 
	// from the camera's point of view of the scene.
	m_pCamera->Spatial().SetRotation(Vector3f(0.5f, 0.3f, 0.0f));
	m_pCamera->Spatial().SetTranslation(Vector3f(-3.0f, 12.0f, -15.0f));

	auto pEffect = new RenderEffectDX11();
	pEffect->SetVertexShader(m_pRenderer11->LoadShader(VERTEX_SHADER,
		std::wstring(L"Lighting.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0"),
		true));

	pEffect->SetPixelShader(m_pRenderer11->LoadShader(PIXEL_SHADER,
		std::wstring(L"Lighting.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0"),
		true));

	m_pGeometry = loadIntro3DPolygonFile( L"skull.txt" );
	m_pGeometry->GenerateInputLayout(pEffect->GetVertexShader());
	m_pGeometry->LoadToBuffers();

	m_pMaterial = MaterialPtr(new MaterialDX11());
	m_pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	m_pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	auto pEntity = new Entity3D();
	pEntity->Visual.SetGeometry(m_pGeometry);
	pEntity->Visual.SetMaterial(m_pMaterial);		// must use a pointer here, not a reference
	pEntity->Transform.Position() = Vector3f(0.0f, 0.0f, 0.0f);

	// Lazy updated in ExecuteTask
	pEntity->Parameters.SetVectorParameter( L"gFresnelR0", { 0.001f, 0.001f, 0.001f, 0.f } );
    pEntity->Parameters.SetVectorParameter( L"gAmbientLight", { 0.25f, 0.25f, 0.35f, 1.0f } );
	pEntity->Parameters.SetVectorParameter( L"gDiffuseAlbedo", { 1.f, 1.f, 1.f, 1.f } );
	pEntity->Parameters.SetScalarParameter( L"gRoughness", 0.1f );
	
	auto actor = new Actor();
	actor->GetNode()->AttachChild(pEntity);
	actor->GetNode()->Controllers.Attach( new RotationController<Node3D>( Vector3f( 0.0f, 1.0f, 0.0f ), -1.0f ) );
	m_pScene->AddActor(actor);

	// Parameters are updated in InitRenderParams for all thread slot
    Vector4f cameraPos( m_pScene->GetRoot()->Transform.Position(), 1.0f );
    m_pScene->Parameters.SetVectorParameter( L"gEyePosW", cameraPos );
	m_pScene->Parameters.SetStructArrayParameter( L"gLights", sizeof(LightInfo), 3, m_Lights );
}

void App::Update()
{
	m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr(new EvtFrameStart(m_pTimer->Elapsed())));

	m_pCamera->Spatial().SetTranslation(Vector3f(-3.0f, 12.0f, -15.0f));

	// Update the scene, and then render all cameras within the scene.

	m_pScene->Update( m_pTimer->Elapsed() );
	m_pScene->Render( m_pRenderer11 );

	// Perform the rendering and presentation for the window.
	m_pRenderer11->Present( m_pWindow->GetHandle(), m_pWindow->GetSwapChain() );
}

void App::Shutdown()
{
}