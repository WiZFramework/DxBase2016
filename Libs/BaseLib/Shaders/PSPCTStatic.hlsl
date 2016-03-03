//--------------------------------------------------------------------------------------
// File: PSPCTSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 Emissive : packoffset(c12);
};

float4 main(PSPCTInput input) : SV_TARGET
{
	//�e�N�X�`���ƃf�t�B�[�Y���烉�C�e�B���O���쐬
	float4 Light = g_texture.Sample(g_sampler, input.tex) * input.color;
	//�G�~�b�V�u�𑫂�
	return saturate(Light + Emissive);
}

