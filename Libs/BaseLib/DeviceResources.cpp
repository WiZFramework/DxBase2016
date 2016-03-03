#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	�p�r: Direct11�f�o�C�X�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl{
		D3D_DRIVER_TYPE	m_D3DDriverType;			//�h���C�o�̃^�C�v
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		// Direct3D �I�u�W�F�N�g�B
		ComPtr<ID3D11Device2> m_D3D11Device;		//�f�o�C�X
		ComPtr<ID3D11DeviceContext2> m_D3D11Context;		//�R���e�L�X�g
		ComPtr<IDXGISwapChain1> m_D3D11SwapChain;	//�X���b�v�`�F�[��


		// Direct2D �`��R���|�[�l���g�B
		ComPtr<ID2D1Factory2>		m_d2dFactory;
		ComPtr<ID2D1Device1>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext1>	m_d2dContext;
//		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		// DirectWrite �`��R���|�[�l���g�B
		ComPtr<IDWriteFactory2>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;

		float m_dpi;


		//�\�z�Ɣj��
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl(){}
		//���\�[�X�̍\�z
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
	};
	//�\�z
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		m_dpi(96.0f)
	{
		try{
			//���\�[�X�̍\�z
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...){
			throw;
		}
	}
	//���\�[�X�̍\�z
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height){

		// Direct2D ���\�[�X�����������܂��B
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// �v���W�F�N�g���f�o�b�O �r���h�Ɋ܂܂�Ă���ꍇ�́ADirect2D �f�o�b�O�� SDK ���C���[����ėL���ɂ��܂��B
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		// Direct2D �t�@�N�g�������������܂��B
		ThrowIfFailed(
			D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory2),
			&options,
			&m_d2dFactory
			),
			L"Factory�쐬�Ɏ��s���܂����B",
			L"D2D1CreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		// DirectWrite �t�@�N�g�������������܂��B
		ThrowIfFailed(
			DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory2),
			&m_dwriteFactory
			),
			L"DirectWrite �t�@�N�g���쐬�Ɏ��s���܂����B",
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
			L"(WIC) �t�@�N�g���쐬�Ɏ��s���܂����B",
			L"CoCreateInstance()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		HRESULT hr;	//API�̖߂�l�p
		//�f�o�C�X�쐬�p�t���O
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		//�f�o�b�O��
#ifdef _DEBUG
//		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//�h���C�o�̃^�C�v��z��
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		//�I���ł���h���C�o�[�̐�
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		//�@�\�Z�b�g�̔z��
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,	//Dx11�@�\�I�����[
		};
		//�I���ł���@�\�̐�
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//�X���b�v�`�F�[���̃f�X�N�v���^�̐ݒ�
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;					//�o�b�t�@��(2�_�u���o�b�t�@)
		sd.BufferDesc.Width = Width;		//�o�b�t�@�̕�
		sd.BufferDesc.Height = Height;	//�o�b�t�@�̍���
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//8 �r�b�g �A���t�@���T�|�[�g���� 4 �����A16 �r�b�g�����Ȃ����K�������t�H�[�}�b�g
		sd.BufferDesc.RefreshRate.Numerator = 60;	//���t���b�V�����[�g�ő�l
		sd.BufferDesc.RefreshRate.Denominator = 1;	//���t���b�V�����[�g�ŏ��l
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�͗p�Ƃ��Ē�`
		sd.OutputWindow = hWnd;		//�E�C���h�E�̃n���h��
		sd.SampleDesc.Count = 1;		//�}���`�T���v�����O ����1
		sd.SampleDesc.Quality = 0;		//�}���`�T���v�����O�N�I���e�B��0�i�ő�j
		//�ȉ��A�X�g�A�A�v���͐ݒ�B�f�X�N�g�b�v�͎w�肵�Ȃ��i�w�肷��ƃt���X�N���[���ɂȂ�Ȃ��j
