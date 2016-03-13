
cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir	: packoffset(c12);
	float4 Param : packoffset(c13);
};

struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};
