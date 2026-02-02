#define PI 3.14159265

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
	float3 tangent: TEXCOORD3;
	float3 binormal: TEXCOORD4; 
};

cbuffer Wave: register(b1)
{
	float3 cameraPosition;
    	float time; 
};

float4 CalcAmbient(float3 normal, float4 color)
{
	float4 AmbientDown = float4(0.01f, 0.03f, 0.08f, 1.f);
	float4 AmbientRange = float4(0.05f, 0.10f, 0.15f, 1.f);

	float up = normal.y * 0.5 + 0.5;

	float4 ambient = AmbientDown + up * AmbientRange;

	return ambient;
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

	float wavelength = 12.f; 
	float freq = 2.f / wavelength;

	int waveCount = 3;
	
	float baseAmp = 0.8f;
	float baseSpeed = 1.5f; 

	float2 grad = float2(0.0f, 0.0f);

	float2 dirs[3] =
	{
		normalize(float2( 1.0,  0.0)), 
		normalize(float2( 0.95, 0.3)), 
		normalize(float2( 0.85,-0.25)) 
	};

	for (int i = 0; i < waveCount; i++)
	{
		float k = 2.0f * PI / wavelength;

		float fi = freq;

		float ai = baseAmp * pow(0.6f,  i); 
    		float si = baseSpeed * (1.f + 0.5f * i);
		float omega = si * k;
    		
    		float phase = k * dot(dirs[i], surface) - omega * time; 
    		
    		float s = sin(phase);
    		float c = cos(phase);
    		
    		height += ai * s;
    		
    		grad.x += ai * c * k * dirs[i].x;
    		grad.y += ai * c * k * dirs[i].y;
	}

	output.position.y += height;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	float3 N = normalize(float3(-grad.x, 1.0f, -grad.y));
	
	output.normal = mul(N, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);
	 
	output.view = cameraPosition.xyz - worldPosition.xyz;
	output.view = normalize(output.view);

	return output; 
}

float4 WaterPixelShader(PixelInputType input) : SV_TARGET
{
	float4 diffuseColor = float4(0.0f, 0.02f, 0.04f, 1.0f);
	float3 lightDirection = float3(-0.5f, -1.0f, -0.7f);

	float4 textureColor;
	float4 color;
	float lightIntensity;

	float3 N = normalize(input.normal);
	float3 L = normalize(-lightDirection);

	textureColor = shaderTexture.Sample(SampleType, input.tex);

	float4 ambientColor = CalcAmbient(N, textureColor);

	lightIntensity = saturate(dot(N, L));
	float4 diffuse = diffuseColor * lightIntensity;

	color = ambientColor * textureColor;
	color += diffuse * textureColor;

	color = saturate(color);

	return color;
}
