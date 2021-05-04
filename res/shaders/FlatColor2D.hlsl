struct CPUOut {
	float2 position : POSITION;
	float4 color : COLOR;
	float2 uvCoord : UVCOORD;
};

struct VertexOut {
	float4 position : SV_Position;
	float4 color : COLOR;
};

cbuffer Material {
	float4 color;
}

float4 PSMain(VertexOut input) : SV_TARGET
{
	return input.color * color;
}

VertexOut VSMain(CPUOut input)
{
	VertexOut output;
	output.position = float4(input.position, 0.0, 1.0);
	output.color = input.color;
	return output;
}
