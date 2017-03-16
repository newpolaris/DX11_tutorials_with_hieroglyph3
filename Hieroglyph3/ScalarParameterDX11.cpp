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
#include "ScalarParameterDX11.h"
//--------------------------------------------------------------------------------
using namespace Glyph3;
//--------------------------------------------------------------------------------
ScalarParameterDX11::ScalarParameterDX11()
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_fScala[i] = 0;
}
//--------------------------------------------------------------------------------
ScalarParameterDX11::ScalarParameterDX11( ScalarParameterDX11& copy )
{
	for ( int i = 0; i <= NUM_THREADS; i++ )
		m_fScala[i] = copy.m_fScala[i];
}
//--------------------------------------------------------------------------------
ScalarParameterDX11::~ScalarParameterDX11()
{
}
//--------------------------------------------------------------------------------
void ScalarParameterDX11::SetParameterData( void* pData, unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );
	auto scala = *static_cast<FScalar*>(pData);
	if (scala != m_fScala[threadID]) {
		m_auiValueID[threadID]++;
		m_fScala[threadID] = scala;
	}
}
//--------------------------------------------------------------------------------
const ParameterType ScalarParameterDX11::GetParameterType()
{
	return( SCALAR );
}
//--------------------------------------------------------------------------------
FScalar ScalarParameterDX11::GetScalar( unsigned int threadID )
{
	assert( threadID >= 0 );
	assert( threadID < NUM_THREADS+1 );

	return( m_fScala[threadID] );
}
//--------------------------------------------------------------------------------
void ScalarParameterDX11::SetScalar(FScalar f, unsigned int threadID)
{
	if ( f != m_fScala[threadID] ) {
		m_auiValueID[threadID]++;
		m_fScala[threadID] = f;
	}
}
//--------------------------------------------------------------------------------