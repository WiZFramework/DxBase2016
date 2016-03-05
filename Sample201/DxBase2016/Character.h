#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
		float m_Span;	//�ړ������̃p�����[�^
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
	};

}
//endof  basedx11
