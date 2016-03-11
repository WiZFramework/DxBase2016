/*!
@file DeviceResources.h
@brief Dx11デバイスリソース

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//汎用的な設定用定義

	//--------------------------------------------------------------------------------------
	//	enum class BlendState;
	//	用途: ブレンドステート
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	//	enum class DepthStencilState;
	//	用途: デプスステンシルステート
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	//	enum class RasterizerState;
	//	用途: ラスタライザステート
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
	};

	//--------------------------------------------------------------------------------------
	//	enum class SamplerState;
	//	用途: サンプラーステート
	//--------------------------------------------------------------------------------------
	enum class SamplerState {
		SamplerNone,
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};


	//--------------------------------------------------------------------------------------
	//	struct PipeLineDesc;
	/*!
	パイプライン定義
	*/
	//--------------------------------------------------------------------------------------
	struct PipeLineDesc {
		//--------------------------------------------------------------------------------------
		//	ブレンドステート
		//--------------------------------------------------------------------------------------
		BlendState m_BlendState;
		//--------------------------------------------------------------------------------------
		//	デプスステンシルステート
		//--------------------------------------------------------------------------------------
		DepthStencilState m_DepthStencilState;
		//--------------------------------------------------------------------------------------
		//	ラスタライザステート
		//--------------------------------------------------------------------------------------
		RasterizerState m_RasterizerState;
	};

	class RenderState;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX11デバイスクラス
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	DeviceResources(
		//		HWND hWnd,		//ウインドウのハンドル 
		//		bool isFullScreen, //フルスクリーンかどうか
		//		UINT Width,			//幅
		//		UINT Height			//高さ
		//	);
		/*!
		@breaf コンストラクタ
		@param HWND hWnd,	ウインドウのハンドル
		@param bool isFullScreen, フルスクリーンかどうか
		@param UINT Width,		幅
		@param UINT Height		高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		//	virtual ~DeviceResources();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//D3Dアクセサ
		//--------------------------------------------------------------------------------------
		//	ID3D11Device2*	GetD3DDevice() const;
		/*!
		@breaf D3D11Device2インターフェイスの取得
		@param なし
		@return	D3D11Device2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Device2* GetD3DDevice() const;
		ID3D11DeviceContext2* GetD3DDeviceContext() const;
		IDXGISwapChain1* GetSwapChain() const;
		D3D_FEATURE_LEVEL GetFeatureLevel() const;

		// D2D アクセサ
		ID2D1Factory2* GetD2DFactory() const;
		ID2D1Device1* GetD2DDevice() const;
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
//		ID2D1Bitmap1* GetD2DTargetBitmap() const;
		IDWriteFactory2* GetDWriteFactory() const;
		IWICImagingFactory2* GetWicImagingFactory() const;
		void InitializeStates(const shared_ptr<RenderState>& StatePtr);	//後始末

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;
		//ムーブ禁止
		DeviceResources(const DeviceResources&&) = delete;
		DeviceResources& operator=(const DeviceResources&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class DrawContext : public ObjectInterface;
	/*!
	描画用コンテクスト（各オブジェクトの描画クラス）
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource;
	class TextureResource;
	class DrawContext : public ObjectInterface {
		void SetVertexShader(ID3D11VertexShader* pVShader,bool SetConstantBuffer);
		bool IsVertexShaderSetConstantBuffer() const;
		void SetPixelShader(ID3D11PixelShader* pPShader,bool SetConstantBuffer);
		bool IsPixelShaderSetConstantBuffer() const;
		void SetInputLayout(ID3D11InputLayout* pInputLayout);

		void DrawIndexedBase(const shared_ptr<GameObject>& GameObjectPtr,
			ID3D11Buffer* pConstantBuffer, const void* CBData,
			const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset);


	public:
		DrawContext();
		virtual ~DrawContext();
		//仮想関数
		virtual void OnPreCreate() override {}
		virtual void OnCreate()override {}
		//パイプラインステートの設定
		void SetBlendState(BlendState State);
		void SetDepthStencilState(DepthStencilState State);
		void SetRasterizerState(RasterizerState State);
		void SetPipeLineDesc(const PipeLineDesc& Desc);
		const PipeLineDesc& GetPipeLineDesc()const;
		template<typename VS>
		void SetVertexShader(bool SetConstantBuffer) {
			SetVertexShader(VS::GetPtr()->GetShader(), SetConstantBuffer);
			SetInputLayout(VS::GetPtr()->GetInputLayout());
		}
		template<typename PS>
		void SetPixelShader(bool SetConstantBuffer) {
			SetPixelShader(PS::GetPtr()->GetShader(), SetConstantBuffer);
		}
		//void SetDefault2DPipeline() {
		//	PipeLineDesc Desc;
		//	Desc.m_BlendState = BlendState::Opaque;
		//	Desc.m_DepthStencilState = DepthStencilState::None;
		//	Desc.m_RasterizerState = RasterizerState::CullBack;
		//	SetPipeLineDesc(Desc);
		//}
		//void SetDefault3DPipeline() {
		//	PipeLineDesc Desc;
		//	Desc.m_BlendState = BlendState::Opaque;
		//	Desc.m_DepthStencilState = DepthStencilState::Default;
		//	Desc.m_RasterizerState = RasterizerState::CullBack;
		//	SetPipeLineDesc(Desc);
		//}
		void AddSamplerAndSrv(UINT PilelineNum, SamplerState state, ID3D11ShaderResourceView* Srv);

		template<typename CBType>
		void DrawIndexed(const shared_ptr<GameObject>& GameObjectPtr,
			const void* CBData,const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset){
			DrawIndexedBase(GameObjectPtr,
				CBType::GetPtr()->GetBuffer(), CBData,
				Mesh, stride, offset);
		}


	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

	};



}

//end basedx11
