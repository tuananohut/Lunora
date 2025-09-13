cbuffer CBMatrix : register(b0)
{
    float4x4 mvp;
}

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
    output.Pos = mul(float4(input.Pos,1.0), mvp);
    output.Col = input.Col;
    return output;
}

float4 pixel_shader(PS_INPUT input) : SV_TARGET
{
    return input.Col;
}
