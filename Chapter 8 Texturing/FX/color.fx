cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
	float4	 gColor;
};

struct VertexIn
{
	float3 Pos   : POSITION;
	float3 Nor	 : NORMAL;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH   : SV_POSITION;
	float4 Normal : NORMAL;
    float4 Color  : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
	vout.Normal = float4(1.f, 1.f, 1.f, 1.f);
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

