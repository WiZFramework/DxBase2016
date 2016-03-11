//--------------------------------------------------------------------------------------
// File: VSPVTBone.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer BoneConstantBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightDir	: packoffset(c12);
	float4 Emissive : packoffset(c13);
	float4 Diffuse	: packoffset(c14);
	float4x3 Bones[72] : packoffset(c15);
};


void Skin(inout VSPNTBoneInput vin, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.indices[i]] * vin.weights[i];
	}

	vin.position.xyz = mul(vin.position, skinning);
	vin.norm = mul(vin.norm, (float3x3)skinning);
}



PSPNTInput main(VSPNTBoneInput input)
{
	PSPNTInput result;

	Skin(input, 4);

	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, World);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	//���C�e�B���O
	result.norm = mul(input.norm, (float3x3)World);
	result.norm = normalize(result.norm);
	//�e�N�X�`��UV
	result.tex = input.tex;
	return result;
}
