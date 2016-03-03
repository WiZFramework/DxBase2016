#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	用途: Direct11デバイスイディオム
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl{
		D3D_DRIVER_TYPE	m_D3DDriverType;			//ドライバのタイプ
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		// Direct3D オブジェクト。
		ComPtr<ID3D11Device2> m_D3D11Device;		//デバイス
		ComPtr<ID3D11DeviceContext2> m_D3D11Context;		//コンテキスト
		ComPtr<IDXGISwapChain1> m_D3D11SwapChain;	//スワップチェーン


		// Direct2D 描画コンポーネント。
		ComPtr<ID2D1Factory2>		m_d2dFactory;
		ComPtr<ID2D1Device1>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext1>	m_d2dContext;
//		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		// DirectWrite 描画コンポーネント。
		ComPtr<IDWriteFactory2>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;

		float m_dpi;


		//構築と破棄
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl(){}
		//リソースの構築
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
	};
	//構築
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		m_dpi(96.0f)
	{
		try{
			//リソースの構築
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...){
			throw;
		}
	}
	//リソースの構築
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height){

		// Direct2D リソースを初期化します。
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// プロジェクトがデバッグ ビルドに含まれている場合は、Direct2D デバッグを SDK レイヤーを介して有効にします。
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		// Direct2D ファクトリを初期化します。
		ThrowIfFailed(
			D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory2),
			&options,
			&m_d2dFactory
			),
			L"Factory作成に失敗しました。",
			L"D2D1CreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		// DirectWrite ファクトリを初期化します。
		ThrowIfFailed(
			DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory2),
			&m_dwriteFactory
			),
			L"DirectWrite ファクトリ作成に失敗しました。",
			L"DWriteCreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_wicFactory)
			),
			L"(WIC) ファクトリ作成に失敗しました。",
			L"CoCreateInstance()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		HRESULT hr;	//APIの戻り値用
		//デバイス作成用フラグ
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		//デバッグ時
#ifdef _DEBUG
//		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//ドライバのタイプを配列化
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		//選択できるドライバーの数
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		//機能セットの配列化
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,	//Dx11機能オンリー
		};
		//選択できる機能の数
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//スワップチェーンのデスクプリタの設定
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;					//バッファ数(2ダブルバッファ)
		sd.BufferDesc.Width = Width;		//バッファの幅
		sd.BufferDesc.Height = Height;	//バッファの高さ
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//8 ビット アルファをサポートする 4 成分、16 ビット符号なし正規化整数フォーマット
		sd.BufferDesc.RefreshRate.Numerator = 60;	//リフレッシュレート最大値
		sd.BufferDesc.RefreshRate.Denominator = 1;	//リフレッシュレート最小値
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//出力用として定義
		sd.OutputWindow = hWnd;		//ウインドウのハンドル
		sd.SampleDesc.Count = 1;		//マルチサンプリング 数は1
		sd.SampleDesc.Quality = 0;		//マルチサンプリングクオリティは0（最大）
		//以下、ストアアプリは設定。デスクトップは指定しない（指定するとフルスクリーンにならない）
