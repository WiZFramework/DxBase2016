
#include "InstanceInc.hlsli"

Texture2D<float4> SimpleTexture : register(t0);
// 深度マップ
Texture2D g_DepthMap : register(t1);

SamplerState SimpleSampler : register(s0);
SamplerState g_SamplerDepthMap : register(s1);


float4 main(PixelShaderInput input) : SV_TARGET
{
	//影があるかないかを決定
	float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
	// テクセルを計算
	float2 texel = float2(input.posLWVP.x / input.posLWVP.w * 0.5f + 0.5f, input.posLWVP.y / input.posLWVP.w * -0.5f + 0.5f);
	float depth = input.posLWVP.z / input.posLWVP.w - 0.00005f;
	float sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
		// 現在の深度値と深度マップ上の深度値を比較
		if (sm < depth){
			//影は70%の明度
			shadowColor.xyz = 0.7f;
		}
		else{
			shadowColor.xyz = 1.0f;
		}
	}
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	//最終色の決定
	//まずライティングの影響
	float4 RetColor = saturate(dot(N1, -lightdir) + float4(0.7, 0.7, 0.7, 1.0));
	//ライトの透明処理はなし
	RetColor.a = 1;
	//テクスチャと合わせる
	RetColor = SimpleTexture.Sample(SimpleSampler, input.tex) * RetColor;
	//影と合わせる
	RetColor.rgb = RetColor.rgb * shadowColor;
	//決定色リターン
	return RetColor;
}

