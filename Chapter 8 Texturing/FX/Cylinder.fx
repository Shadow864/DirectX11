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
	float4x4 gViewProj;
	float4x4 gTexTransform;
	float4	 gDiffuse;
	float4	 gSpecular;
	float4   gAmbient;
	
};

cbuffer cbFixed
{
	//
	// Compute texture coordinates to stretch texture over quad.
	//

	float2 gTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
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
};

struct VertexOut
{
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
};

struct GeoOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex	   : TEXCOORD0;
	uint   PrimID  : SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosW		= mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW	= mul(vin.NormalL, (float3x3)gWorldInvTranspose);
    return vout;
}


// We expand each point into a quad (4 vertices), so the maximum number of vertices
// we output per geometry shader invocation is 4.
[maxvertexcount(4)]
void GS(line VertexOut gin[2], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	
	float4 v[4];
	v[1] = float4(gin[0].PosW, 1);
	v[0] = float4(gin[0].PosW + float3(0, 1, 0), 1);
	v[3] = float4(gin[1].PosW, 1);
	v[2] = float4(gin[1].PosW + float3(0, 1, 0), 1);

	float3 normal[4];
	normal[1] = gin[0].PosW;
	normal[0] = gin[0].PosW;
	normal[3] = gin[1].PosW;
	normal[2] = gin[1].PosW;
	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosW = v[i].xyz;
		gout.NormalW = normal[i];
		gout.Tex = gTexC[3-i];
		gout.PrimID = primID;

		triStream.Append(gout);
	}
}

float4 PS(GeoOut pin, uniform bool gUseTexture, uniform bool gFogEnabled) : SV_Target
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
		//SetGeometryShader(NULL);
		SetGeometryShader(CompileShader(gs_4_0, GS() ) );
		SetPixelShader(CompileShader(ps_4_0, PS(false, true)));
    }
}


technique11 LightTexture
{
	pass P0
	{

		SetVertexShader(CompileShader(vs_4_0, VS()));
		//SetGeometryShader(NULL);
		SetGeometryShader(CompileShader(gs_4_0, GS()));
		SetPixelShader(CompileShader(ps_4_0, PS(true, true)));
	}
}


