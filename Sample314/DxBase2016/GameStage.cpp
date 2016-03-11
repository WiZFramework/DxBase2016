
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
		//�p�����[�^��4096.0f�ɂ��Ă���
		//�e���ׂ�������ƃ��������ʂɏ����̂Œ��ӁI
		CreateDefaultRenderTargets(4096.0f);
		//�e�̃r���[�T�C�Y�̐ݒ�B���̒l������������Ɖe���\�������͈͂��������Ȃ�B
		//�l���������ق����e�ׂ͍����Ȃ�
		//�X�^�e�B�b�N�ϐ��Ȃ̂ň�x�ݒ肵���炻�̒l�̓X�e�[�W�𒴂��ĕێ������B
		Shadowmap::SetViewSize(16.0f);
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

	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox(){
		//�z��̏�����
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox(){
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject(){

		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");

		wstring MediaPath = App::GetApp()->m_wstrRelativeDataPath;
		//XML�t�@�C���̌���
		wstring XMLFilename = MediaPath + L"GameStage.xml";
		//���[�J�����XML�t�@�C���ǂݎ��N���X���쐬
		XmlDocReader GameStageXml(XMLFilename);
		//�����o�֐��ŃQ�[���I�u�W�F�N�g�̃m�[�h���擾
		auto ObjectsRoot = GameStageXml.GetSelectSingleNode(L"GameStage/GameObjects");
		if (!ObjectsRoot){
			throw BaseException(
				L"GameStage/GameObjects��������܂���",
				L"if (!ObjectsRoot)",
				L"GameStage::CreateSeekObject()"
				);
		}
		//static�֐��Ăяo���ŁASeekObject�m�[�h�̔z��𓾂�
		auto SeekObjects = XmlDocReader::GetSelectNodes(ObjectsRoot, L"SeekObject");
		//static�֐��Ăяo���ŁASeekObject�m�[�h�̐��𓾂�
		long lCountNode = XmlDocReader::GetLength(SeekObjects);
		for (long i = 0; i < lCountNode; i++){
			//static�֐��Ăяo���ŁA�ʂ̃m�[�h�𓾂�
			auto PositionNode = XmlDocReader::GetItem(SeekObjects, i);
			//static�֐��Ăяo���ŁA�ʒu�m�[�h�𓾂�
			auto StartPosNode = XmlDocReader::GetSelectSingleNode(PositionNode, L"StartPosition");
			//static�֐��Ăяo���ŁA�m�[�h���̕�����𓾂�
			wstring StartPosStr = XmlDocReader::GetText(StartPosNode);
			if (StartPosStr == L""){
				throw BaseException(
					L"�X�^�[�g�ʒu���󔒂ł�",
					L"if (StartPosStr == L\"\")",
					L"GameStage::CreateSeekObject()"
					);
			}
			vector< wstring > StrVec;
			Util::WStrToTokenVector(StrVec, StartPosStr, L',');
			//���������z�񂩂�f�[�^�����o��
			AddGameObject<SeekObject>(Vector3(StrVec[0], StrVec[1], StrVec[2]));
		}
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer(){
		wstring strXmlData = App::GetApp()->m_wstrRelativeDataPath + L"GameData.xml";
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(strXmlData);
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�J�v�Z���̍쐬
	void GameStage::CreateCapsule(){
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
			{ 20.0f, 0, -20.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<CapsuleObject>(v);
		}
	}

	//�q�b�g���鋅�̂̍쐬
	void GameStage::CreateSphere(){
		//�z��̏�����
		vector<Vector3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, -25.0f },
		};
		//�z�u�I�u�W�F�N�g�̍쐬
		for (auto v : Vec){
			AddGameObject<SphereObject>(v);
		}
	}



	//�Փ˂���X�N�G�A�̍쐬
	void GameStage::CreateHitTestSquare(){
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"SquareGroup");
		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 1.0f, 1.0f),
			Vector3(0, 0.0f, 0.0f),
			Vector3(0.0f, 0.5f, 10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -15.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(20.0f, 0.4f, -15.0f)
			);
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
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�J�v�Z���쐬
			CreateCapsule();
			//���̍쐬
			CreateSphere();
			//�Փ˂���X�N�G�A�̍쐬
			CreateHitTestSquare();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}




}
//endof  basedx11
