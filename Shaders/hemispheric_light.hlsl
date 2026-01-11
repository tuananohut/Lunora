Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix; 
};

struct VertexInputType
{
	float4 position: POSITION;
	float2 tex: TEXCOORD0;
	float3 normal: NORMAL; 
};
	
struct PixelInputType
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
	float3 normal: TEXCOORD1; 
};

cbuffer HemiConstants
{
    float3 AmbientDown;
    float pad0; 
    float3 AmbientRange;
    float pad1; 
};

PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output; 
}

float3 CalcAmbient(float3 normal, float3 color)
{
	float up = normal.y * 0.5 + 0.5;

	float3 ambient = AmbientDown.xyz + up * AmbientRange.xyz;

	return ambient * color;
}

float4 LightPixelShader(PixelInputType input): SV_TARGET
{
	float4 textureColor;  
	float3 ambientColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
	textureColor *= textureColor; 

	ambientColor = CalcAmbient(input.normal, textureColor.xyz);

	return float4(ambientColor, 1.0); 
}