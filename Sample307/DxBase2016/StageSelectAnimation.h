#pragma once
#include "stdafx.h"

namespace basedx11{
	
	/**
	 * @class StageSelectAnimation
	 * @brief ステージセレクト時のアニメーション.
	 * @author Program 荻野 彩樹, Design 古川 拓也
	 * @date 2015/05/21 Start
	 * @date 2015/07/28 Release Update
	 */
	class StageSelectAnimation : public SS5ssae 
	{
	public : 
		//	---------- Method ------------.
		StageSelectAnimation( shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& AnimationName ) ;
		virtual ~StageSelectAnimation () {}

		virtual void OnCreate() override ;

		virtual void OnUpdate() ;

		void PlaySE ( const wstring& i_wsSEName ) ;
		//	------------------------------.

		//	---------- Accessor ----------.
		shared_ptr< StateMachine<StageSelectAnimation> > GetStateMachine() const
		{
			return m_StateMachine;
		}

		const float GetCursorLimit () const
		{
			return m_fCursorLimit ;
		}

		const bool GetIsCursorMove () const
		{
			return m_bIsCursorMove ;
		}

		void SetIsCursorMove ( const bool i_bIsCursorMove )
		{
			m_bIsCursorMove= i_bIsCursorMove ;
		}
		//	------------------------------.

		//	----------- Member -----------.
		//	NONE.
		//	------------------------------.

	private :
		//	----------- Method -----------.
		//	: スティックの状態を管理する.
		void StickManage () ;
		//	------------------------------.

		//	----------- Member -----------.
		shared_ptr< StateMachine<StageSelectAnimation> >  m_StateMachine ;	//	: ステートマシーン.

		bool m_bIsCursorMove ;	//	: カーソルが操作可能かどうか.
		float m_fCursorLimit ;	//	: スティックが倒されたと判断する制御値.
		//	------------------------------.
	};


	//--------------------------------------------------------------------------------------
	//	class OpeningState : public ObjState<StageSelectAnimation>
	//	用途: ステージセレクト画面展開アニメーションステート.
	//--------------------------------------------------------------------------------------
	class OpeningState : public ObjState<StageSelectAnimation>
	{
		OpeningState(){}
	public:

		static shared_ptr<OpeningState> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class OpenToStage1State : public ObjState<StageSelectAnimation>
	//	用途: オープニングからステージ1セレクトアニメーションステート.
	//--------------------------------------------------------------------------------------
	class OpenToStage1State : public ObjState<StageSelectAnimation>
	{
		OpenToStage1State(){}
	public:

		static shared_ptr<OpenToStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class Stage1ToStage2State : public ObjState<StageSelectAnimation>
	//	用途: ステージ1からステージ2セレクトアニメーションステート.
	//--------------------------------------------------------------------------------------
	class Stage1ToStage2State : public ObjState<StageSelectAnimation>
	{
		Stage1ToStage2State(){}
	public:

		static shared_ptr<Stage1ToStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage2ToStage3State : public ObjState<StageSelectAnimation>
	//	用途: ステージ2からステージ3セレクトアニメーションステート.
	//--------------------------------------------------------------------------------------
	class Stage2ToStage3State : public ObjState<StageSelectAnimation>
	{
		Stage2ToStage3State(){}
	public:

		static shared_ptr<Stage2ToStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage3ToStage2State : public ObjState<StageSelectAnimation>
	//	用途: ステージ3からステージ2セレクトアニメーションステート.
	//--------------------------------------------------------------------------------------
	class Stage3ToStage2State : public ObjState<StageSelectAnimation>
	{
		Stage3ToStage2State(){}
	public:

		static shared_ptr<Stage3ToStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage2ToStage1State : public ObjState<StageSelectAnimation>
	//	用途: ステージ2からステージ1セレクトアニメーションステート.
	//--------------------------------------------------------------------------------------
	class Stage2ToStage1State : public ObjState<StageSelectAnimation>
	{
		Stage2ToStage1State(){}
	public:

		static shared_ptr<Stage2ToStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage1State : public ObjState<StageSelectAnimation>
	//	用途: ステージ1ノイズアニメーションステート.
	//--------------------------------------------------------------------------------------
	class NoiseStage1State : public ObjState<StageSelectAnimation>
	{
		NoiseStage1State(){}
	public:

		static shared_ptr<NoiseStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage2State : public ObjState<StageSelectAnimation>
	//	用途: ステージ2ノイズアニメーションステート.
	//--------------------------------------------------------------------------------------
	class NoiseStage2State : public ObjState<StageSelectAnimation>
	{
		NoiseStage2State(){}
	public:

		static shared_ptr<NoiseStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage3State : public ObjState<StageSelectAnimation>
	//	用途: ステージ3ノイズアニメーションステート.
	//--------------------------------------------------------------------------------------
	class NoiseStage3State : public ObjState<StageSelectAnimation>
	{
		NoiseStage3State(){}
	public:

		static shared_ptr<NoiseStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage1State : public ObjState<StageSelectAnimation>
	//	用途: ステージ1決定アニメーションステート.
	//--------------------------------------------------------------------------------------
	class DecideStage1State : public ObjState<StageSelectAnimation>
	{
		DecideStage1State(){}
	public:

		static shared_ptr<DecideStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage2State : public ObjState<StageSelectAnimation>
	//	用途: ステージ2決定アニメーションステート.
	//--------------------------------------------------------------------------------------
	class DecideStage2State : public ObjState<StageSelectAnimation>
	{
		DecideStage2State(){}
	public:

		static shared_ptr<DecideStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage3State : public ObjState<StageSelectAnimation>
	//	用途: ステージ3決定アニメーションステート.
	//--------------------------------------------------------------------------------------
	class DecideStage3State : public ObjState<StageSelectAnimation>
	{
		DecideStage3State(){}
	public:

		static shared_ptr<DecideStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};
}