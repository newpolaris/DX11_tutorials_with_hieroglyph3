//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ScalarParameterWriterDX11.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
ScalarParameterWriterDX11::ScalarParameterWriterDX11()
	: m_Value( 0.0f )
{
}
//--------------------------------------------------------------------------------
ScalarParameterWriterDX11::~ScalarParameterWriterDX11()
{
}
//--------------------------------------------------------------------------------
void ScalarParameterWriterDX11::SetRenderParameterRef( ScalarParameterDX11* pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void ScalarParameterWriterDX11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetScalarParameter( m_pParameter, &m_Value);
}
//--------------------------------------------------------------------------------
void ScalarParameterWriterDX11::SetValue( const FScalar& Value )
{
	m_Value = Value;
}
//--------------------------------------------------------------------------------
FScalar ScalarParameterWriterDX11::GetValue()
{
	return( m_Value );
}
//--------------------------------------------------------------------------------
void ScalarParameterWriterDX11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( &m_Value );
}
//--------------------------------------------------------------------------------
RenderParameterDX11* ScalarParameterWriterDX11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------