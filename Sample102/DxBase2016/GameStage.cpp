
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	�p�r: �Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	//���\�[�X�̍쐬
	void GameStage::CreateResourses(){
		wstring strTexture = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}

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
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, ViewBkColor, 1, 0.0f, 1.0f);
		//�ŏ��̃r���[�̃r���[�̃��C�g�̐ݒ�
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
	}


	//�v���[�g�̍쐬
	void GameStage::CreatePlate(){
		//�X�e�[�W�ւ̃Q�[���I�u�W�F�N�g�̒ǉ�
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTransMatrix = Ptr->GetComponent<TransformMatrix>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(200.0f, 200.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.0f, 0.0f)
			);
		PtrTransMatrix->SetWorldMatrix(WorldMat);
		
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"SKY_TX");
		//�����ɉe���f�肱�ނ悤�ɂ���
		DrawComp->SetOwnShadowActive(true);

		
	}

	//�A�N�V�����I�u�W�F�N�g�̍쐬
	void GameStage::CreateActionObject(){
		//�z�u�p�̃e�[�u��
		vector<Vector3> Vec{
			{ 0, 0.125f, 0 },
			{ 0, 0.125f, 1.0f },
			{ 0, 0.125f, 2.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto Val : Vec){
			AddGameObjectWithParam<ActionObject>(Val);
		}
	}


	//������
	void GameStage::OnCreate(){
		try{
			//���\�[�X�̍쐬
			CreateResourses();
			//�r���[�ނ��쐬����
			CreateViews();
			//�v���[�g���쐬����
			CreatePlate();
			//�A�N�V�����I�u�W�F�N�g�̍쐬
			CreateActionObject();
		}
		catch (...){
			throw;
		}
	}




}
//endof  basedx11
