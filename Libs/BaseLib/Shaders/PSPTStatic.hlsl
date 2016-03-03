
//--------------------------------------------------------------------------------------
// File: VSPTStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
	float4 Diffuse : packoffset(c13);
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSPTInput input) : SV_TARGET
{
	//テクスチャとデフィーズからライティングを作成
	float4 Light = g_texture.Sample(g_sampler, input.tex) * saturate(Diffuse);
	//エミッシブを足す
	return saturate(Light + Emissive);
}
