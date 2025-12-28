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

cbuffer HemiConstants : register( b0 )
{
	float3 AmbientDown : packoffset( c0 );
 	float3 AmbientRange : packoffset( c1 );
}

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

	float3 Ambient = AmbientDown + up * AmbientUp;

	return Ambient * color;
}

float4 LightPixelShader(PixelInputType input): SV_TARGET
{
	
	float3 normal = normalize(input.normal);

	
}