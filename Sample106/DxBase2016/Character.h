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
		float m_InStartTime;	//発射後の経過時間
		bool m_IsEnemyBall;		//敵側の砲弾かどうか
		shared_ptr< StateMachine<ShellBall> >  m_StateMachine;	//ステートマシーン
	public:
		//構築と破棄
		ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
			const Vector3& JumpVec, bool IsEnemyBall);
		virtual ~ShellBall();
		//アクセサ
		shared_ptr< StateMachine<ShellBall> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//再初期化
		void Refresh(const Vector3& StartPos,
			const Vector3& JumpVec, bool IsEnemyBall);
		//爆発を演出する関数
		//発射後一定の時間がたったら衝突をアクティブにする
		void HitTestCheckMotion();
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



	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		shared_ptr< StateMachine<RollingTorus> >  m_StateMachine;	//ステートマシーン
		Vector3 m_StartPos;
		float m_YRot;
		const float m_MaxRotationSpeed;
		float m_RotationSpeed;
		//砲弾の発射
		void StartShellBall();
	public:
		//構築と破棄
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//アクセサ
		shared_ptr< StateMachine<RollingTorus> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(const shared_ptr<GameObject>& other) override;

		//モーションを実装する関数群
		//必要であれば回転するモーション
		void RotationMotion();
		//確率により砲弾を発射するモーション
		void ShellThrowMotion();

		//砲弾と衝突した瞬間の処理
		void ShellHitMotion();
		//砲弾と衝突した後の処理
		//落下終了したらtrueを返す
		bool ShellHitMoveMotion();
	};

	//--------------------------------------------------------------------------------------
	//	class TorusDefaultState : public ObjState<RollingTorus>;
	//	用途: 通常状態
	//--------------------------------------------------------------------------------------
	class TorusDefaultState : public ObjState<RollingTorus>
	{
		TorusDefaultState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<TorusDefaultState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<RollingTorus>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<RollingTorus>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<RollingTorus>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class TorusShellHitState : public ObjState<RollingTorus>;
	//	用途: 砲弾が命中したときの処理
	//--------------------------------------------------------------------------------------
	class TorusShellHitState : public ObjState<RollingTorus>
	{
		TorusShellHitState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<TorusShellHitState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<RollingTorus>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<RollingTorus>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<RollingTorus>& Obj)override;
	};





	//--------------------------------------------------------------------------------------
	//	class ScoreObject : public GameObject;
	//	用途: スコアを表示するオブジェクト
	//--------------------------------------------------------------------------------------
	class ScoreObject : public GameObject{
		//イベントマシーン
		shared_ptr< EventMachine<ScoreObject> >  m_EventMachine;
		//ポイント
		size_t m_Point;
		//敵側(トーラス)のポイント
		size_t m_EnemyPoint;

	public:
		//構築と破棄
		ScoreObject(const shared_ptr<Stage>& StagePtr);
		virtual ~ScoreObject();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//イベントのハンドラ
		virtual void OnEvent(const shared_ptr<Event>& event)override;
		//イベントが発生したときの演出関数
		//ポイントの加算
		void AddPointMotion(size_t sz);
		//敵ポイントの加算
		void AddEnemyPointMotion(size_t sz);
	};


	//--------------------------------------------------------------------------------------
	//	class TorusHitEvent : public EventState<ScoreObject>;
	//	用途: トーラスが砲弾に当たったイベント
	//--------------------------------------------------------------------------------------
	class TorusHitEvent : public EventState<ScoreObject>
	{
		TorusHitEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<TorusHitEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event)override;
	};

	//--------------------------------------------------------------------------------------
	//	class PlayerHitEvent : public EventState<ScoreObject>;
	//	用途: プレイヤーが砲弾に当たったイベント
	//--------------------------------------------------------------------------------------
	class PlayerHitEvent : public EventState<ScoreObject>
	{
		PlayerHitEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<PlayerHitEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event)override;
	};


}
//endof  basedx11
