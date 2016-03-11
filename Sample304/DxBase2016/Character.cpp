#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//������
	void FixedBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//��������������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox(){}

	//������
	void MoveBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();


		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSprite::NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0){
	}
	NumberSprite::~NumberSprite(){}

	//������
	void NumberSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(128.0f, 128.0f), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"NUMBER_TX");
		//��������
		SetAlphaActive(true);
		//���㌴�_
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftTopZeroPlusDownY);
		//�X�v���C�g�̒��̃��b�V������o�b�N�A�b�v�̎擾
		auto& SpVertexVec = PtrSprite->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		//�e�������Ƃ�UV�l���܂ޒ��_�f�[�^��z�񉻂��Ă���
		for (size_t i = 0; i < 10; i++){
			float from = ((float)i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//���㒸�_
				VertexPositionColorTexture(
				SpVertexVec[0].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 0)
				),
				//�E�㒸�_
				VertexPositionColorTexture(
				SpVertexVec[1].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 0)
				),
				//�������_
				VertexPositionColorTexture(
				SpVertexVec[2].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 1.0f)
				),
				//�E�����_
				VertexPositionColorTexture(
				SpVertexVec[3].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 1.0f)
				),
			};
			m_NumberVertexVec.push_back(NumVirtex);
		}
	}

	void NumberSprite::OnUpdate(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		size_t Num = (size_t)m_TotalTime;
		Num = Num % 10;
		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto MeshRes = PtrSprite->GetMeshResource();
		//���I��UV�l���ς�钸�_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_NumberVertexVec[Num]);
	}
	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BarSprite::BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0.0f){
	}
	BarSprite::~BarSprite(){}

	//������
	void BarSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		//�����ɒ��_��ύX���Ă��܂��̂ŁA�T�C�Y�͓K���ł悢
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(SpriteWidth, SpriteHeight), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		//����
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, SpriteHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 0))
			);
		//�E��
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, SpriteHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 0))
			);
		//����
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f))
			);
		//�E��
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f))
			);
		//�X�v���C�g�̒��̃��b�V���̎擾
		auto MeshRes = PtrSprite->GetMeshResource();
		//���_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);

		PtrSprite->SetTextureResource(L"BAR_TX");
		//�������_
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftBottomZeroPlusUpY);
	}

	//�ω�
	void BarSprite::OnUpdate(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 2.0f;
		//sin��-1.0����1.0�܂Ŏ�肤��̂Œ���
		float v = cos(m_TotalTime) * 0.5f + 0.5f;
		float y = SpriteHeight - SpriteHeight * v;

		m_BarVirtexVec[0].position.y = y;
		m_BarVirtexVec[0].textureCoordinate.y = v;
		m_BarVirtexVec[1].position.y = y;
		m_BarVirtexVec[1].textureCoordinate.y = v;

		auto PtrSprite = GetComponent<PCTSpriteDraw>();

		//�X�v���C�g�̒��̃��b�V���̎擾
		auto MeshRes = PtrSprite->GetMeshResource();
		//���_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);
	}


}
//endof  basedx11
