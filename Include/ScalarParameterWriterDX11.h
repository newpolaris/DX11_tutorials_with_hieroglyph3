//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ScalarParameterWriterDX11
//
//--------------------------------------------------------------------------------
#ifndef ScalarParameterWriterDX11_h
#define ScalarParameterWriterDX11_h
//--------------------------------------------------------------------------------
#include "ParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class ScalarParameterWriterDX11 : public ParameterWriter
	{
	public:
		ScalarParameterWriterDX11();
		virtual ~ScalarParameterWriterDX11();

		void SetRenderParameterRef( ScalarParameterDX11* pParam );
		void SetValue( const FScalar& Value );
		FScalar GetValue();

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual RenderParameterDX11* GetRenderParameterRef();

	protected:
		ScalarParameterDX11*			m_pParameter;
		FScalar							m_Value;
	};
};
//--------------------------------------------------------------------------------
#endif // VectorParameterWriterDX11_h
//--------------------------------------------------------------------------------

