
#include "InstanceInc.hlsli"


PixelShaderInput main(VertexShaderInput input)
{
	//ピクセルシェーダーに渡す変数
	PixelShaderInput vertexShaderOutput;
	//頂点の位置を変換
	float4 pos = float4(input.pos.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, input.mat);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	vertexShaderOutput.pos = pos;
	//テクスチャのUV値を設定
	vertexShaderOutput.tex = input.tex;
	//ライティング用に法線をワールド変換して設定
	vertexShaderOutput.norm = mul(input.norm, (float3x3)input.mat);
	//影用の値を計算
	float4 modelPos = float4(input.pos.xyz, 1.0f);
	modelPos = mul(modelPos, input.mat);
	float4 lightSpacePos = mul(modelPos, LView);
	lightSpacePos = mul(lightSpacePos, LProjection);
	vertexShaderOutput.lightSpacePos = lightSpacePos;
	// Light ray
	float3 lRay = LPos.xyz - modelPos.xyz;
	vertexShaderOutput.lRay = lRay;
	vertexShaderOutput.View = EyePos.xyz - modelPos.xyz;

	float4x4 LightWorldViewProj = input.mat;
	LightWorldViewProj = mul(LightWorldViewProj, LView);
	LightWorldViewProj = mul(LightWorldViewProj, LProjection);
	vertexShaderOutput.posLWVP = mul(input.pos, LightWorldViewProj);

	//ピクセルシェーダに出力
	//returnはピクセルシェーダに渡すことを意味する
	return vertexShaderOutput;
}
