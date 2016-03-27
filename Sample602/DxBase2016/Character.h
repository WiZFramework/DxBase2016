#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Posision;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& Scale,const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveSphere : public GameObject;
	//	�p�r: �ړ����鋅��
	//--------------------------------------------------------------------------------------
	class MoveSphere : public GameObject{
		Vector3 m_Scale;
		Vector3 m_StartPosision;
		Vector3 m_Posision;
		//���x
		Vector3 m_Velocity;
		//�ċA�I�ɏՓ˔��������
		bool RecTestSphereObb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime);
	public:
		//�\�z�Ɣj��
		MoveSphere(const shared_ptr<Stage>& StagePtr, const Vector3& Scale, const Vector3& StartPos);
		virtual ~MoveSphere();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};


}
//endof  basedx11
