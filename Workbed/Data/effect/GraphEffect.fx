Matrix World;
Matrix View;
Matrix Projection;

Texture2D DiffuseTexture;

float4 myLightDir[1];
float4 myLightColor[1];

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, Projection);
	output.Pos.x -= 1;
	output.Pos.y += 1;
	output.Color = input.Color;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return input.Color;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}