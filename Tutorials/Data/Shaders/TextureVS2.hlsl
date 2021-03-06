//--------------------------------------------------------------------------------
// TextureVS.hlsl
//
// This vertex shader serves as a pass through for pre-transformed vertices.  This
// is typically used for full screen quad rendering.
//
// Copyright (C) 2010 Jason Zink.  All rights reserved.
//--------------------------------------------------------------------------------

cbuffer ImageViewingData
{
	float4 WindowSize;		// The size of the window being used
	float4 ImageSize;		// The size of the image being displayed
	float4 ViewingParams;	// x,y = tex coords for center of view
};							// z,w = zoom amount for x,y directions

struct VS_INPUT
{
	float4 position : POSITION;
    float2 tex		: TEXCOORDS0;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
    float2 tex      : TEXCOORD0;    
};


VS_OUTPUT VSMAIN( in VS_INPUT input )
{
	VS_OUTPUT output;
	
	output.position = input.position;
    output.tex = input.tex;
	//output.tex.xy = (input.tex * WindowSize.xy * (1.0f / ViewingParams.zw) + ViewingParams.xy) / ImageSize.xy;
	return output;
}

