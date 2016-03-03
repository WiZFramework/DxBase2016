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
		m_Span(2.0f)
	{
	}
	Box::~Box(){}

	//初期化
	void Box::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		PtrTransform->SetPosition(m_StartPos);
		//OBBの衝突判定をつける
		auto PtrObb = AddComponent<CollisionObb>();
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明有効
		SetAlphaActive(true);
	}

	//更新
	void Box::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Pos = PtrTransform->GetPosition();
		Pos.x += m_Span * ElapsedTime;
		if (abs(Pos.x) > 10.0f){
			m_Span *= -1.0f;
		}
		PtrTransform->SetPosition(Pos);
	}

	void Box::OnCollision(const shared_ptr<GameObject>& other){
		//もし衝突したら反対方向に移動
		m_Span *= -1.0f;
	}



}
//endof  basedx11
