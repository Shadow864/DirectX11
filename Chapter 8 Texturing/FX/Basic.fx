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

struct GeoOut2
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
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


[maxvertexcount(6)]
void GS_Normal(triangle VertexOut gin[3], uint primID : SV_PrimitiveID, inout LineStream<GeoOut2> lineStream, uniform bool gSmooth)
{

	float3 v1 = gin[0].PosW - gin[1].PosW;
	float3 v2 = gin[1].PosW - gin[2].PosW;
	float3 normal = normalize(cross(v1, v2));

	float3 vt = normalize(gin[0].PosW - gEyePosW);

	// Take the dot product of the normal with the view direction
	float d = dot(vt, normal);

	// Emit a primitive only if the sign of the dot product is positive
	if (d > 0.0)
		return;

	if (gSmooth)
	{
		GeoOut2 gout[2];
		float3 pos = float3(0, 0, 0);
		for (int i = 0; i < 3; ++i)
		{
			pos += gin[i].PosW;
		}
		pos /= 3.0f;

		gout[0].PosW = pos;
		gout[0].PosH = mul(float4(gout[0].PosW, 1.0f), gViewProj);

		gout[1].PosW = gout[0].PosW + normal;
		gout[1].PosH = mul(float4(gout[1].PosW, 1.0f), gViewProj);

		lineStream.Append(gout[0]);
		lineStream.Append(gout[1]);

		lineStream.RestartStrip();
	}
	else
	{
		GeoOut2 gout[6];
		[unroll]
		for (int i = 0; i < 3; ++i)
		{
			gout[i * 2].PosW = gin[i].PosW;
			gout[i * 2].PosH = mul(float4(gout[i * 2].PosW, 1.0f), gViewProj);

			gout[i * 2 + 1].PosW = gin[i].PosW + gin[i].NormalW;
			gout[i * 2 + 1].PosH = mul(float4(gout[i * 2 + 1].PosW, 1.0f), gViewProj);


			lineStream.Append(gout[i * 2]);
			lineStream.Append(gout[i * 2 + 1]);
			lineStream.RestartStrip();
		}

		lineStream.RestartStrip();
	}

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

	ambient.w = 1;
	diffuse.w = 1;
	spec.w = 1;

	float4 spec2 = float4(spec.x, spec.y, spec.z, 0);

	float4 sum = ambient;
	sum += spec2;
	sum += diffuse;

	
	float4 litColor = texColor * (sum);
	
	if (gFogEnabled)
	{
		float s = saturate((distToEye - gFogStart) / gFogRange);

		litColor = lerp(litColor, gFogColor, s);// (1 - s) * litColor + s * gFogColor;
	}

	// Common to take alpha from diffuse material.
	litColor.a =  gDiffuse.a * texColor.a;
	
	//float depthValue = pin.PosH.z / pin.PosH.w;

	//float4 depth_color = float4(depthValue, depthValue, depthValue, 1.0);

	return litColor;

}


float4 PS2(GeoOut2 pin, uniform bool gUseTexture, uniform bool gFogEnabled) : SV_Target
{
	float3 toEye = gEyePosW - pin.PosW;

	float distToEye = length(toEye);

	toEye /= distToEye;


	float4 litColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
	if (gFogEnabled)
	{
		float s = saturate((distToEye - gFogStart) / gFogRange);

		litColor = lerp(litColor, gFogColor, s);// (1 - s) * litColor + s * gFogColor;
	}
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

technique11 Normal
{
	pass P1
	{

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(CompileShader(gs_4_0, GS_Normal(false)));
		SetPixelShader(CompileShader(ps_4_0, PS2(true, true)));
	}
}

technique11 NormalSmooth
{
	pass P1
	{

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(CompileShader(gs_4_0, GS_Normal(true)));
		SetPixelShader(CompileShader(ps_4_0, PS2(true, true)));
	}
}


