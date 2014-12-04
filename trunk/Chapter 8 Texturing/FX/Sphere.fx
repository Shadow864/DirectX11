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
	float2 SizeW	: SIZE;
};

struct VertexOut
{
	float3 PosW		: POSITION;
	float2 SizeW	: SIZE;
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
	vout.SizeW = vin.SizeW;
    return vout;
}


// We expand each point into a quad (4 vertices), so the maximum number of vertices
// we output per geometry shader invocation is 4.
[maxvertexcount(60)]
void GS(point VertexOut gin[1], uint primID : SV_PrimitiveID, inout TriangleStream<GeoOut> triStream)
{
	

	// Put a cap on the number of subdivisions.
	//numSubdivisions = MathHelper::Min(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.

	
	const float X = 0.525731f;
	const float Z = 0.850651f;

	float3 pos[12] =
	{
		float3(-X, 0.0f, Z),	float3(X, 0.0f, Z),
		float3(-X, 0.0f, -Z),	float3(X, 0.0f, -Z),
		float3(0.0f, Z, X),		float3(0.0f, Z, -X),
		float3(0.0f, -Z, X),	float3(0.0f, -Z, -X),
		float3(Z, X, 0.0f),		float3(-Z, X, 0.0f),
		float3(Z, -X, 0.0f),	float3(-Z, -X, 0.0f)
	};

	float2 tex_cord[12];
	float3 normals[12];

	int k[60] =
	{
		1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
		1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
		3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
		10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
	};

	float radius = 1;// gin[0].SizeW.x;

	//// Project vertices onto sphere and scale.
	for (int i = 0; i < 12; ++i)
	{
		// Project onto unit sphere.
		float3 n = normalize(pos[i]);

		//	// Project onto sphere.
		float3 p = n * radius;
		pos[i] = p;
		normals[i] = n;


		// Derive texture coordinates from spherical coordinates.
		float theta = AngleFromXY(pos[i].x, pos[i].z);
		
		float phi = acos(pos[i].y / radius);

		tex_cord[i].x = theta / 6.28;
		tex_cord[i].y = phi / 3.14;


	}

	GeoOut gout;
	[unroll]
	for (int i = 0; i < 60; )
	{
		for (int j = 0; j < 3; ++j)
		{

			int index = k[i + j];
			gout.PosW = mul(float4(pos[index], 1.0f), gWorld).xyz;
			gout.PosH = mul(float4(gout.PosW, 1.0f), gViewProj);
			gout.NormalW = normals[index];// mul(float4(normals[index], 1.0f), gWorld).xyz;
			gout.Tex = tex_cord[index];
			gout.PrimID = primID;

			triStream.Append(gout);
		}
		i += 3;
		triStream.RestartStrip();
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


