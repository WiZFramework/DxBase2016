
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------

	//�r���[�ނ̍쐬
	void GameStage::CreateViews(){
		//�ŏ��Ƀf�t�H���g�̃����_�����O�^�[�Q�b�g�ނ��쐬����
		//�p�����[�^��2048.0f�̓V���h�E�}�b�v�̃T�C�Y�B�傫���قǉe���ׂ����Ȃ�i4096.0f�Ȃǁj
		//�e���ׂ�������ƃ��������ʂɏ����̂Œ��ӁI
		CreateDefaultRenderTargets(2048.0f);
		//�e�̃r���[�T�C�Y�̐ݒ�B���̒l������������Ɖe���\�������͈͂��������Ȃ�B
		//�l���������ق����e�ׂ͍����Ȃ�
		//�X�^�e�B�b�N�ϐ��Ȃ̂ň�x�ݒ肵���炻�̒l�̓X�e�[�W�𒴂��ĕێ������B
		Shadowmap::SetViewSize(32.0f);
		//�}���`�r���[�R���|�[�l���g�̎擾
		auto PtrMultiView = GetComponent<MultiView>();
		//�}���`�r���[�Ƀr���[�̒ǉ�
		auto PtrView = PtrMultiView->AddView();
		//�r���[�̋�`��ݒ�i�Q�[���T�C�Y�S�́j
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		//�r���[�̔w�i�F
		Color4 ViewBkColor(0.0f, 0.125f, 0.3f, 1.0f);
		//�ŏ��̃r���[�Ƀp�����[�^�̐ݒ�
		PtrView->ResetParamaters<Camera, MultiLight>(rect, ViewBkColor, 1, 0.0f, 1.0f);
		//�ŏ��̃r���[�̃r���[�̃��C�g�̐ݒ�
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
	}




	//������
	void GameStage::OnCreate(){
		try{
			//�r���[�ނ��쐬����
			CreateViews();
		}
		catch (...){
			throw;
		}
	}



}
//endof  basedx11
