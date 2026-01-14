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
	float amplitude; // A 
	float2 direction; // k
	float time;
	float speed; // w 
};

PixelInputType WaterVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);

	float2 surface = output.position.xz; 

	float phase = dot(direction, surface) + speed * time;
	
	float height = amplitude * sin(phase);	

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