#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	�p�r: �A�N�V��������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActionObject::ActionObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr){}
	ActionObject::~ActionObject(){}


	//������
	void ActionObject::OnCreateWithParam(const Vector3& StartPos){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = AddComponent<Transform>();
		Ptr->SetPosition(StartPos);
		Ptr->SetScale(1.0f, 0.25f, 0.5f);	
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);

		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionObb>();


		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(0, 0.0f, -2.0f));
		PtrAction->AddMoveBy(2.0f, Vector3(-4.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(0, 0, 2.0f));
		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();

		//�e�̍쐬
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`��
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//���b�V���̓o�^
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		//�e�N�X�`���̓o�^
		PtrDraw->SetTextureResource(L"TRACE_TX");


		//���ߏ�������
		SetAlphaActive(true);
	}

	void ActionObject::OnCollision(const shared_ptr<GameObject>& other){
		//�����ƏՓ˂��Ă�����
		auto ColPtr = GetComponent<CollisionObb>();
		auto PtrTrans = GetComponent<Transform>();
		auto Pos = PtrTrans->GetPosition();

		auto OtherPtr = other->GetComponent<Transform>();
		auto OtherPos = OtherPtr->GetPosition();

		auto Span = Pos - OtherPos;
		auto Span2 = Pos - OtherPos;

		//�Փ˂��ĂȂ��Ǝv����ʒu�܂ňړ�
		Span.Normalize();
		Span.y = 0.0f;
		Span *= 0.125f;
		Pos += Span;
		Pos.y = 0.125f;
		PtrTrans->SetPosition(Pos);
		//�Փˌ�ړ������̌v�Z
		Span2.Normalize();
		Span2 *= 3.0f;
		Span2.y = 0.0f;

		//���݂��̏Փˏ����N���A����
		//�������Ă����Ɣ�r�I�X���[�Y�Ȕ��˂�������
		ColPtr->ClearBothHitObject();

		//�A�N�V�����̎擾
		auto PtrAction = GetComponent<Action>();
		//���݂̂��ׂẴA�N�V�������N���A
		PtrAction->AllActionClear();
		//�A�N�V�����̐ݒ肵����
		PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
		PtrAction->AddMoveBy(1.0f, Span2);
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();

	}




}
//endof  basedx11
