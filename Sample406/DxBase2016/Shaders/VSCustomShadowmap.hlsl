

cbuffer CustomShadowmapConstantBuffer : register(b0)
{
	float4x4 World				: packoffset(c0);
	float4x4 View				: packoffset(c4);
	float4x4 Projection				: packoffset(c8);
};

struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float4 color : COLOR0;
};


float4 main(VertexShaderInput input) : SV_POSITION
{
	float4 Output;
	input.pos.w = 1.0f;
	Output = mul(input.pos, World);
	Output = mul(Output, View);
	Output = mul(Output, Projection);
	return Output;
}