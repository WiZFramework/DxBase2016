/*!
@file Rigidbody.cpp
@brief Gravity,Rigidbody,Collision,SteeringComponentとその派生クラス

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Gravity::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Gravity::Impl{
		Vector3 m_Gravity;				//重力加速度
		Vector3 m_InvGravity;			//打ち消す重力加速度
		Vector3 m_GravityVelocity;		//重力加速度による現在の速度
		float m_BaseY;	//最下落下地点
		float m_RayUnderSize;	//レイを下部に打ち込む長さ
		weak_ptr<GameObject> m_SecondParent;	//第2の親
	public:
		Impl() :
			m_Gravity(0, -9.8f, 0),
			m_InvGravity(0,0,0),
			m_GravityVelocity(0, 0, 0),
			m_BaseY(0),
			m_RayUnderSize(1.25f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Gravity : public Component ;
	//	用途: 重力
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Gravity::Gravity(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Gravity::~Gravity(){}

	//アクセサ

	const Vector3& Gravity::GetGravity() const{
		return pImpl->m_Gravity;
	}
	void Gravity::SetGravity(const Vector3& gravity){
		pImpl->m_Gravity = gravity;
	}
	void Gravity::SetGravity(float x, float y, float z){
		pImpl->m_Gravity = Vector3(x, y, z);
	}

	const Vector3& Gravity::GetInvGravity() const{
		return pImpl->m_InvGravity;
	}
	void Gravity::SetInvGravity(const Vector3& InvGravity){
		pImpl->m_InvGravity = InvGravity;
	}
	void Gravity::SetInvGravity(float x, float y, float z){
		pImpl->m_InvGravity = Vector3(x, y, z);
	}

	void Gravity::SetInvGravityDefault(){
		pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
	}

	const Vector3& Gravity::GetGravityVelocity() const{
		return pImpl->m_GravityVelocity;
	}

	void Gravity::SetGravityVelocity(const Vector3& GravityVelocity){
		pImpl->m_GravityVelocity = GravityVelocity;
	}
	void Gravity::SetGravityVelocity(float x, float y, float z){
		pImpl->m_GravityVelocity = Vector3(x, y, z);
	}

	bool Gravity::IsGravityVelocityZero(){
		return (pImpl->m_GravityVelocity == Vector3(0, 0, 0));
	}
	void Gravity::SetGravityVelocityZero(){
		pImpl->m_GravityVelocity.Zero();
	}
	float Gravity::GetBaseY() const{
		return pImpl->m_BaseY;
	}
	void Gravity::SetBaseY(float y){
		pImpl->m_BaseY = y;
	}

	float Gravity::GetRayUnderSize() const{
		return pImpl->m_RayUnderSize;
	}
	void Gravity::SetRayUnderSize(float f){
		pImpl->m_RayUnderSize = f;
	}
	shared_ptr<GameObject> Gravity::GetOnObject() const{
		return GetGameObject()->GetComponent<Transform>()->GetParent();
	}
	void Gravity::SetOnObject(const shared_ptr<GameObject>& OnObject){
		if (IsGameObjectActive()){
			if (!OnObject){
				throw BaseException(
					L"無効なオブジェクトはOnObjectに設定できません。",
					L"if (!OnObject)",
					L"Gravity::SetOnObject()"
					);
			}
			GetGameObject()->GetComponent<Transform>()->SetParent(OnObject);
		}
	}

	void Gravity::ClearOnObject(){
		if (IsGameObjectActive() && GetOnObject()){
			GetGameObject()->GetComponent<Transform>()->ClearParent();
		}
	}

	void Gravity::CheckBaseY(){
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				Vector3 Pos = PtrT->GetPosition();
				if (pImpl->m_Gravity.y <= 0){
					if (Pos.y <= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
					}
				}
				else{
					if (Pos.y >= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						pImpl->m_InvGravity = pImpl->m_Gravity * -1.0f;
					}
				}
				//位置を設定
				PtrT->SetPosition(Pos);
			}
		}
	}

	void Gravity::StartJump(const Vector3& StartVec,float EscapeSpan){
		//反発0
		SetInvGravity(0, 0, 0);
		SetGravityVelocity(StartVec);
		//ジャンプして親オブジェクトボリュームからだ出できないとき対応
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		Vector3 EscapeVec = StartVec;
		EscapeVec *= EscapeSpan;
		Pos += EscapeVec;
		PtrT->SetPosition(Pos);
	}
	void Gravity::StartJump(float x, float y, float z,float EscapeSpan){
		StartJump(Vector3(x, y, z), EscapeSpan);
	}


	//操作
	void Gravity::OnUpdate(){
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				Vector3 Pos = PtrT->GetPosition();
				//自由落下加速度を計算
				pImpl->m_GravityVelocity += pImpl->m_Gravity * ElapsedTime;
				//打消しの加速度
				pImpl->m_GravityVelocity += pImpl->m_InvGravity * ElapsedTime;
				Pos += pImpl->m_GravityVelocity * ElapsedTime;
				if (pImpl->m_Gravity.y <= 0){
					if (Pos.y <= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						SetInvGravityDefault();
					}
				}
				else{
					if (Pos.y >= pImpl->m_BaseY){
						Pos.y = pImpl->m_BaseY;
						SetGravityVelocityZero();
						SetInvGravityDefault();
					}
				}
				//位置を設定
				PtrT->SetPosition(Pos);
			}
		}
	}


	//ヒットしたオブジェクトを調べ、それが下部にあったらOnObjectに設定する（その上に乗る）
	void Gravity::HitObjectToOnObject(){
		auto PtrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (!PtrCollision){
			//Collisionがないので処理は行わない
			return;
		}
		//各コンポーネントの取得
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//ヒットしたオブジェクトを取得
		auto HitGameObjectPtr = PtrCollision->GetHitObject();
		if (HitGameObjectPtr){
			if (CheckOnObject(HitGameObjectPtr)){
				if (HitGameObjectPtr != GetOnObject()){
					if (GetOnObject()){
						ClearOnObject();
					}
					SetOnObject(HitGameObjectPtr);
					Vector3 InvGravity(GetGravity() * 1.0f);
					auto ParMat = GetOnObject()->GetComponent<Transform>()->GetWorldMatrix();
					Quaternion Qt = ParMat.QtInMatrix();
					ParMat.Identity();
					ParMat.RotationQuaternion(Qt);
					Vector4 v;
					ParMat.Inverse(&v);
					InvGravity.Transform(ParMat);
					InvGravity.y = -GetGravity().y;
					SetInvGravity(InvGravity);
					SetGravityVelocityZero();
				}
				//ヒットオブジェクトはクリア
				PtrCollision->SetHitObject(nullptr);
			}
			else{
				ClearOnObject();
				SetInvGravity(0, 0, 0);
			}
		}
		else{
			if (!CheckOnObject(GetOnObject())){
				ClearOnObject();
				SetInvGravity(0, 0, 0);
			}
			else{
				//継続して乗っている
				Vector3 InvGravity(GetGravity() * 1.0f);
				auto ParMat = GetOnObject()->GetComponent<Transform>()->GetWorldMatrix();
				Quaternion Qt = ParMat.QtInMatrix();
				ParMat.Identity();
				ParMat.RotationQuaternion(Qt);
				Vector4 v;
				ParMat.Inverse(&v);
				InvGravity.Transform(ParMat);
				InvGravity.y = -GetGravity().y;
				SetInvGravity(InvGravity);
				if (InvGravity.x == 0 && InvGravity.z == 0){
					SetGravityVelocityZero();
				}
			}
		}
	}




	//onオブジェクトが現在も有効かどうかをチェックする
	bool Gravity::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		if (!OnObject){
			return false;
		}
		//Collisionに処理を任せる
		auto PtrCollision = GetGameObject()->GetComponent<Collision>(false);
		if (PtrCollision){
			return PtrCollision->CheckOnObjectBase(OnObject);
		}
		return false;
	}

	//GravityのUpdate2はオブジェクトに乗る処理を行う
	void Gravity::Update2(){
		if (IsGameObjectActive()){
			//ヒットしているオブジェクトがあれば
			//それが下にあるOBBならonObjectに設定
			HitObjectToOnObject();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SteeringComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SteeringComponent::Impl{
		float m_Weight;
		Impl() :
			m_Weight(1.0f)
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class SteeringComponent : public Component ;
	//	用途: 操舵コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	SteeringComponent::SteeringComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	SteeringComponent::~SteeringComponent(){}

	float SteeringComponent::GetWeight() const{ return pImpl->m_Weight; }
	void SteeringComponent::SetWeight(float f){ pImpl->m_Weight = f; }




	//--------------------------------------------------------------------------------------
	//	struct SeekSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SeekSteering::Impl{
		Vector3 m_TargetPosition;
		Impl() :
			m_TargetPosition(0, 0, 0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SeekSteering : public SteeringComponent;
	//	用途: Seek操舵
	//--------------------------------------------------------------------------------------
	SeekSteering::SeekSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				//所持オブジェクトがあればその位置に初期化
				pImpl->m_TargetPosition = Vector3(0,0,0);
//				pImpl->m_TargetPosition = PtrT->GetPosition();
			}
		}
	}
	SeekSteering::~SeekSteering(){}

	//アクセサ

	const Vector3& SeekSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void SeekSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void SeekSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x,y,z);
	}
	//操作
	void SeekSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Seek(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
					TransPtr->GetPosition(), RigidPtr->GetMaxSpeed()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ArriveSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ArriveSteering::Impl{
		Vector3 m_TargetPosition;
		float m_Decl;
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_Decl(3.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ArriveSteering : public SteeringComponent;
	//	用途: Arrive操舵
	//--------------------------------------------------------------------------------------
	ArriveSteering::ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ArriveSteering::~ArriveSteering(){}

	//アクセサ
	const Vector3& ArriveSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void ArriveSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void ArriveSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	float ArriveSteering::GetDecl() const{ return pImpl->m_Decl; }
	void ArriveSteering::SetDecl(float f){ pImpl->m_Decl = f; }


	//操作
	void ArriveSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Arrive(RigidPtr->GetVelocity(), pImpl->m_TargetPosition,
					TransPtr->GetPosition(), RigidPtr->GetMaxSpeed(), pImpl->m_Decl) *  GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PursuitSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct PursuitSteering::Impl{
		Vector3 m_TargetPosition;	//目標の位置
		Quaternion m_TargetQuaternion;		//目標の向き
		Vector3 m_TargetVelocity;	//目標の速度
		Impl() :
			m_TargetPosition(0, 0, 0),
			m_TargetQuaternion(),
			m_TargetVelocity(0, 0, 0)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class PursuitSteering : public SteeringComponent;
	//	用途: Pursuit操舵
	//--------------------------------------------------------------------------------------
	PursuitSteering::PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	PursuitSteering::~PursuitSteering(){}

	//アクセサ
	const Vector3& PursuitSteering::GetTargetPosition() const{
		return pImpl->m_TargetPosition;
	}
	void PursuitSteering::SetTargetPosition(const Vector3& Vec){
		pImpl->m_TargetPosition = Vec;
	}
	void PursuitSteering::SetTargetPosition(float x, float y, float z){
		pImpl->m_TargetPosition = Vector3(x, y, z);
	}

	const Quaternion& PursuitSteering::GetTargetQuaternion() const{
		return pImpl->m_TargetQuaternion;
	}
	void PursuitSteering::SetTargetQuaternion(const Quaternion& Qt){
		pImpl->m_TargetQuaternion = Qt;
	}
	void PursuitSteering::SetTargetRotation(const Vector3& rotation){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(rotation);
	}
	void PursuitSteering::SetTargetRotation(float x, float y, float z){
		SetTargetRotation(Vector3(x, y, z));
	}
	const Vector3& PursuitSteering::GetTargetVelocity() const{
		return pImpl->m_TargetVelocity;
	}
	void PursuitSteering::SetTargetVelocity(const Vector3& velocity){
		pImpl->m_TargetVelocity = velocity;
	}
	void PursuitSteering::SetTargetVelocity(float x, float y, float z){
		SetTargetVelocity(Vector3(x, y, z));
	}

	//操作
	void PursuitSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Pursuit(RigidPtr->GetVelocity(), TransPtr->GetPosition(), TransPtr->GetRotation(), RigidPtr->GetMaxSpeed(),
					pImpl->m_TargetVelocity, pImpl->m_TargetPosition, pImpl->m_TargetQuaternion.GetRotation()) *  GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WanderSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct WanderSteering::Impl{
		Vector3 m_WanderTarget;	//徘徊目標の回転係数（操舵によって書き換えられる）
		float m_WanderRadius;	//徘徊半径
		float m_WanderDistance;	//徘徊円までの距離
		float m_WanderJitter;	//ランダム変異の最大値
		Impl() :
			m_WanderTarget(0,0,0),
			m_WanderRadius(1.5f),
			m_WanderDistance(1.0f),
			m_WanderJitter(0.5f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class WanderSteering : public SteeringComponent;
	//	用途: Wander操舵
	//--------------------------------------------------------------------------------------
	WanderSteering::WanderSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WanderSteering::~WanderSteering(){}
	//アクセサ
	const Vector3& WanderSteering::GetWanderTarget() const{
		return pImpl->m_WanderTarget;
	}
	void WanderSteering::SetWanderTarget(const Vector3& target){
		pImpl->m_WanderTarget = target;
	}
	float WanderSteering::GetWanderRadius() const{
		return pImpl->m_WanderRadius;
	}
	void WanderSteering::SetWanderRadius(float f){
		pImpl->m_WanderRadius = f;
	}
	float WanderSteering::GetWanderDistance() const{
		return pImpl->m_WanderDistance;
	}
	void WanderSteering::SetWanderDistance(float f){
		pImpl->m_WanderDistance = f;
	}
	float WanderSteering::GetWanderJitter() const{
		return pImpl->m_WanderJitter;
	}
	void WanderSteering::SetWanderJitter(float f){
		pImpl->m_WanderJitter = f;
	}

	void WanderSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Wander(TransPtr->GetWorldMatrix(),
					pImpl->m_WanderRadius, pImpl->m_WanderDistance, pImpl->m_WanderJitter, pImpl->m_WanderTarget) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct WallAvoidanceSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct WallAvoidanceSteering::Impl{
		vector<PLANE> m_PlaneVec;//回避すべき面の配列
		bool m_WallArrived;	//壁と衝突したか
		Impl():
			m_WallArrived(false)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class WallAvoidanceSteering : public SteeringComponent;
	//	用途: WallAvoidance操舵(壁回避)
	//--------------------------------------------------------------------------------------
	WallAvoidanceSteering::WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	WallAvoidanceSteering::~WallAvoidanceSteering(){}

	//アクセサ
	vector<PLANE>& WallAvoidanceSteering::GetPlaneVec() const{
		return pImpl->m_PlaneVec;
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<PLANE>& planevec){
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec){
			pImpl->m_PlaneVec.push_back(v);
		}
	}
	void WallAvoidanceSteering::SetPlaneVec(const vector<Plane>& planevec){
		//必ずクリアする
		pImpl->m_PlaneVec.clear();
		for (auto v : planevec){
			PLANE p(v);
			pImpl->m_PlaneVec.push_back(p);
		}
	}


	//壁と衝突しているか
	bool WallAvoidanceSteering::IsWallArribed() const{
		return pImpl->m_WallArrived;
	}


	//操作
	void WallAvoidanceSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			pImpl->m_WallArrived = false;
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0,0,0);
				WorkForce = Steering::WallAvoidance(TransPtr->GetWorldMatrix(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(), pImpl->m_PlaneVec) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
				if (WorkForce.Length() > 0.0f){
					//壁に衝突している
					pImpl->m_WallArrived = true;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ObstacleAvoidanceSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ObstacleAvoidanceSteering::Impl{
		vector<SPHERE> m_ObstacleSphereVec;		//回避すべき障害物のSPHERE配列
		float m_RoadWidth;
		float m_RoadHeight;
		Impl():
			m_RoadWidth(0.5f),
			m_RoadHeight(0.5f)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class ObstacleAvoidanceSteering : public SteeringComponent;
	//	用途: ObstacleAvoidance操舵(障害物回避)
	//--------------------------------------------------------------------------------------
	ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ObstacleAvoidanceSteering::~ObstacleAvoidanceSteering(){}
	//アクセサ
	//障害物の配列
	const vector<SPHERE>& ObstacleAvoidanceSteering::GetObstacleSphereVec() const{
		return pImpl->m_ObstacleSphereVec;
	}
	void ObstacleAvoidanceSteering::SetObstacleSphereVec(const vector<SPHERE>& spherevec){
		pImpl->m_ObstacleSphereVec.clear();
		for (auto sp : spherevec){
			pImpl->m_ObstacleSphereVec.push_back(sp);
		}
	}

	//道の高さ
	float ObstacleAvoidanceSteering::GetRoadWidth() const{
		return pImpl->m_RoadWidth;
	}
	void ObstacleAvoidanceSteering::SetRoadWidth(float f){
		pImpl->m_RoadWidth = f;
	}
	//道幅
	float ObstacleAvoidanceSteering::GetRoadHeight() const{
		return pImpl->m_RoadHeight;
	}
	void ObstacleAvoidanceSteering::SetRoadHeight(float f){
		pImpl->m_RoadHeight = f;
	}


	void ObstacleAvoidanceSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0, 0, 0);
				WorkForce = Steering::ObstacleAvoidance(TransPtr->GetWorldMatrix(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed(),pImpl->m_RoadWidth,pImpl->m_RoadHeight,
					pImpl->m_ObstacleSphereVec) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct FollowPathSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct FollowPathSteering::Impl{
		Path m_Path;	//経路をあらわすパス
		float m_Decl;	//減速値
		float m_WaypointSpan;	//経路の中心からの距離
		Impl():
			m_Decl(3.0f),
			m_WaypointSpan(2.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class FollowPathSteering : public SteeringComponent;
	//	用途: FollowPath操舵(経路追従)
	//--------------------------------------------------------------------------------------
	FollowPathSteering::FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	FollowPathSteering::~FollowPathSteering(){}

	//アクセサ
	void FollowPathSteering::SetPathList(const list<Vector3>& pathlist){
		pImpl->m_Path.SetList(pathlist);
	}

	float FollowPathSteering::GetDecl() const{
		return pImpl->m_Decl;
	}
	void FollowPathSteering::SetDecl(float f){
		pImpl->m_Decl = f;
	}

	float FollowPathSteering::GetWaypointSpan() const{
		return pImpl->m_WaypointSpan;
	}
	void FollowPathSteering::SetWaypointSpan(float f){
		pImpl->m_WaypointSpan = f;
	}

	bool FollowPathSteering::GetLooped() const{
		return pImpl->m_Path.GetLooped();
	}
	bool FollowPathSteering::IsLooped() const{
		return pImpl->m_Path.GetLooped();
	}
	void FollowPathSteering::SetLooped(bool b){
		pImpl->m_Path.SetLooped(b);
	}

	bool FollowPathSteering::IsFinished() const{
		return pImpl->m_Path.IsFinished();
	}

	//操作
	void FollowPathSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (RigidPtr && TransPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce(0, 0, 0);
				WorkForce = Steering::FollowPath(pImpl->m_Path,
					pImpl->m_WaypointSpan,
					TransPtr->GetPosition(),
					RigidPtr->GetVelocity(),
					RigidPtr->GetMaxSpeed(),
					pImpl->m_Decl) 
					* GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}

	}

	//--------------------------------------------------------------------------------------
	//	struct AlignmentSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct AlignmentSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class AlignmentSteering : public SteeringComponent;
	//	用途: Alignment操舵(整列)
	//--------------------------------------------------------------------------------------
	AlignmentSteering::AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	AlignmentSteering::~AlignmentSteering(){}

	//アクセサ
	shared_ptr<GameObjectGroup> AlignmentSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"AlignmentSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void AlignmentSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}
	//操作
	void AlignmentSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Alignment(GetGameObjectGroup(), GetGameObject()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct CohesionSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CohesionSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class CohesionSteering : public SteeringComponent;
	//	用途: Cohesion操舵(結合)
	//--------------------------------------------------------------------------------------
	CohesionSteering::CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group):
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	CohesionSteering::~CohesionSteering(){}

	//アクセサ
	shared_ptr<GameObjectGroup> CohesionSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"CohesionSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void CohesionSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}

	//操作
	void CohesionSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Cohesion(GetGameObjectGroup(), GetGameObject(),
					RigidPtr->GetVelocity(), RigidPtr->GetMaxSpeed()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SeparationSteering::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SeparationSteering::Impl{
		weak_ptr<GameObjectGroup> m_Group;
		Impl(const shared_ptr<GameObjectGroup>& Group) :
			m_Group(Group)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SeparationSteering : public SteeringComponent;
	//	用途: Separation操舵
	//--------------------------------------------------------------------------------------
	SeparationSteering::SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<GameObjectGroup>& Group) :
		SteeringComponent(GameObjectPtr),
		pImpl(new Impl(Group))
	{}
	SeparationSteering::~SeparationSteering(){}
	//アクセサ
	shared_ptr<GameObjectGroup> SeparationSteering::GetGameObjectGroup() const{
		if (pImpl->m_Group.expired()){
			throw BaseException(
				L"グループは無効です",
				L"グループの存在を確認して下さい",
				L"SeparationSteering::GetGameObjectGroup()"
				);
		}
		return pImpl->m_Group.lock();
	}
	void SeparationSteering::SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_Group = Group;
	}

	void SeparationSteering::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto RigidPtr = GetGameObject()->GetComponent<Rigidbody>();
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && RigidPtr){
				Vector3 Force = RigidPtr->GetForce();
				Vector3 WorkForce;
				WorkForce = Steering::Separation(GetGameObjectGroup(), GetGameObject()) * GetWeight();
				Steering::AccumulateForce(Force, WorkForce, RigidPtr->GetMaxForce());
				RigidPtr->SetForce(Force);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct Rigidbody::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Rigidbody::Impl{
		Vector3 m_Velocity;				//速度
		bool m_VelocityZeroCommand;		//Transフォームが変化がないときに、速度を0にするかどうか
		Vector3 m_HitVelocity;		//衝突した瞬間の速度
		float m_MaxSpeed;				//最高速度
		Vector3 m_MinVelocity;			//最低速度（XYZを指定できる）
		Vector3 m_AngularVelocity;		//回転

		float m_Mass;					//質量（キログラム）
		float m_Reflection;						//反発係数
		Vector3 m_Force;				//現在のフォース（子コンポーネントで変更される）
		float m_Friction;				//摩擦係数（Forceの逆向きに働く必ず0から-1.0の間）
		float m_MaxForce;				//最高フォース
		Impl() :
			m_Velocity(0, 0, 0),
			m_VelocityZeroCommand(false),
			m_HitVelocity(0,0,0),
			m_MaxSpeed(10.0f),
			m_MinVelocity(0.01f,0.01f,0.01f),
			m_AngularVelocity(0, 0, 0),
			m_Mass(1.0f),
			m_Reflection(1.0f),
			m_Force(0, 0, 0),
			m_Friction(0),
			m_MaxForce(30.0f)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	用途: 自動位置コントロール
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Rigidbody::Rigidbody(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Rigidbody::~Rigidbody(){}

	//アクセサ
	
	bool Rigidbody::IsVelocityZeroCommand() const{
		return pImpl->m_VelocityZeroCommand;
	}
	
	void Rigidbody::SetVelocityZeroCommand(bool b){
		pImpl->m_VelocityZeroCommand = b;
	}


	const Vector3& Rigidbody::GetVelocity() const{ return pImpl->m_Velocity; }
	void Rigidbody::SetVelocity(const Vector3& Velocity){ 
		pImpl->m_Velocity = Velocity; 
		pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
		if (pImpl->m_Velocity.Length() < pImpl->m_MinVelocity.Length()){
			pImpl->m_Velocity.Zero();
		}
	}
	void Rigidbody::SetVelocity(float x, float y, float z){ 
		SetVelocity(Vector3(x, y, z));
	}

	const Vector3& Rigidbody::GetHitVelocity() const{ return pImpl->m_HitVelocity; }
	void Rigidbody::SetHitVelocity(const Vector3& Velocity){ pImpl->m_HitVelocity = Velocity; }
	void Rigidbody::SetHitVelocity(float x, float y, float z){ pImpl->m_HitVelocity = Vector3(x, y, z); }



	float Rigidbody::GetMaxSpeed() const{ return pImpl->m_MaxSpeed; }
	void Rigidbody::SetMaxSpeed(float f){ pImpl->m_MaxSpeed = f; }

	const Vector3& Rigidbody::GetMinVelocity() const{
		return pImpl->m_MinVelocity;
	}
	void Rigidbody::SetMinVelocity(const Vector3& Velocity){
		pImpl->m_MinVelocity = Velocity;
	}
	void Rigidbody::SetMinVelocity(float x, float y, float z){
		SetMinVelocity(Vector3(x,y,z));
	}


	const Vector3& Rigidbody::GetAngularVelocity() const{ return pImpl->m_AngularVelocity; }
	void Rigidbody::SetAngularVelocity(const Vector3& AngularVelocity){ pImpl->m_AngularVelocity = AngularVelocity; }
	void Rigidbody::SetAngularVelocity(float x, float y, float z){ pImpl->m_AngularVelocity = Vector3(x, y, z); }

	float Rigidbody::GetMass() const{ return pImpl->m_Mass; }
	void Rigidbody::SetMass(float f){ pImpl->m_Mass = f; }

	float Rigidbody::GetReflection() const{
		return pImpl->m_Reflection;
	}
	void Rigidbody::SetReflection(float f){
		pImpl->m_Reflection = f;
	}


	const Vector3& Rigidbody::GetForce() const{ return pImpl->m_Force; }
	void Rigidbody::SetForce(const Vector3& Force){ pImpl->m_Force = Force; }
	void Rigidbody::SetForce(float x, float y, float z){ pImpl->m_Force = Vector3(x, y, z); }

	float Rigidbody::GetFriction() const{
		return pImpl->m_Friction;
	}
	void Rigidbody::SetFriction(float f){
		if (f >= 0){
			pImpl->m_Friction = 0.0f;
		}
		else if (f <= -1.0f){
			pImpl->m_Friction = -1.0f;
		}
		else{
			pImpl->m_Friction = f;
		}
	}


	float Rigidbody::GetMaxForce() const{ return pImpl->m_MaxForce; }
	void Rigidbody::SetMaxForce(float f){ pImpl->m_MaxForce = f; }

	void Rigidbody::OnUpdate(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			if (PtrT){
				//前回のターンからの時間
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				//フォースが変更されていたら
				if (pImpl->m_Force.Length() > 0){
					//フォースが0以上なら
					//質量を計算して加速を求める
					Vector3 Accel = pImpl->m_Force / pImpl->m_Mass;
					pImpl->m_Velocity += Accel * ElapsedTime;
				}
				if (pImpl->m_Velocity.Length() >= pImpl->m_MaxSpeed){
					pImpl->m_Velocity.ClampLength(0, pImpl->m_MaxSpeed);
				}
				//減速値を求める
				auto  Friction = pImpl->m_Mass * pImpl->m_Friction;
				if (Friction <= -1.0f){
					Friction = -1.0f;
				}
				else if (Friction >= 0.0f){
					Friction = 0.0f;
				}
				Vector3 VFriction = pImpl->m_Velocity * Friction;
				pImpl->m_Velocity += VFriction;
				if (abs(pImpl->m_Velocity.x) < pImpl->m_MinVelocity.x){
					pImpl->m_Velocity.x = 0;
				}
				if (abs(pImpl->m_Velocity.y) < pImpl->m_MinVelocity.y){
					pImpl->m_Velocity.y = 0;
				}
				if (abs(pImpl->m_Velocity.z) < pImpl->m_MinVelocity.z){
					pImpl->m_Velocity.z = 0;
				}
				Vector3 Pos = PtrT->GetPosition();
				Pos += pImpl->m_Velocity * ElapsedTime;
				PtrT->SetPosition(Pos);
				Quaternion Qt = PtrT->GetQuaternion();
				Quaternion QtSpan;
				QtSpan.RotationRollPitchYawFromVector(pImpl->m_AngularVelocity * ElapsedTime);
				Qt *= QtSpan;
				Qt.Normalize();
				PtrT->SetQuaternion(Qt);
				//速度をバックアップする
				SetHitVelocity(pImpl->m_Velocity);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Collision::Impl{
		bool m_Fixed;		//静止オブジェクトかどうか
		bool m_SendEventActive;	//衝突したときにSendイベントを発生するかどうか
		bool m_PostEventActive;	//衝突したときにPostイベントを発生するかどうか
		float m_PostDispatchTime;		//Postを発行する場合の遅延時間
		wstring m_EventString;	//衝突したときのイベントキー
		bool m_AutoAfterCollision;	//自動後処理を行うかどうか
		bool m_NewDestOnly;	//新しい衝突のみ後処理する
		weak_ptr<GameObject> m_BeforeHitObject;		//一つ前に衝突した相手
		weak_ptr<GameObject> m_HitObject;		//衝突した相手
		float m_HitTime;	//ヒットしたときの前ターンからの経過時間
		weak_ptr<GameObject> m_SubDispatchObject;	//衝突した事を知らせるサブオブジェクト
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース

		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//判定から除外するグループ



		float m_EscapeSpanMin;
		float m_EscapeAlignPlus;
		Impl() :
			m_Fixed(false),
			m_SendEventActive(false),
			m_PostEventActive(false),
			m_PostDispatchTime(0),
			m_EventString(L"CollisionEvent"),
			m_AutoAfterCollision(true),
			m_NewDestOnly(true),
			m_HitTime(0),
			m_EscapeSpanMin(0.15f),
			m_EscapeAlignPlus(1.2f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	用途: 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Collision::Collision(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Collision::~Collision(){}

	//アクセサ
	shared_ptr<GameObject> Collision::GetHitObject() const{
		if (pImpl->m_HitObject.expired()){
			return nullptr;
		}
		return pImpl->m_HitObject.lock();
	}
	void Collision::SetHitObject(const shared_ptr<GameObject>& Ptr){
		pImpl->m_HitObject = Ptr;
	}

	void Collision::ClearBothHitObject(){
		auto Other = GetHitObject();
		if (Other){
			auto OtherColl = Other->GetComponent<Collision>(false);
			if (OtherColl){
				OtherColl->SetHitObject(nullptr);
			}
		}
		SetHitObject(nullptr);
	}


	void Collision::ResetHitObject(){
		pImpl->m_BeforeHitObject = pImpl->m_HitObject;
		pImpl->m_HitObject.reset();
	}

	shared_ptr<GameObject> Collision::GetSubDispatchObject() const{
		if (pImpl->m_SubDispatchObject.expired()){
			return nullptr;
		}
		return pImpl->m_SubDispatchObject.lock();
	}
	void Collision::SetSubDispatchObject(const shared_ptr<GameObject>& Ptr){
		pImpl->m_SubDispatchObject = Ptr;
	}


	bool Collision::IsNewDest() const{
		if (pImpl->m_HitObject.expired()){
			//新しいヒットがなければfalse
			return false;
		}
		else{
			//新しいヒットがあった
			if (pImpl->m_BeforeHitObject.expired()){
				//一つ前がヒットしてない
				return true;
			}
			if (pImpl->m_BeforeHitObject.lock() == pImpl->m_HitObject.lock()){
				//一つ前と同じ
				return false;
			}
			else{
				//一つ前と違う
				return true;
			}
		}
	}


	bool Collision::NeedAfterCollision() const{
		if (pImpl->m_AutoAfterCollision){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}

	bool Collision::NeedSendEvent() const{
		if (pImpl->m_SendEventActive){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}

	bool Collision::NeedPostEvent() const{
		if (pImpl->m_PostEventActive){
			if (pImpl->m_NewDestOnly){
				return IsNewDest();
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}


	shared_ptr<MeshResource> Collision::GetMeshResource() const{
		if (pImpl->m_MeshResource.expired()){
			return nullptr;
		}
		return pImpl->m_MeshResource.lock();
	}
	void Collision::SetMeshResource(const shared_ptr<MeshResource>& Ptr){
		pImpl->m_MeshResource = Ptr;
	}

	bool Collision::GetFixed() const{
		return pImpl->m_Fixed;
	}
	bool Collision::IsFixed() const{
		return pImpl->m_Fixed;
	}
	void Collision::SetFixed(bool b){
		pImpl->m_Fixed = b;
	}

	bool Collision::GetSendEventActive() const{
		return pImpl->m_SendEventActive;
	}
	bool Collision::IsSendEventActive() const{
		return pImpl->m_SendEventActive;
	}
	void Collision::SetSendEventActive(bool b){
		pImpl->m_SendEventActive = b;
	}

	bool Collision::GetPostEventActive() const{
		return pImpl->m_PostEventActive;
	}
	bool Collision::IsPostEventActive() const{
		return pImpl->m_PostEventActive;
	}
	void Collision::SetPostEventActive(bool b){
		pImpl->m_PostEventActive = b;
	}

	float Collision::GetPostDispatchTime() const{
		return pImpl->m_PostDispatchTime;
	}
	void Collision::SetPostDispatchTime(float f){
		if (f <= 0.0f){
			f = 0.0f;
		}
		pImpl->m_PostDispatchTime = f;
	}
	wstring Collision::GetEventString() const{
		return pImpl->m_EventString;
	}
	void Collision::SetEventString(const wstring& str){
		if (str == L""){
			throw BaseException(
				L"イベントストリングが空白です",
				L"文字列を確認して下さい",
				L"Collision::SetEventString()"
				);
		}
		pImpl->m_EventString = str;
	}


	bool Collision::GetAutoAfterCollision() const{
		return pImpl->m_AutoAfterCollision;
	}
	bool Collision::IsAutoAfterCollision() const{
		return pImpl->m_AutoAfterCollision;
	}
	void Collision::SetAutoAfterCollision(bool b){
		pImpl->m_AutoAfterCollision = b;
	}

	bool Collision::GetNewDestOnly() const{
		return pImpl->m_NewDestOnly;
	}
	bool Collision::IsNewDestOnly() const{
		return pImpl->m_NewDestOnly;
	}
	void Collision::SetNewDestOnly(bool b){
		pImpl->m_NewDestOnly = b;
	}

	//抜け出し処理の係数
	float Collision::GetEscapeSpanMin() const{
		return pImpl->m_EscapeSpanMin;
	}
	void Collision::SetEscapeSpanMin(float f){
		pImpl->m_EscapeSpanMin = f;
	}

	float Collision::GetEscapeAlignPlus() const{
		return pImpl->m_EscapeAlignPlus;
	}
	void Collision::SetEscapeAlignPlus(float f){
		pImpl->m_EscapeAlignPlus = f;
	}

	float Collision::GetHitTime() const{
		return pImpl->m_HitTime;
	}
	void Collision::SetHitTime(float f){
		pImpl->m_HitTime = f;
	}


	//判定から除外するオブジェクトグループの設定
	void Collision::SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group){
		pImpl->m_ExcludeCollisionGroup = Group;
	}

	//除外グループに属しているかを調べる
	bool Collision::ChkExcludeCollisionObject(const shared_ptr<GameObject>&  CheckGameObject){
		//除外グループがあるかどうか
		if (!pImpl->m_ExcludeCollisionGroup.expired()){
			auto Group = pImpl->m_ExcludeCollisionGroup.lock();
			//除外グループの配列を得る
			auto GroupVec = Group->GetGroupVector();
			for (auto ExcludePtr : GroupVec){
				if (!ExcludePtr.expired()){
					//グループに属する各オブジェクトを得る
					auto ExcludeObj = ExcludePtr.lock();
					if (ExcludeObj != GetGameObject()){
						//そのオブジェクトが自分自身以外
						if (ExcludeObj == CheckGameObject){
							return true;
						}
					}
				}
			}
		}
		return false;
	}


	bool Collision::CheckOnObjectBase(const shared_ptr<GameObject>& OnObject){
		//仮想関数呼び出し
		return CheckOnObject(OnObject);
	}


	//操作
	void Collision::OnUpdate(){
		//Collisionが有効かどうか
		if (!IsUpdateActive()){
			return;
		}
		//Fixedかどうか
		if (IsFixed()){
			return;
		}
		//すでにヒットしている
		if (GetHitObject()){
			return;
		}
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto PtrT = GetGameObject()->GetComponent<Transform>();
			auto GravityPtr = GetGameObject()->GetComponent<Gravity>(false);
			if (PtrT){
				//前回のターンからの時間
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				auto ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();
				for (auto ObjPtr : ObjVec){
					if ((GetGameObject() != ObjPtr) && ObjPtr->IsUpdateActive()){
						if (GravityPtr && GravityPtr->GetOnObject() == ObjPtr){
							//乗ってるオブジェクトとは判定しない
							continue;
						}
						if (ChkExcludeCollisionObject(ObjPtr)){
							//除外オブジェクトとはとは判定しない
							continue;
						}
						//相手のCollisionを取得
						auto CollisionPtr = ObjPtr->GetComponent<Collision>(false);
						if (CollisionPtr){
							if (!CollisionPtr->IsUpdateActive()){
								//相手のCollisionが無効
								continue;
							}
							if (CollisionPtr->GetHitObject()){
								//もうすでに相手がヒットしている
								continue;
							}
						}
						auto CollisionSpherePtr = ObjPtr->GetComponent<CollisionSphere>(false);
						auto CollisionCapsulePtr = ObjPtr->GetComponent<CollisionCapsule>(false);
						auto CollisionObbPtr = ObjPtr->GetComponent<CollisionObb>(false);
						if (CollisionSpherePtr){
							CollisionWithSphere(ObjPtr);
						}
						else if (CollisionCapsulePtr){
							CollisionWithCapsule(ObjPtr);
						}
						else if (CollisionObbPtr){
							CollisionWithObb(ObjPtr);
						}
					}
				}
				//もしHitしてたら、相手のHitObjectにこちらを設定
				auto HitObject = GetHitObject();
				if (HitObject){
					auto OtherPtr = HitObject->GetComponent<Collision>();
					if (!OtherPtr->IsFixed()){
						OtherPtr->SetHitObject(GetGameObject());
						OtherPtr->SetHitTime(GetHitTime());
						HitObject->GetComponent<Transform>()->LerpBeforeToNow(GetHitTime());

					}
				}
			}
		}
	}

	//Rigidbodyがある場合のUpdate2
	//衝突法線を求める。
	//最近接点を求める
	void Collision::Update2WithRigidbody2(){
		if (IsGameObjectActive()){
			auto PtrSrcRigidbody = GetGameObject()->GetComponent<Rigidbody>();
			if (IsFixed() || !GetHitObject()){
				//動かないか、衝突してないか
				return;
			}
			Vector3 HitNormal;	//衝突の法線
			Vector3 ClosestPoint; //最近接点
			GetNormalClosetPointWithHitObject(GetHitObject(), HitNormal, ClosestPoint);
			//反発を計算
			auto PtrGrav = GetGameObject()->GetComponent<Gravity>(false);
			if (PtrGrav){
				//蓄積した落下の加速度を0にする
				PtrGrav->SetGravityVelocityZero();
			}
			//反発方向
			auto DestGameObject = GetHitObject();
			//Dest側のTransformとRigidbodyを取得
			auto PtrDestTrans = DestGameObject->GetComponent<Transform>();
			auto PtrDestRegid = DestGameObject->GetComponent<Rigidbody>();
			auto PtrDestColl = DestGameObject->GetComponent<Collision>();

			Vector3 Reflect = Vector3EX::Reflect(PtrSrcRigidbody->GetHitVelocity(), HitNormal);
			if (!PtrDestColl->IsFixed()){
				////相手がFixでなければ、力を分散する
				Vector3 v1Temp = PtrSrcRigidbody->GetHitVelocity() * (PtrSrcRigidbody->GetMass() - PtrDestRegid->GetMass());
				Vector3 v2Temp = PtrDestRegid->GetHitVelocity() * 2;
				v2Temp *= PtrDestRegid->GetMass();
				Reflect = (v1Temp + v2Temp) / (PtrSrcRigidbody->GetMass() + PtrDestRegid->GetMass());
			}
			//反発率
			Reflect *= PtrSrcRigidbody->GetReflection();
			PtrSrcRigidbody->SetVelocity(Reflect);
			//////////////////////////////////////
			//追い出し処理
			EscapeFromDestObject(DestGameObject, HitNormal, ClosestPoint);

		}
	}
	//反発、追い出しなど事後処理
	void Collision::Update2(){
		auto PtrSrcGravity = GetGameObject()->GetComponent<Gravity>(false);
		if (GetHitObject() && IsUpdate2Active()){
			//衝突していてかつIsUpdate2Activeなら
			shared_ptr<void> PtrVoid = static_pointer_cast<void>(GetHitObject());
			if (NeedSendEvent()){
				//Sendイベントを出す
				SendEvent(GetGameObject(), GetGameObject(), pImpl->m_EventString, PtrVoid);
				if (GetSubDispatchObject()){
					//サブで送信するオブジェクトがあれば送信
					SendEvent(GetGameObject(), GetSubDispatchObject(), pImpl->m_EventString, PtrVoid);
				}
			}
			if (NeedPostEvent()){
				//Postイベントを出す
				PostEvent(pImpl->m_PostDispatchTime, GetGameObject(), GetGameObject(), pImpl->m_EventString, PtrVoid);
				if (GetSubDispatchObject()){
					//サブで送信するオブジェクトがあれば送信
					PostEvent(pImpl->m_PostDispatchTime, GetGameObject(), GetSubDispatchObject(), pImpl->m_EventString, PtrVoid);
				}
			}
			if (NeedAfterCollision()){
				//自動後処理
				if (!IsFixed()){
					//Rigidbodyがあれば処理を任せる
					auto PtrSrcRigidbody = GetGameObject()->GetComponent<Rigidbody>(false);
					auto PtrDestRigidbody = GetHitObject()->GetComponent<Rigidbody>(false);
					if (PtrSrcRigidbody && PtrDestRigidbody){
						Update2WithRigidbody2();
					}
				}
			}
		}
		//ResetHitObjectはOnLastUpdate後の処理へ移動
		//ResetHitObject();
		if (PtrSrcGravity){
			if (PtrSrcGravity->GetOnObject()){
				EscapeFromDestParent(PtrSrcGravity->GetOnObject());
			}
			PtrSrcGravity->CheckBaseY();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl{
		float m_MakedDiameter;					//作成時の直径
		Impl() :
			m_MakedDiameter(1.0f)
		{}
		~Impl(){}

	};


	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionSphere::CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionSphere::~CollisionSphere(){}

	void CollisionSphere::OnCreate(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_SPHERE"));
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionSphere::GetMakedDiameter() const{
		return pImpl->m_MakedDiameter;
	}
	void CollisionSphere::SetMakedDiameter(float f){
		pImpl->m_MakedDiameter = f;
	}
	float CollisionSphere::GetMakedRadius() const{
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionSphere::SetMakedRadius(float f){
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	SPHERE CollisionSphere::GetSphere() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		//このオブジェクトのSPHEREを作成
		SPHERE Ret(MatBase.PosInMatrix(), MatBase.ScaleInMatrix().x * 0.5f);
		return Ret;
	}
	SPHERE CollisionSphere::GetBeforeSphere() const{
		SPHERE Ret = GetSphere();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Ret.m_Center = PtrT->GetBeforeWorldMatrix().PosInMatrix();
		return Ret;
	}

	Matrix4X4 CollisionSphere::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionSphere::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//相手にCollisionSphereが見つかった
		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforeSphere = DestCollisionSpherePtr->GetBeforeSphere();

		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestSphere, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
	}

	void CollisionSphere::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//相手にCollisionCapsuleが見つかった
		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforeCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();

		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforeCapsule.GetCenter();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestCapsule, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//Capsuleが回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::SPHERE_CAPSULE(SrcSphere, DestCapsule, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}


	void CollisionSphere::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		Vector3 SrcVelocity = SrcSphere.m_Center - SrcBeforSphere.m_Center;
		//相手にCollisionObbが見つかった
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;

		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//OBBが回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::SPHERE_OBB(SrcSphere, DestObb, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}

	void CollisionSphere::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		//相手の型を確認
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//相手が球体
			SPHERE SrcSphere = GetSphere();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			//Noamalに設定
			Normal = SrcSphere.m_Center - DestSphere.m_Center;
			Normal.Normalize();
			//最近接点を導き出す
			auto ClosPoint = Normal * DestSphere.m_Radius;
			ClosestPoint = ClosPoint + DestSphere.m_Center;
		}
		else if (DestCollisionCapsulePtr){
			//相手にCollisionObbが見つかった
			SPHERE SrcSphere = GetSphere();
			CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
			//最近接点を得る
			HitTest::SPHERE_CAPSULE(SrcSphere, DestCapsule, ClosestPoint);
			Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
		}
		else if (DestCollisionObbPtr){
			//相手にCollisionObbが見つかった
			SPHERE SrcSphere = GetSphere();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//最近接点を得る
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
		}
		else{
			Normal = Vector3(0, 0, 0);
			ClosestPoint = Vector3(0, 0, 0);
		}
	}

	void CollisionSphere::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		SPHERE SrcSphere = GetSphere();
		//最近接点からNormal方向に半径ぶん移動
		Vector3 EscV = Normal;
		EscV.Normalize();
		float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
		EscV *= EscF;
		SrcSphere.m_Center = ClosestPoint + EscV;
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		PtrTrans->SetPosition(SrcSphere.m_Center);
	}

	void CollisionSphere::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		auto DestCollisionObbPtr = ParentObject->GetComponent<CollisionObb>(false);
		if (DestCollisionObbPtr){
			//相手にCollisionObbが見つかった
			SPHERE SrcSphere = GetSphere();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//最近接点を得る
			Vector3 ClosestPoint;
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Vector3 Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.0001f, Normal);
			//最近接点からNormal方向に半径ぶん移動
			float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
			Normal *= EscF;
			SrcSphere.m_Center = ClosestPoint + Normal;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcSphere.m_Center);
		}
	}

	//Gravityから呼ばれる
	bool CollisionSphere::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//下のオブジェクトはOBB以外は設定できない
			return false;
		}
		//自分はSphere
		SPHERE SrcSphere = GetSphere();
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();

		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y -= SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y -= SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;

			//上に乗ってるかどうかを検証
			//レイを打ち込んでみる
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		else{
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y += SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y += SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;
			//下に乗ってるかどうかを検証
			//レイを打ち込んでみる
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		return false;
	}



	//--------------------------------------------------------------------------------------
	//	struct CollisionCapsule::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionCapsule::Impl{
		float m_MakedDiameter;			//作成時の直径
		float m_MakedHeight;			//作成時高さ
		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionCapsule::CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionCapsule::~CollisionCapsule(){}

	//初期化
	void CollisionCapsule::OnCreate(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_CAPSULE"));
		SetDrawActive(false);
	}

	//アクセサ
	float CollisionCapsule::GetMakedDiameter() const{
		return pImpl->m_MakedDiameter;
	}
	void CollisionCapsule::SetMakedDiameter(float f){
		pImpl->m_MakedDiameter = f;
	}
	float CollisionCapsule::GetMakedRadius() const{
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionCapsule::SetMakedRadius(float f){
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	float CollisionCapsule::GetMakedHeight() const{
		return pImpl->m_MakedHeight;
	}
	void CollisionCapsule::SetMakedHeight(float f){
		pImpl->m_MakedHeight = f;
	}

	CAPSULE CollisionCapsule::GetCapsule() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		//このオブジェクトのCAPSULEを作成
		CAPSULE Ret(pImpl->m_MakedDiameter * 0.5f, Vector3(0, pImpl->m_MakedHeight * -0.5f, 0),
			Vector3(0, pImpl->m_MakedHeight * 0.5f, 0),
			PtrT->GetWorldMatrix());
		return Ret;
	}

	CAPSULE CollisionCapsule::GetBeforeCapsule() const{
		CAPSULE Ret = GetCapsule();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Ret.SetCenter(PtrT->GetBeforeWorldMatrix().PosInMatrix());
		return Ret;
	}
	Matrix4X4 CollisionCapsule::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_MakedDiameter, pImpl->m_MakedHeight, pImpl->m_MakedDiameter);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionCapsule::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//相手にCollisionSphereが見つかった
		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforeSphere = DestCollisionSpherePtr->GetBeforeSphere();

		Vector3 DestVelocity = DestSphere.m_Center - DestBeforeSphere.m_Center;
		//CapsuleがSrcなので、Sphere側を動かす処理にする
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcCapsule, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//Capsuleが回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::SPHERE_CAPSULE(DestSphere, SrcCapsule, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}
	void CollisionCapsule::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){

		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//相手にCollisionCapsuleが見つかった
		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforeCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();

		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforeCapsule.GetCenter();
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestCapsule, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//Capsuleが回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec1, RetVec2;
		if (HitTest::CAPSULE_CAPSULE(SrcCapsule, DestCapsule, RetVec1, RetVec2)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}
	void CollisionCapsule::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		Vector3 SrcVelocity = SrcCapsule.GetCenter() - SrcBeforCapsule.GetCenter();
		//相手にCollisionCapsuleが見つかった
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();

		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;
		Vector3 SpanVelocity = SrcVelocity - DestVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::CAPSULE_OBB(SrcCapsule, DestObb, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}

	void CollisionCapsule::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		//Src側はCapsule
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//相手にCollisionSphereが見つかった
			CAPSULE SrcCapsle = GetCapsule();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			Vector3 RetVec;
			//最近接点を得る
			float t;
			HitTest::ClosetPtPointSegment(DestSphere.m_Center, SrcCapsle.m_PointA, SrcCapsle.m_PointB, t, ClosestPoint);
			Normal = SrcCapsle.GetCenter() - ClosestPoint;
			Normal.Normalize();
		}
		else if (DestCollisionCapsulePtr){
			//相手にDestCollisionCapsulePtrが見つかった
			CAPSULE SrcCapsle = GetCapsule();
			CAPSULE DestCapsle = DestCollisionCapsulePtr->GetCapsule();
			Vector3 RetVecSrc, RetVecDest;
			//最近接点を得る
			HitTest::CAPSULE_CAPSULE(SrcCapsle, DestCapsle, RetVecSrc, RetVecDest);
			//衝突法線を得る
			Normal = RetVecSrc - RetVecDest;
			Normal.Normalize();
			//Dest側の最近接点を得る
			ClosestPoint = Normal;
			ClosestPoint *= DestCapsle.m_Radius;
			ClosestPoint += RetVecDest;
		}
		else if (DestCollisionObbPtr){
			//相手にDestCollisionObbPtrが見つかった
			CAPSULE SrcCapsle = GetCapsule();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			int flg;
			ClosestPoint = HitTest::ClosestPtCapsuleOBB(SrcCapsle, DestObb, flg);

			if (flg < 0){
				Normal = SrcCapsle.m_PointA - ClosestPoint;
				Normal.Normalize();
			}
			else if (flg > 0){
				Normal = SrcCapsle.m_PointB - ClosestPoint;
				Normal.Normalize();
			}
			else{
				float t;
				Vector3 RetVec;
				HitTest::ClosetPtPointSegment(ClosestPoint, SrcCapsle.m_PointB, SrcCapsle.m_PointA, t, RetVec);
				Normal = RetVec - ClosestPoint;
				Normal.Normalize();
			}
		}
		else{
			Normal = Vector3(0, 0, 0);
			ClosestPoint = Vector3(0, 0, 0);
		}
	}

	void CollisionCapsule::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		CAPSULE SrcCapsule = GetCapsule();
		//最近接点からNormal方向にEscFぶん移動
		Vector3 EscV = Normal;
		EscV.Normalize();
		float EscF = SrcCapsule.m_Radius * GetEscapeSpanMin();
		EscV *= EscF;
		Vector3 Center = SrcCapsule.GetCenter();
		Center += EscV;
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		PtrTrans->SetPosition(Center);
	}

	void CollisionCapsule::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		auto DestCollisionObbPtr = ParentObject->GetComponent<CollisionObb>(false);
		if (DestCollisionObbPtr){
			//相手にCollisionObbが見つかった
			CAPSULE SrcCapsule = GetCapsule();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			int flg;
			HitTest::ClosestPtCapsuleOBB(SrcCapsule, DestObb, flg);
			if (flg >= 0){
				//下側の球体のみ対応
				return;
			}

			SPHERE SrcSphere;
			SrcSphere.m_Center = SrcCapsule.m_PointA;
			SrcSphere.m_Radius = SrcCapsule.m_Radius;
			//最近接点を得る
			Vector3 ClosestPoint;
			HitTest::SPHERE_OBB(SrcSphere, DestObb, ClosestPoint);
			Vector3 Normal = SrcSphere.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.0001f, Normal);
			//最近接点からNormal方向に半径ぶん移動
			float EscF = SrcSphere.m_Radius + (SrcSphere.m_Radius * GetEscapeSpanMin());
			Normal *= EscF;
			SrcSphere.m_Center = ClosestPoint + Normal;
			Vector3 Span = SrcSphere.m_Center - SrcCapsule.m_PointA;
			SrcCapsule.m_PointA = SrcSphere.m_Center;
			SrcCapsule.m_PointB += Span;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcCapsule.GetCenter());
		}
	}

	//Gravityから呼ばれる
	bool CollisionCapsule::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//下のオブジェクトはOBB以外は設定できない
			return false;
		}
		//自分はCapsule
		CAPSULE SrcSCapsule = GetCapsule();
		//下のSphereで判別
		SPHERE SrcSphere;
		SrcSphere.m_Center = SrcSCapsule.m_PointA;
		SrcSphere.m_Radius = SrcSCapsule.m_Radius;
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();

		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y -= SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y -= SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;

			//上に乗ってるかどうかを検証
			//レイを打ち込んでみる
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}
		else{
			Vector3 StartPoint = Vector3(0, 0, 0);
			StartPoint.y += SrcSphere.m_Radius *0.9f;
			StartPoint.Transform(DestObb.GetRotMatrix());
			StartPoint += SrcSphere.m_Center;
			Vector3 EndPoint = Vector3(0, 0, 0);
			EndPoint.y += SrcSphere.m_Radius * PtrGravity->GetRayUnderSize();
			EndPoint.Transform(DestObb.GetRotMatrix());
			EndPoint += SrcSphere.m_Center;
			//下に乗ってるかどうかを検証
			//レイを打ち込んでみる
			if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
				return true;
			}
		}


		return false;
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionObb::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CollisionObb::Impl{
		float m_Size;					//作成時のサイズ
		Impl() :
			m_Size(1.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CollisionObb::CollisionObb(const shared_ptr<GameObject>& GameObjectPtr):
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionObb::~CollisionObb(){}

	//初期化
	void CollisionObb::OnCreate(){
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(L"DEFAULT_CUBE"));
		SetDrawActive(false);
	}


	//アクセサ
	float CollisionObb::GetMakedSize() const{
		return pImpl->m_Size;
	}
	void CollisionObb::SetMakedSize(float f){
		pImpl->m_Size = f;
	}

	OBB CollisionObb::GetObb() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}
	OBB CollisionObb::GetBeforeObb() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetBeforeWorldMatrix();
		OBB Ret(Vector3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
		return Ret;
	}
	Matrix4X4 CollisionObb::GetCollisionMatrix() const{
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		Matrix4X4 MatBase;
		MatBase.Scaling(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size);
		MatBase *= PtrT->GetWorldMatrix();
		return MatBase;
	}

	void CollisionObb::CollisionWithSphere(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//相手にCollisionSphereが見つかった

		SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
		SPHERE DestBeforSphere = DestCollisionSpherePtr->GetBeforeSphere();


		Vector3 DestVelocity = DestSphere.m_Center - DestBeforSphere.m_Center;
		//OBBがSrcなので、Sphere側を動かす処理にする
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforSphere, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::SPHERE_OBB(DestSphere, SrcObb, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}

	void CollisionObb::CollisionWithCapsule(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//相手にCollisionSphereが見つかった

		CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
		CAPSULE DestBeforCapsule = DestCollisionCapsulePtr->GetBeforeCapsule();


		Vector3 DestVelocity = DestCapsule.GetCenter() - DestBeforCapsule.GetCenter();
		//OBBがSrcなので、Capsule側を動かす処理にする
		Vector3 SpanVelocity = DestVelocity - SrcVelocity;
		SpanVelocity /= ElapsedTime;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforCapsule, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
			//ヒット地点まで戻る
			PtrT->LerpBeforeToNow(HitTime);
			//ヒットオブジェクトを設定
			//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
			//設定される
			SetHitTime(HitTime);
			SetHitObject(DestObj);
		}
		//回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		Vector3 RetVec;
		if (HitTest::CAPSULE_OBB(DestCapsule, SrcObb, RetVec)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}



	void CollisionObb::CollisionWithObb(const shared_ptr<GameObject>& DestObj){
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>();
		auto PtrT = GetGameObject()->GetComponent<Transform>();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//前の衝突でTransformが変わってる可能性があるので、ループ内で取得
		OBB SrcObb = GetObb();
		OBB SrcBeforObb = GetBeforeObb();
		Vector3 SrcVelocity = SrcObb.m_Center - SrcBeforObb.m_Center;
		//相手にCollisionObbが見つかった
		OBB DestObb = DestCollisionObbPtr->GetObb();
		OBB DestBeforeObb = DestCollisionObbPtr->GetBeforeObb();
		Vector3 DestVelocity = DestObb.m_Center - DestBeforeObb.m_Center;

		if (SrcObb.m_Size.Length() <= DestObb.m_Size.Length()){
			Vector3 SpanVelocity = SrcVelocity - DestVelocity;
			SpanVelocity /= ElapsedTime;
			float HitTime = 0;
			if (HitTest::CollisionTestObbObb(SrcBeforObb, SpanVelocity, DestObb, 0, ElapsedTime, HitTime)){
				//ヒット地点まで戻る
				PtrT->LerpBeforeToNow(HitTime);
				//ヒットオブジェクトを設定
				//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
				//設定される
				SetHitTime(HitTime);
				SetHitObject(DestObj);
			}
		}
		else{
			Vector3 SpanVelocity = DestVelocity - SrcVelocity;
			SpanVelocity /= ElapsedTime;
			float HitTime = 0;
			if (HitTest::CollisionTestObbObb(DestBeforeObb, SpanVelocity, SrcObb, 0, ElapsedTime, HitTime)){
				//ヒット地点まで戻る
				PtrT->LerpBeforeToNow(HitTime);
				//ヒットオブジェクトを設定
				//続いて残りも判定するが、最終的に、一番Before地点に近いオブジェクトが
				//設定される
				SetHitTime(HitTime);
				SetHitObject(DestObj);
			}
		}
		//回転している場合などここまででヒット漏れする可能性がある
		//その場合は現時点でのヒットとする
		if (HitTest::OBB_OBB(SrcObb, DestObb)){
			SetHitTime(0);
			SetHitObject(DestObj);
		}
	}

	void CollisionObb::GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint){
		auto DestCollisionSpherePtr = DestObj->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestObj->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestObj->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			//相手にCollisionSphereが見つかった
			OBB SrcObb = GetObb();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			//最近接点を得る
			HitTest::SPHERE_OBB(DestSphere, SrcObb, ClosestPoint);
			//法線はDest側（球体側）の法線を出して逆向きにする
			Normal = DestSphere.m_Center - ClosestPoint;
			Normal *= -1.0f;
			Normal.Normalize();
		}
		else if (DestCollisionCapsulePtr){
			//相手にDestCollisionCapsulePtrが見つかった
			OBB SrcObb = GetObb();
			CAPSULE DestCapsle = DestCollisionCapsulePtr->GetCapsule();
			HitTest::CAPSULE_OBB(DestCapsle, SrcObb, ClosestPoint);
			float t;
			Vector3 RetVec;
			HitTest::ClosetPtPointSegment(ClosestPoint, DestCapsle.m_PointA, DestCapsle.m_PointB, t, RetVec);
			Normal = ClosestPoint - RetVec;
			Normal *= -1.0f;
			Normal.Normalize();
		}
		else if (DestCollisionObbPtr){
			//相手にDestCollisionObbPtrが見つかった
			OBB SrcObb = GetObb();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			//最近接点を得る
			HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, ClosestPoint);
			Normal = SrcObb.m_Center - ClosestPoint;
			Normal.Normalize();
			DestObb.GetNearNormalRot(Normal, 0.5f, Normal);
		}
	}

	void CollisionObb::EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint){
		auto DestCollisionSpherePtr = DestGameObject->GetComponent<CollisionSphere>(false);
		auto DestCollisionCapsulePtr = DestGameObject->GetComponent<CollisionCapsule>(false);
		auto DestCollisionObbPtr = DestGameObject->GetComponent<CollisionObb>(false);
		if (DestCollisionSpherePtr){
			OBB SrcObb = GetObb();
			SPHERE DestSphere = DestCollisionSpherePtr->GetSphere();
			float BaseF = Vector3EX::Length(ClosestPoint - DestSphere.m_Center);
			float NewF = DestSphere.m_Radius + (DestSphere.m_Radius * GetEscapeSpanMin());
			float MoveF = NewF - BaseF;
			Vector3 MoveV = Normal * MoveF;
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center + MoveV);
		}
		else if (DestCollisionCapsulePtr){
			OBB SrcObb = GetObb();
			SrcObb.m_Size *= GetEscapeAlignPlus();
			CAPSULE DestCapsule = DestCollisionCapsulePtr->GetCapsule();
			Vector3 EscapeSpan;
			EscapeSpan = Normal;
			EscapeSpan.Normalize();
			EscapeSpan *= (SrcObb.GetMinSize() * GetEscapeSpanMin());
			Vector3 RetVec;
			int count = 0;
			while (HitTest::CAPSULE_OBB(DestCapsule, SrcObb, RetVec)){
				SrcObb.m_Center += EscapeSpan;
				EscapeSpan *= GetEscapeAlignPlus();
				count++;
				if (count > 30){
					break;
				}
			}
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center);
		}
		else if (DestCollisionObbPtr){
			//相手にDestCollisionObbPtrが見つかった
			OBB SrcObb = GetObb();
			SrcObb.m_Size *= GetEscapeAlignPlus();
			OBB DestObb = DestCollisionObbPtr->GetObb();
			Vector3 EscapeSpan;
			EscapeSpan = Normal;
			EscapeSpan.Normalize();
			EscapeSpan *= (SrcObb.GetMinSize() * GetEscapeSpanMin());
			Vector3 RetVec;
			int count = 0;
			while (HitTest::OBB_OBB(SrcObb, DestObb)){
				SrcObb.m_Center += EscapeSpan;
				EscapeSpan *= GetEscapeAlignPlus();
				count++;
				if (count > 30){
					break;
				}
			}
			auto PtrTrans = GetGameObject()->GetComponent<Transform>();
			PtrTrans->SetPosition(SrcObb.m_Center);
		}
	}

	void CollisionObb::EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject){
		//OBBがOBBに乗る形は未対応
	}

	//Gravityから呼ばれる
	bool CollisionObb::CheckOnObject(const shared_ptr<GameObject>& OnObject){
		auto UnderObjectCollisionObbPtr = OnObject->GetComponent<CollisionObb>(false);
		if (!UnderObjectCollisionObbPtr){
			//下のオブジェクトはOBB以外は設定できない
			return false;
		}
		//自分はOBB
		OBB SrcObb = GetObb();
		OBB DestObb = UnderObjectCollisionObbPtr->GetObb();
		auto PtrGravity = GetGameObject()->GetComponent<Gravity>();
		if (PtrGravity->GetGravity().y <= 0){
			//SrcObbから底辺の４点を求める
			//中心からの相対距離で作成する
			vector<Vector3> BottomPoints = {
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, -SrcObb.m_Size.y, -SrcObb.m_Size.z),
			};
			for (auto p : BottomPoints){
				Vector3 StartPoint = p + Vector3(0, 0.1f, 0);
				StartPoint.Transform(DestObb.GetRotMatrix());
				StartPoint += SrcObb.m_Center;
				Vector3 EndPoint = p + Vector3(0, -0.1f, 0);
				EndPoint.Transform(DestObb.GetRotMatrix());
				EndPoint += SrcObb.m_Center;
				//上に乗ってるかどうかを検証
				//レイを打ち込んでみる
				if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
					return true;
				}
			}
		}
		else{
			//SrcObbから上辺の４点を求める
			//中心からの相対距離で作成する
			vector<Vector3> BottomPoints = {
				Vector3(SrcObb.m_Size.x, SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, SrcObb.m_Size.y, SrcObb.m_Size.z),
				Vector3(SrcObb.m_Size.x, SrcObb.m_Size.y, -SrcObb.m_Size.z),
				Vector3(-SrcObb.m_Size.x, SrcObb.m_Size.y, -SrcObb.m_Size.z),
			};
			for (auto p : BottomPoints){
				Vector3 StartPoint = p + Vector3(0, 0.1f, 0);
				StartPoint.Transform(DestObb.GetRotMatrix());
				StartPoint += SrcObb.m_Center;
				Vector3 EndPoint = p + Vector3(0, 0.1f, 0);
				EndPoint.Transform(DestObb.GetRotMatrix());
				EndPoint += SrcObb.m_Center;
				//上に乗ってるかどうかを検証
				//レイを打ち込んでみる
				if (HitTest::SEGMENT_OBB(StartPoint, EndPoint, DestObb)){
					return true;
				}
			}

		}
		return false;
	}


}

//end basedx11

