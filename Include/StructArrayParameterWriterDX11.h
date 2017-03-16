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
// StructArrayParameterWriterDX11
//
//--------------------------------------------------------------------------------
#ifndef StructArrayParameterWriterDX11_h
#define StructArrayParameterWriterDX11_h
//--------------------------------------------------------------------------------
#include "ParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Glyph3
{
	class StructArrayParameterWriterDX11 : public ParameterWriter
	{
	public:
		StructArrayParameterWriterDX11();
		virtual ~StructArrayParameterWriterDX11();

		void SetRenderParameterRef( StructArrayParameterDX11* pParam );
		void SetValue( void* Value );
		void SetCount( int count );
		void SetSize( int count );
		int GetCount() const;
		int GetSize() const;

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual RenderParameterDX11* GetRenderParameterRef();

	protected:
		StructArrayParameterDX11*		m_pParameter;
		void*							m_pValue;
		int								m_iCount;
		int								m_iSize;
	};
};
//--------------------------------------------------------------------------------
#endif // ParameterWriter_h
//--------------------------------------------------------------------------------


