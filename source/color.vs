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

	float3 color = 0.5 + 0.5 * cos(output.position.xyz + float3(Time, Time * 2.f, Time * 3.f));
	
	output.color = float4(color, 1.f);
	
	// output.color = input.color;

	return output;
}
