#pragma once

#include "stdafx.h"

namespace basedx11{

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

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~MoveBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class NumberSprite : public GameObject{
		Vector3 m_StartPos;
		//���_�̔z��̔z��i2�����z��j
		vector< vector<VertexPositionColorTexture> > m_NumberVertexVec;
		float m_TotalTime;
	public:
		//�\�z�Ɣj��
		NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~NumberSprite();
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class BarSprite : public GameObject{
		Vector3 m_StartPos;
		float m_TotalTime;
		//���_�̃o�b�N�A�b�v
		vector<VertexPositionColorTexture> m_BarVirtexVec;
		//�X�v���C�g�̕��ƍ���
		const float SpriteHeight = 256.0f;
		const float SpriteWidth = 64.0f;
	public:
		//�\�z�Ɣj��
		BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BarSprite();
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;
	};



}
//endof  basedx11
