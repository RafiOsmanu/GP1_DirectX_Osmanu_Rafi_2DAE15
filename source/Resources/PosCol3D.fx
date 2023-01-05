//------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------
float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap; //or mirror. clamp, border
	AddressV = Wrap;//or mirror. clamp, border
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap; //or mirror. clamp, border
	AddressV = Wrap;//or mirror. clamp, border
};

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	AddressU = Wrap; //or mirror. clamp, border
	AddressV = Wrap;//or mirror. clamp, border
};

//------------------------------------------------------------------
// Input/Output Structs
//------------------------------------------------------------------

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float2 UV  : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
	float2 UV  : TEXCOORD;
};

//------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
	output.Color = input.Color;
	output.UV = input.UV;
	return output;
}

//------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------
float4 PS_Point(VS_OUTPUT input) : SV_TARGET
{
	//Output.RGBColor = g_MeshTexture.Sample(MeshTextureSampler, In.TextureUV) * In.Diffuse;
	
	input.Color = gDiffuseMap.Sample(samPoint, input.UV);

	return float4(input.Color, 1.f);
}

float4 PS_Linear(VS_OUTPUT input) : SV_TARGET 
{
	input.Color = gDiffuseMap.Sample(samLinear, input.UV);
	return float4(input.Color, 1.0f);
}

float4 PS_Anisotropic(VS_OUTPUT input) : SV_TARGET 
{
	input.Color = gDiffuseMap.Sample(samAnisotropic, input.UV);
	return float4(input.Color, 1.0f);
}

//------------------------------------------------------------------
// Technique
//------------------------------------------------------------------
technique11 Techniques
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Point()));
	}

	pass p1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Linear()));
	}

	pass p2
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Anisotropic()));
	}
}





