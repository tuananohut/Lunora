Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);

// SKY BOX 
TextureCube SkyMap : register(t1);
SamplerState SkySampler : register(s1);

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

	float wavelength = 0.8f; 
	float freq = 2.f / wavelength;

	float baseAmp = 0.08f; 
	float baseSpeed = 0.2f;

	float2 grad = float2(0.0f, 0.0f);

	float2 dirs[3] =
	{
		normalize(float2(-0.9,  0.2)),
		normalize(float2(-0.7,  0.6)),
		normalize(float2(-0.3, -1.0))
	};

	/*
	for (int i = 1; i <= 3; i++)
	{
		float fi = freq * i;
		float ai = baseAmp / (i * i);
		float si = baseSpeed * i;
		     
		height += Wave(dirs[i-1], fi, si, ai, surface, t);
	}
	*/

	for (int i = 0; i < 3; i++)
	{
		float fi = freq * (i + 1);
    		float ai = baseAmp / ((i + 1) * (i + 1));
    		float si = baseSpeed * (i + 1);
    		
    		float phase = dot(dirs[i], surface) * fi + si * fi * time;
    		
    		float s = sin(phase);
    		float c = cos(phase);
    		
    		height += ai * s;
    		
    		grad.x += ai * c * fi * dirs[i].x;
    		grad.y += ai * c * fi * dirs[i].y;
	}

	float dist = length(cameraPosition.xz - output.position.xz);
	float horizonFade = saturate((dist - 150.0f) / 200.0f);

	height *= (1.0f - horizonFade);

	output.position.y += height;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	float3 normal = normalize(float3(-grad.x, 1.0f, -grad.y));

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
	float4 diffuseColor = float4(0.f, 0.02f, 0.04f, 1.f);
	float3 lightDirection = float3(-0.5f, -1.f, -0.7f);
	float4 specularColor = float4(1.f, 1.f, 1.f, 1.f); 
	float specularPower = 64.f; 

	float4 textureColor;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float specular;
	
	float dist = length(input.view); 
	float fadeStart = 200.0f;
	float fadeEnd   = 350.0f;

	float fog = saturate((dist - fadeStart) / (fadeEnd - fadeStart));
	float horizonNormalFade = fog; 

	float3 N = normalize(input.normal);
	float3 V = normalize(input.view);
	float3 L = normalize(-lightDirection);

	float NdotL = saturate(dot(N, L));

	float3 H = normalize(L + V);
	float specularTerm = pow(saturate(dot(N, H)), specularPower);

	float3 reflectionDir = reflect(-V, N);
	float t = saturate(reflectionDir.y * 0.5 + 0.5);

	float3 envColor = lerp(float3(0.05, 0.1, 0.2), float3(0.4, 0.6, 0.9), t);

	N = normalize(lerp(N, float3(0,1,0), horizonNormalFade));

	float NdotV = saturate(dot(N, V));
	float F0 = 0.02f;
	float fresnel = F0 + (1.0f - F0) * pow(1.0f - NdotV, 5.0f);
	
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	ambientColor = CalcAmbient(N, textureColor); 

	color = ambientColor * textureColor; 

	specular = float4(0.f, 0.f, 0.f, 0.f);

	lightIntensity = saturate(dot(N, L));

	if (lightIntensity > 0.f)
	{
		color += (diffuseColor * lightIntensity);

		color = saturate(color);

		// reflection = normalize(2.f * lightIntensity * N - L);

		// specular = pow(saturate(dot(reflection, input.view)), specularPower);
	}

	specular *= (1.0f - fog);
	
	float3 horizonColor = float3(0.4f, 0.55f, 0.7f); 
	
	color.rgb *= (1.0f - fresnel);
	color.rgb += specularColor.rgb * specularTerm * fresnel;
	color.rgb += envColor * specularTerm * fresnel;

	return color;
}
