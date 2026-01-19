Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);

cbuffer MatrixBuffer: register(b0)
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
	float3 view: TEXCOORD2; 
};

cbuffer Wave: register(b1)
{
	float3 cameraPosition;
    	float time; 
};


/*
cbuffer WaveLightReflection: register(b2)
{
	float3 lightDirection;
	float specularPower;
	float4 specularColor; 
};
*/

float Wave(
    float2 dir,
    float freq,
    float speed,
    float amp,
    float2 pos,
    float time)
{
	float constant = speed * freq; 
	return amp * sin(dot(dir, pos) * freq + constant * time);
}

PixelInputType WaterVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition; 

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);

	float2 surface = output.position.xz; 

	float height = 0.f;

	float t = time;

	float wavelength = 1.0; 
	float freq = 2.0 / wavelength; 
	
	height += Wave(normalize(float2( 1.0,  0.2)), 0.8, 1.5, 0.15, surface, t);
	height += Wave(normalize(float2(-0.7,  1.0)), 1.6, 2.2, 0.08, surface, t);
	height += Wave(normalize(float2( 0.3, -1.0)), 3.2, 3.0, 0.04, surface, t);
	height += Wave(normalize(float2(-1.0, -0.4)), 6.4, 4.5, 0.02, surface, t);
	height += Wave(normalize(float2( -1.0,-0.2)), 0.4, 3.5, 0.18, surface, t);
	height += Wave(normalize(float2( 0.7, -1.0)), 2.6, 4.2, 0.08, surface, t);
	height += Wave(normalize(float2(-0.3,  1.0)), 3.2, 3.0, 0.04, surface, t);
	height += Wave(normalize(float2( 1.0,  0.4)), 6.4, 4.5, 0.02, surface, t);

	output.position.y += height;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);

	output.view = cameraPosition.xyz - worldPosition.xyz;
	output.view = normalize(output.view);

	return output; 
}

float4 WaterPixelShader(PixelInputType input): SV_TARGET
{
	// float specularPower;
    	// float4 specularColor;
	// float3 reflection;
	// float4 specular;
	// float4 textureColor;
	// float lightIntensity;
	// 
	// lightIntensity = saturate(dot(input.normal, lightDir));
	// 
	textureColor = shaderTexture.Sample(SampleType, input.tex); 
	// 
	// if (lightIntensity > 0.f)
	// {
	// 
	// }
	
	
	return textureColor; 
}







