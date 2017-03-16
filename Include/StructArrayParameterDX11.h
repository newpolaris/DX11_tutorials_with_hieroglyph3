//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// StructArrayParameterDX11
//
//--------------------------------------------------------------------------------
#ifndef StructArrayParameterDX11_h
#define StructArrayParameterDX11_h
//--------------------------------------------------------------------------------
#include "RenderParameterDX11.h"
#include "Matrix4f.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class StructArrayParameterDX11 : public RenderParameterDX11
	{
	public:
		StructArrayParameterDX11( int size, int count );
		StructArrayParameterDX11( StructArrayParameterDX11& copy );
		StructArrayParameterDX11& operator=( StructArrayParameterDX11& parameter );
		virtual ~StructArrayParameterDX11();

		virtual void SetParameterData( void* pData, unsigned int threadID = 0 );

		virtual const ParameterType GetParameterType();
		int GetCount();
		int GetSize();
		void* GetData( unsigned int threadID = 0 );

	protected:
		int					m_iStructCount;
		int					m_iStructSize;
		std::vector<BYTE>   m_Structs[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif // StructArrayParameterDX11_h
//--------------------------------------------------------------------------------