//		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		sd.Windowed = TRUE;			//�E�C���h�E���[�h�ō쐬���Ă��Ƃ���t��s�N���[���ɂ���

		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext>	temp_context;
		ComPtr<IDXGISwapChain>	temp_swapChain;


		//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++){
			//�h���C�o�^�C�v��z�񂩂�擾
			m_D3DDriverType = driverTypes[driverTypeIndex];
			//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
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
			//���������炻�̃h���C�o���g��
			if (SUCCEEDED(hr))
				break;
		}
		ThrowIfFailed(
			hr, 
			L"DX11�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ɏ��s���܂����B", 
			L"D3D11CreateDeviceAndSwapChain()", 
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		//���\�[�X���o�[�W�����A�b�v����
		ThrowIfFailed(
			temp_device.As(&m_D3D11Device),
			L"DX11�f�o�C�X�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_device.As(&m_D3D11Device)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_context.As(&m_D3D11Context),
			L"DX11�R���e�N�X�g�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_context.As(&m_D3D11Context)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		ThrowIfFailed(
			temp_swapChain.As(&m_D3D11SwapChain),
			L"DX11�X���b�v�`�F�[���̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_swapChain.As(&m_D3D11SwapChain)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		// Direct2D �f�o�C�X �I�u�W�F�N�g�ƁA�Ή�����R���e�L�X�g���쐬���܂��B

		ComPtr<IDXGIDevice3> dxgiDevice;

		ThrowIfFailed(
			m_D3D11Device.As(&dxgiDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d3dDevice.As(&dxgiDevice)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d2dFactory->CreateDevice()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
			),
			L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
			L"m_d2dDevice->CreateDeviceContext()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		if (isFullScreen){
			ThrowIfFailed(
				m_D3D11SwapChain->SetFullscreenState(true, NULL),
				L"�t���X�N���[���ڍs�Ɏ��s���܂����B",
				L"m_D3D11SwapChain->SetFullscreenState(true, NULL)",
				L"DeviceResources::Impl::CreateDeviceResources()"
				);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//	�p�r: Direct11�f�o�C�X�ȂǃN���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		pImpl(new Impl(hWnd,isFullScreen,Width,Height))
	{
	}
	DeviceResources::~DeviceResources(){}
	//�A�N�Z�T
	ID3D11Device2*	DeviceResources::GetD3DDevice() const { return pImpl->m_D3D11Device.Get(); }
	ID3D11DeviceContext2* DeviceResources::GetD3DDeviceContext() const { return pImpl->m_D3D11Context.Get(); }
	IDXGISwapChain1* DeviceResources::GetSwapChain() const { return pImpl->m_D3D11SwapChain.Get(); }
	D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const { return pImpl->m_D3DFeatureLevel; }

	// D2D �A�N�Z�T�[�B
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
		//��n��
		//�R���X�^���g�o�b�t�@�N���A
		pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		//�V�F�[�_�[���\�[�X���N���A
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//�T���v���[���N���A
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//�V�F�[�_�̃N���A
		pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
		//�C���v�b�g���C�A�E�g�̃N���A
		pID3D11DeviceContext->IASetInputLayout(nullptr);
		//�f�t�H���g�̐ݒ�
		//���j�A�T���v���[��ݒ肵�Ă���
		ID3D11SamplerState* samplerState = StatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//�u�����h�h��Ԃ�
		pID3D11DeviceContext->OMSetBlendState(StatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�����_�[�X�e�[�g�O�ʂ̂ݕ`��
		pID3D11DeviceContext->RSSetState(StatePtr->GetCullBack());
		//�f�v�X�X�e���V���g�p
		pID3D11DeviceContext->OMSetDepthStencilState(StatePtr->GetDepthDefault(), 0);

	}


	//--------------------------------------------------------------------------------------
	//	struct DrawContext::Impl;
	//	�p�r: DrawContext�C�f�B�I��
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
		//�p�C�v���C���\�[�X�̒�`
		vector<PipelineResource> m_PipelineResourceVec;
		//�p�C�v���C���̒�`
		PipeLineDesc m_PipeLineDesc;
		//�V�F�[�_�[
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

	//�p�C�v���C���X�e�[�g�̐ݒ�
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
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�W�̎擾
		auto PtrStage = GameObjectPtr->GetStage();
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//�e�N�X�`��������΃e�N�X�`���̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		if (Tex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, Tex->GetShaderResourceView().GetAddressOf());
		}
		else {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		}
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = PtrStage->GetRenderState();
		//�T���v���[
		//null�ɏ�����
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
		//�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		*/
		//�f�v�X�X�e���V��
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
		//�u�����h�X�e�[�g�ƃ��X�^���C�U��ݒ肵�ĕ`��
		//�����AAlpha���^�Ȃ�AlphaBlendEx�ɐݒ肵�A�����łȂ���΁A�w��ɏ]���B
		if (Alpha) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//�`��
			pID3D11DeviceContext->Draw(Mesh->GetNumVertices(), 0);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//�`��
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
			//�`��
			pID3D11DeviceContext->Draw(Mesh->GetNumVertices(), 0);
		}
		//��n��
		//��n��
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates(RenderStatePtr);
	}

	void DrawContext::DrawIndexed(const shared_ptr<GameObject>& GameObjectPtr, 
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset,
		const shared_ptr<TextureResource>& Tex, bool Alpha) {
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�W�̎擾
		auto PtrStage = GameObjectPtr->GetStage();
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//�e�N�X�`��������΃e�N�X�`���̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		if (Tex) {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, Tex->GetShaderResourceView().GetAddressOf());
		}
		else {
			pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		}
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = PtrStage->GetRenderState();
		//�T���v���[
		//null�ɏ�����
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
		//�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		*/
		//�f�v�X�X�e���V��
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
		//�u�����h�X�e�[�g�ƃ��X�^���C�U��ݒ肵�ĕ`��
		//�����AAlpha���^�Ȃ�AlphaBlendEx�ɐݒ肵�A�����łȂ���΁A�w��ɏ]���B
		if (Alpha) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//�`��
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//�`��
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
			//�`��
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		//��n��
		pImpl->m_PipelineResourceVec.clear();
		Dev->InitializeStates(RenderStatePtr);
	}

	void DrawContext::DrawIndexedBase(const shared_ptr<GameObject>& GameObjectPtr,
		ID3D11Buffer* pConstantBuffer, const void* CBData,
		const shared_ptr<MeshResource>& Mesh, UINT stride, UINT offset){
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�W�̎擾
		auto PtrStage = GameObjectPtr->GetStage();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = PtrStage->GetRenderState();
		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, CBData, 0, 0);
		//�R���X�^���g�o�b�t�@�̊e�V�F�[�_�֐ݒu
		if (pImpl->m_VertexShaderConstant){
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		if (pImpl->m_PixelShaderConstant){
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		}
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(pImpl->m_pVertexShader, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(pImpl->m_pPixelShader, nullptr, 0);
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(pImpl->m_pInputLayout);
		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		for (auto res : pImpl->m_PipelineResourceVec){
			pID3D11DeviceContext->PSSetShaderResources(res.m_PipelineNum, 1, &res.m_ShaderResourceView);
			//null�ɏ�����
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
			//�T���v���[��ݒ�
			pID3D11DeviceContext->PSSetSamplers(res.m_PipelineNum, 1, &pSampler);
		}
		//�f�v�X�X�e���V��
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
		//�u�����h�X�e�[�g�ƃ��X�^���C�U��ݒ肵�ĕ`��
		//�����A�����`��Ȃ�AlphaBlendEx�ɐݒ肵�A�����łȂ���΁A�w��ɏ]���B
		if (GameObjectPtr->GetAlphaActive()) {
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//�`��
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
			//���X�^���C�U�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//�`��
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
			//�`��
			pID3D11DeviceContext->DrawIndexed(Mesh->GetNumIndicis(), 0, 0);
		}
		//��n��
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
