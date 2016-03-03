#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	用途: アクションする配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionObject::ActionObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr){
	}
	ActionObject::~ActionObject(){}


	//初期化
	void ActionObject::OnCreateWithParam(const Vector3& StartPos){
		//Transformだけは追加しなくても取得できる
		auto Ptr = AddComponent<Transform>();
		Ptr->SetPosition(StartPos);
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);

		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddRotateBy(1.0f, Vector3(0, XM_PI, 0));
		PtrAction->AddRotateInterval(1.0f);

		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));
		PtrAction->AddMoveBy(0.5f, Vector3(0, 1.0f, -1.0f), Lerp::Linear, Lerp::EaseOut, Lerp::Linear);
		PtrAction->AddMoveBy(0.5f, Vector3(0, -1.0f, -1.0f), Lerp::Linear, Lerp::Easein, Lerp::Linear);
		PtrAction->AddMoveBy(2.0f, Vector3(-4.0f, 0, 0));
		PtrAction->AddMoveBy(1.0f, Vector3(0, 0, 2.0f));
		PtrAction->AddMoveBy(1.0f, Vector3(2.0f, 0, 0));

		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//テクスチャの登録
		PtrDraw->SetTextureResource(L"TRACE_TX");


		//透過処理する
		SetAlphaActive(true);

	}




}
//endof  basedx11
