
#include "SimpleInc.hlsli"


PixelShaderInput main(VertexShaderInput input)
{
	//ピクセルシェーダーに渡す変数
	PixelShaderInput vertexShaderOutput;
	//頂点の位置を変換
	float4 pos = float4(input.pos.xyz, 1.0f);
	//ワールド変換
	pos = mul(pos, World);
	//ビュー変換
	pos = mul(pos, View);
	//射影変換
	pos = mul(pos, Projection);
	//ピクセルシェーダに渡す変数に設定
	vertexShaderOutput.pos = pos;
	//頂点色を設定
	vertexShaderOutput.color = input.color;
	//ライティング用に法線をワールド変換して設定
	vertexShaderOutput.norm = mul(input.norm, (float3x3)World);
	//ピクセルシェーダに出力
	//returnはピクセルシェーダに渡すことを意味する
	return vertexShaderOutput;
}
