struct VertexIn {
	float2 position : POSITION;
};

struct VertexOut {
	float4 position : SV_Position;
};

cbuffer Material {
	float4 color;
};

float4 PSMain(VertexOut input) : SV_TARGET
{
	return float4(input.position.xyz, 1.0);
}

VertexOut VSMain(VertexIn input)
{
	VertexOut output;
	output.position = float4(input.position, 0.0, 1.0);
	return output;
}
