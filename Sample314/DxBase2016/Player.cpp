
#include "stdafx.h"
#include "Project.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr, const wstring& XmlFileName) :
		GameObject(StagePtr),
		m_DataFileName(XmlFileName),
		m_XmlDoc(XmlFileName),
		m_MaxSpeed(40.0f),	//最高速度
		m_Decel(0.95f),	//減速値
		m_Mass(1.0f)	//質量
	{}

	Player::~Player(){
		//ルートノードをxpathで選択
		auto RootNode = m_XmlDoc.GetSelectSingleNode(L"GameData");
		if (RootNode){
			auto PlayerNode = m_XmlDoc.GetSelectSingleNode(RootNode, L"Player");
			if (!PlayerNode){
				//プレイヤーのNodeがない
				PlayerNode = m_XmlDoc.AddChildNode(RootNode, L"Player");
			}
			auto Ptr = GetComponent<Transform>();
			wstring PlayerPosStr(L"");
			auto Pos = Ptr->GetPosition();
			PlayerPosStr += Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",";
			PlayerPosStr += Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",";
			PlayerPosStr += Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed);
			//PositionというアトリビュートにPlayerPosStrを設定
			m_XmlDoc.SetAttribute(PlayerNode, L"Position", PlayerPosStr.c_str());
			//保存
			m_XmlDoc.Save(m_DataFileName, false);
		}

	}


	//初期化
	void Player::OnCreate(){

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		//プレイヤーのアトリビュートノードをxpathで選択
		auto PlayerAtt = m_XmlDoc.GetSelectSingleNode(L"GameData/Player/@Position");
		if (!PlayerAtt){
			//値がない
			//初期位置
			Ptr->SetPosition(0, 0.125f, 0);
		}
		else{
			vector<wstring> PosTokenVec;
			wstring PlayerPosStr = m_XmlDoc.GetText(PlayerAtt);
			Util::WStrToTokenVector(PosTokenVec, PlayerPosStr, L',');
			//初期位置
			Ptr->SetPosition(Vector3(PosTokenVec[0], PosTokenVec[1], PosTokenVec[2]));
		}

		/*

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		*/

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

		auto PtrTransform = GetComponent<Transform>();
		auto Coll = GetComponent<CollisionSphere>();
		auto Sp = Coll->GetSphere();
		//平面との交差を判定
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		for (auto Ptr : Group->GetGroupVector()){
			if (!Ptr.expired()){
				auto SquarePtr = dynamic_pointer_cast< HitTestSquare>(Ptr.lock());
				if (SquarePtr){
					Vector3 RetVec, Normal;
					if (SquarePtr->HitTestSphere(Sp, RetVec, Normal)){
						//ヒットしたので位置をヒット位置に修正する
						Vector3 SetPos = RetVec + -Normal * Sp.m_Radius;
						PtrTransform->SetPosition(SetPos);
						auto PtrGrav = GetComponent<Gravity>();
						//蓄積した落下の加速度を0にする
						PtrGrav->SetGravityVelocityZero();
						auto PtrRidit = GetComponent<Rigidbody>();
						//反発
						Vector3 Refalct = Vector3EX::Reflect(PtrRidit->GetVelocity(), Normal);
						PtrRidit->SetVelocity(Refalct);

					}
				}
			}
		}


		//auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		//wstring FPS(L"FPS: ");
		//FPS += Util::UintToWStr(fps);

		//wstring str = FPS;
		////文字列をつける
		//auto PtrString = GetComponent<StringSprite>();
		//PtrString->SetText(str);

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";


		auto Pos = GetComponent<Transform>()->GetWorldMatrix().PosInMatrix();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring GravStr(L"Gravity:\t");
		auto Grav = GetComponent<Gravity>()->GetGravity();
		GravStr += L"X=" + Util::FloatToWStr(Grav.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(Grav.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(Grav.z, 6, Util::FloatModify::Fixed) + L"\n";


		wstring GravityStr(L"GravityVelocity:\t");
		auto GravityVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravityStr += L"X=" + Util::FloatToWStr(GravityVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Y=" + Util::FloatToWStr(GravityVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Z=" + Util::FloatToWStr(GravityVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring InvGravityStr(L"InvGravity: ");
		auto InvGravity = GetComponent<Gravity>()->GetInvGravity();
		InvGravityStr += L"X=" + Util::FloatToWStr(InvGravity.x, 6, Util::FloatModify::Fixed) + L", ";
		InvGravityStr += L"Y=" + Util::FloatToWStr(InvGravity.y, 6, Util::FloatModify::Fixed) + L", ";
		InvGravityStr += L"Z=" + Util::FloatToWStr(InvGravity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring OnObjectStr(L"OnObject: ");
		auto OnObject = GetComponent<Gravity>()->GetOnObject();
		if (OnObject){
			OnObjectStr += Util::UintToWStr((UINT)OnObject.get()) + L"\n";
		}
		else{
			OnObjectStr += L"NULL\n";
		}
		wstring statestr = L"JUMP: ";
		if (m_StateMachine->GetCurrentState() == DefaultState::Instance()){
			statestr = L"DEFAULT\n";
		}

		wstring str = FPS + PositionStr + RididStr + GravStr + GravityStr + InvGravityStr + OnObjectStr + statestr;
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
