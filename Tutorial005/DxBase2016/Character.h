#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	�p�r: �����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class MoveObject : public GameObject{
	public:
		//�\�z�Ɣj��
		MoveObject(const shared_ptr<Stage>& StagePtr);
		virtual ~MoveObject();
		virtual void OnCreate() override{}
		//������
		void OnCreateWithParam(const Vector3& StartPos,
			const Vector3& StartVelocity, bool Type);
		//�^�[���̍ŏI�X�V��
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		//Vector3 m_Scale;
		//Vector3 m_Rotation;
		//Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override{}
		void OnCreateWithParam(const Vector3& Scale,
			const Vector3& Rotation,const Vector3& Position);
	};

}
//endof  basedx11
