//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "PCH.h"
#include "StructArrayParameterWriterDX11.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
StructArrayParameterWriterDX11::StructArrayParameterWriterDX11()
{
	m_pValue = nullptr;
	m_pParameter = nullptr;
	m_iCount = 0;
	m_iSize = 0;
}
//--------------------------------------------------------------------------------
StructArrayParameterWriterDX11::~StructArrayParameterWriterDX11()
{
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::SetRenderParameterRef( StructArrayParameterDX11* pParam )
{
	m_pParameter = pParam;
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::WriteParameter( IParameterManager* pParamMgr )
{
	pParamMgr->SetStructArrayParameter( m_pParameter, m_iSize, m_iCount, m_pValue );
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::SetValue( void* Value )
{
	m_pValue = Value;
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::SetCount( int count )
{
	m_iCount = count;
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::SetSize( int size)
{
	m_iSize = size;
}
//--------------------------------------------------------------------------------
int StructArrayParameterWriterDX11::GetCount( ) const
{
	return m_iCount;
}
//--------------------------------------------------------------------------------
int StructArrayParameterWriterDX11::GetSize( ) const
{
	return m_iSize;
}
//--------------------------------------------------------------------------------
void StructArrayParameterWriterDX11::InitializeParameter( )
{
	m_pParameter->InitializeParameterData( m_pValue );
}
//--------------------------------------------------------------------------------
RenderParameterDX11* StructArrayParameterWriterDX11::GetRenderParameterRef()
{
	return( m_pParameter );
}
//--------------------------------------------------------------------------------

