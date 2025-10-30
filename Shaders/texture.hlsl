Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);

cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj; 
};

struct VertexInputType
{
	float4 position: POSITION;
	float2 tex: TEXCOORD0; 
};

struct PixelInputType
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{	       
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.tex = input.tex;

	return output; 
}

float4 TexturePixelShader(PixelInputType input): SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);

	return textureColor; 
}