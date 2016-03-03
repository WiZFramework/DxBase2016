#pragma once
#include "stdafx.h"

namespace basedx11{



	//--------------------------------------------------------------------------------------
	//	class RenderState;
	//	用途: レンダリングステートクラス
	//--------------------------------------------------------------------------------------
	class RenderState{
	public:
		//--------------------------------------------------------------------------------------
		//	RenderState();
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		RenderState();
		//デストラクタ
		virtual ~RenderState();

		//--------------------------------------------------------------------------------------
		//	用途: ブレンドステートアクセッサ
		//	戻り値: ブレンドステートのポインタ
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetOpaque()const;
		ID3D11BlendState* GetAlphaBlend()const;
		ID3D11BlendState* GetAlphaBlendEx()const;
		ID3D11BlendState* GetAdditive()const;
		ID3D11BlendState* GetNonPremultiplied()const;
		//--------------------------------------------------------------------------------------
		//	用途: デプスステンシルステートアクセッサ
		//	戻り値: デプスステンシルステートのポインタ
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilState* GetDepthNone()const;
		ID3D11DepthStencilState* GetDepthDefault()const;
		ID3D11DepthStencilState* GetDepthRead()const;
		//--------------------------------------------------------------------------------------
		//	用途: ラスタライザステートアクセッサ
		//	戻り値: ラスタライザステートのポインタ
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullNone()const;
		ID3D11RasterizerState* GetCullNoneScissor()const;
		ID3D11RasterizerState* GetCullFront()const;
		ID3D11RasterizerState* GetCullFrontScissor()const;
		ID3D11RasterizerState* GetCullBack()const;
		ID3D11RasterizerState* GetCullBackScissor()const;
		ID3D11RasterizerState* GetWireframe()const;
		ID3D11RasterizerState* GetWireframeScissor()const;
		//--------------------------------------------------------------------------------------
		//	用途: サンプラーステートアクセッサ
		//	戻り値: サンプラーステートのポインタ
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetPointWrap()const;
		ID3D11SamplerState* GetPointClamp()const;
		ID3D11SamplerState* GetLinearWrap()const;
		ID3D11SamplerState* GetLinearClamp()const;
		ID3D11SamplerState* GetAnisotropicWrap()const;
		ID3D11SamplerState* GetAnisotropicClamp()const;
		ID3D11SamplerState* GetComparisonLinear()const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		RenderState(const RenderState&) = delete;
		RenderState& operator=(const RenderState&) = delete;
		//ムーブ禁止
		RenderState(const RenderState&&) = delete;
		RenderState& operator=(const RenderState&&) = delete;
	};



	//--------------------------------------------------------------------------------------
	//	class RenderTarget;
	//	用途: レンダーターゲット基底クラス
	//--------------------------------------------------------------------------------------
	class RenderTarget{
	protected:
		//構築と破棄
		RenderTarget(){}
		virtual ~RenderTarget(){}
	public:
		//レンダリングターゲットを開始する
		//	＊純粋仮想関数
		virtual void ClearViews() = 0;
		//	＊純粋仮想関数
		virtual void StartRenderTarget() = 0;
		//レンダリングターゲットを終了する
		//	＊純粋仮想関数
		virtual void EndRenderTarget() = 0;
	private:
		//コピー禁止
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		//ムーブ禁止
		RenderTarget(const RenderTarget&&) = delete;
		RenderTarget& operator=(const RenderTarget&&) = delete;
	};

	class View;
	class Stage;

	//--------------------------------------------------------------------------------------
	//	class ShadowMapRenderTarget: public RenderTarget;
	//	用途: シャドウマップのレンダリングターゲット
	//--------------------------------------------------------------------------------------
	class ShadowMapRenderTarget : public RenderTarget{
	public:
		//構築と破棄
		ShadowMapRenderTarget(float ShadowMapDimension);
		virtual ~ShadowMapRenderTarget();

		//アクセサ
		float GetShadowMapDimension() const;
		ID3D11ShaderResourceView* GetShaderResourceView() const;
		ID3D11Texture2D*		GetDepthStencil() const;
		ID3D11DepthStencilView*	GetDepthStencilView() const;

		//操作
		//レンダリングターゲットをクリアする
		virtual void ClearViews()override;
		//レンダリングターゲットを開始する
		virtual void StartRenderTarget()override;
		//レンダリングターゲットを終了する
		virtual void EndRenderTarget()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	用途: デフォルトのレンダーターゲット
	//--------------------------------------------------------------------------------------
	class DefaultRenderTarget : public RenderTarget{
	public:
		//構築と破棄
		explicit DefaultRenderTarget(const shared_ptr<Stage>& stage);
		virtual ~DefaultRenderTarget();

		//アクセサ
		ID3D11RenderTargetView* GetRenderTargetView() const;
		ID3D11Texture2D*		GetDepthStencil() const;
		ID3D11DepthStencilView*	GetDepthStencilView() const;
		ID2D1Bitmap1*			GetD2DTargetBitmap() const;

		//操作
		//スクリーン全体を黒でクリアする
		virtual void ClearViews()override;
		//レンダリングターゲットを開始する
		virtual void StartRenderTarget()override;
		//レンダリングターゲットを終了する
		virtual void EndRenderTarget()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





}
//end basedx11
