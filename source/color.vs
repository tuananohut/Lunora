cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	float Time;
};

struct VertexInputType
{
	float4 position: POSITION;
	float4 color : COLOR;	
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float4 color: COLOR; 
};

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);
	
	output.color = input.color;

	return output;
}
