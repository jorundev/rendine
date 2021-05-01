struct VertexIn {
	float2 position : POSITION;
};

struct VertexOut {
	float2 position : POSITION;
};

cbuffer Material {
	float4 color;
};

float4 PSMain(VertexOut input) : SV_TARGET
{
	return color;
}

VertexOut VSMain(VertexIn input)
{
	VertexOut output;
	output.position = input.position;
	return output;
}
