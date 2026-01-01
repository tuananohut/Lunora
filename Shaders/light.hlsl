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
	float3 normal: NORMAL; 
	float2 tex: TEXCOORD0;
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
 	float3 AmbientRange; 
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

float4 CalcAmbient(float3 normal, float3 color)
{
	float up = normal.y * 0.5 + 0.5;

	float3 Ambient = AmbientDown + up * AmbientUp;

	return float4(Ambient * color, 1.f);
}

float4 LightPixelShader(PixelInputType input): SV_TARGET
{
	float4 textureColor; 
	float3 color; 
	float4 ambientColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex); 

	color = float3(1.f, 1.f, 1.f);

	ambientColor = float4(CalcAmbient(input.normal, color));

	return ambientColor; 
}