
cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Param : packoffset(c12);
};


struct VertexShaderInput
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};
