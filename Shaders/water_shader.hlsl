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

cbuffer Wave
{
	float time;
};


float Wave(
    float2 dir,
    float freq,
    float speed,
    float amp,
    float2 pos,
    float time)
{
    return amp * sin(dot(dir, pos) * freq + speed * time);
}

PixelInputType WaterVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);

	float2 surface = output.position.xz; 

	float height = 0.f;

	float t = time;

	height += Wave(normalize(float2( 1.0,  0.2)), 0.8, 1.5, 0.15, surface, t);
	height += Wave(normalize(float2(-0.7,  1.0)), 1.6, 2.2, 0.08, surface, t);
	height += Wave(normalize(float2( 0.3, -1.0)), 3.2, 3.0, 0.04, surface, t);
	height += Wave(normalize(float2(-1.0, -0.4)), 6.4, 4.5, 0.02, surface, t);
	height += Wave(normalize(float2( -1.0,-0.2)), 0.4, 3.5, 0.18, surface, t);
	height += Wave(normalize(float2( 0.7, -1.0)), 2.6, 4.2, 0.08, surface, t);
	height += Wave(normalize(float2(-0.3,  1.0)), 3.2, 3.0, 0.04, surface, t);
	height += Wave(normalize(float2( 1.0,  0.4)), 6.4, 4.5, 0.02, surface, t);

	output.position.y += height;

	output.position.y += height; 

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output; 
}

float4 WaterPixelShader(PixelInputType input): SV_TARGET
{
	
}