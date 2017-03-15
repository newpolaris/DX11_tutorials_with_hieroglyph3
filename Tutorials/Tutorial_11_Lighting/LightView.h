#pragma once

#include "ViewPerspective.h"
#include "RendererDX11.h"

using namespace Glyph3;

class Glyph3::Entity3D;

class LightView : public ViewPerspective
{
public:
	LightView( RendererDX11& Renderer, ResourcePtr RenderTarget, ResourcePtr DepthTarget = 0 );
	virtual ~LightView();
	void ExecuteTask(PipelineManagerDX11 * pPipelineManager, IParameterManager * pParamManager);
	std::wstring GetName();
};