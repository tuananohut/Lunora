struct VSIn
{
	uint vertexId: SV_VertexID;
};

struct VSOut
{
	float4 pos: SV_Position;
	float4 color: color;
};

VSOut VSmain(VSIn input)
{
	VSOut output;

	if (input.vertexId == 0)
 	   output.pos = float4(0.0, 0.5, 0.5, 1.0);
 	else if (input.vertexId == 2)
 	     output.pos = float4(0.5, -0.5, 0.5, 1.0);
 	else if (input.vertexId == 1)
 	     output.pos = float4(-0.5, -0.5, 0.5, 1.0);

 	output.color = clamp(output.pos, 0, 1);

 	return output;
}