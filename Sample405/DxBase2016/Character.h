#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	�Ǝ��̃��b�V�����\�[�X
	//--------------------------------------------------------------------------------------
	class CustomDrawBox : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//���b�V�����\�[�X
		shared_ptr<MeshResource> m_MeshResource;
		//���b�V�����\�[�X�̍쐬
		void CreateMeshResource();
		//���_�̕ύX
		void UpdateMeshResource();
		//�o�ߎ���
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawBox();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//endof  basedx11