//		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		sd.Windowed = TRUE;			//ウインドウモードで作成してあとからフルsクリーンにする

		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext>	temp_context;
		ComPtr<IDXGISwapChain>	temp_swapChain;


		//デバイスとスワップチェーンの作成
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++){
			//ドライバタイプを配列から取得
			m_D3DDriverType = driverTypes[driverTypeIndex];
			//デバイスとスワップチェーンの作成
			hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				m_D3DDriverType,
				nullptr,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&temp_swapChain,
				&temp_device,
				&m_D3DFeatureLevel,
				&temp_context
				);
			//成功したらそのドライバを使う
			if (SUCCEEDED(hr))
				break;
		}
		ThrowIfFailed(
			hr, 
			L"DX11デバイスとスワップチェーンの作成に失敗しました。", 
			L"D3D11CreateDeviceAndSwapChain()", 
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		//リソースをバージョンアップする
		ThrowIfFailed(
			temp_device.As(&m_D3D11Device),
			L"DX11デバイスのバージョンアップに失敗しました。",
			L"temp_device.As(&m_D3D11Device)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_context.As(&m_D3D11Context),
			L"DX11コンテクストのバージョンアップに失敗しました。",
			L"temp_context.As(&m_D3D11Context)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		ThrowIfFailed(
			temp_swapChain.As(&m_D3D11SwapChain),
			L"DX11スワップチェーンのバージョンアップに失敗しました。",
			L"temp_swapChain.As(&m_D3D11SwapChain)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		// Direct2D デバイス オブジェクトと、対応するコンテキストを作成します。

		ComPtr<IDXGIDevice3> dxgiDevice;

		ThrowIfFailed(
			m_D3D11Device.As(&dxgiDevice),
			L"dxgiDeviceのバージョンアップに失敗しました。",
			L"m_d3dDevice.As(&dxgiDevice)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
			L"dxgiDeviceのバージョンアップに失敗しました。",
			L"m_d2dFactory->CreateDevice()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
			),
			L"2dデバイスコンテキスト作成に失敗しました。",
			L"m_d2dDevice->CreateDeviceContext()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		if (isFullScreen){
			ThrowIfFailed(
				m_D3D11SwapChain->SetFullscreenState(true, NULL),
				L"フルスクリーン移行に失敗しました。",
				L"m_D3D11SwapChain->SetFullscreenState(true, NULL)",
				L"DeviceResources::Impl::CreateDeviceResources()"
				);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//	用途: Direct11デバイスなどクラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		pImpl(new Impl(hWnd,isFullScreen,Width,Height))
	{
	}
	DeviceResources::~DeviceResources(){}
	//アクセサ
	ID3D11Device2*	DeviceResources::GetD3DDevice() const { return pImpl->m_D3D11Device.Get(); }
	ID3D11DeviceContext2* DeviceResources::GetD3DDeviceContext() const { return pImpl->m_D3D11Context.Get(); }
	IDXGISwapChain1* DeviceResources::GetSwapChain() const { return pImpl->m_D3D11SwapChain.Get(); }
	D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const { return pImpl->m_D3DFeatureLevel; }

	// D2D アクセサー。
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const					{ return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const					{ return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	DeviceResources::GetD2DDeviceContext() const				{ return pImpl->m_d2dContext.Get(); }
	IDWriteFactory2*		DeviceResources::GetDWriteFactory() const				{ return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const			{ return pImpl->m_wicFactory.Get(); }

	void DeviceResources::InitializeStates(const shared_ptr<RenderState>& StatePtr){
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11Buffer* pNullConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };

		auto pID3D11DeviceContext = GetD3DDeviceContext();
		//後始末
		//コンスタントバッファクリア
		pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		//シェーダーリソースもクリア
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//サンプラーもクリア
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//シェーダのクリア
		pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
		//インプットレイアウトのクリア
		pID3D11DeviceContext->IASetInputLayout(nullptr);
		//デフォルトの設定
		//リニアサンプラーを設定しておく
		ID3D11SamplerState* samplerState = StatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//ブレンド塗りつぶし
		pID3D11DeviceContext->OMSetBlendState(StatePtr->GetOpaque(), nullptr, 0xffffffff);
		//レンダーステート前面のみ描画
		pID3D11DeviceContext->RSSetState(StatePtr->GetCullBack());
		//デプスステンシル使用
		pID3D11DeviceContext->OMSetDepthStencilState(StatePtr->GetDepthDefault(), 0);

	}


	//--------------------------------------------------------------------------------------
	//	struct DrawContext::Impl;
	//	用途: DrawContextイディオム
	//--------------------------------------------------------------------------------------
	struct DrawContext::Impl {
		struct PipelineResource{
			UINT m_PipelineNum;
			SamplerState m_SamplerState;
			ID3D11ShaderResourceView* m_ShaderResourceView;
			PipelineResource() :
				m_PipelineNum(0), m_SamplerState(SamplerState::SamplerNone), m_ShaderResourceView(nullptr)
			{}
		};
		//パイプライリソースの定義
		vector<PipelineResource> m_PipelineResourceVec;
		//パイプラインの定義
		PipeLineDesc m_PipeLineDesc;
		//シェーダー
		ID3D11VertexShader* m_pVertexShader;
		bool m_VertexShaderConstant;
		ID3D11PixelShader* m_pPixelShader;
		bool m_PixelShaderConstant;
		ID3D11InputLayout* m_pInputLayout;
		Impl() :
			m_pVertexShader(nullptr),
			m_VertexShaderConstant(false),
			m_pPixelShader(nullptr),
			m_PixelShaderConstant(false),
			m_pInputLayout(nullptr)
		{}
	};
	//--------------------------------------------------------------------------------------
	//	class DrawContext : public ObjectInterface;
	//--------------------------------------------------------------------------------------
	DrawContext::DrawContext() :
		pImpl(new Impl())
	{}
	DrawContext::~DrawContext() {}

	//パイプラインステートの設定
	void DrawContext::SetBlendState(BlendState State) {
		pImpl->m_PipeLineDesc.m_BlendState = State;
	}
	void DrawContext::SetDepthStencilState(DepthStencilState State) {
		pImpl->m_PipeLineDesc.m_DepthStencilState = State;
	}
	void DrawContext::SetRasterizerState(RasterizerState State) {
		pImpl->m_PipeLineDesc.m_RasterizerState = State;
	}
	void DrawContext::SetPipeLineDesc(const PipeLineDesc& Desc) {
		pImpl->m_PipeLineDesc = Desc;
	}

	const PipeLineDesc& DrawContext::GetPipeLineDesc()const {
		return pImpl->m_PipeLineDesc;
	}

	void DrawContext::AddSamplerAndSrv(UINT PilelineNum, SamplerState state,ID3D11ShaderResourceView* Srv){
		Impl::PipelineResource res;
		res.m_PipelineNum = PilelineNum;
		res.m_SamplerState = state;
		res.m_ShaderResourceView = Srv;
		pImpl->m_PipelineResourceVec.push_back(res);
	}


	void DrawContext::DrawVertex(const shared_ptr<GameObject>& GameObjectPtr, 
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset,
		const shared_ptr<TextureResource>& Tex, bool Alpha) {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステージの取得
		auto PtrStage = GameObjectPtr->GetStage();
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//テクスチャがあればテクスチャの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		if (Tex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, Tex->GetShaderResourceView().GetAddressOf());
		}
		else {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		}
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		/*
		switch (GetPipeLineDesc().m_SamplerState) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		*/
		//デプスステンシル
		switch (GetPipeLineDesc().m_DepthStencilState) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//ブレンドステートとラスタライザを設定して描画
		//もし、Alphaが真ならAlphaBlendExに設定し、そうでなければ、指定に従う。
		if (Alpha) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//描画
			pID3D11DeviceContext->Draw(Mesh->GetNumVertices(), 0);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//描画
			pID3D11DeviceContext->Draw(Mesh->GetNumVertices(), 0);
		}
		else {
			switch (GetPipeLineDesc().m_BlendState) {
			case BlendState::Opaque:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				break;
			case BlendState::AlphaBlend:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				break;
			case BlendState::Additive:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
				break;
			case BlendState::NonPremultiplied:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
				break;
			}
			switch (GetPipeLineDesc().m_RasterizerState) {
			case RasterizerState::CullBack:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				break;
			case RasterizerState::CullFront:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				break;
			case RasterizerState::CullNone:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
				break;
			case RasterizerState::Wireframe:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				break;
			}
			//描画
			pID3D11DeviceContext->Draw(Mesh->GetNumVertices(), 0);
		}
		//後始末
		//後始末
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates(RenderStatePtr);
	}

	void DrawContext::DrawIndexed(const shared_ptr<GameObject>& GameObjectPtr, 
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset,
		const shared_ptr<TextureResource>& Tex, bool Alpha) {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステージの取得
		auto PtrStage = GameObjectPtr->GetStage();
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//テクスチャがあればテクスチャの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		if (Tex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, Tex->GetShaderResourceView().GetAddressOf());
		}
		else {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		}
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		/*
		switch (GetPipeLineDesc().m_SamplerState) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		*/
		//デプスステンシル
		switch (GetPipeLineDesc().m_DepthStencilState) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//ブレンドステートとラスタライザを設定して描画
		//もし、Alphaが真ならAlphaBlendExに設定し、そうでなければ、指定に従う。
		if (Alpha) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		else {
			switch (GetPipeLineDesc().m_BlendState) {
			case BlendState::Opaque:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				break;
			case BlendState::AlphaBlend:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				break;
			case BlendState::Additive:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
				break;
			case BlendState::NonPremultiplied:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
				break;
			}
			switch (GetPipeLineDesc().m_RasterizerState) {
			case RasterizerState::CullBack:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				break;
			case RasterizerState::CullFront:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				break;
			case RasterizerState::CullNone:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
				break;
			case RasterizerState::Wireframe:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				break;
			}
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		//後始末
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates(RenderStatePtr);
	}

	void DrawContext::DrawIndexedBase(const shared_ptr<GameObject>& GameObjectPtr,
		ID3D11Buffer* pConstantBuffer, const void* CBData,
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステージの取得
		auto PtrStage = GameObjectPtr->GetStage();
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, CBData, 0, 0);
		//コンスタントバッファの各シェーダへ設置
		if (pImpl->m_VertexShaderConstant){
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		if (pImpl->m_PixelShaderConstant){
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		for (auto res : pImpl->m_PipelineResourceVec){
			pID3D11DeviceContext->PSSetShaderResources(res.m_PipelineNum, 1, &res.m_ShaderResourceView);
			//nullに初期化
			ID3D11SamplerState* pSampler = nullptr;
			switch (res.m_SamplerState) {
			case SamplerState::SamplerNone:
			break;
			case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
			case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
			case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
			case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
			case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
			case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
			case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
			}
			//サンプラーを設定
			pID3D11DeviceContext->PSSetSamplers(res.m_PipelineNum, 1, &pSampler);
		}
		//デプスステンシル
		switch (GetPipeLineDesc().m_DepthStencilState) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//ブレンドステートとラスタライザを設定して描画
		//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
		if (GameObjectPtr->GetAlphaActive()) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			//ラスタライザステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		else {
			switch (GetPipeLineDesc().m_BlendState) {
			case BlendState::Opaque:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				break;
			case BlendState::AlphaBlend:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				break;
			case BlendState::Additive:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
				break;
			case BlendState::NonPremultiplied:
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
				break;
			}
			switch (GetPipeLineDesc().m_RasterizerState) {
			case RasterizerState::CullBack:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				break;
			case RasterizerState::CullFront:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				break;
			case RasterizerState::CullNone:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
				break;
			case RasterizerState::Wireframe:
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				break;
			}
			//描画
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		//後始末
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates(RenderStatePtr);
	}



	void DrawContext::SetVertexShader(ID3D11VertexShader* pVShader, bool SetConstantBuffer) {
		pImpl->m_pVertexShader = pVShader;
		pImpl->m_VertexShaderConstant = SetConstantBuffer;
	}

	bool DrawContext::IsVertexShaderSetConstantBuffer() const{
		return pImpl->m_VertexShaderConstant;
	}


	void DrawContext::SetPixelShader(ID3D11PixelShader* pPShader, bool SetConstantBuffer) {
		pImpl->m_pPixelShader = pPShader;
		pImpl->m_PixelShaderConstant = SetConstantBuffer;
	}

	bool DrawContext::IsPixelShaderSetConstantBuffer() const{
		return pImpl->m_PixelShaderConstant;
	}


	void DrawContext::SetInputLayout(ID3D11InputLayout* pInputLayout) {
		pImpl->m_pInputLayout = pInputLayout;
	}




}
//end basedx11
