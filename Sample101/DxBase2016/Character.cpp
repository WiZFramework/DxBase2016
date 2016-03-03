#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_Span(3.0f)
	{
	}
	Box::~Box(){}

	//初期化
	void Box::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明有効
		SetAlphaActive(true);
		//相対移動アクションをつける
		auto PtrMoveBy = AddComponent<MoveBy>();
		PtrMoveBy->SetParams(2.0f, Vector3(m_Span, 0, 0));
		//アクション実行開始
		PtrMoveBy->Run();
	}

	//更新
	void Box::OnLastUpdate(){
		//相対移動アクションを取り出す
		auto PtrMoveBy = GetComponent<MoveBy>();
		if (PtrMoveBy->IsArrived()){
			//到着していたら移動方向を逆転させて、MoveByを初期化
			m_Span *= -1.0f;
			PtrMoveBy->SetParams(2.0f, Vector3(m_Span, 0, 0));
			//アクション実行開始
			PtrMoveBy->Run();
		}
	}



}
//endof  basedx11
