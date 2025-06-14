cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position: POSITION;
	float4 color: COLOR;
};

VertexInputType ColorVertexShader(VertexInputType input)
{
	VertexInputType output;

	output.position = input.position;

	output.color = input.color;

	return output;
}