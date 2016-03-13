

cbuffer CustomShadowmapConstantBuffer : register(b0)
{
	float4x4 View				: packoffset(c0);
	float4x4 Projection				: packoffset(c4);
};

struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // インスタンスごとに設定される行列
	uint InstanceId           : SV_InstanceID;   // インスタンスＩＤ
};


float4 main(VertexShaderInput input) : SV_POSITION
{
	float4 Output;
	input.pos.w = 1.0f;
	Output = mul(input.pos, input.mat);
	Output = mul(Output, View);
	Output = mul(Output, Projection);
	return Output;
}