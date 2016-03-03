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
		m_Span(1.0f),
		m_StartPos(StartPos)
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
		//透明処理
		SetAlphaActive(true);
	}

	//更新
	void Box::OnUpdate(){
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		//位置の取得
		auto Pos = PtrTransform->GetPosition();
		//値を変化させる
		Pos.x += m_Span * ElapsedTime;
		if (abs(Pos.x) > 3.0f){
			//もし左右3.0を超えたら反転
			m_Span *= -1.0f;
		}
		//新しい位置を設定
		PtrTransform->SetPosition(Pos);
	}




}
//endof  basedx11
