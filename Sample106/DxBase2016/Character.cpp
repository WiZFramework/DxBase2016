#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ShellBall : public GameObject;
	//	用途: 砲弾
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ShellBall::ShellBall(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos,
		const Vector3& JumpVec, bool IsEnemyBall) :
		GameObject(StagePtr), m_StartPos(StartPos),
		m_NowScale(0.25f, 0.25f, 0.25f), m_JumpVec(JumpVec),
		m_InStartTime(0), m_IsEnemyBall(IsEnemyBall)
	{}
	ShellBall::~ShellBall(){}
	//初期化
	void ShellBall::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetRotation(0, 0, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//衝突判定をつける
		auto PtrCollision = AddComponent<CollisionSphere>();
		//衝突は無効にしておく
		PtrCollision->SetUpdateActive(false);
		//砲弾のグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		//砲弾同士は衝突しないようにしておく
		PtrCollision->SetExcludeCollisionGroup(Group);


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
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 1.0f, 0, 1.0f));
		}

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<ShellBall> >(GetThis<ShellBall>());
		//最初のステートをFiringStateに設定
		m_StateMachine->SetCurrentState(FiringState::Instance());
		//FiringStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<ShellBall>());

	}

	void ShellBall::Refresh(const Vector3& StartPos, const Vector3& JumpVec, bool IsEnemyBall){
		SetUpdateActive(true);
		SetDrawActive(true);
		m_StartPos = StartPos;
		m_JumpVec = JumpVec;
		m_InStartTime = 0;
		m_IsEnemyBall = IsEnemyBall;
		//Transform取得
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_NowScale);
		Ptr->SetPosition(m_StartPos);
		//描画コンポーネント
		auto PtrDraw = GetComponent<PNTStaticDraw>();
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 1.0f, 0, 1.0f));
		}
		//衝突判定を呼び出す
		auto PtrCollision = GetComponent<CollisionSphere>();
		//衝突は無効にしておく
		PtrCollision->SetUpdateActive(false);

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
	//発射後一定の時間がたったら衝突をアクティブにする
	void ShellBall::HitTestCheckMotion(){
		//衝突判定を呼び出す
		auto PtrCollision = GetComponent<CollisionSphere>();
		if (PtrCollision->IsUpdateActive()){
			//既に衝突は有効
			return;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_InStartTime += ElapsedTime;
		if (m_InStartTime > 0.5f){
			//衝突を有効にする
			PtrCollision->SetUpdateActive(true);
		}
	}


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
		//爆発中の色
		if (m_IsEnemyBall){
			PtrDraw->SetDiffuse(Color4(1.0f, 0.0f, 0, 1.0f));
		}
		else{
			PtrDraw->SetDiffuse(Color4(0.0f, 0.0f, 1.0f, 1.0f));
		}

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
		//経過時間によって衝突を有効にする
		Obj->HitTestCheckMotion();
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



	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_YRot(0),
		m_MaxRotationSpeed(20.0f),
		m_RotationSpeed(0.0f)
	{}
	RollingTorus::~RollingTorus(){}
	//初期化
	void RollingTorus::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5f, 0.5f);
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//衝突判定をつける
		auto PtrCollision = AddComponent<CollisionSphere>();

		//トーラスのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"RollingTorusGroup");
		//トーラス同士は衝突しないようにしておく
		PtrCollision->SetExcludeCollisionGroup(Group);


		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.25f);
		//ジャンプスタート
		PtrGravity->StartJump(Vector3(0, 4.0f, 0));

		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_TORUS");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_TORUS");

		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<RollingTorus> >(GetThis<RollingTorus>());
		//最初のステートをDefaultStateに設定
		m_StateMachine->SetCurrentState(TorusDefaultState::Instance());
		//DefaultStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<RollingTorus>());

	}

	void RollingTorus::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	void RollingTorus::OnCollision(const shared_ptr<GameObject>& other){
		//当たったらステート変更
		GetStateMachine()->ChangeState(TorusShellHitState::Instance());
	}

	//モーションを実装する関数群
	//必要であれば回転するモーション
	void RollingTorus::RotationMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_YRot += (m_RotationSpeed * ElapsedTime);
		if (m_RotationSpeed > 0){
			m_RotationSpeed -= 0.05f;
		}
		auto Ptr = GetComponent<Transform>();
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
	}

	//砲弾を発射する関数
	//ShellThrowMotion()から呼ばれる
	void RollingTorus::StartShellBall(){
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto Pos = GetComponent<Transform>()->GetPosition();
		auto ShellAngle = PlayerPos - Pos;
		float len = ShellAngle.Length();
		ShellAngle.y = 0;
		ShellAngle.Normalize();
		ShellAngle *= len;
		//打ち上げの上向きの初速度を追加（値は固定）
		ShellAngle += Vector3(0.0f, 5.0f, 0);
		//グループ内に空きがあればそのオブジェクトを再利用
		//そうでなければ新規に作成
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		for (auto Ptr : ShellVec){
			//Ptrはweak_ptrなので有効性チェックが必要
			if (!Ptr.expired()){
				auto ShellPtr = dynamic_pointer_cast<ShellBall>(Ptr.lock());
				if (ShellPtr){
					if ((!ShellPtr->IsUpdateActive()) && (!ShellPtr->IsDrawActive())){
						ShellPtr->Refresh(Pos, ShellAngle, true);
						return;
					}
				}
			}
		}
		//ここまで来たら空きがなかったことになる
		//砲弾の追加
		auto Sh = GetStage()->AddGameObject<ShellBall>(Pos, ShellAngle, true);
		//グループに追加
		Group->IntoGroup(Sh);
	}


	//確率により砲弾を発射するモーション
	void RollingTorus::ShellThrowMotion(){
		//100分の1の確率で砲弾を発射
		if (Util::DivProbability(100)){
			StartShellBall();
		}
	}

	//砲弾と衝突した瞬間の処理
	void RollingTorus::ShellHitMotion(){
		//衝突判定を得る
		auto PtrCollision = GetComponent<CollisionSphere>();
		//ヒットしたときのみこの関数は呼ばれるので
		//PtrCollision->GetHitObject()には必ず値が入っているが
		//一応、確認しておく
		if (PtrCollision->GetHitObject()){
			auto ShellPtr = dynamic_pointer_cast<ShellBall>(PtrCollision->GetHitObject());
			if (ShellPtr){
				//相手が砲弾だった
				//相手がプレイヤーの可能性があるので、チェックする
				//スコアオブジェクトにイベント送出
				auto PtrScoreObject = GetStage()->GetSharedGameObject<ScoreObject>(L"ScoreObject");
				PostEvent(0, GetThis<RollingTorus>(), PtrScoreObject, L"TorusHit");
			}
			//相手のTransformを得る。
			auto PtrOtherTrans = PtrCollision->GetHitObject()->GetComponent<Transform>();
			//相手の場所を得る
			auto OtherPos = PtrOtherTrans->GetPosition();

			//Transformを得る。
			auto PtrTrans = GetComponent<Transform>();
			//場所を得る
			auto Pos = PtrTrans->GetPosition();

			//飛ぶ方向を計算する
			Pos -= OtherPos;
			Pos.Normalize();
			Pos.y = 0;
			Pos *= 4.0f;
			Pos += Vector3(0, 4.0f, 0);

			//回転開始
			m_RotationSpeed = m_MaxRotationSpeed;
			//衝突を相手も含めなしにする
			PtrCollision->ClearBothHitObject();
			//衝突は無効にしておく
			PtrCollision->SetUpdateActive(false);

			//重力を得る
			auto PtrGravity = GetComponent<Gravity>();
			//ジャンプスタート
			PtrGravity->StartJump(Pos);

		}


	}
	//砲弾と衝突した後の処理
	//落下終了したらtrueを返す
	bool RollingTorus::ShellHitMoveMotion(){
		//重力を得る
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->IsGravityVelocityZero()){
			//落下終了
			//衝突判定を得る
			auto PtrCollision = GetComponent<CollisionSphere>();
			//衝突を有効にする
			PtrCollision->SetUpdateActive(true);
			return true;
		}
		return false;
	}



	//--------------------------------------------------------------------------------------
	//	class TorusDefaultState : public ObjState<RollingTorus>;
	//	用途: 通常状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<TorusDefaultState> TorusDefaultState::Instance(){
		static shared_ptr<TorusDefaultState> instance;
		if (!instance){
			instance = shared_ptr<TorusDefaultState>(new TorusDefaultState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void TorusDefaultState::Enter(const shared_ptr<RollingTorus>& Obj){
		//何もしない
	}
	//ステート実行中に毎ターン呼ばれる関数
	void TorusDefaultState::Execute(const shared_ptr<RollingTorus>& Obj){
		Obj->RotationMotion();
		Obj->ShellThrowMotion();
	}
	//ステートにから抜けるときに呼ばれる関数
	void TorusDefaultState::Exit(const shared_ptr<RollingTorus>& Obj){
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	//	class TorusShellHitState : public ObjState<RollingTorus>;
	//	用途: 砲弾が命中したときの処理
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<TorusShellHitState> TorusShellHitState::Instance(){
		static shared_ptr<TorusShellHitState> instance;
		if (!instance){
			instance = shared_ptr<TorusShellHitState>(new TorusShellHitState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void TorusShellHitState::Enter(const shared_ptr<RollingTorus>& Obj){
		Obj->ShellHitMotion();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void TorusShellHitState::Execute(const shared_ptr<RollingTorus>& Obj){
		Obj->RotationMotion();
		if (Obj->ShellHitMoveMotion()){
			//落下終了ならステート変更
			Obj->GetStateMachine()->ChangeState(TorusDefaultState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void TorusShellHitState::Exit(const shared_ptr<RollingTorus>& Obj){
		//何もしない
	}






	//--------------------------------------------------------------------------------------
	//	class ScoreObject : public GameObject;
	//	用途: スコアを表示するオブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ScoreObject::ScoreObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr), m_Point(0), m_EnemyPoint(0)
	{}
	ScoreObject::~ScoreObject(){}
	//初期化
	void ScoreObject::OnCreate(){
		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));
		//イベントマシンの構築
		m_EventMachine = make_shared< EventMachine<ScoreObject>>(GetThis<ScoreObject>());
		//イベントキーとイベントステートを結び付ける
		m_EventMachine->AddEventState(L"TorusHit", TorusHitEvent::Instance());
		m_EventMachine->AddEventState(L"PlayerHit", PlayerHitEvent::Instance());

	}
	void ScoreObject::OnUpdate(){
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		wstring str(L"ポイント: ");
		str += Util::UintToWStr(m_Point);
		str += L"\n敵ポイント: ";
		str += Util::UintToWStr(m_EnemyPoint);
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		str += L"\n砲弾グループの砲弾数: ";
		str += Util::UintToWStr(ShellVec.size());

		PtrString->SetText(str);
	}

	//イベントのハンドラ
	void ScoreObject::OnEvent(const shared_ptr<Event>& event){
		//ハンドラ関数呼び出し
		//これでイベントが振り分けられる
		m_EventMachine->HandleEvent(event);
	}
	//ポイントの加算
	void ScoreObject::AddPointMotion(size_t sz){
		m_Point += sz;
	}
	//敵ポイントの加算
	void ScoreObject::AddEnemyPointMotion(size_t sz){
		m_EnemyPoint += sz;
	}




	//--------------------------------------------------------------------------------------
	//	class TorusHitEvent : public EventState<ScoreObject>;
	//	用途: トーラスが砲弾に当たったイベント
	//--------------------------------------------------------------------------------------
	//イベントステートのインスタンスを得る
	shared_ptr<TorusHitEvent> TorusHitEvent::Instance(){
		static shared_ptr<TorusHitEvent> instance;
		if (!instance){
			instance = shared_ptr<TorusHitEvent>(new TorusHitEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void TorusHitEvent::Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event){
		//ポイント加算
		Obj->AddPointMotion(1);
	}

	//--------------------------------------------------------------------------------------
	//	class PlayerHitEvent : public EventState<ScoreObject>;
	//	用途: プレイヤーが砲弾に当たったイベント
	//--------------------------------------------------------------------------------------
	//イベントステートのインスタンスを得る
	shared_ptr<PlayerHitEvent> PlayerHitEvent::Instance(){
		static shared_ptr<PlayerHitEvent> instance;
		if (!instance){
			instance = shared_ptr<PlayerHitEvent>(new PlayerHitEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void PlayerHitEvent::Enter(const shared_ptr<ScoreObject>& Obj, const shared_ptr<Event>& event){
		//敵ポイント加算
		Obj->AddEnemyPointMotion(1);
	}




}
//endof  basedx11
