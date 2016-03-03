#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	用途: 動くオブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveObject::MoveObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	MoveObject::~MoveObject(){}

	//初期化
	void MoveObject::OnCreateWithParam(const Vector3& StartPos, const Vector3& StartVelocity, bool Type){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(StartPos);
		if (Type){
			//Sphereの衝突判定をつける
			auto PtrSphere = AddComponent<CollisionSphere>();
		}
		else{
			//OBBの衝突判定をつける
			auto PtrObb = AddComponent<CollisionObb>();
		}
		//物理計算をつける
		auto PtrRigit = AddComponent<Rigidbody>();
		PtrRigit->SetVelocity(StartVelocity);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		if (Type){
			ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		}
		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		if (Type){
			PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		}
		else{
			PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		}
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明有効
		SetAlphaActive(true);
	}

	//更新
	void MoveObject::OnLastUpdate(){
		auto PtrRigit = GetComponent<Rigidbody>();
		auto Velo = PtrRigit->GetVelocity();
		//自動反発で速度yが０以外になる場合があるので調整
		Velo.y = 0;
		PtrRigit->SetVelocity(Velo);
	}

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	FixedBox::~FixedBox(){}

	//初期化
	void FixedBox::OnCreateWithParam(const Vector3& Scale,
		const Vector3& Rotation,const Vector3& Position){

		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(Scale);
		PtrTransform->SetRotation(Rotation);
		PtrTransform->SetPosition(Position);

		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetOwnShadowActive(true);
	}


}
//endof  basedx11
