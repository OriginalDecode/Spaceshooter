
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
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	//output.Pos = mul(input.Pos, World);
	//output.Pos = mul(output.Pos, View);
	output.Pos = mul(input.Pos, Projection);
	output.Pos.x -= 1;
	output.Pos.y += 1;
	output.Tex = input.Tex;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float4 color = DiffuseTexture.Sample(linearSampling, input.Tex);

	return color;
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