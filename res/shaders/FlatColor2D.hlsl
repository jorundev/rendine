struct Mesh2D {
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

cbuffer Camera {
	float4x4 mvp;
}

float4 PSMain(VertexOut input) : SV_TARGET
{
	return input.color * color;
}

VertexOut VSMain(Mesh2D input)
{
	VertexOut output;
	output.position = mvp * float4(input.position, 0.0, 1.0);
	output.color = input.color;
	return output;
}
