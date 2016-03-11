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

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.5f);

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
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			auto PtrGravity = GetComponent<Gravity>();
			if (PtrGravity->GetGravityVelocity().Length() <= 0){
				//地面にいるときのみジャンプできる
				//Aボタンが押された瞬間なら小ジャンプ
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
					//ジャンプはショートタイプ
					Jump<ShortType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//Bボタンが押された瞬間なら中ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
					//ジャンプはミディアムタイプ
					Jump<MediumType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//Yボタンが押された瞬間なら大ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y){
					//ジャンプはロングタイプ
					Jump<LongType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
				//Xボタンが押された瞬間なら大大ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X){
					//ジャンプはロングロングタイプ
					Jump<LongLongType> j;
					PtrGravity->StartJump(0, j(), 0);
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_YRot(0),
		m_RotationSpeed(0.0f)
	{}
	RollingTorus::~RollingTorus(){}
	//初期化
	void RollingTorus::OnCreate(){
		//Transformだけは追加しなくても取得できる
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.5f);

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_TORUS");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_TORUS");
		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));


	}

	void RollingTorus::OnUpdate(){
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			auto PtrGravity = GetComponent<Gravity>();
			if (PtrGravity->GetGravityVelocity().Length() <= 0){
				//地面にいるときのみジャンプできる
				//Aボタンが押された瞬間なら小ジャンプ
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
					Jump<ShortType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 5.0f;
				}
				//Bボタンが押された瞬間なら中ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
					Jump<MediumType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 10.0f;
				}
				//Yボタンが押された瞬間なら大ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y){
					Jump<LongType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 15.0f;
				}
				//Xボタンが押された瞬間なら大大ジャンプ
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X){
					Jump<LongLongType> j;
					PtrGravity->StartJump(0, j(), 0);
					m_RotationSpeed = 20.0f;
				}
			}
		}

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_YRot += (m_RotationSpeed * ElapsedTime);
		if (m_RotationSpeed > 0){
			m_RotationSpeed -= 0.05f;
		}
		auto Ptr = GetComponent<Transform>();
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);

	}




}
//endof  basedx11
