Matrix World;
Matrix View;
Matrix Projection;

Texture2D DiffuseTexture;
Texture2D NormalTexture;

float4 DirectionalLightDir[1];
float4 DirectionalLightColor[1];

float4 PointLightPosition[3];
float4 PointLightColor[3];
float PointLightRange[3];


SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Tex = input.Tex;
	
	
	output.Normal = mul(input.Normal, World);
	output.BiNormal = input.BiNormal;
	output.Tangent = mul(input.Tangent, World);

	output.WorldPosition = mul(input.Position, World);
	
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linearSampling, input.Tex) * 2 - 1;
	
	input.Normal = normalize(input.Normal);
	input.BiNormal = normalize(input.BiNormal);
	input.Tangent  = normalize(input.Tangent);
	
	float3x3 tangentSpaceMatrix = float3x3(input.Tangent, input.BiNormal, input.Normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));

	float ambient = 0.1;
	float4 finalColor = DiffuseTexture.Sample(linearSampling, input.Tex) * ambient;
	
	for(int i = 0; i < 1; ++i)
	{
		float lambert = dot((float3)DirectionalLightDir[i], norm);
		float4 lightColor = saturate(lambert * DirectionalLightColor[i]);
		finalColor += lightColor;
	}
	
	
	for(int i = 0; i < 0; ++i)
	{
		//Vector from light to pixel
		float4 lightVec = PointLightPosition[i] - input.WorldPosition;

		//Dist betweem light and pixel
		float distance = length(lightVec);

		//normalize lightvector
		lightVec = normalize(lightVec);


		//attenuation
		float attenuation = 1.f / (1.f + 0.1f * distance + 0.01f * distance * distance);
		float fallOff = 1.f - (distance / (PointLightRange[i] + 0.00001f));
		float totalAttenuation = attenuation * fallOff;


		//add color to finalColor
		float lambert = dot((float3)lightVec, norm);
		float4 lightColor = saturate(lambert * PointLightColor[i]);
		finalColor += lightColor * totalAttenuation;
	}
	
	
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