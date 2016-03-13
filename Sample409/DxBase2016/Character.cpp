#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	IMPLEMENT_DX11_CONSTANT_BUFFER(CBBall)
	IMPLEMENT_DX11_VERTEX_SHADER(VSBall, App::GetApp()->m_wstrRelativeShadersPath + L"VSBall.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSBall, App::GetApp()->m_wstrRelativeShadersPath + L"PSBall.cso")
	IMPLEMENT_DX11_GEOMETRY_SHADER(GSBall, App::GetApp()->m_wstrRelativeShadersPath + L"GSBall.cso")

	//--------------------------------------------------------------------------------------
	//	class Ball : public GameObject;
	//	�p�r: ���_���ύX�����{�[��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Ball::Ball(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_Explode(0)
	{}

	//������
	void Ball::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetPosition(m_StartPos);
		Ptr->SetScale(1.0f, 1.0f, 1.0f);	//���a���[�g���̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();
		//���ߏ�������
		SetAlphaActive(true);
	}

	//�ω�
	void Ball::OnUpdate(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Explode += ElapsedTime;
		if (m_Explode >= XM_PI){
			m_Explode = 0;
		}
	}



	//�`��
	void Ball::OnDraw(){
		auto PtrStage = GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrT = GetComponent<Transform>();
		//�X�e�[�W����J���������o��
		auto PtrCamera = PtrStage->GetTargetCamera();
		//�}���`���C�g�����o��
		auto Lights = PtrStage->GetTargetMultiLight();
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��(�V���h�E�p)
		auto PtrLight = PtrStage->GetTargetLight(0);
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE");
		//�e�N�X�`���̎擾
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(L"TRACE_TX");
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = PtrStage->GetRenderState();
		if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){
			//�R���X�^���g�o�b�t�@�̐ݒ�
			BallConstantBuffer Cb;
			Cb.m_World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());
			Cb.m_View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
			Cb.m_Proj = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
			Cb.m_LightDir = PtrLight->GetDirectional();
			Cb.m_Explode = abs(sin(m_Explode) / 2.0f);
			//�R���X�^���g�o�b�t�@�̍X�V
			ID3D11Buffer* pConstantBuffer = CBBall::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &Cb, 0, 0);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//���_�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//���_�V�F�[�_�̐ݒ�
			pID3D11DeviceContext->VSSetShader(VSBall::GetPtr()->GetShader(), nullptr, 0);

			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };
			//�e�N�X�`����ݒ�
			pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
			//���j�A�T���v���[��ݒ�
			ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
			pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
			//�s�N�Z���V�F�[�_�̐ݒ�
			pID3D11DeviceContext->PSSetShader(PSBall::GetPtr()->GetShader(), nullptr, 0);
			//�W�I���g���V�F�[�_�̐ݒ�
			pID3D11DeviceContext->GSSetShader(GSBall::GetPtr()->GetShader(), nullptr, 0);

			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSBall::GetPtr()->GetInputLayout());

			//�R���X�^���g�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			//�����_�����O�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//�`��
			pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			//�����_�����O�X�e�[�g
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//�`��
			pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			//��n��
			Dev->InitializeStates(RenderStatePtr);
		}
	}



}
//endof  basedx11
