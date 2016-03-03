#pragma once

#include "stdafx.h"

namespace basedx11{


	template <typename T>
	class MyStateMachine;

	//--------------------------------------------------------------------------------------
	//	class MyObjState;
	//	用途: ステート実装テンプレートクラス(抽象クラス)
	//--------------------------------------------------------------------------------------
	template <typename T>
	class MyObjState{
	public:
		//--------------------------------------------------------------------------------------
		//	MyObjState();
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		MyObjState(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~MyObjState();
		//	用途: デストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		virtual ~MyObjState(){}
		//--------------------------------------------------------------------------------------
		//	virtual void Enter(
		//	const shared_ptr<T>& Obj		//ステートを保持するオブジェクト
		//	) = 0;
		//	用途: ステートに入ったときに実行される
		//	戻り値: なし（純粋仮想関数）
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Execute(
		//	const shared_ptr<T>& Obj		//ステートを保持するオブジェクト
		//	) = 0;
		//	用途: Updateのときに実行される
		//	戻り値: なし（純粋仮想関数）
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Exit(
		//	const shared_ptr<T>& Obj		//ステートを保持するオブジェクト
		//	) = 0;
		//	用途: ステートを出るときに実行される
		//	戻り値: なし（純粋仮想関数）
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<T>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	//	template <typename T>
	//	class MyStateMachine;
	//	用途: ステートマシン実装テンプレートクラス(抽象クラス)
	//	＊ステートを管理する
	//--------------------------------------------------------------------------------------
	template <typename T>
	class MyStateMachine
	{
	private:
		//このステートマシンを持つオーナー
		weak_ptr<T> m_Owner;
		//現在のステート
		weak_ptr< MyObjState<T> > m_CurrentState;
		//一つ前のステート
		weak_ptr< MyObjState<T> > m_PreviousState;
	public:
		//--------------------------------------------------------------------------------------
		//	MyStateMachine(
		//	const shared_ptr<T>& owner	//このステートマシンを保持するオーナー
		//	);
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		explicit MyStateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
		{}
		//--------------------------------------------------------------------------------------
		//	virtual ~MyStateMachine();
		//	用途: デストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		virtual ~MyStateMachine(){}
		//--------------------------------------------------------------------------------------
		//	用途: Setアクセッサ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void SetCurrentState(const shared_ptr< MyObjState<T> >& s){ m_CurrentState = s; }
		void SetPreviousState(const shared_ptr< MyObjState<T> >& s){ m_PreviousState = s; }
		//--------------------------------------------------------------------------------------
		//	用途: Getアクセッサ
		//	戻り値: MyObjState<T>*
		//--------------------------------------------------------------------------------------
		shared_ptr< MyObjState<T> >  GetCurrentState() const {
			if (!m_CurrentState.expired()){
				return m_CurrentState.lock();
			}
			return nullptr;
		}
		shared_ptr< MyObjState<T> >  GetPreviousState()const {
			if (!m_pPreviousState.expired()){
				return m_pPreviousState.lock();
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		//	void  Update() const
		//	用途: ステートを更新する
		//	戻り値: なし
		//	＊メンバは変更しないのでconst関数で問題ない
		//--------------------------------------------------------------------------------------
		void Update() const{
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				auto Ptr = m_CurrentState.lock();
				Ptr->Execute(m_Owner.lock());
			}
		}
		//--------------------------------------------------------------------------------------
		//	void  ChangeState(
		//	const shared_ptr< MyObjState<T> >& NewState	//新しいステート
		//　);
		// 用途: ステートを変更する
		// 戻り値: なし
		//--------------------------------------------------------------------------------------
		void  ChangeState(const shared_ptr< MyObjState<T> >& NewState){
			//元のステートを保存
			m_PreviousState = m_CurrentState;
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				//元のステートに終了を知らせる
				auto Ptr = m_CurrentState.lock();
				Ptr->Exit(m_Owner.lock());
			}
			//新しいステートをカレントに設定
			m_CurrentState = NewState;
			if (!m_CurrentState.expired() && !m_Owner.expired()){
				//元のステートに終了を知らせる
				auto Ptr = m_CurrentState.lock();
				Ptr->Enter(m_Owner.lock());
			}
		}
		//--------------------------------------------------------------------------------------
		//	void RevertToPreviousState();
		// 用途: ステートを一つ前のステートに戻す
		// 戻り値: なし
		//--------------------------------------------------------------------------------------
		void RevertToPreviousState(){
			ChangeState(m_pPreviousState);
		}
		//--------------------------------------------------------------------------------------
		//bool IsInState(
		//const MyObjState<T>& st	//調べるステート
		//)const;
		//用途: カレントステートが指定したステートになってるかどうかをチェック
		//戻り値: なし
		//--------------------------------------------------------------------------------------
		bool IsInState(const shared_ptr< MyObjState<T> >& st)const{
			if (m_CurrentState.expired()){
				return false;
			}
			return typeid(m_CurrentState.lock()) == typeid(st);
		}
	};


	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//プレートの作成
		void CreatePlate();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void OnCreate()override;
	};
}
//endof  basedx11
