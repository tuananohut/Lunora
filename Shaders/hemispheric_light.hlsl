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
};

cbuffer HemiConstants: register(b1)
{
    float3 AmbientDown;
    float pad1; 
    float3 AmbientRange;
    float time; /*This is for wave test, normally it is pad0*/	
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

PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);

	/* Wave Test */
	float amplitude = 0.1f; // A 
	float2 direction = float2(1.f, -1.f); // k
	float speed = 2.f; // w 

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
	
	/*Wave Test*/

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

	float3 ambient = AmbientDown.xyz + up * AmbientRange.xyz;

	return ambient * color;
}

float4 LightPixelShader(PixelInputType input): SV_TARGET
{
	float4 textureColor;  
	float3 ambientColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);

	ambientColor = CalcAmbient(input.normal, textureColor.xyz);

	return float4(ambientColor, 1.0); 
}