//--------------------------------------------------------------------------------------
// File: VSPTDirect.hlsl
//
//--------------------------------------------------------------------------------------
#include "INCStructs.hlsli"


PSPTInput main(float4 position : SV_POSITION, float2 uv : TEXCOORD)
{
	PSPTInput result;

	result.position = position;
	result.tex = uv;

	return result;
}
