#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	�p�r: �����I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveObject::MoveObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	MoveObject::~MoveObject(){}

	//������
	void MoveObject::OnCreateWithParam(const Vector3& StartPos, const Vector3& StartVelocity, bool Type){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(StartPos);
		if (Type){
			//Sphere�̏Փ˔��������
			auto PtrSphere = AddComponent<CollisionSphere>();
		}
		else{
			//OBB�̏Փ˔��������
			auto PtrObb = AddComponent<CollisionObb>();
		}
		//�����v�Z������
		auto PtrRigit = AddComponent<Rigidbody>();
		PtrRigit->SetVelocity(StartVelocity);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		if (Type){
			ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		}
		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		if (Type){
			PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		}
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//�����L��
		SetAlphaActive(true);
	}

	//�X�V
	void MoveObject::OnLastUpdate(){
		auto PtrRigit = GetComponent<Rigidbody>();
		auto Velo = PtrRigit->GetVelocity();
		//���������ő��xy���O�ȊO�ɂȂ�ꍇ������̂Œ���
		Velo.y = 0;
		PtrRigit->SetVelocity(Velo);
	}

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	FixedBox::~FixedBox(){}

	//������
	void FixedBox::OnCreateWithParam(const Vector3& Scale,
		const Vector3& Rotation,const Vector3& Position){

		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(Scale);
		PtrTransform->SetRotation(Rotation);
		PtrTransform->SetPosition(Position);

		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetOwnShadowActive(true);
	}


}
//endof  basedx11
