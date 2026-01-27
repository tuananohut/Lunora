cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix; 
};

cbuffer ColorBuffer
{
	float4 apexColor;
	float4 centerColor; 
};

struct VertexInputType
{
	float4 position: POSITION; 
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 domePosition: TEXCOORD0; 
};

PixelInputType SkyDomeVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.domePosition = input.position; 

	return output; 
}

float4 SkyDomePixelShader(PixelInputType input): SV_TARGET
{
	float height;
	float4 outputColor;

	height = input.domePosition.y;

	if (height < 0.0)
	{
		height = 0.f; 
	}

	outputColor = lerp(centerColor, apexColor, height);

	return outputColor;
}