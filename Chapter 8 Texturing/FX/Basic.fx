#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	float3 gEyePosW;

	float4 gFogColor;
	float  gFogStart;
	float  gFogRange;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj; 
	float4x4 gTexTransform;
	float4	 gDiffuse;
	float4	 gSpecular;
	float4   gAmbient;
	
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};


struct VertexIn
{
	float3 PosL		: POSITION0;
	float3 NormalL	: NORMAL0;
	float2 Tex		: TEXCOORD0;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
	float2 Tex		: TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

    return vout;
}

float4 PS(VertexOut pin, uniform bool gUseTexture, uniform bool gFogEnabled) : SV_Target
{

    // Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	float3 toEye = gEyePosW - pin.PosW;

	float distToEye = length(toEye);

	toEye /= distToEye;

	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	if (gUseTexture)
	{
		texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);

		clip(texColor.a - 0.1f);
	}

	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Sum the light contribution from each light source.
	float4 A, D, S;
	ComputeDirectionalLight(gDiffuse, gSpecular, gAmbient, gDirLight, pin.NormalW, toEye, A, D, S);
	
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = texColor * (ambient + diffuse) + spec;
	
	if (gFogEnabled)
	{
		float s = saturate((distToEye - gFogStart) / gFogRange);

		litColor = lerp(litColor, gFogColor, s);// (1 - s) * litColor + s * gFogColor;
	}

	// Common to take alpha from diffuse material.
	litColor.a = gDiffuse.a * texColor.a;
	
	//float depthValue = pin.PosH.z / pin.PosH.w;

	//float4 depth_color = float4(depthValue, depthValue, depthValue, 1.0);

	return litColor;

}

technique11 Light
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS(false, true)));
    }
}


technique11 LightTexture
{
	pass P0
	{

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS(true, true)));
	}
}


