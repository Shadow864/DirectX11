#include "LightHelper.fx"

// Returns the polar angle of the point (x,y) in [0, 2*PI).
float AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atan(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f *  3.14; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atan(y / x) + 3.14; // in [0, 2*pi).

	return theta;
}
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

void Subdivide(VertexOut inVerts[3], out VertexOut outVerts[6])
{
	outVerts[0].PosW	= inVerts[0].PosW;
	outVerts[0].NormalW = inVerts[0].NormalW;
	outVerts[0].Tex		= inVerts[0].Tex;

	outVerts[1].PosW	= 0.5f * (inVerts[0].PosW + inVerts[1].PosW);
	outVerts[1].NormalW = 0.5f * (inVerts[0].NormalW + inVerts[1].NormalW);
	outVerts[1].Tex		= 0.5f * (inVerts[0].Tex + inVerts[1].Tex);

	outVerts[2].PosW	= 0.5f * (inVerts[0].PosW + inVerts[2].PosW);
	outVerts[2].NormalW = 0.5f * (inVerts[0].NormalW + inVerts[2].NormalW);
	outVerts[2].Tex		= 0.5f * (inVerts[0].Tex + inVerts[2].Tex);

	outVerts[3].PosW	= 0.5f * (inVerts[1].PosW + inVerts[2].PosW);
	outVerts[3].NormalW = 0.5f * (inVerts[1].NormalW + inVerts[2].NormalW);
	outVerts[3].Tex		= 0.5f * (inVerts[1].Tex + inVerts[2].Tex);

	outVerts[4].PosW	= inVerts[2].PosW;
	outVerts[4].NormalW = inVerts[2].NormalW;
	outVerts[4].Tex		= inVerts[2].Tex;

	outVerts[5].PosW	= inVerts[1].PosW;
	outVerts[5].NormalW = inVerts[1].NormalW;
	outVerts[5].Tex		= inVerts[1].Tex;

}

void SubdivideOutput(VertexOut inVerts[6], inout TriangleStream<GeoOut> triStream)
{
	GeoOut gout[6];
	[unroll]
	for (int i = 0; i < 6; ++i)
	{
		gout[i].PosW = (inVerts[i].PosW);
		gout[i].PosH = mul(float4(gout[i].PosW, 1.0f), gViewProj);
		gout[i].NormalW = inVerts[i].NormalW;
		gout[i].Tex = inVerts[i].Tex;
		gout[i].PrimID = i;
	}

	[unroll]
	for (int j = 0; j < 5; ++j)
	{
		triStream.Append(gout[j]);
	}

	triStream.RestartStrip();

	triStream.Append(gout[1]);
	triStream.Append(gout[5]);
	triStream.Append(gout[3]);
}

void Subdivide(VertexOut gin[3], int level, inout TriangleStream<GeoOut> triStream)
{
	
	if (level == 0)
	{
		GeoOut gout[3];
		[unroll]
		for (int i = 0; i < 3; ++i)
		{
			gout[i].PosW = gin[i].PosW;
			gout[i].PosH = mul(float4(gout[i].PosW, 1.0f), gViewProj);
			gout[i].NormalW = gin[i].NormalW;
			gout[i].Tex = gin[i].Tex;
			gout[i].PrimID = i;

			triStream.Append(gout[i]);
		}

		triStream.RestartStrip();
	}
	else if (level == 1)
	{
		VertexOut v[6];
		Subdivide(gin, v);
		SubdivideOutput(v, triStream);
		triStream.RestartStrip();
	}
	else
	{
		VertexOut v[6];
		Subdivide(gin, v);

		VertexOut tri0[3] = { v[0], v[1], v[2] };
		VertexOut tri1[3] = { v[1], v[3], v[2] };
		VertexOut tri2[3] = { v[2], v[3], v[4] };
		VertexOut tri3[3] = { v[1], v[5], v[3] };


		Subdivide(tri0, v);
		SubdivideOutput(v, triStream);
		triStream.RestartStrip();

		Subdivide(tri1, v);
		SubdivideOutput(v, triStream);
		triStream.RestartStrip();

		Subdivide(tri2, v);
		SubdivideOutput(v, triStream);
		triStream.RestartStrip();

		Subdivide(tri3, v);
		SubdivideOutput(v, triStream);
		triStream.RestartStrip();
	}
}

[maxvertexcount(64)]
void GS(triangle VertexOut gin[3], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{

	float d = distance(gEyePosW, float3(0,0,0));

	uint level = 0;

	if (d < 15)
		level = 2;
	else if (d < 30)
		level = 1;
	else
		level = 0;

	Subdivide(gin, level, triStream);
		
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
	
	ambient += A ;
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


