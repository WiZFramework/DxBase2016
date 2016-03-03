#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBShadow : public ConstantBuffer<CBShadow>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBShadow, CBShadow::Deleter> CBShadow::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSShadowmap : public VertexShader<VSShadowmap, VertexPositionNormalTexture>;
	//	用途: VSShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSShadowmap, VSShadowmap::Deleter> VSShadowmap::m_Ptr;
	//構築
	VSShadowmap::VSShadowmap():
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class VSShadowmapBone : public VertexShader<VSShadowmapBone, VertexPositionNormalTextureSkinning>;
	//	用途: VSShadowmapBone頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSShadowmapBone, VSShadowmapBone::Deleter> VSShadowmapBone::m_Ptr;
	//構築
	VSShadowmapBone::VSShadowmapBone() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmapBone.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class PSShadowmap : public PixelShader<PSShadowmap>;
	//	用途: PSShadowmapピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSShadowmap, PSShadowmap::Deleter> PSShadowmap::m_Ptr;
	//構築
	PSShadowmap::PSShadowmap() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSShadowmap.cso")
	{}


	//PDirect
	IMPLEMENT_DX11_VERTEX_SHADER(VSPDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPDirect.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPDirect.cso")
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPDirect)


	//PCDirect
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCDirect.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCDirect.cso")

	//PTDirect
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTDirect.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTDirect.cso")

	//PCTDirect
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTDirect.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTDirect.cso")


	//Sprite用コンスタントバッファ
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBDiffuseSprite)

	//PCSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"VSPCSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"PSPCSprite.cso")


	//PTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"VSPTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"PSPTSprite.cso")

	//PCTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + +L"PSPCTSprite.cso")

	//PCStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPCStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")


	//PTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTStatic.cso")

	//PCTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPCTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTStatic.cso")



	//PNTStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticNoTexture, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticNoTexture.cso")


	//PNTStaticShadow
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNTStaticShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow2.cso")


}
//end basedx11
