#include "stdafx.h"
#include "Project.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class SeekCamera : public GameObject;
	//	用途: 追いかけるカメラオブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekCamera::SeekCamera(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SeekCamera::~SeekCamera(){}

	//初期化
	void SeekCamera::OnCreate(){
		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(m_StartPos);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		PtrSeek->SetWeight(5.0f);
		PtrSeek->SetUpdateActive(false);

		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		PtrArrive->SetWeight(5.0f);
		PtrArrive->SetUpdateActive(false);


		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");

		//使用しているのはCameraである
		auto PtrCamera = dynamic_pointer_cast<Camera>(GetStage()->GetCamera(0));
		//LookAtCameraである
		auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (!PtrLookAtCamera && PtrCamera){
			//Cameraを使用してるならオブジェクトカメラに設定
			PtrCamera->SetCameraObject(GetThis<SeekCamera>());
		}
		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SeekCamera> >(GetThis<SeekCamera>());
		//最初のステートをSeekFarStateに設定
		m_StateMachine->SetCurrentState(SeekCameraFarState::Instance());
		//初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<SeekCamera>());
	}

	void SeekCamera::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}



	//モーションを実装する関数群

	//カメラの向かう位置を得る
	Vector3 SeekCamera::GetCameraTargetMotion(bool IsFar){
		Vector3 CameraTarget(0, 1.25f, -4.0f);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			CameraTarget = PtrPlayer->GetComponent<Transform>()->GetPosition();
			CameraTarget.y += 1.0f;

			auto Rot = PtrPlayer->GetComponent<Transform>()->GetRotation();
			Vector3 RotVec(sin(Rot.y), 0, cos(Rot.y));
			RotVec.Normalize();

			if (IsFar){
				RotVec *= -1.0f;
			}
			else{
				RotVec *= -4.0f;
			}
			CameraTarget += RotVec;

			//使用しているのはCameraである
			auto PtrCamera = dynamic_pointer_cast<Camera>(GetStage()->GetCamera(0));
			//LookAtCameraである
			auto PtrLookAtCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
			if (!PtrLookAtCamera && PtrCamera){
				//Cameraを使用してるならカメラの注目点をプレイヤーに設定
				PtrCamera->SetAt(PtrPlayer->GetComponent<Transform>()->GetPosition());
			}
		}
		return CameraTarget;
	}

	//プレイヤーとの距離を得る
	float SeekCamera::GetToPlayerLemngth(){
		Vector3 PlayerPos(0, 0.25, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
		}
		auto LenVec = PlayerPos - GetComponent<Transform>()->GetPosition();
		return Vector3EX::Length(LenVec);
	}



	void  SeekCamera::SeekStartMoton(){
		auto Target = GetCameraTargetMotion(true);
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(Target);

	}
	bool  SeekCamera::SeekUpdateMoton(){
		auto Target = GetCameraTargetMotion(true);
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(Target);
		if (GetToPlayerLemngth() <= 5.0f){
			return true;
		}
		return false;
	}
	void  SeekCamera::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekCamera::ArriveStartMoton(){
		auto Target = GetCameraTargetMotion(false);
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(Target);
	}
	bool  SeekCamera::ArriveUpdateMoton(){
		auto Target = GetCameraTargetMotion(false);
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(Target);
		if (GetToPlayerLemngth() > 5.0f){
			return true;
		}
		return false;
	}
	void  SeekCamera::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(false);
	}



	//--------------------------------------------------------------------------------------
	//	class SeekCameraFarState : public ObjState<SeekCamera>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<SeekCameraFarState> SeekCameraFarState::Instance(){
		static shared_ptr<SeekCameraFarState> instance(new SeekCameraFarState);
		return instance;
	}
	void SeekCameraFarState::Enter(const shared_ptr<SeekCamera>& Obj){
		Obj->SeekStartMoton();
	}
	void SeekCameraFarState::Execute(const shared_ptr<SeekCamera>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(SeekCameraNearState::Instance());
		}
	}
	void SeekCameraFarState::Exit(const shared_ptr<SeekCamera>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class SeekCameraNearState : public ObjState<SeekCamera>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<SeekCameraNearState> SeekCameraNearState::Instance(){
		static shared_ptr<SeekCameraNearState> instance(new SeekCameraNearState);
		return instance;
	}
	void SeekCameraNearState::Enter(const shared_ptr<SeekCamera>& Obj){
		Obj->ArriveStartMoton();
	}
	void SeekCameraNearState::Execute(const shared_ptr<SeekCamera>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(SeekCameraFarState::Instance());
		}
	}
	void SeekCameraNearState::Exit(const shared_ptr<SeekCamera>& Obj){
		Obj->ArriveEndMoton();
	}



	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject(){}

	//初期化
	void SeekObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek操舵
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive操舵
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arriveは無効にしておく
		PtrArrive->SetUpdateActive(false);

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<SeekObject>());
		//分離行動をつける
		AddComponent<SeparationSteering>(Group);
		//Obbの衝突判定をつける
		auto PtrColl = AddComponent<CollisionObb>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明処理をする
		SetAlphaActive(true);

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//最初のステートをSeekFarStateに設定
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//ユーティリティ関数群
	Vector3 SeekObject::GetPlayerPosition() const{
		//もしプレイヤーが初期化化されてない場合には、Vector3(0,m_BaseY,0)を返す
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const{
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}

	//モーションを実装する関数群
	void  SeekObject::SeekStartMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(GetPlayerPosition());

	}
	bool  SeekObject::SeekUpdateMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() <= m_StateChangeSize){
			return true;
		}
		return false;
	}
	void  SeekObject::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekObject::ArriveStartMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(GetPlayerPosition());
	}
	bool  SeekObject::ArriveUpdateMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() > m_StateChangeSize){
			//プレイヤーとの距離が一定以上ならtrue
			return true;
		}
		return false;
	}
	void  SeekObject::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		//Arriveコンポーネントを無効にする
		PtrArrive->SetUpdateActive(false);
	}

	//操作
	void SeekObject::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}
	void SeekObject::OnLastUpdate(){
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f){
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
		//常にyはm_BaseY
		auto Pos = PtrTransform->GetPosition();
		Pos.y = m_BaseY;
		PtrTransform->SetPosition(Pos);
	}
	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance(){
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->SeekStartMoton();
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance(){
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveStartMoton();
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveEndMoton();
	}



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//初期化
	void FixedBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//反発を実装する場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox(){}

	//初期化
	void MoveBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();


		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();



		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}


	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	CapsuleObject::~CapsuleObject(){}

	//初期化
	void CapsuleObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Capsuleの衝突判定をつける
		auto CollPtr = AddComponent<CollisionCapsule>();
		CollPtr->SetFixed(true);
		//コリジョンのワイアフレーム表示
		AddComponent<PNTCollisionDraw>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}


	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject(){}
	//初期化
	void SphereObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphereの衝突判定をつける
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//コリジョンのワイアフレーム表示
		AddComponent<PNTCollisionDraw>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");
	}


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	用途: 球と衝突判定するスクエア
	//--------------------------------------------------------------------------------------
	//構築と破棄
	HitTestSquare::HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos)
	{}
	HitTestSquare::~HitTestSquare(){}

	void HitTestSquare::OnCreate(){
		auto TrancePtr = AddComponent<Transform>();
		TrancePtr->SetScale(m_StartScale);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(m_StartRotation);
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(m_StartPos);
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SQUARE");


		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<HitTestSquare>());

	}

	PLANE HitTestSquare::GetPLANE() const{
		auto TrasnsPtr = GetComponent<Transform>();
		//表面上に3つの点を使ってPLANEを作成
		//1つ目の点は中心
		Vector3 Point0 = TrasnsPtr->GetPosition();
		//2つ目は-0.5,-0.5,0の点をワールド変換したもの
		Vector3 Point1(-0.5f, -0.5f, 0);
		Point1.Transform(TrasnsPtr->GetWorldMatrix());
		//3つ目は0.5,-0.5,0の点をワールド変換したもの
		Vector3 Point2(0.5f, -0.5f, 0);
		Point2.Transform(TrasnsPtr->GetWorldMatrix());
		//3点を使って面を作成
		PLANE ret(Point0, Point1, Point2);
		return ret;
	}

	COLRECT HitTestSquare::GetCOLRECT() const{
		auto TrasnsPtr = GetComponent<Transform>();
		COLRECT rect(1.0f, 1.0f, TrasnsPtr->GetWorldMatrix());
		return rect;
	}


	//点との最近接点を返す
	void HitTestSquare::ClosestPtPoint(const Vector3& Point, Vector3& Ret){
		COLRECT rect = GetCOLRECT();
		Vector3 d = Point - rect.m_Center;
		Ret = rect.m_Center;
		for (int i = 0; i < 2; i++){
			float dist = Vector3EX::Dot(d, rect.m_Rot[i]);
			if (dist > rect.m_UVec[i]){
				dist = rect.m_UVec[i];
			}
			if (dist < -rect.m_UVec[i]){
				dist = -rect.m_UVec[i];
			}
			Ret += rect.m_Rot[i] * dist;
		}
	}


	//点との距離を返す（戻り値がマイナスなら裏側）
	float HitTestSquare::GetDistPointPlane(const Vector3& Point) const{
		PLANE pl = GetPLANE();
		return (Vector3EX::Dot(Point, pl.m_Normal) - pl.m_DotValue) / Vector3EX::Dot(pl.m_Normal, pl.m_Normal);
	}

	//ヒットテストをして最近接点と、Squareの法線を返す
	bool HitTestSquare::HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal){
		PLANE pl = GetPLANE();
		Normal = pl.m_Normal;
		//四角形との最近接点を得る
		ClosestPtPoint(Sp.m_Center, RetVec);
		//最近接点が半径以下なら衝突している
		if (Vector3EX::Length(Sp.m_Center - RetVec) <= Sp.m_Radius){
			return true;
		}
		return false;
	}



}
//endof  basedx11
