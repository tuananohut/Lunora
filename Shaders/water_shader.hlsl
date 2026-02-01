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

	float viewDist = length(cameraPosition.xz - output.position.xz);
	float wavelength = 10.f; 
	float freq = 2.f / wavelength;

	int waveCount = 3;
	
	float baseAmp = 0.6f;
	float baseSpeed = 1.f; 

	float2 grad = float2(0.0f, 0.0f);

	float2 dirs[3] =
	{
		normalize(float2(-0.9,  0.2)),
		normalize(float2(-0.7,  0.6)),
		normalize(float2(-0.3, -1.0))
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

	float dist = length(cameraPosition.xz - output.position.xz);
	float horizonFade = saturate((dist - 150.0f) / 200.0f);

	// height *= (1.0f - horizonFade);
	// grad *= (1.0f - horizonFade);

	float choppy = 0.6f;

	output.position.x += choppy * grad.x;
	output.position.z += choppy * grad.y;

	output.position.y += height;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;

	float3 N = normalize(float3(-grad.x, 1.0f, -grad.y));
	float3 T = normalize(float3(1, 0, 0) - N * dot(N, float3(1, 0, 0)));
	float3 B = normalize(cross(N, T));

	output.normal = mul(N, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent  = normalize(mul(T, (float3x3)worldMatrix));
	output.binormal = normalize(mul(B, (float3x3)worldMatrix));

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
	float4 specularColor = float4(0.04f, 0.045f, 0.05f, 1.f);

	float4 textureColor;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;

	float3 N = normalize(input.normal);
	float3 V = normalize(input.view);
	float3 L = normalize(-lightDirection);

	float NdotL = saturate(dot(N, L));

	float3 H = normalize(L + V);

	float eta = 1.0 / 1.333; 
	float3 refractDir = refract(-V, N, eta);
	
	float NdotV = saturate(dot(N, V));
	float F0 = 0.02f;
	float fresnel = F0 + (1.0f - F0) * pow(1.0f - NdotV, 3.0f);

	float depthFactor = saturate(1.0f - NdotV);
	float3 deepWaterColor = float3(0.0f, 0.15f, 0.25f);
	float3 shallowWaterColor = float3(0.05f, 0.25f, 0.3f);

	float3 refractColor = lerp(shallowWaterColor, deepWaterColor, depthFactor);

	float specularPower = lerp(4.f, 16.f, fresnel);

	float specularTerm = pow(saturate(dot(N, H)), specularPower);
	specularTerm *= 0.35f;

	float anisotropicSpec = 1.f;

	float3 reflectionDir = reflect(-V, N);
	float t = saturate(reflectionDir.y * 0.5 + 0.5);

	float horizon = smoothstep(0.45, 0.55, reflectionDir.y);
	float3 horizonColor = float3(0.8, 0.85, 0.9);

	float3 envColor = lerp(float3(0.05, 0.1, 0.2), float3(0.4, 0.6, 0.9), t);
		
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	ambientColor = CalcAmbient(N, textureColor); 

	color = ambientColor * textureColor; 

	specular = float4(0.f, 0.f, 0.f, 0.f);

	lightIntensity = saturate(dot(N, L));

	if (lightIntensity > 0.f)
	{
		color += diffuseColor * lightIntensity * (1.0f - fresnel) * 0.5f;

		color = saturate(color);

		// reflection = normalize(2.f * lightIntensity * N - L);

		// specular = pow(saturate(dot(reflection, input.view)), specularPower);
	}

	float3 reflectionColor = envColor + specularColor.rgb * specularTerm;
	float3 refractionColor = refractColor;

	float3 finalColor = lerp(refractionColor, reflectionColor, fresnel);
	finalColor *= saturate(NdotL * 0.5 + 0.5);

	color.rgb = finalColor;

	// color.rgb = lerp(color.rgb, color.rgb * 0.6f, fresnel);

	// color.rgb += specularColor.rgb * specularTerm * anisotropicSpec * fresnel;

	// color.rgb += envColor * fresnel;

	return color;
}
