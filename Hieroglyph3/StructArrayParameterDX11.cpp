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
#include "StructArrayParameterDX11.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
StructArrayParameterDX11::StructArrayParameterDX11( int size, int count )
{
	if ( count < 1 )
		count = 1;

	m_iStructCount = count; 
	m_iStructSize = size; 

	for (int i = 0; i <= NUM_THREADS; i++)
		m_Structs[i].assign(size*count, 0);
}
//--------------------------------------------------------------------------------
StructArrayParameterDX11::StructArrayParameterDX11( StructArrayParameterDX11& copy )
{
	m_iStructCount = copy.m_iStructCount;
	m_iStructSize = copy.m_iStructSize;
	for (int i = 0; i <= NUM_THREADS; i++)
		m_Structs[i] = copy.m_Structs[i];
}
//--------------------------------------------------------------------------------
StructArrayParameterDX11& StructArrayParameterDX11::operator=( StructArrayParameterDX11& parameter )
{
	m_iStructCount = parameter.m_iStructCount;
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_Structs[i] = parameter.m_Structs[i];
   return *this;  // Assignment operator returns left side.
}
//--------------------------------------------------------------------------------
StructArrayParameterDX11::~StructArrayParameterDX11()
{
}
//--------------------------------------------------------------------------------
void StructArrayParameterDX11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	// TODO: This isn't very safe - the caller could supply less than the correct 
	//       amount of matrices...  I need a better way to set this parameter data.

	if ( 0 != memcmp( pData, m_Structs[threadID].data(), m_Structs[threadID].size()) ) {
		m_auiValueID[threadID]++;
		memcpy( m_Structs[threadID].data(), pData, m_Structs[threadID].size() );
	}
}
//--------------------------------------------------------------------------------
const ParameterType StructArrayParameterDX11::GetParameterType()
{
	return( STRUCT_ARRAY );
}
//--------------------------------------------------------------------------------
int StructArrayParameterDX11::GetCount()
{
	return( m_iStructCount );
}
//--------------------------------------------------------------------------------
int StructArrayParameterDX11::GetSize()
{
	return( m_iStructSize );
}
//--------------------------------------------------------------------------------
void* StructArrayParameterDX11::GetData( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_Structs[threadID].data() );
}
//--------------------------------------------------------------------------------