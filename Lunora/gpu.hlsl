cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 Col : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Col : COLOR;
};

PS_INPUT vertex_shader(VS_INPUT input)
{
    PS_INPUT output;

    input.Pos.w = 1.0f;

    output.Pos = mul(input.position, world);
    output.Pos = mul(output.position, view);
    output.Pos = mul(output.position, proj);

    output.Col = input.Col;
    
    return output;
}

float4 pixel_shader(PS_INPUT input) : SV_TARGET
{
    return input.Col;
}
