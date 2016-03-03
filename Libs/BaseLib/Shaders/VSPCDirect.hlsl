//--------------------------------------------------------------------------------------
// File: VSPCDirect.hlsl
//
//--------------------------------------------------------------------------------------
#include "INCStructs.hlsli"

PSPCInput main(float4 position : SV_POSITION, float4 color : COLOR)
{
	PSPCInput result;

	result.position = position;
	result.color = color;

	return result;
}
