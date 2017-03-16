//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// ScalarParameterDX11
//
//--------------------------------------------------------------------------------
#ifndef ScalarParameterDX11_h
#define ScalarParameterDX11_h
//--------------------------------------------------------------------------------
#include "RenderParameterDX11.h"
#include "Vector4f.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class ScalarParameterDX11 : public RenderParameterDX11
	{
	public:
		ScalarParameterDX11();
		ScalarParameterDX11( ScalarParameterDX11& copy );
		virtual ~ScalarParameterDX11();

		virtual void SetParameterData( void* pData, unsigned int threadID = 0 );
		virtual const ParameterType GetParameterType();
		
		FScalar GetScalar( unsigned int threadID = 0 );
		void SetScalar( FScalar fScala, unsigned int threadID = 0 );

	protected:
		FScalar m_fScala[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif // ScalarParameterDX11_h
//--------------------------------------------------------------------------------

