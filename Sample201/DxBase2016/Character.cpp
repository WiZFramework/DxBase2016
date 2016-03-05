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
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();

		if (Key.m_bPushKeyTbl['F'] || Key.m_bPushKeyTbl[VK_LBUTTON]){
			Pos.x -= ElapsedTime;
		}
		else if (Key.m_bPushKeyTbl['G'] || Key.m_bPushKeyTbl[VK_RBUTTON]){
			Pos.x += ElapsedTime;
		}

		//もし、キーを押した瞬間かマウスをクリックした瞬間をとってくる場合は、
		//上記をコメントにし以下を有効にする
		/*
		if (Key.m_bPressedKeyTbl['F'] || Key.m_bPressedKeyTbl[VK_LBUTTON]){
		Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bPressedKeyTbl['G'] || Key.m_bPressedKeyTbl[VK_RBUTTON]){
		Pos.x += ElapsedTime * 5.0f;
		}
		*/
		

		//もし、キーを離した瞬間かマウスをアップした瞬間をとってくる場合は、
		//上記をコメントにし以下を有効にする
		/*
		if (Key.m_bUpKeyTbl['F'] || Key.m_bUpKeyTbl[VK_LBUTTON]){
		Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bUpKeyTbl['G'] || Key.m_bUpKeyTbl[VK_RBUTTON]){
		Pos.x += ElapsedTime * 5.0f;
		}
		*/

		PtrTransform->SetPosition(Pos);
	}


}
//endof  basedx11
