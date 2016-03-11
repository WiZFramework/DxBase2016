#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	�p�r: ���̃A�j���[�V����
	//--------------------------------------------------------------------------------------
	//�\�z�Ə���
	EggAnime::EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_2head.ssae", L"attack1", true),
		m_Span(100.0f)
	{}
	EggAnime::~EggAnime(){}
	//������
	void EggAnime::OnCreate(){
		//���ƂȂ�I�u�W�F�N�g����A�j���[�V�����I�u�W�F�N�g�ւ̍s��̐ݒ�
		//�쐬���ꂽ�O���t�B�b�N�Ƃ̒����Ɏg�p
		Matrix4X4 mat;
		mat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0, 0, 0),
			Vector3(0, 0, 0)
			);
		SetToAnimeMatrix(mat);

		//�ʒu�̏�����
		auto PtrT = AddComponent<Transform>();
		//�X�v���C�g�ł�SpriteStdio�́A�T�C�Y32.0f��̃X�v���C�g���쐬����̂�
		//����ɍ��킹�ăX�P�[�����O����
		PtrT->SetScale(-8.0f, 8.0f, 1.0f);
		//�|�W�V�����̓s�N�Z���P�ʃZ���^�[���킹�i�Œ�j
		PtrT->SetPosition(Vector3(0, -250.0f, 0.0f));
		//�e�N���X��OnCreate��K���Ă�
		SS5ssae::OnCreate();
		//�l�͕b������̃t���[����
		SetFps(30.0f);
	}
	void EggAnime::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrT = GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		Pos.x += (m_Span * ElapsedTime);
		if (abs(Pos.x) > 400.0f){
			m_Span *= -1.0f;
		}
		PtrT->SetPosition(Pos);
		//�ړ������Ɍ�����ς���
		if (m_Span < 0){
			PtrT->SetScale(8, 8, 1);
		}
		else{
			PtrT->SetScale(-8, 8, 1);
		}
		//�A�j���[�V�������X�V����
		UpdateAnimeTime(ElapsedTime);
	}



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


}
//endof  basedx11
