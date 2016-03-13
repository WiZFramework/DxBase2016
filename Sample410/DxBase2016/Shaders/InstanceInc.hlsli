
cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 View	: packoffset(c0);
	float4x4 Projection	: packoffset(c4);
	float4 LightDir	: packoffset(c8);
	float4 Param : packoffset(c9);
	float4 LPos	: packoffset(c10);
	float4 EyePos	: packoffset(c11);
	float4x4 LView	: packoffset(c12);
	float4x4 LProjection	: packoffset(c16);
};

struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // インスタンスごとに設定される行列
	uint InstanceId           : SV_InstanceID;   // インスタンスＩＤ
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float3 lRay		: NORMAL1;
	float3 View : NORMAL2;
	float2 tex : TEXCOORD0;
	float4 posLWVP  : POSITION1;
	float4 lightSpacePos : POSITION2;
};
