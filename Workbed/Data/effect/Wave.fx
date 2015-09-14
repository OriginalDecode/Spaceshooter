Matrix World;
Matrix View;
Matrix Projection;

float Time;
float DeltaTime;

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
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNorm : BINORMAL;
	float3 Tang : TANGENT;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	

	output.Pos = mul(input.Pos, World);
	output.Pos.x += cos((Time * 2) + output.Pos.y);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	

	output.Tex = input.Tex;
	output.Norm = mul(input.Norm, World);
	
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float ambient = 0.3;
	float4 finalColor = DiffuseTexture.Sample(linearSampling, input.Tex) * ambient;
	float lambert = dot((float3)myLightDir[0], input.Norm);
	float4 lightColor = saturate(lambert * myLightColor[0]);
	finalColor += lightColor;
	
	finalColor.a = 1;
	
	return finalColor;
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