#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	StageSelectAnimation::StageSelectAnimation( shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& AnimationName ) 
		:	SS5ssae( StagePtr, BaseDir, L"StageSelect.ssae", AnimationName, true ),
			m_StateMachine( nullptr ),
			m_bIsCursorMove( true ),
			m_fCursorLimit( 0.65f )
	{

	}

	void StageSelectAnimation::OnCreate ()
	{
		SS5ssae::OnCreate();

		//	: ステートマシンの構築.
		m_StateMachine = make_shared< StateMachine<StageSelectAnimation> >( GetThis<StageSelectAnimation>() ) ;
		m_StateMachine->SetCurrentState( OpeningState::Instance() ) ;
		m_StateMachine->GetCurrentState()->Enter( GetThis<StageSelectAnimation>() ) ;

		//	: 値は秒あたりのフレーム数.
		SetFps( 60.0f ) ;

		//	: スプライトのサイズスケーリング.
		auto PtrT = AddComponent<Transform>();
		//スプライト版のSpriteStdioは、サイズ32.0f基準のスプライトを作成するので
		//それに合わせてスケーリングする
		PtrT->SetScale(4.0f, 4.0f, 1.0f);

//		SetSpritePixelParMeter( 4.0f ) ;

		//	: ループは行わない.
		SetLooped( false ) ;

		//	: サウンドを登録.
		auto pMultiSoundEffect = AddComponent<MultiSoundEffect>() ;
		pMultiSoundEffect->AddAudioResource( L"Cursor" ) ;
		pMultiSoundEffect->AddAudioResource( L"Decide" ) ;
	}

	void StageSelectAnimation::OnUpdate ()
	{
		const float fTimeSpan = App::GetApp()->GetElapsedTime();

		m_StateMachine->Update() ;

		UpdateAnimeTime( fTimeSpan ) ;

		StickManage() ;
	}

	//	: 指定した効果音を再生する.
	void StageSelectAnimation::PlaySE ( const wstring& i_wsSEName )
	{
		auto pMultiSoundEffect = AddComponent<MultiSoundEffect>() ;
		pMultiSoundEffect->Start( i_wsSEName, 0, 0.5f ) ;
	}

	//	: スティックの状態を管理する.
	void StageSelectAnimation::StickManage ()
	{
		//	: コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec() ;

		//	: スティックがリミット以下の場合.
		if ( CntlVec[0].fThumbLY < m_fCursorLimit && CntlVec[0].fThumbLY > -m_fCursorLimit ) {	
			//	: カーソルの移動を許可.
			m_bIsCursorMove = true ;
		}
	}


	//	---------- Status ----------.
	//	: Opening.
	shared_ptr<OpeningState> OpeningState::Instance(){
		static shared_ptr<OpeningState> instance;
		if (!instance){
			instance = shared_ptr<OpeningState>(new OpeningState);
		}
		return instance;
	}

	void OpeningState::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		
	}

	void OpeningState::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ) {
			Obj->GetStateMachine()->ChangeState( OpenToStage1State::Instance() ) ;
		}
	}

	//	: OpenToStage1State.
	shared_ptr<OpenToStage1State> OpenToStage1State::Instance(){
		static shared_ptr<OpenToStage1State> instance;
		if (!instance){
			instance = shared_ptr<OpenToStage1State>(new OpenToStage1State);
		}
		return instance;
	}

	void OpenToStage1State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_OpenToStage1" ) ;
		Obj->SetFps(120.0f);
		Obj->SetLooped( false ) ;
	}

	void OpenToStage1State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ){
			Obj->GetStateMachine()->ChangeState( NoiseStage1State::Instance() ); 
		}
	}

	void OpenToStage1State::Exit(const shared_ptr<StageSelectAnimation>& Obj)
	{

	}

	//	: Stage1ToStage2State.
	shared_ptr<Stage1ToStage2State> Stage1ToStage2State::Instance(){
		static shared_ptr<Stage1ToStage2State> instance;
		if (!instance){
			instance = shared_ptr<Stage1ToStage2State>(new Stage1ToStage2State);
		}
		return instance;
	}

	void Stage1ToStage2State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage1ToStage2" ) ;
		Obj->SetFps(120.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Cursor") ;
	}

	void Stage1ToStage2State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ){
			Obj->GetStateMachine()->ChangeState( NoiseStage2State::Instance() ); 
			Obj->SetIsCursorMove( false ) ;
		}
	}

	//	: Stage2ToStage3State.
	shared_ptr<Stage2ToStage3State> Stage2ToStage3State::Instance(){
		static shared_ptr<Stage2ToStage3State> instance;
		if (!instance){
			instance = shared_ptr<Stage2ToStage3State>(new Stage2ToStage3State);
		}
		return instance;
	}

	void Stage2ToStage3State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage2ToStage3" ) ;
		Obj->SetFps(120.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Cursor") ;
	}

	void Stage2ToStage3State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ){
			Obj->GetStateMachine()->ChangeState( NoiseStage3State::Instance() ); 
		}
	}

	//	: Stage3ToStage2State.
	shared_ptr<Stage3ToStage2State> Stage3ToStage2State::Instance(){
		static shared_ptr<Stage3ToStage2State> instance;
		if (!instance){
			instance = shared_ptr<Stage3ToStage2State>(new Stage3ToStage2State);
		}
		return instance;
	}

	void Stage3ToStage2State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage3ToStage2" ) ;
		Obj->SetFps(120.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Cursor") ;
	}

	void Stage3ToStage2State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ){
			Obj->GetStateMachine()->ChangeState( NoiseStage2State::Instance() ); 
		}
	}

	//	: Stage2ToStage1State.
	shared_ptr<Stage2ToStage1State> Stage2ToStage1State::Instance(){
		static shared_ptr<Stage2ToStage1State> instance;
		if (!instance){
			instance = shared_ptr<Stage2ToStage1State>(new Stage2ToStage1State);
		}
		return instance;
	}

	void Stage2ToStage1State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage2ToStage1" ) ;
		Obj->SetFps(120.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Cursor") ;
	}

	void Stage2ToStage1State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ){
			Obj->GetStateMachine()->ChangeState( NoiseStage1State::Instance() ); 
		}
	}

	//	: NoiseStage1State.
	shared_ptr<NoiseStage1State> NoiseStage1State::Instance(){
		static shared_ptr<NoiseStage1State> instance;
		if (!instance){
			instance = shared_ptr<NoiseStage1State>(new NoiseStage1State);
		}
		return instance;
	}

	void NoiseStage1State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage1" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( true ) ;
	}

	void NoiseStage1State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		//	: コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();

		//	: Aボタンが押されたら.
		if ( CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A ) {
			//	: ステージを決定.
			Obj->GetStateMachine()->ChangeState( DecideStage1State::Instance() ) ;
		}

		//	: スティックが倒されたら.
		if ( Obj->GetIsCursorMove() && CntlVec[0].fThumbLY <= -Obj->GetCursorLimit() ) {	
			//	: 選択を移動.
			Obj->GetStateMachine()->ChangeState( Stage1ToStage2State::Instance() ) ;
			Obj->SetIsCursorMove( false ) ;
		}
	}

	//	: NoiseStage2State.
	shared_ptr<NoiseStage2State> NoiseStage2State::Instance(){
		static shared_ptr<NoiseStage2State> instance;
		if (!instance){
			instance = shared_ptr<NoiseStage2State>(new NoiseStage2State);
		}
		return instance;
	}

	void NoiseStage2State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage2" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( true ) ;
	}

	void NoiseStage2State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		//	: コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();

		//	: Aボタンが押されたら.
		if ( CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A ) {
			//	: ステージを決定.
			Obj->GetStateMachine()->ChangeState( DecideStage2State::Instance() ) ;
		}

		//	: スティックが倒されたら.
		if ( Obj->GetIsCursorMove() && CntlVec[0].fThumbLY >= Obj->GetCursorLimit() ) {	
			//	: 選択を移動.
			Obj->GetStateMachine()->ChangeState( Stage2ToStage1State::Instance() ) ;
			Obj->SetIsCursorMove( false ) ;
		}
		else if ( Obj->GetIsCursorMove() && CntlVec[0].fThumbLY <= -Obj->GetCursorLimit() ) {	
			//	: 選択を移動.
			Obj->GetStateMachine()->ChangeState( Stage2ToStage3State::Instance() ) ;
			Obj->SetIsCursorMove( false ) ;
		}
	}

	//	: NoiseStage3State.
	shared_ptr<NoiseStage3State> NoiseStage3State::Instance(){
		static shared_ptr<NoiseStage3State> instance;
		if (!instance){
			instance = shared_ptr<NoiseStage3State>(new NoiseStage3State);
		}
		return instance;
	}

	void NoiseStage3State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Noise_Stage3" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( true ) ;
	}

	void NoiseStage3State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		//	: コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();

		//	: Aボタンが押されたら.
		if ( CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A ) {
			//	: ステージを決定.
			Obj->GetStateMachine()->ChangeState( DecideStage3State::Instance() ) ;
		}

		//	: スティックが倒されたら.
		if ( Obj->GetIsCursorMove() && CntlVec[0].fThumbLY >= Obj->GetCursorLimit() ) {	
			//	: 選択を移動.
			Obj->GetStateMachine()->ChangeState( Stage3ToStage2State::Instance() ) ;
			Obj->SetIsCursorMove( false ) ;
		}
	}

	//	: DecideStage1State.
	shared_ptr<DecideStage1State> DecideStage1State::Instance(){
		static shared_ptr<DecideStage1State> instance;
		if (!instance){
			instance = shared_ptr<DecideStage1State>(new DecideStage1State);
		}
		return instance;
	}

	void DecideStage1State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Decide_Stage1" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Decide") ;
	}

	void DecideStage1State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ) {
			Obj->PostEvent(0, Obj->GetThis<StageSelectAnimation>(), App::GetApp()->GetSceneBase(), L"ToGame");
		}	
	}

	//	: DecideStage2State.
	shared_ptr<DecideStage2State> DecideStage2State::Instance(){
		static shared_ptr<DecideStage2State> instance;
		if (!instance){
			instance = shared_ptr<DecideStage2State>(new DecideStage2State);
		}
		return instance;
	}

	void DecideStage2State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Decide_Stage2" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Decide") ;
	}

	void DecideStage2State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ) {
			Obj->PostEvent(0, Obj->GetThis<StageSelectAnimation>(), App::GetApp()->GetSceneBase(), L"ToGame");
		}	
	}

	//	: DecideStage3State.
	shared_ptr<DecideStage3State> DecideStage3State::Instance(){
		static shared_ptr<DecideStage3State> instance;
		if (!instance){
			instance = shared_ptr<DecideStage3State>(new DecideStage3State);
		}
		return instance;
	}

	void DecideStage3State::Enter(const shared_ptr<StageSelectAnimation>& Obj)
	{
		Obj->ChangeAnimation( L"Decide_Stage3" ) ;
		Obj->SetFps(60.0f);
		Obj->SetLooped( false ) ;
		Obj->PlaySE(L"Decide") ;
	}

	void DecideStage3State::Execute(const shared_ptr<StageSelectAnimation>& Obj)
	{
		if( Obj->IsAnimeEnd() ) {
			Obj->PostEvent(0, Obj->GetThis<StageSelectAnimation>(), App::GetApp()->GetSceneBase(), L"ToGame");
		}	
	}
	//	----------------------------.
}