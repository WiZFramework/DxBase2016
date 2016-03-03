
#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_MaxSpeed(40.0f),	//最高速度
		m_Decel(0.95f),	//減速値
		m_Mass(1.0f)	//質量
	{}

	//初期化
	void Player::OnCreate(){
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//Rigidbodyをつける
		auto PtrRedit = AddComponent<Rigidbody>();
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.125f);
		//衝突判定をつける
		auto PtrCol = AddComponent<CollisionSphere>();

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//透明処理
		SetAlphaActive(true);
		//0番目のビューのカメラを得る
		//LookAtCameraである
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//最初のステートをDefaultStateに設定
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	//移動の向きを得る
	Vector3 Player::GetAngle(){
		Vector3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 && CntlVec[0].fThumbLY != 0){
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetCamera(0);
				//進行方向の向きを計算
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				Angle.Normalize();
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}


	//更新
	void Player::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの更新が行われる(Execute()関数が呼ばれる)
		m_StateMachine->Update();
	}

	void Player::OnCollision(const shared_ptr<GameObject>& other){
		if (GetStateMachine()->GetCurrentState() == JumpState::Instance()){
			GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//ターンの最終更新時
	void Player::OnLastUpdate(){
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";

		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		wstring ShellStr(L"砲弾グループの砲弾数: ");
		ShellStr += Util::UintToWStr(ShellVec.size());

		wstring str = FPS + ShellStr;
		//文字列をつける
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}



	//モーションを実装する関数群
	//移動して向きを移動方向にする
	void Player::MoveRotationMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbodyを取り出す
		auto PtrRedit = GetComponent<Rigidbody>();
		//現在の速度を取り出す
		auto Velo = PtrRedit->GetVelocity();
		//目的地を最高速度を掛けて求める
		auto Target = Angle * m_MaxSpeed;
		//目的地に向かうために力のかける方向を計算する
		//Forceはフォースである
		auto Force = Target - Velo;
		//yは0にする
		Force.y = 0;
		//加速度を求める
		auto Accel = Force / m_Mass;
		//ターン時間を掛けたものを速度に加算する
		Velo += (Accel * ElapsedTime);
		//減速する
		Velo *= m_Decel;
		//速度を設定する
		PtrRedit->SetVelocity(Velo);
		//回転の計算
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f){
			//ベクトルをY軸回転に変換
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else{
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);
	}

	//Aボタンでジャンプするどうかを得る
	bool Player::IsJumpMotion(){
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//Aボタンが押された瞬間ならジャンプ
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				return true;
			}
		}
		return false;
	}
	//Aボタンでジャンプする瞬間の処理
	void Player::JumpMotion(){
		auto PtrTrans = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		if (PtrTrans->GetParent()){
			//親がいたら、アクションコンポーネントの移動アクションを探す
			//移動ボックスに乗っている場合、その慣性をジャンプに加算する
			auto ActionPtr = PtrTrans->GetParent()->GetComponent<Action>(false);
			if (ActionPtr){
				JumpVec += ActionPtr->GetVelocity();
			}
		}
		PtrGravity->StartJump(JumpVec);
	}
	//Aボタンでジャンプしている間の処理
	//ジャンプ終了したらtrueを返す
	bool Player::JumpMoveMotion(){
		auto PtrTransform = GetComponent<Transform>();
		//重力
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0 || PtrTransform->GetParent()){
			return true;
		}
		return false;
	}

	//Bボタンで砲弾を発射するどうかを得る
	bool Player::IsShellThrowMotion(){
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//Bボタンが押された瞬間なら砲弾発射
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B){
				return true;
			}
		}
		return false;
	}
	//Bボタンで砲弾を発射する処理
	void Player::ShellThrowMotion(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//砲弾の追加
		auto PtrTrans = GetComponent<Transform>();
		//プレイヤーの向きを得る
		auto PlayerAngle = PtrTrans->GetRotation();
		Vector3 ShellSpeed(sin(PlayerAngle.y), 0, cos(PlayerAngle.y));
		ShellSpeed *= 10.0f;
		Vector3 Velo = GetComponent<Rigidbody>()->GetVelocity();
		Velo.y = 0;
		//移動スピードを加算
		ShellSpeed += Velo;
		//打ち上げの上向きの初速度を追加（値は固定）
		ShellSpeed += Vector3(0.0f, 6.0f, 0);
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
						ShellPtr->Refresh(PtrTrans->GetPosition(), ShellSpeed);
						return;
					}
				}
			}
		}
		//ここまで来たら空きがなかったことになる
		//砲弾の追加
		auto Sh = GetStage()->AddGameObject<ShellBall>(PtrTrans->GetPosition(), ShellSpeed);
		//グループに追加
		Group->IntoGroup(Sh);
	}



	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	用途: 通常移動
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<DefaultState> DefaultState::Instance(){
		static shared_ptr<DefaultState> instance;
		if (!instance){
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void DefaultState::Enter(const shared_ptr<Player>& Obj){
		//何もしない
	}
	//ステート実行中に毎ターン呼ばれる関数
	void DefaultState::Execute(const shared_ptr<Player>& Obj){
		Obj->MoveRotationMotion();
		if (Obj->IsShellThrowMotion()){
			Obj->ShellThrowMotion();
		}
		if (Obj->IsJumpMotion()){
			//Jumpボタンでステート変更
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void DefaultState::Exit(const shared_ptr<Player>& Obj){
		//何もしない
	}


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	用途: ジャンプ状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<JumpState> JumpState::Instance(){
		static shared_ptr<JumpState> instance;
		if (!instance){
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void JumpState::Enter(const shared_ptr<Player>& Obj){
		//ジャンプ中も移動可能とする
		Obj->MoveRotationMotion();
		Obj->JumpMotion();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void JumpState::Execute(const shared_ptr<Player>& Obj){
		//ジャンプ中も移動可能とする
		Obj->MoveRotationMotion();
		//ジャンプ中も砲弾発射可能
		if (Obj->IsShellThrowMotion()){
			Obj->ShellThrowMotion();
		}
		if (Obj->JumpMoveMotion()){
			//通常状態に戻る
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void JumpState::Exit(const shared_ptr<Player>& Obj){
		//何もしない
	}


}
//endof  basedx11
