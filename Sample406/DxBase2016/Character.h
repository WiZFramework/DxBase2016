#pragma once

#include "stdafx.h"

namespace basedx11{

	//�V�F�[�_�ސ錾�i�}�N���g�p�j
	//�J�X�^���V���h�E�}�b�v�p�R���X�^���g�o�b�t�@�\����
	struct CustomShadowmapConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomShadowmap, CustomShadowmapConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomShadowmap, VertexPositionNormalColor)
	//�J�X�^���`��R���X�^���g�o�b�t�@�\����
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//�ėp�p�����[�^
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionNormalColor)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)

	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	�p�r: �J�X�^���V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap{
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CustomShadowmap();
		//����
		virtual void OnDraw()override;

	};

	//--------------------------------------------------------------------------------------
	//	class CustomDrawOctahedron : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��8�ʑ�
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//�z�F�p�^�[��
		int m_Pattern;
		//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�C���f�b�N�X�̐�
		UINT m_NumIndicis;
		//�o�b�N�A�b�v�p�̒��_�z��
		vector<VertexPositionNormalColor> m_BackupVirtex;
		//���b�V���̍쐬
		void CreateCustomMesh();
		//���_�̕ύX
		void UpdateCustomMesh();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos,
			int Pattern);
		virtual ~CustomDrawOctahedron();
		//�A�N�Z�T
		const ComPtr<ID3D11Buffer>& GetVertexBuffer() const{
			return m_VertexBuffer;
		}
		const ComPtr<ID3D11Buffer>& GetIndexBuffer() const{
			return m_IndexBuffer;
		}
		UINT GetNumVertices() const{
			return m_NumVertices;
		}
		UINT GetNumIndicis() const{
			return m_NumIndicis;
		}

		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
