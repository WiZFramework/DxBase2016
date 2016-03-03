//--------------------------------------------------------------------------------------
// File: VSPDirect.hlsl
//
//--------------------------------------------------------------------------------------
#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	float4 Diffuse : packoffset(c0);
};



PSPCInput main(float4 position : SV_POSITION)
{
	PSPCInput result;

	result.position = position;
	result.color = Diffuse;

	return result;
}
