#pragma once

#include "stdafx.h"

namespace basedx11{

	//�J�X�^���`��R���X�^���g�o�b�t�@�\����
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 Param;	//�ėp�p�����[�^
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//�V�F�[�_�錾�i�}�N���g�p�j
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)

	//--------------------------------------------------------------------------------------
	//	class CustomDrawVortex : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��Q����
	//--------------------------------------------------------------------------------------
	class CustomDrawVortex : public GameObject{
		Vector3 m_StartPos;
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�o�b�N�A�b�v�p�̒��_�z��
		vector<VertexPositionColor> m_BackupVirtex;
		//���b�V���̍쐬
		void CreateCustomMesh();
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawVortex(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CustomDrawVortex();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
