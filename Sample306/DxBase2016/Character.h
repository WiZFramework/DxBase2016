#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	�p�r: ���̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	class EggAnime : public SS5ssae{
		float m_Span;
	public:
		//�\�z�Ə���
		EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir);
		virtual ~EggAnime();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate()override;
	};


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


}
//endof  basedx11
