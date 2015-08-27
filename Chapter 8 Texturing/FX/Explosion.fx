#include "LightHelper.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	float3 gEyePosW;

	float4 gFogColor;
	float  gFogStart;
	float  gFogRange;

	float  gTime;
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
	float2 Tex		: TEXCOORD0;
};

struct VertexOut
{
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
	float2 Tex		: TEXCOORD;
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

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);


	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	return vout;
}


[maxvertexcount(3)]
void GS(triangle VertexOut gin[3], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{

	
	float3 v1 = gin[0].PosW - gin[1].PosW;
		float3 v2 = gin[1].PosW - gin[2].PosW;
		float3 normal = normalize(cross(v1, v2));


	float time = (gTime * 1.2f) % 3.0;


	float speed = 0;
	if (time - 1.0 > 0)
	{
		speed = ((time - 1.0) / 1.0f) * (((float)primID % 7) + 3);
	}

	GeoOut gout[3];
	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		gout[i].PosW = gin[i].PosW + normal * time * speed;
		gout[i].PosH = mul(float4(gout[i].PosW, 1.0f), gViewProj);
		gout[i].NormalW = gin[i].NormalW;
		gout[i].Tex = gin[i].Tex;
		gout[i].PrimID = i;

		triStream.Append(gout[i]);
	}

	triStream.RestartStrip();
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
	float4 ambient = float4(0.2f, 0.2f, 0.2f, 0.0f);
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
		SetVertexShader(CompileShader(vs_4_0, VS()));
		//SetGeometryShader(NULL);
		SetGeometryShader(CompileShader(gs_4_0, GS()));
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


