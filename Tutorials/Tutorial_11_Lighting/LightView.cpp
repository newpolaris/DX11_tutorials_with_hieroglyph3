#include "LightView.h"
#include "Scene.h"

LightView::LightView(RendererDX11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget)
: ViewPerspective(Renderer, RenderTarget, DepthTarget)
{
}


LightView::~LightView()
{
}

void LightView::ExecuteTask(PipelineManagerDX11* pPipelineManager, IParameterManager* pParamManager)
{
	// unbind the shader resource view, it is not necessary if the renderer is running in multithreading mode (default)
	// pPipelineManager->ClearPipelineState();
	ViewPerspective::ExecuteTask(pPipelineManager, pParamManager);
}

std::wstring LightView::GetName()
{
	return (L"LightView");
}