//--------------------------------------------------------------------------------------
// File: VSPCTDirect.hlsl
//
//--------------------------------------------------------------------------------------
#include "INCStructs.hlsli"


PSPCTInput main(VSPCTInput input)
{
	PSPCTInput result;

	result.position = input.position;
	result.color = input.color;
	result.tex = input.tex;

	return result;
}