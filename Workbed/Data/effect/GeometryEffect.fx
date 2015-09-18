Matrix World;
Matrix View;
Matrix Projection;

Texture2D DiffuseTexture;

float4 DirectionalLightDir[1];
float4 DirectionalLightColor[1];

float4 PointLightPosition[3];
float4 PointLightColor[3];
float PointLightRange[3];

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = input.Color;
	output.Norm = mul(input.Norm, World);
	
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float ambient = 0.5;
	float4 finalColor = input.Color * ambient;
	float lambert = dot((float3)DirectionalLightDir[0], input.Norm);
	float4 lightColor = saturate(lambert * DirectionalLightColor[0]);
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