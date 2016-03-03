#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	用途: 砲弾
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ShellBall::ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
		const Vector3& JumpVec) :
		GameObject(StagePtr), m_StartPos(StartPos),
		m_NowScale(0.25f, 0.25f, 0.25f), m_JumpVec(JumpVec)
	{}
	ShellBall::~ShellBall(){}
	//初期化
	void ShellBall::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetRotation(0, 0, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.125f);
		//ジャンプスタート
		PtrGravity->StartJump(m_JumpVec);

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<ShellBall> >(GetThis<ShellBall>());
		//最初のステートをFiringStateに設定
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}

	void ShellBall::Refresh(const Vector3& StartPos, const Vector3& JumpVec){
		SetUpdateActive(true);
		SetDrawActive(true);
		m_StartPos = StartPos;
		m_JumpVec = JumpVec;
		//Transform取得
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetPosition(m_StartPos);
		//描画コンポーネント
		auto PtrDraw = GetComponent<PNTStaticDraw>();
		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		//重力を取り出す
		auto PtrGravity = GetComponent<Gravity>();
		//ジャンプスタート
		PtrGravity->StartJump(m_JumpVec);

		//今のステートをFiringStateに設定
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}


	void ShellBall::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
		//ステートマシンを使うことでUpdate処理を分散できる
	}

	//爆発を演出する関数
	//地面についたかどうか
	bool ShellBall::IsArrivedBaseMotion(){
		//重力を取り出す
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->IsGravityVelocityZero()){
			//落下速度が0ならtrue
			return true;
		}
		return false;
	}

	//爆発の開始
	void ShellBall::ExplodeStartMotion(){
		//Transform取得
		auto Ptr = GetComponent<Transform>();
		m_NowScale = Vector3(5.0f, 5.0f, 5.0f);
		Ptr->SetScale(m_NowScale);
		//描画コンポーネント
		auto PtrDraw = GetComponent<PNTStaticDraw>();
		//爆発中は赤
		PtrDraw->SetDiffuse(Color4(1.0f, 0.0f, 0, 1.0f));
	}

	//爆発の演出(演出終了で更新と描画を無効にする）
	void ShellBall::ExplodeExcuteMotion(){
		//Transform取得
		auto Ptr = GetComponent<Transform>();
		m_NowScale *= 0.9f;
		if (m_NowScale.x < 0.25f){
			m_NowScale = Vector3(0.25f, 0.25f, 0.25f);
			//表示と更新をしないようにする
			//こうするとこの後、更新及び描画系は全く呼ばれなくなる
			//再び更新描画するためには、外部から操作が必要（プレイヤーが呼び起こす）
			SetUpdateActive(false);
			SetDrawActive(false);
		}
		Ptr->SetScale(m_NowScale);
	}


	//--------------------------------------------------------------------------------------
	//	class FiringState : public ObjState<ShellBall>;
	//	用途: 発射から爆発までのステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<FiringState> FiringState::Instance(){
		static shared_ptr<FiringState> instance;
		if (!instance){
			instance = shared_ptr<FiringState>(new FiringState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void FiringState::Enter(const shared_ptr<ShellBall>& Obj){
		//何もしない
	}
	//ステート実行中に毎ターン呼ばれる関数
	void FiringState::Execute(const shared_ptr<ShellBall>& Obj){
		//落下終了かどうかチェック
		if (Obj->IsArrivedBaseMotion()){
			//落下終了ならステート変更
			Obj->GetStateMachine()->ChangeState(ExplodeState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void FiringState::Exit(const shared_ptr<ShellBall>& Obj){
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	//	class ExplodeState : public ObjState<ShellBall>;
	//	用途: 爆発最中のステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<ExplodeState> ExplodeState::Instance(){
		static shared_ptr<ExplodeState> instance;
		if (!instance){
			instance = shared_ptr<ExplodeState>(new ExplodeState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void ExplodeState::Enter(const shared_ptr<ShellBall>& Obj){
		//爆発の開始
		Obj->ExplodeStartMotion();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void ExplodeState::Execute(const shared_ptr<ShellBall>& Obj){
		//爆発演出の実行
		Obj->ExplodeExcuteMotion();
	}
	//ステートにから抜けるときに呼ばれる関数
	void ExplodeState::Exit(const shared_ptr<ShellBall>& Obj){
		//何もしない
	}




}
//endof  basedx11
