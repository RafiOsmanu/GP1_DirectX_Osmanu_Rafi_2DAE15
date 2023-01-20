//------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------
float4x4 gWorldViewProj : WorldViewProjection;

Texture2D gDiffuseMap : DiffuseMap;

float4x4 gWorldMatrix : WORLD;
float4x4 gViewInverseMatrix : VIEWINVERSE;

bool gUseNormal = true;


float gPi = 3.14159265359f;

float gLightIntensity = 7.0f;

float gShininess = 25.0f;

float3 gLightDirection = float3(.577f, -.577f, .577f);


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
    float3 WorldPosition : WORLD;
	float2 UV  : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
    float3 WorldPosition : WORLD;
	float2 UV  : TEXCOORD;
    float3 Normal : NORMAL;
};

//------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
    output.WorldPosition = mul(float4(input.WorldPosition, 1.f), gWorldMatrix);
	output.UV = input.UV;
    output.Normal = mul(normalize(input.Normal), (float3x3) gWorldMatrix);
	return output;
}

//------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------

float4 PixelShading(SamplerState sampleState, VS_OUTPUT input)
{
	//diffuse reflectivity
	const float kd = 1.f;

	//observedArea
	float observedArea = 0;
    observedArea = saturate(dot(input.Normal, -gLightDirection));
	
	//Diffuse
    const float4 lambertDiffuse = (kd * gDiffuseMap.Sample(sampleState, input.UV)) / gPi;

    return (lambertDiffuse * gLightIntensity * observedArea);
}

float4 PS_Point(VS_OUTPUT input) : SV_TARGET
{
	//Output.RGBColor = g_MeshTexture.Sample(MeshTextureSampler, In.TextureUV) * In.Diffuse;
	
    //input.WorldPosition = PixelShading(samPoint, input);

    return PixelShading(samPoint, input);
}

float4 PS_Linear(VS_OUTPUT input) : SV_TARGET 
{
    //input.WorldPosition = PixelShading(samLinear, input);
    return PixelShading(samLinear, input);
}

float4 PS_Anisotropic(VS_OUTPUT input) : SV_TARGET 
{
    //input.WorldPosition = PixelShading(samAnisotropic, input);
    return PixelShading(samAnisotropic, input);
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





