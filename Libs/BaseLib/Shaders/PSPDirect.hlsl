//--------------------------------------------------------------------------------------
// File: PSPDirect.hlsl
//
//--------------------------------------------------------------------------------------
#include "INCStructs.hlsli"


float4 main(PSPCInput input) : SV_TARGET
{
	return input.color;
}