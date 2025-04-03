// Ambient Lighting

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position: POSITION;
    float3 normal: NORMAL;
};

struct PixelInputType
{
    float4 position: SV_POSITION;
    float3 normal: NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.normal = mul(input.normal, (float3x3)worldMatrix);

    output.normal = normalize(output.normal);

    return output;
}