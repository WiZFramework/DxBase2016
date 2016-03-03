#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	�p�r: �A�N�V��������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject{
	public:
		//�\�z�Ɣj��
		ActionObject(const shared_ptr<Stage>& StagePtr);
		virtual ~ActionObject();
		//������
		virtual void OnCreate() override{}
		void OnCreateWithParam(const Vector3& StartPos);
		//�Փˎ�
		virtual void OnCollision(const shared_ptr<GameObject>& other)override;
	};
}
//endof  basedx11
