/*!
@file Rigidbody.h
@brief Gravity,Rigidbody,Collision,SteeringComponentとその派生クラス

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class Gravity : public Component ;
	//	用途: 重力
	//--------------------------------------------------------------------------------------
	class Gravity : public Component{
		//onオブジェクトが現在も有効かどうかをチェックする
		bool CheckOnObject(const shared_ptr<GameObject>& OnObject);
		//ヒットしたオブジェクトを調べ、それが下部にあったらOnObjectに設定する（その上に乗る）
		void HitObjectToOnObject();
	public:
		explicit Gravity(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Gravity();
		//アクセサ
		const Vector3& GetGravity() const;
		void SetGravity(const Vector3& Gravity);
		void SetGravity(float x, float y, float z);

		const Vector3& GetInvGravity() const;
		void SetInvGravity(const Vector3& InvGravity);
		void SetInvGravity(float x, float y, float z);
		void SetInvGravityDefault();


		const Vector3& GetGravityVelocity() const;
		void SetGravityVelocity(const Vector3& GravityVelocity);
		void SetGravityVelocity(float x, float y, float z);

		bool IsGravityVelocityZero();
		void SetGravityVelocityZero();


		float GetBaseY() const;
		void SetBaseY(float y);

		float GetRayUnderSize() const;
		void SetRayUnderSize(float f);

		void CheckBaseY();

		shared_ptr<GameObject> GetOnObject() const;
		void SetOnObject(const shared_ptr<GameObject>& OnObject);
		void ClearOnObject();


		//ジャンプのスタート
		void StartJump(const Vector3& StartVec,float EscapeSpan = 0.001f);
		void StartJump(float x, float y, float z,float EscapeSpan = 0.001f);

		//操作
		virtual void OnUpdate()override;
		virtual void Update2();
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SteeringComponent : public Component ;
	//	用途: 操舵コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class SteeringComponent : public Component{
	protected:
		explicit SteeringComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SteeringComponent();
	public:
		float GetWeight() const;
		void SetWeight(float f);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SeekSteering : public SteeringComponent;
	//	用途: Seek操舵
	//--------------------------------------------------------------------------------------
	class SeekSteering : public SteeringComponent{
	public:
		explicit SeekSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SeekSteering();

		//アクセサ
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ArriveSteering : public SteeringComponent;
	//	用途: Arrive操舵
	//--------------------------------------------------------------------------------------
	class ArriveSteering : public SteeringComponent{
	public:
		explicit ArriveSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ArriveSteering();

		//アクセサ
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		float GetDecl() const;
		void SetDecl(float f);


		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PursuitSteering : public SteeringComponent;
	//	用途: Pursuit操舵
	//--------------------------------------------------------------------------------------
	class PursuitSteering : public SteeringComponent{
	public:
		explicit PursuitSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PursuitSteering();

		//アクセサ
		const Vector3& GetTargetPosition() const;
		void SetTargetPosition(const Vector3& Vec);
		void SetTargetPosition(float x, float y, float z);

		const Quaternion& GetTargetQuaternion() const;
		void SetTargetQuaternion(const Quaternion& Qt);
		void SetTargetRotation(const Vector3& rotation);
		void SetTargetRotation(float x, float y, float z);

		const Vector3& GetTargetVelocity() const;
		void SetTargetVelocity(const Vector3& velocity);
		void SetTargetVelocity(float x, float y, float z);


		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class WanderSteering : public SteeringComponent;
	//	用途: Wander操舵
	//--------------------------------------------------------------------------------------
	class WanderSteering : public SteeringComponent{
	public:
		explicit WanderSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~WanderSteering();

		//アクセサ
		const Vector3& GetWanderTarget() const;
		void SetWanderTarget(const Vector3& target);

		float GetWanderRadius() const;
		void SetWanderRadius(float f);
		float GetWanderDistance() const;
		void SetWanderDistance(float f);
		float GetWanderJitter() const;
		void SetWanderJitter(float f);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class WallAvoidanceSteering : public SteeringComponent;
	//	用途: WallAvoidance操舵(壁回避)
	//--------------------------------------------------------------------------------------
	class WallAvoidanceSteering : public SteeringComponent{
	public:
		explicit WallAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~WallAvoidanceSteering();

		//アクセサ
		//回避する壁の配列
		vector<PLANE>& GetPlaneVec() const;
		void SetPlaneVec(const vector<PLANE>& planevec);
		//Plane型を渡す
		void SetPlaneVec(const vector<Plane>& planevec);

		//壁と衝突しているか
		bool IsWallArribed() const;

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class ObstacleAvoidanceSteering : public SteeringComponent;
	//	用途: ObstacleAvoidance操舵(障害物回避)
	//--------------------------------------------------------------------------------------
	class ObstacleAvoidanceSteering : public SteeringComponent{
	public:
		explicit ObstacleAvoidanceSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~ObstacleAvoidanceSteering();

		//アクセサ
		//障害物の配列
		const vector<SPHERE>& GetObstacleSphereVec() const;
		void SetObstacleSphereVec(const vector<SPHERE>& spherevec);
		//道の高さ
		float GetRoadWidth() const;
		void SetRoadWidth(float f);
		//道幅
		float GetRoadHeight() const;
		void SetRoadHeight(float f);


		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	


	//--------------------------------------------------------------------------------------
	//	class FollowPathSteering : public SteeringComponent;
	//	用途: FollowPath操舵(経路追従)
	//--------------------------------------------------------------------------------------
	class FollowPathSteering : public SteeringComponent{
	public:
		explicit FollowPathSteering(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~FollowPathSteering();

		//アクセサ
		///経路をあらわすリスト
		void SetPathList(const list<Vector3>& pathlist);

		float GetDecl() const;
		void SetDecl(float f);

		float GetWaypointSpan() const;
		void SetWaypointSpan(float f);

		bool GetLooped() const;
		bool IsLooped() const;
		void SetLooped(bool b);

		bool IsFinished() const;

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class AlignmentSteering : public SteeringComponent;
	//	用途: Alignment操舵(整列)
	//--------------------------------------------------------------------------------------
	class AlignmentSteering : public SteeringComponent{
	public:
		AlignmentSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~AlignmentSteering();
		//アクセサ
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CohesionSteering : public SteeringComponent;
	//	用途: Cohesion操舵(結合)
	//--------------------------------------------------------------------------------------
	class CohesionSteering : public SteeringComponent{
	public:
		CohesionSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~CohesionSteering();
		//アクセサ
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SeparationSteering : public SteeringComponent;
	//	用途: Separation操舵
	//--------------------------------------------------------------------------------------
	class SeparationSteering : public SteeringComponent{
	public:
		SeparationSteering(const shared_ptr<GameObject>& GameObjectPtr,
			const shared_ptr<GameObjectGroup>& Group);
		virtual ~SeparationSteering();
		//アクセサ
		shared_ptr<GameObjectGroup> GetGameObjectGroup() const;
		void SetGameObjectGroup(const shared_ptr<GameObjectGroup>& Group);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Rigidbody : public Component ;
	//	用途: 物理計算付自動位置コントロール
	//--------------------------------------------------------------------------------------
	class Rigidbody : public Component {
	public:
		//構築と破棄
		explicit Rigidbody(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Rigidbody();
		//アクセサ
		const Vector3& GetVelocity() const;
		void SetVelocity(const Vector3& Velocity);
		void SetVelocity(float x, float y, float z);

		bool IsVelocityZeroCommand() const;
		void SetVelocityZeroCommand(bool b);

		const Vector3& GetHitVelocity() const;
		void SetHitVelocity(const Vector3& Velocity);
		void SetHitVelocity(float x, float y, float z);


		float GetMaxSpeed() const;
		void SetMaxSpeed(float f);

		const Vector3& GetMinVelocity() const;
		void SetMinVelocity(const Vector3& Velocity);
		void SetMinVelocity(float x, float y, float z);


		const Vector3& GetAngularVelocity() const;
		void SetAngularVelocity(const Vector3& AngularVelocity);
		void SetAngularVelocity(float x, float y, float z);


		float GetMass() const;
		void SetMass(float f);

		float GetReflection() const;			//反発係数
		void SetReflection(float f);

		const Vector3& GetForce() const;
		void SetForce(const Vector3& Force);
		void SetForce(float x, float y, float z);

		float GetFriction() const;
		void SetFriction(float f);

		float GetMaxForce() const;
		void SetMaxForce(float f);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

	};

	class MeshResource;
	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	用途: 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class Collision : public Component {
		friend class Gravity;
		bool IsNewDest() const;
		bool NeedAfterCollision() const;
		bool NeedSendEvent() const;
		bool NeedPostEvent() const;
		//除外グループに属しているかを調べる
		bool ChkExcludeCollisionObject(const shared_ptr<GameObject>&  CheckGameObject);

		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) = 0;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) = 0;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) = 0;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint) = 0;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal,const Vector3& ClosestPoint) = 0;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject) = 0;
		//onオブジェクトが現在も有効かどうかをチェックする
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject) = 0;
	protected:
		//構築と破棄
		explicit Collision(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Collision();
		void SetMeshResource(const shared_ptr<MeshResource>& Ptr);
		//Rigidbodyがある場合のUpdate2
		void Update2WithRigidbody2();
		//抜け出し処理の係数
		float GetEscapeSpanMin() const;
		void SetEscapeSpanMin(float f);
		float GetEscapeAlignPlus() const;
		void SetEscapeAlignPlus(float f);
	public:
		//アクセサ
		shared_ptr<MeshResource> GetMeshResource() const;

		bool GetFixed() const;
		bool IsFixed() const;
		void SetFixed(bool b);

		bool GetSendEventActive() const;
		bool IsSendEventActive() const;
		void SetSendEventActive(bool b);

		bool GetPostEventActive() const;
		bool IsPostEventActive() const;
		void SetPostEventActive(bool b);

		float GetPostDispatchTime() const;			//Postの際の遅延時間
		void SetPostDispatchTime(float f);


		wstring GetEventString() const;
		void SetEventString(const wstring& str);


		bool GetAutoAfterCollision() const;
		bool IsAutoAfterCollision() const;
		void SetAutoAfterCollision(bool b);

		bool GetNewDestOnly() const;
		bool IsNewDestOnly() const;
		void SetNewDestOnly(bool b);

		shared_ptr<GameObject> GetHitObject() const;
		void SetHitObject(const shared_ptr<GameObject>& Ptr);

		void ResetHitObject();
		void ClearBothHitObject();

		shared_ptr<GameObject> GetSubDispatchObject() const;
		void SetSubDispatchObject(const shared_ptr<GameObject>& Ptr);

		//判定から除外するオブジェクトグループの設定
		void SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group);

		float GetHitTime() const;
		void SetHitTime(float f);

		//onオブジェクトが現在も有効かどうかをチェックする
		//Gravityから呼ばれる
		bool CheckOnObjectBase(const shared_ptr<GameObject>& OnObject);

		//仮想関数
		virtual Matrix4X4 GetCollisionMatrix() const = 0;

		//操作
		virtual void OnUpdate()override;
		virtual void Update2();
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionSphere : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//GravityからCollision経由で呼ばれる
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//構築と破棄
		explicit CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionSphere();
		//初期化
		virtual void OnCreate() override;

		//アクセサ
		float GetMakedDiameter() const;	//作成時の直径
		void SetMakedDiameter(float f);		//作成時の直径
		float GetMakedRadius() const;	//作成時の半径
		void SetMakedRadius(float f);			//作成時の半径


		SPHERE GetSphere() const;
		SPHERE GetBeforeSphere() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//操作
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionCapsule : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//GravityからCollision経由で呼ばれる
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//構築と破棄
		explicit CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionCapsule();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		float GetMakedDiameter() const;	//作成時の直径
		void SetMakedDiameter(float f);		//作成時の直径
		float GetMakedRadius() const;	//作成時の半径
		void SetMakedRadius(float f);			//作成時の半径
		float GetMakedHeight() const;	//作成時の高さ
		void SetMakedHeight(float f);		//作成時の高さ

		CAPSULE GetCapsule() const;
		CAPSULE GetBeforeCapsule() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//操作
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionObb : public Collision {
		virtual void CollisionWithSphere(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithCapsule(const shared_ptr<GameObject>& DestObj) override;
		virtual void CollisionWithObb(const shared_ptr<GameObject>& DestObj) override;

		virtual void GetNormalClosetPointWithHitObject(const shared_ptr<GameObject>& DestObj, Vector3& Normal, Vector3& ClosestPoint)override;
		virtual void EscapeFromDestObject(const shared_ptr<GameObject>&  DestGameObject, const Vector3& Normal, const Vector3& ClosestPoint)override;
		virtual void EscapeFromDestParent(const shared_ptr<GameObject>&  ParentObject)override;
		//GravityからCollision経由で呼ばれる
		virtual bool CheckOnObject(const shared_ptr<GameObject>& OnObject)override;
	public:
		//構築と破棄
		explicit CollisionObb(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CollisionObb();
		//初期化
		virtual void OnCreate() override;

		//アクセサ
		float GetMakedSize() const;	//作成時の1辺の長さ
		void SetMakedSize(float f);


		OBB GetObb() const;
		OBB GetBeforeObb() const;
		virtual Matrix4X4 GetCollisionMatrix() const override;

		//操作
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





}

//end basedx11
