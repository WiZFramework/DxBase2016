/*!
@file DeviceResources.h
@brief Dx11�f�o�C�X���\�[�X

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//�ėp�I�Ȑݒ�p��`

	//--------------------------------------------------------------------------------------
	//	enum class BlendState;
	//	�p�r: �u�����h�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	//	enum class DepthStencilState;
	//	�p�r: �f�v�X�X�e���V���X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	//	enum class RasterizerState;
	//	�p�r: ���X�^���C�U�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
	};

	//--------------------------------------------------------------------------------------
	//	enum class SamplerState;
	//	�p�r: �T���v���[�X�e�[�g
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
	�p�C�v���C����`
	*/
	//--------------------------------------------------------------------------------------
	struct PipeLineDesc {
		//--------------------------------------------------------------------------------------
		//	�u�����h�X�e�[�g
		//--------------------------------------------------------------------------------------
		BlendState m_BlendState;
		//--------------------------------------------------------------------------------------
		//	�f�v�X�X�e���V���X�e�[�g
		//--------------------------------------------------------------------------------------
		DepthStencilState m_DepthStencilState;
		//--------------------------------------------------------------------------------------
		//	���X�^���C�U�X�e�[�g
		//--------------------------------------------------------------------------------------
		RasterizerState m_RasterizerState;
	};

	class RenderState;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX11�f�o�C�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	DeviceResources(
		//		HWND hWnd,		//�E�C���h�E�̃n���h�� 
		//		bool isFullScreen, //�t���X�N���[�����ǂ���
		//		UINT Width,			//��
		//		UINT Height			//����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param HWND hWnd,	�E�C���h�E�̃n���h��
		@param bool isFullScreen, �t���X�N���[�����ǂ���
		@param UINT Width,		��
		@param UINT Height		����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		//	virtual ~DeviceResources();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//D3D�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ID3D11Device2*	GetD3DDevice() const;
		/*!
		@breaf D3D11Device2�C���^�[�t�F�C�X�̎擾
		@param �Ȃ�
		@return	D3D11Device2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Device2* GetD3DDevice() const;
		ID3D11DeviceContext2* GetD3DDeviceContext() const;
		IDXGISwapChain1* GetSwapChain() const;
		D3D_FEATURE_LEVEL GetFeatureLevel() const;

		// D2D �A�N�Z�T
		ID2D1Factory2* GetD2DFactory() const;
		ID2D1Device1* GetD2DDevice() const;
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
//		ID2D1Bitmap1* GetD2DTargetBitmap() const;
		IDWriteFactory2* GetDWriteFactory() const;
		IWICImagingFactory2* GetWicImagingFactory() const;
		void InitializeStates(const shared_ptr<RenderState>& StatePtr);	//��n��

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;
		//���[�u�֎~
		DeviceResources(const DeviceResources&&) = delete;
		DeviceResources& operator=(const DeviceResources&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class DrawContext : public ObjectInterface;
	/*!
	�`��p�R���e�N�X�g�i�e�I�u�W�F�N�g�̕`��N���X�j
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
		//���z�֐�
		virtual void OnPreCreate() override {}
		virtual void OnCreate()override {}
		//�p�C�v���C���X�e�[�g�̐ݒ�
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
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	};



}

//end basedx11
