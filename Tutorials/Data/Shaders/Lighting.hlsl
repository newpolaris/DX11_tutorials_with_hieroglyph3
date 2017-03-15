// Defaults for number of lights.
#ifndef NUM_DIR_LIGHTS
    #define NUM_DIR_LIGHTS 3
#endif

#define MaxLights 16

struct Light
{
	float3 Strength;
    float FalloffStart; // point/spot light only
    float3 Direction;   // directional/spot light only
    float FalloffEnd;   // point/spot light only
    float3 Position;    // point light only
    float SpotPower;    // spot light only
};

struct Material
{
    float4 DiffuseAlbedo;
    float3 FresnelR0;
    float Shininess;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // Linear falloff.
    return saturate((falloffEnd-d) / (falloffEnd - falloffStart));
}

// Schlick gives an approximation to Fresnel reflectance (see pg. 233 "Real-Time Rendering 3rd Ed.").
// R0 = ( (n-1)/(n+1) )^2, where n is the index of refraction.
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
    const float m = mat.Shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor*roughnessFactor;

    // Our spec formula goes outside [0,1] range, but we are 
    // doing LDR rendering.  So scale it down a bit.
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
	L.Direction = float3( 0.57735f, -0.57735f, 0.57735f );
	L.Strength = float3( 0.8f, 0.8f, 0.8f );

    // The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float4 ComputeLighting(Light gLights[MaxLights], Material mat,
                       float3 pos, float3 normal, float3 toEye,
                       float3 shadowFactor)
{
    float3 result = 0.0f;

    int i = 0;

#if (NUM_DIR_LIGHTS > 0)
    for(i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        result += shadowFactor[i] * ComputeDirectionalLight(gLights[i], mat, normal, toEye);
    }
#endif
    return float4(result, 0.0f);
}

// #include "LightHelper.hlsl"

cbuffer cbPerObject : register(b0)
{
	float4x4 WorldMatrix;
}; 

cbuffer cbMaterial : register(b1)
{
	float4 gDiffuseAlbedo;
	float4 gFresnelR0_Roughness;
	// xyz, w
}

cbuffer cbPass : register(b2)
{
    float4x4 ViewProjMatrix;
    float3 gEyePosW;
    float4 gAmbientLight;
    // Light gLights[MaxLights];
};

struct VS_INPUT
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
};

struct VS_OUT 
{
    float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
};

VS_OUT VSMAIN(VS_INPUT input)
{
    VS_OUT output;
    float4 PosW = mul(float4(input.PosL, 1.0f), WorldMatrix);
	output.PosW = PosW.xyz;
	output.NormalW = mul(input.NormalL, (float3x3)WorldMatrix);
	output.PosH = mul(PosW, ViewProjMatrix);
    return output;
};

float4 PSMAIN(VS_OUT input) : SV_Target
{
	float4 texColor = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuseAlbedo = texColor * gDiffuseAlbedo;

	// Interpolating normal can unnormalize it, so normalize it.
    input.NormalW = normalize(input.NormalW);

	// The toEye vector is used in lighting.
	float3 toEyeW = normalize(gEyePosW - input.PosW);
	// Start with a sum of zero. 

	// Indirect lighting.
	float4 ambient = gAmbientLight*diffuseAlbedo;
	float Roughness = gFresnelR0_Roughness.w;
	const float shininess = 1.0f - Roughness;

	float3 gFresnelR0 = gFresnelR0_Roughness.xyz;
	Material mat = { diffuseAlbedo, gFresnelR0, shininess };
	float3 shadowFactor = 1.0f;
	Light gLights[MaxLights];
	float4 directLight = ComputeLighting(gLights, mat, input.PosW,
		input.NormalW, toEyeW, shadowFactor);

    float4 litColor = ambient + directLight;

    // Common convention to take alpha from diffuse material.
    litColor.a = gDiffuseAlbedo.a;
	input.PosH.z += 0.1;
    return litColor;
}
