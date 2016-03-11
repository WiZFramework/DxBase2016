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
		bool IsEnemyBall()const {
			return m_IsEnemyBall;
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
	//	class Enemy : public GameObject;
	//	用途: 敵キャラの親クラス
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject{
		shared_ptr< StateMachine<Enemy> >  m_StateMachine;	//ステートマシーン
		//砲弾の発射
		void StartShellBall();
	protected:
		Vector3 m_StartPos;
		//構築と破棄
		Enemy(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Enemy();
		//派生クラスは、必ずこの仮想関数を実装する
		virtual void ExtMotionMethod() = 0;
		virtual void ShellHitMotionMethod() = 0;

	public:
		//アクセサ
		shared_ptr< StateMachine<Enemy> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//衝突時
		virtual void OnCollision(const shared_ptr<GameObject>& other) override;

		//モーションを実装する関数群

		//この関数はメソッド呼び出し関数として実装する
		void ExtMotion();
		//砲弾と衝突した瞬間の処理
		void ShellHitMotion();

		//確率により砲弾を発射するモーション
		void ShellThrowMotion();
		//砲弾と衝突した後の処理
		//落下終了したらtrueを返す
		bool ShellHitMoveMotion();
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyDefaultState : public ObjState<Enemy>;
	//	用途: 通常状態
	//--------------------------------------------------------------------------------------
	class EnemyDefaultState : public ObjState<Enemy>
	{
		EnemyDefaultState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<EnemyDefaultState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class EnemyShellHitState : public ObjState<Enemy>;
	//	用途: 砲弾が命中したときの処理
	//--------------------------------------------------------------------------------------
	class EnemyShellHitState : public ObjState<Enemy>
	{
		EnemyShellHitState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<EnemyShellHitState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Enemy>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Enemy>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Enemy>& Obj)override;
	};




	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public Enemy;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	class RollingTorus : public Enemy{
		float m_YRot;
		const float m_MaxRotationSpeed;
		float m_RotationSpeed;
	protected:
		//派生クラスは、必ずこの仮想関数を実装する
		virtual void ExtMotionMethod()override;
		virtual void ShellHitMotionMethod()override;
	public:
		//構築と破棄
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//初期化
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class ColoringOctahedron : public Enemy;
	//	用途: 色が変わる8面体
	//--------------------------------------------------------------------------------------
	class ColoringOctahedron : public Enemy{
	protected:
		//派生クラスは、必ずこの仮想関数を実装する
		virtual void ExtMotionMethod()override;
		virtual void ShellHitMotionMethod()override;
	public:
		//構築と破棄
		ColoringOctahedron(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~ColoringOctahedron();
		//初期化
		virtual void OnCreate() override;
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
	class EnemyHitEvent : public EventState<ScoreObject>
	{
		EnemyHitEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<EnemyHitEvent> Instance();
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
