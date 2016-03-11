#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	�p�r: ���j���[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage{
		//�r���[�̍쐬
		void CreateViews();
		//������̍쐬
		void CreateString();
	public:
		//�\�z�Ɣj��
		MenuStage() :Stage(){}
		virtual ~MenuStage(){}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViews();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�Œ�̃{�b�N�X�̍쐬
		void CreateFixedBox();
		//�㉺�ړ����Ă���{�b�N�X�̍쐬
		void CreateMoveBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//������
		virtual void OnCreate()override;
	};
}
//endof  basedx11
