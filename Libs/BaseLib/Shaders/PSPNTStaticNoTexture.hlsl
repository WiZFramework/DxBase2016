//--------------------------------------------------------------------------------------
// File: PSPVTStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer SimpleConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir	: packoffset(c12);
	float4 Emissive : packoffset(c13);
	float4 Diffuse	: packoffset(c14);
};

float4 main(PSPNTInput input) : SV_TARGET
{
	//�@�����C�e�B���O
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	float4 Light = saturate(dot(N1, -lightdir) + Diffuse);
	Light.a = Diffuse.a;
	//�G�~�b�V�u�𑫂�
	return saturate(Light + Emissive);
}
