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
	float4 ambientColor;
	float4 diffuseColor;
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

float2 WaveDeriv(
    float2 dir,
    float freq,
    float speed,
    float amp,
    float2 pos,
    float time)
{
    float phase = dot(dir, pos) * freq + speed * freq * time;
    float c = amp * cos(phase) * freq;

    return float2(
        c * dir.x, 
        c * dir.y  
    );
}

PixelInputType WaterVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition; 

	input.position.w = 1.f;

	output.position = mul(input.position, worldMatrix);

	float waveScale = 0.1f; 
	float2 surface = output.position.xz * waveScale; 

	float height = 0.f;

	float t = time;

	float wavelength = 1.0; 
	float freq = 2.0 / wavelength; 
	
	height += Wave(normalize(float2( 1.0,  0.2)), 0.6, 1.2, 0.20, surface, t);
	height += Wave(normalize(float2(-0.7,  1.0)), 1.2, 1.8, 0.08, surface, t);
	height += Wave(normalize(float2( 0.3, -1.0)), 2.4, 2.5, 0.03, surface, t);
	height += Wave(normalize(float2(-1.0, -0.4)), 3.6, 3.0, 0.02, surface, t);

	output.position.y += height;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	float2 grad = float2(0.0f, 0.0f);

	grad += WaveDeriv(normalize(float2( 1.0,  0.2)), 0.6, 1.2, 0.20, surface, t) * waveScale;
	grad += WaveDeriv(normalize(float2(-0.7,  1.0)), 1.2, 1.8, 0.08, surface, t) * waveScale;
	grad += WaveDeriv(normalize(float2( 0.3, -1.0)), 2.4, 2.5, 0.03, surface, t) * waveScale;
	grad += WaveDeriv(normalize(float2(-1.0, -0.4)), 3.6, 3.0, 0.02, surface, t) * waveScale;


	float3 normal;
	
	normal.x = -grad.x;
	normal.y = 1.0f;
	normal.z = -grad.y;

	normal = normalize(normal);

	output.normal = mul(normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);
	 
	output.view = cameraPosition.xyz - worldPosition.xyz;
	output.view = normalize(output.view);

	return output; 
}

float4 WaterPixelShader(PixelInputType input): SV_TARGET
{
	float4 ambientColor = float4(0.05f, 0.05f, 0.05f, 1.f);
	float4 diffuseColor = float4(0.11f, 0.302f, 0.553f, 1.f);
	float3 lightDirection = float3(1.f, 0.f, 1.f);
	float4 specularColor = float4(1.f, 1.f, 1.f, 1.f); 
	float specularPower = 64.f; 

	float4 textureColor;
	float3 lightDir; 
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;

	float3 N = normalize(input.normal);
	float3 V = normalize(input.view);

	float NdotV = saturate(dot(N, V));
	float F0 = 0.02f;
	float fresnel = F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);
	
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	color = ambientColor; 

	specular = float4(0.f, 0.f, 0.f, 0.f);

	lightDir = normalize(-lightDirection); 

	lightIntensity = saturate(dot(N, lightDir));

	if (lightIntensity > 0.f)
	{
		color += (diffuseColor * lightIntensity);

		color = saturate(color);

		reflection = normalize(2.f * lightIntensity * N - lightDir);

		specular = pow(saturate(dot(reflection, input.view)), specularPower);
	}

	color = color * textureColor;

	color.rgb *= (1.0f - fresnel);
	color.rgb += specularColor.rgb * specular * fresnel;

	return color; 
}
