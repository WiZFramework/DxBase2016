#pragma once

#include "stdafx.h"

namespace basedx11{


	//Shadow

	struct ShadowConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMVECTOR Bones[3 * 72];	//Bone用
		ShadowConstantBuffer() {
			memset(this, 0, sizeof(ShadowConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow,ShadowConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBShadow : public ConstantBuffer<CBShadow, ShadowConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	用途: VSShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>{
	public:
		//構築
		VSShadowmap();
	};

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSShadowmapBone頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>{
	public:
		//構築
		VSShadowmapBone();
	};

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	用途: PSShadowmapピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PSShadowmap : public PixelShader<PSShadowmap>{
	public:
		//構築
		PSShadowmap();
	};

	//PDirect
	DECLARE_DX11_VERTEX_SHADER(VSPDirect, VertexPosition)
	DECLARE_DX11_PIXEL_SHADER(PSPDirect)
	struct PDirectConstantBuffer
	{
		Color4 Diffuse;
		PDirectConstantBuffer() {
			memset(this, 0, sizeof(PDirectConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPDirect, PDirectConstantBuffer)

	//PCDirect
	DECLARE_DX11_VERTEX_SHADER(VSPCDirect, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCDirect)

	//PTDirect
	DECLARE_DX11_VERTEX_SHADER(VSPTDirect, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTDirect)

	//PCTDirect
	DECLARE_DX11_VERTEX_SHADER(VSPCTDirect, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTDirect)


	//スプライト用コンスタントバッファ構造体
	struct SpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)
	// Diffuse入りコンスタントバッファ
	struct DiffuseSpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		Color4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBDiffuseSprite, DiffuseSpriteConstantBuffer)

	//PCSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCSprite, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCSprite)

	//PTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)

	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)




	//PCStatic
	struct PCStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Color4 Emissive;
		PCStaticConstantBuffer() {
			memset(this, 0, sizeof(PCStaticConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPCStatic, PCStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)



	//PTStatic
	struct PTStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Color4 Emissive;
		Color4 Diffuse;
		PTStaticConstantBuffer() {
			memset(this, 0, sizeof(PTStaticConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPTStatic, PTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPTStatic, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTStatic)


	//PCTStatic
	struct PCTStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Color4 Emissive;
		PCTStaticConstantBuffer() {
			memset(this, 0, sizeof(PCTStaticConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPCTStatic, PCTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPCTStatic, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTStatic)


	//PNTStatic
	struct PNTStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStatic, PNTStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticNoTexture)


	struct PNTStaticShadowConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		Vector4 LightPos;
		Vector4 EyePos;
		XMUINT4 ActiveFlg;			//テクスチャ=xがアクティブかどうか
		Matrix4X4 LightView;
		Matrix4X4 LightProjection;
		PNTStaticShadowConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticShadowConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBPNTStaticShadow, PNTStaticShadowConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)








}

//end basedx11