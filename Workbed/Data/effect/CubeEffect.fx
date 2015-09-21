#include "ShaderVariables.fx"

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
};

PS_INPUT_POS_NORM_TEX VS(VS_INPUT_POS_NORM_TEX input)
{
	PS_INPUT_POS_NORM_TEX output = (PS_INPUT_POS_NORM_TEX)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	output.Norm = mul(input.Norm, World);
	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_TEX input) : SV_Target
{
	float ambient = 0.1f;
	float4 finalColor = DiffuseTexture.Sample(linearSampling, input.Tex) * ambient;
	
	for(int i = 0; i < 1; ++i)
	{
		float lambert = dot((float3)DirectionalLights[i].Direction, input.Norm);
		float4 lightColor = saturate(lambert * DirectionalLights[i].Color);
		finalColor += lightColor;
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