#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	用途: 砲弾
	//--------------------------------------------------------------------------------------
	class ShellBall : public GameObject{
		Vector3 m_StartPos;
		Vector3 m_NowScale;
		Vector3 m_JumpVec;
		shared_ptr< StateMachine<ShellBall> >  m_StateMachine;	//ステートマシーン
	public:
		//構築と破棄
		ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
			const Vector3& JumpVec);
		virtual ~ShellBall();
		//アクセサ
		shared_ptr< StateMachine<ShellBall> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//再初期化
		void Refresh(const Vector3& StartPos, const Vector3& JumpVec);
		//爆発を演出する関数
		//地面についたかどうか
		bool IsArrivedBaseMotion();
		//爆発の開始
		void ExplodeStartMotion();
		//爆発の演出(演出終了で更新と描画を無効にする）
		void ExplodeExcuteMotion();

	};

	//--------------------------------------------------------------------------------------
	//	class FiringState : public ObjState<ShellBall>;
	//	用途: 発射から爆発までのステート
	//--------------------------------------------------------------------------------------
	class FiringState : public ObjState<ShellBall>
	{
		FiringState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<FiringState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<ShellBall>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<ShellBall>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<ShellBall>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class ExplodeState : public ObjState<ShellBall>;
	//	用途: 爆発最中のステート
	//--------------------------------------------------------------------------------------
	class ExplodeState : public ObjState<ShellBall>
	{
		ExplodeState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<ExplodeState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<ShellBall>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<ShellBall>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<ShellBall>& Obj)override;
	};

}
//endof  basedx11
