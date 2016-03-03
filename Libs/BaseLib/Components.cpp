#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Component::Impl{
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//updateするかどうか
		bool m_Update2Active{ true };	//update2するかどうか
		bool m_DrawActive{ true };		//Drawするかどうか

		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
			{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	用途: コンポーネント基底クラス
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		Object(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component(){}
	//アクセサ
	shared_ptr<GameObject> Component::GetGameObject() const{
		if (pImpl->m_GameObject.expired()){
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (pImpl->m_GameObject.expired())",
				L"Component::GetGameObject()"
				);
		}
		else{
			return pImpl->m_GameObject.lock();
		}
	}
	bool Component::IsGameObjectActive() const{
		return !pImpl->m_GameObject.expired();
	}
	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr){
		pImpl->m_GameObject = GameObjectPtr;
	}
	shared_ptr<Stage> Component::GetStage() const{
		auto GamePtr = GetGameObject();
		auto StagePtr = dynamic_pointer_cast<Stage>(GamePtr);
		if (StagePtr){
			return StagePtr;
		}
		else{
			return GamePtr->GetStage();
		}
	}


	bool Component::IsUpdateActive() const{
		return pImpl->m_UpdateActive;
	}
	bool Component::GetUpdateActive() const{
		return pImpl->m_UpdateActive;
	}
	void Component::SetUpdateActive(bool b){
		pImpl->m_UpdateActive = b;
	}

	bool Component::IsUpdate2Active() const{
		return pImpl->m_Update2Active;
	}
	bool Component::GetUpdate2Active() const{
		return pImpl->m_Update2Active;
	}
	void Component::SetUpdate2Active(bool b){
		pImpl->m_Update2Active = b;
	}
	void Component::SetUpdateAllActive(bool b){
		pImpl->m_UpdateActive = b;
		pImpl->m_Update2Active = b;
	}



	bool Component::IsDrawActive() const{
		return pImpl->m_DrawActive;
	}
	bool Component::GetDrawActive() const{
		return pImpl->m_DrawActive;
	}
	void Component::SetDrawActive(bool b){
		pImpl->m_DrawActive = b;
	}


	//--------------------------------------------------------------------------------------
	//	struct TransformMatrix::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct TransformMatrix::Impl {
		bool m_Init{ false };	//初期化済みかどうか（1回目のUpdateで、Beforeに値を入れる）
		Matrix4X4 m_BeforeWorldMatrix{ Matrix4X4() };			//アップデート直前のワールド行列
		Matrix4X4 m_WorldMatrix{ Matrix4X4() };
		Impl()
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	//	class TransformMatrix : public Component ;
	// 行列のみ保持する変換クラス
	//--------------------------------------------------------------------------------------
	bool TransformMatrix::IsInit() const {
		return pImpl->m_Init;
	}
	void TransformMatrix::SetInitTrue() {
		pImpl->m_Init = true;
	}


	TransformMatrix::TransformMatrix(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	TransformMatrix::~TransformMatrix() {}
	const Matrix4X4& TransformMatrix::GetWorldMatrix() const {
		return pImpl->m_WorldMatrix;
	}
	void TransformMatrix::SetWorldMatrix(const Matrix4X4& WorldMatrix) {
		pImpl->m_WorldMatrix = WorldMatrix;
	}
	const Matrix4X4& TransformMatrix::GetBeforeWorldMatrix() const {
		return pImpl->m_BeforeWorldMatrix;
	}
	void TransformMatrix::SetToBefore() {
		pImpl->m_BeforeWorldMatrix = pImpl->m_WorldMatrix;

	}

	void TransformMatrix::OnUpdate() {
		if (!pImpl->m_Init) {
			SetToBefore();
			pImpl->m_Init = true;
		}
	}







	//--------------------------------------------------------------------------------------
	//	struct Transform::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Transform::Impl {
		bool m_PriorityPosition{ false };	//行列計算を位置優先にするかどうか
		weak_ptr<GameObject> m_Parent;	//親ゲームオブジェクト

		Matrix4X4 m_BeforeLocalMatrix{ Matrix4X4() };			//アップデート直前のローカル行列
		Vector3 m_BeforeScale{ 1.0f, 1.0f, 1.0f };			//スケーリングはローカルワールド共通	
		Quaternion m_BeforeLocalQuaternion{ Quaternion() };		//一つ前のローカルクオータニオン
		Quaternion m_BeforeWorldQuaternion{ Quaternion() };		//一つ前のワールドクオータニオン
		Vector3 m_BeforeLocalPosition{ 0, 0, 0 };	//一つ前のローカル位置
		Vector3 m_BeforeWorldPosition{ 0, 0, 0 };	//一つ前のワールド位置

		Matrix4X4 m_LocalMatrix{ Matrix4X4() };
		Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };		//スケーリングはローカルワールド共通
		Quaternion m_LocalQuaternion{ Quaternion() };
		Quaternion m_Quaternion{ Quaternion() };
		Vector3 m_LocalPosition{ 0, 0, 0 };
		Vector3 m_Position{ 0, 0, 0 };
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	用途: 変化
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		TransformMatrix(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform() {}

	//アクセサ
	bool Transform::IsPriorityPosition()const {
		return pImpl->m_PriorityPosition;
	}
	bool Transform::GetPriorityPosition()const {
		return pImpl->m_PriorityPosition;
	}
	void Transform::SetPriorityPosition(bool b) {
		pImpl->m_PriorityPosition = b;
	}


	//BeforeGetter
	const Matrix4X4& Transform::GetBeforeLocalMatrix() const {
		return pImpl->m_BeforeLocalMatrix;
	}
	const Vector3& Transform::GetBeforeScale() const {
		return pImpl->m_BeforeScale;
	}
	const Quaternion& Transform::GetBeforeWorldQuaternion() const {
		return pImpl->m_BeforeWorldQuaternion;
	}
	const Quaternion& Transform::GetBeforeLocalQuaternion() const {
		return pImpl->m_BeforeLocalQuaternion;
	}
	const Vector3& Transform::GetBeforeWorldPosition() const {
		return pImpl->m_BeforeWorldPosition;
	}
	const Vector3& Transform::GetBeforeLocalPosition() const {
		return pImpl->m_BeforeLocalPosition;
	}


	//Getter&Setter
	void Transform::SetWorldMatrix(const Matrix4X4& WorldMatrix) {
		throw BaseException(
			L"Transformでは、行列は直接変更できません",
			L"TransformMatrixコンポネントを使用してください",
			L"Transform::SetWorldMatrix()"
			);
	}

	const Vector3& Transform::GetScale() const {
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const Vector3& Scale) {
		//Scaleはローカルワールド共通
		pImpl->m_Scale = Scale;
		//WorldSetterはすぐに計算する
		CalcMatrix();
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(Vector3(x, y, z));
	}

	const Quaternion& Transform::GetQuaternion() const {
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const Quaternion& quaternion) {
		//入力はワールド座標である
		if (GetParent()) {
			auto Ptr = GetParent()->GetComponent<TransformMatrix>();
			Matrix4X4 Parmatrix = Ptr->GetWorldMatrix();
			//スケーリングは1.0にする
			Parmatrix.ScaleIdentity();
			Vector4 v;
			Parmatrix.Inverse(&v);
			Quaternion ParQt = Parmatrix.QtInMatrix();
			pImpl->m_LocalQuaternion = quaternion * ParQt;
		}
		else {
			pImpl->m_LocalQuaternion = quaternion;
		}
		//WorldSetterはすぐに計算する
		CalcMatrix();
	}
	Vector3 Transform::GetRotation() const {
		return pImpl->m_Quaternion.GetRotation();
	}

	void Transform::SetRotation(const Vector3& Rot) {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetQuaternion(Qt);
	}
	void Transform::SetRotation(float x, float y, float z) {
		SetRotation(Vector3(x, y, z));
	}

	const Vector3& Transform::GetPosition() const {
		return pImpl->m_Position;
	}

	void Transform::SetPosition(const Vector3& Position) {
		//入力はワールド座標である
		if (auto ParPtr = GetParent()) {
			Vector3 Temp = Position - pImpl->m_Position;
			pImpl->m_LocalPosition += Temp;
		}
		else {
			pImpl->m_LocalPosition = Position;
		}
		//Setterはすぐに計算する
		CalcMatrix();
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(Vector3(x, y, z));
	}

	const Matrix4X4& Transform::GetLocalMatrix() const {
		return pImpl->m_LocalMatrix;
	}

	const Quaternion& Transform::GetLocalQuaternion() const {
		return pImpl->m_LocalQuaternion;
	}
	void Transform::SetLocalQuaternion(const Quaternion& quaternion, bool CalcFlg) {
		pImpl->m_LocalQuaternion = quaternion;
		//Setterはすぐに計算する
		if (CalcFlg) {
			CalcMatrix();
		}
	}
	Vector3 Transform::GetLocalRotation() const {
		return pImpl->m_LocalQuaternion.GetRotation();
	}

	void Transform::SetLocalRotation(const Vector3& Rot, bool CalcFlg) {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetLocalQuaternion(Qt, CalcFlg);
	}
	void Transform::SetLocalRotation(float x, float y, float z, bool CalcFlg) {
		SetLocalRotation(Vector3(x, y, z), CalcFlg);
	}


	const Vector3& Transform::GetLocalPosition() const {
		return pImpl->m_LocalPosition;
	}

	void Transform::SetLocalPosition(const Vector3& Position, bool CalcFlg) {
		pImpl->m_LocalPosition = Position;
		//Setterはすぐに計算する
		if (CalcFlg) {
			CalcMatrix();
		}
	}

	void Transform::SetLocalPosition(float x, float y, float z, bool CalcFlg) {
		SetLocalPosition(Vector3(x, y, z), CalcFlg);
	}


	shared_ptr<GameObject> Transform::GetParent() const {
		if (!pImpl->m_Parent.expired()) {
			return pImpl->m_Parent.lock();
		}
		return nullptr;
	}
	void Transform::SetParent(const shared_ptr<GameObject>& Ptr) {
		if (Ptr) {
			if (!pImpl->m_Parent.expired()) {
				ClearParent();
			}
			pImpl->m_Parent = Ptr;
			auto ParTransPtr = Ptr->GetComponent<TransformMatrix>();
			Matrix4X4 ParMatrix = ParTransPtr->GetWorldMatrix();
			ParMatrix.ScaleIdentity();
			Vector4 Vec;
			ParMatrix.Inverse(&Vec);
			auto Matrix = GetLocalMatrix() * ParMatrix;
			//設定時は計算しない
			SetLocalQuaternion(Matrix.QtInMatrix(), false);
			SetLocalPosition(Matrix.PosInMatrix(), false);
			CalcMatrix();
		}
		else {
			ClearParent();
		}
	}
	void Transform::ClearParent() {
		pImpl->m_Parent.reset();
		//設定時は計算しない
		SetLocalQuaternion(GetQuaternion(), false);
		SetLocalPosition(GetPosition(), false);
		CalcMatrix();
	}


	//計算
	void Transform::CalcMatrix() {
		if (IsGameObjectActive()) {
			if (pImpl->m_PriorityPosition) {
				pImpl->m_LocalMatrix.STRTransformation(
					pImpl->m_Scale,
					pImpl->m_LocalPosition,
					pImpl->m_LocalQuaternion
					);
			}
			else {
				pImpl->m_LocalMatrix.DefTransformation(
					pImpl->m_Scale,
					pImpl->m_LocalQuaternion,
					pImpl->m_LocalPosition
					);
			}
			//親がある場合は、各値は親の配下になる。
			if (GetParent()) {
				auto PtrTrans = GetParent()->GetComponent<TransformMatrix>();
				Matrix4X4 Parmatrix = PtrTrans->GetWorldMatrix();
				//スケーリングは1.0にする
				Parmatrix.ScaleIdentity();
				TransformMatrix::SetWorldMatrix(pImpl->m_LocalMatrix * Parmatrix);
				pImpl->m_Quaternion = GetWorldMatrix().QtInMatrix();
				pImpl->m_Position = GetWorldMatrix().PosInMatrix();
			}
			else {
				//親がない
				TransformMatrix::SetWorldMatrix(pImpl->m_LocalMatrix);
				pImpl->m_Quaternion = pImpl->m_LocalQuaternion;
				pImpl->m_Position = pImpl->m_LocalPosition;
			}
		}
	}

	void Transform::SetToBefore() {
		if (GetGameObject()) {
			auto PtrRidid = GetGameObject()->GetComponent<Rigidbody>(false);
			if (PtrRidid && !IsInit()) {
				float Len = Vector3EX::Length(PtrRidid->GetVelocity());
				float PosLen = Vector3EX::Length(pImpl->m_Position - pImpl->m_BeforeWorldPosition);
				if (PtrRidid->IsVelocityZeroCommand() && Len > 0 && PosLen <= 0) {
					PtrRidid->SetVelocity(0, 0, 0);
				}
			}
		}
		pImpl->m_BeforeLocalMatrix = pImpl->m_LocalMatrix;
		TransformMatrix::SetToBefore();
		pImpl->m_BeforeScale = pImpl->m_Scale;
		pImpl->m_BeforeLocalQuaternion = pImpl->m_LocalQuaternion;
		pImpl->m_BeforeWorldQuaternion = pImpl->m_Quaternion;
		pImpl->m_BeforeLocalPosition = pImpl->m_LocalPosition;
		pImpl->m_BeforeWorldPosition = pImpl->m_Position;
	}

	void Transform::LerpBeforeToNow(float LerpTime) {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//LerpTimeは衝突タイムなので、少し前に置く
		if (LerpTime >= 0.001f) {
			LerpTime -= 0.001f;
		}

		pImpl->m_Scale = Lerp::CalculateLerp<Vector3>(
			pImpl->m_BeforeScale, pImpl->m_Scale, 0, ElapsedTime, LerpTime, Lerp::Linear
			);

		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_BeforeLocalQuaternion,
			pImpl->m_LocalQuaternion,
			LerpTime / ElapsedTime
			);
		Qt.Normalize();
		pImpl->m_LocalQuaternion = Qt;

		pImpl->m_LocalPosition = Lerp::CalculateLerp<Vector3>(
			pImpl->m_BeforeLocalPosition, pImpl->m_LocalPosition, 0, ElapsedTime, LerpTime, Lerp::Linear
			);
		CalcMatrix();
	}
	//操作
	void Transform::OnUpdate() {
		CalcMatrix();
		if (!IsInit()) {
			SetToBefore();
			SetInitTrue();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ActionComponent::Impl{
		bool m_Run;			//移動中かどうか
		bool m_Arrived;	//到着したかどうか
		float m_TotalTime;	//移動にかける時間
		float m_NowTime;	//現在の時間
		Impl() :
			m_Run(false),
			m_Arrived(false),
			m_TotalTime(0),
			m_NowTime(0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	用途: アクション系コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionComponent::ActionComponent(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	ActionComponent::~ActionComponent(){}

	//アクセサ
	void ActionComponent::SetRun(bool b){
		pImpl->m_Run = b;
	}
	void  ActionComponent::SetArrived(bool b){
		pImpl->m_Arrived = b;
	}


	bool ActionComponent::IsRun()const{
		return pImpl->m_Run;
	}
	bool ActionComponent::GetRun()const{
		return pImpl->m_Run;
	}

	bool ActionComponent::IsArrived()const{
		return pImpl->m_Arrived;
	}
	bool ActionComponent::GetArrived()const{
		return pImpl->m_Arrived;
	}

	float ActionComponent::GetTotalTime() const{
		return pImpl->m_TotalTime;
	}
	void ActionComponent::SetTotalTime(float f){
		pImpl->m_TotalTime = f;
	}

	float ActionComponent::GetNowTime() const{
		return pImpl->m_NowTime;
	}
	void ActionComponent::SetNowTime(float f){
		pImpl->m_NowTime = f;
	}

	//NowTimeを加算して、TotalTimeと比較する
	bool ActionComponent::AdditionalNowTime(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive() && pImpl->m_Run){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr && pImpl->m_TotalTime > 0){
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime){
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//到着した
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}


	void ActionComponent::Stop(){
		pImpl->m_Run = false;
	}
	void ActionComponent::ReStart(){
		if (!pImpl->m_Arrived){
			//到着してなければ動作させる
			pImpl->m_Run = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class ActionInterval : public  ActionComponent ;
	//	用途: アクションのインターバル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActionInterval::ActionInterval(const shared_ptr<GameObject>& GameObjectPtr):
		ActionComponent(GameObjectPtr)
	{}
	ActionInterval::~ActionInterval(){}

	void ActionInterval::SetParams(float TotalTime){
		SetTotalTime(TotalTime);
	}

	void ActionInterval::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}
	void ActionInterval::OnUpdate(){
		if (IsGameObjectActive() && GetRun()){
			if (!AdditionalNowTime()){
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct ScaleComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ScaleComponent::Impl{
		Vector3 m_StartScale;	//開始倍率
		Vector3 m_TargetScale;	//目的倍率
		Lerp::rate m_Rate;	//補間方法
		Impl() :
			m_StartScale(1.0f, 1.0f, 1.0f),
			m_TargetScale(1.0f, 1.0f, 1.0f),
			m_Rate(Lerp::Linear)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleComponent : public Component ;
	//	用途: 拡大縮小アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ScaleComponent::ScaleComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	ScaleComponent::~ScaleComponent(){}


	//アクセサ
	Lerp::rate ScaleComponent::GetRate() const{ return pImpl->m_Rate; }
	void ScaleComponent::SetRate(const Lerp::rate r){ pImpl->m_Rate = r; }

	const Vector3& ScaleComponent::GetStartScale() const{ return pImpl->m_StartScale; }
	void ScaleComponent::SetStartScale(const Vector3& StartPosition){ pImpl->m_StartScale = StartPosition; }
	void ScaleComponent::SetStartScale(float x, float y, float z){ pImpl->m_StartScale = Vector3(x, y, z); }

	const Vector3& ScaleComponent::GetTargetScale() const{ return pImpl->m_TargetScale; }
	void ScaleComponent::SetTargetScale(const Vector3& TargetScale){ pImpl->m_TargetScale = TargetScale; }
	void ScaleComponent::SetTargetScale(float x, float y, float z){ pImpl->m_TargetScale = Vector3(x, y, z); }

	//操作
	void ScaleComponent::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_StartScale = TransPtr->GetScale();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	void ScaleComponent::OnUpdate(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Vector3 Scale = Lerp::CalculateLerp<Vector3>(
					pImpl->m_StartScale,
					pImpl->m_TargetScale,
					0,
					GetTotalTime(),
					GetNowTime(),
					pImpl->m_Rate
					);
				TransPtr->SetScale(Scale);
			}
			else{
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetScale(pImpl->m_TargetScale);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//現在スケールを計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 ScaleComponent::GetNowScale() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetScale;
		}
		Vector3 Scale = Lerp::CalculateLerp(
			pImpl->m_StartScale,
			pImpl->m_TargetScale,
			0,
			GetTotalTime(),
			GetNowTime(),
			pImpl->m_Rate
			);
		return Scale;
	}



	//--------------------------------------------------------------------------------------
	//	class ScaleTo : public  ScaleComponent ;
	//	用途: 目的の拡大率に拡大縮小
	//--------------------------------------------------------------------------------------
	ScaleTo::ScaleTo(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr){}
	ScaleTo::~ScaleTo(){}
	void ScaleTo::SetParams(float TotalTime, const Vector3& TargetScale, Lerp::rate Rate){
		SetTargetScale(TargetScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}

	//--------------------------------------------------------------------------------------
	//	struct ScaleBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct ScaleBy::Impl{
		Vector3 m_LocalScale;	//相対スケール
	public:
		Impl() :
			m_LocalScale(1.0f, 1.0f, 1.0f)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class ScaleBy : public  ScaleComponent;
	//	用途: 目的の相対スケールにに拡大縮小
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ScaleBy::ScaleBy(const shared_ptr<GameObject>& GameObjectPtr) :
		ScaleComponent(GameObjectPtr), pImpl(new Impl()){}
	ScaleBy::~ScaleBy(){}


	//アクセサ
	const Vector3& ScaleBy::GetLocalScale() const{ return pImpl->m_LocalScale; }
	void ScaleBy::SetLocalScale(const Vector3& LocalScale){ pImpl->m_LocalScale = LocalScale; }
	void ScaleBy::SetLocalScale(float x, float y, float z){ pImpl->m_LocalScale = Vector3(x, y, z); }

	void ScaleBy::SetParams(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate){
		SetLocalScale(LocalScale);
		SetTotalTime(TotalTime);
		SetRate(Rate);
	}


	//操作
	void ScaleBy::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Vector3 Target = TransPtr->GetScale();
				Target.x *= pImpl->m_LocalScale.x;
				Target.y *= pImpl->m_LocalScale.y;
				Target.z *= pImpl->m_LocalScale.z;
				SetTargetScale(Target);
				ScaleComponent::Run();
			}
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct RotateComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct RotateComponent::Impl{
		Quaternion m_StartQuaternion;	//開始回転
		Quaternion m_TargetQuaternion;	//終了回転
	public:
		Impl() :
			m_StartQuaternion(),
			m_TargetQuaternion()
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class RotateComponent : public ActionComponent ;
	//	用途: 回転アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateComponent::RotateComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{
	}
	RotateComponent::~RotateComponent(){}


	//アクセサ
	const Quaternion& RotateComponent::GetStartQuaternion() const{ return pImpl->m_StartQuaternion; }
	void RotateComponent::SetStartQuaternion(const Quaternion& StartQuaternion){
		pImpl->m_StartQuaternion = StartQuaternion;
		pImpl->m_StartQuaternion.Normalize();
	}

	Vector3 RotateComponent::GetStartRotate() const{
		return pImpl->m_StartQuaternion.GetRotation();
	}
	void RotateComponent::SetStartRotate(const Vector3& StartRotate){
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(StartRotate);
		pImpl->m_StartQuaternion.Normalize();
	}
	void RotateComponent::SetStartRotate(float x, float y, float z){
		pImpl->m_StartQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_StartQuaternion.Normalize();
	}

	const Quaternion& RotateComponent::GetTargetQuaternion() const{
		return pImpl->m_TargetQuaternion;
	}
	void RotateComponent::SetTargetQuaternion(const Quaternion& TargetQuaternion){
		pImpl->m_TargetQuaternion = TargetQuaternion;
		pImpl->m_TargetQuaternion.Normalize();
	}
	Vector3 RotateComponent::GetTargetRotate() const{
		return pImpl->m_TargetQuaternion.GetRotation();
	}
	void RotateComponent::SetTargetRotate(const Vector3& TargetRotate){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(TargetRotate);
		pImpl->m_TargetQuaternion.Normalize();
	}
	void RotateComponent::SetTargetRotate(float x, float y, float z){
		pImpl->m_TargetQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_TargetQuaternion.Normalize();
	}

	//操作
	void RotateComponent::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				//クオータニオンから現在の回転ベクトルを得る
				pImpl->m_StartQuaternion = TransPtr->GetQuaternion();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}
	void RotateComponent::OnUpdate(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Quaternion Qt = QuaternionEX::Slerp(
					pImpl->m_StartQuaternion,
					pImpl->m_TargetQuaternion,
					GetNowTime() / GetTotalTime()
					);
				Qt.Normalize();
				TransPtr->SetQuaternion(Qt);
			}
			else{
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetQuaternion(pImpl->m_TargetQuaternion);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
		}
	}

	//現在回転を計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 RotateComponent::GetNowRotate() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetQuaternion.GetRotation();
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
			);
		Qt.Normalize();
		return Qt.GetRotation();
	}
	Quaternion RotateComponent::GetNowQuaternion() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetQuaternion;
		}
		Quaternion Qt = QuaternionEX::Slerp(
			pImpl->m_StartQuaternion,
			pImpl->m_TargetQuaternion,
			GetNowTime() / GetTotalTime()
			);
		Qt.Normalize();
		return Qt;
	}



	//--------------------------------------------------------------------------------------
	//	class RotateTo : public  MoveComponent ;
	//	用途: 目的の角度に回転
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateTo::RotateTo(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr)
	{}
	RotateTo::~RotateTo(){}

	void RotateTo::SetParams(float TotalTime, const Vector3& TargetRotate){
		SetTargetRotate(TargetRotate);
		SetTotalTime(TotalTime);
	}

	void RotateTo::SetParams(float TotalTime, const Quaternion& TargetQuaternion){
		SetTargetQuaternion(TargetQuaternion);
		SetTotalTime(TotalTime);
	}


	//--------------------------------------------------------------------------------------
	//	struct RotateBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct RotateBy::Impl{
		Quaternion m_LocalQuaternion;	//相対回転
		Impl() :
			m_LocalQuaternion()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class RotateBy : public  RotateComponent ;
	//	用途: 目的の相対角度に回転
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RotateBy::RotateBy(const shared_ptr<GameObject>& GameObjectPtr) :
		RotateComponent(GameObjectPtr), pImpl(new Impl()){}
	RotateBy::~RotateBy(){}

	//アクセサ
	const Quaternion& RotateBy::GetLocalQuaternion() const{ return pImpl->m_LocalQuaternion; }
	void RotateBy::SetLocalQuaternion(const Quaternion& LocalQuaternion){
		pImpl->m_LocalQuaternion = LocalQuaternion;
		pImpl->m_LocalQuaternion.Normalize();
	}
	Vector3 RotateBy::GetLocalRotate() const{
		return pImpl->m_LocalQuaternion.GetRotation();
	}
	void RotateBy::SetLocalRotate(const Vector3& LocalRotate){
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(LocalRotate);
		pImpl->m_LocalQuaternion.Normalize();
	}
	void RotateBy::SetLocalRotate(float x, float y, float z){
		pImpl->m_LocalQuaternion.RotationRollPitchYawFromVector(Vector3(x, y, z));
		pImpl->m_LocalQuaternion.Normalize();
	}

	void RotateBy::SetParams(float TotalTime, const Vector3& LocalRotate){
		SetTotalTime(TotalTime);
		SetLocalRotate(LocalRotate);
	}

	void RotateBy::SetParams(float TotalTime, const Quaternion& LocalQuaternion){
		SetTotalTime(TotalTime);
		SetLocalQuaternion(LocalQuaternion);
	}


	//操作
	void RotateBy::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Quaternion NowQt = TransPtr->GetQuaternion();
				Quaternion Target = NowQt * pImpl->m_LocalQuaternion;
				SetTargetQuaternion(Target);
				RotateComponent::Run();
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct MoveComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MoveComponent::Impl{
		Vector3 m_StartPosition;	//開始地点
		Vector3 m_TargetPosition;	//目的地点
		Lerp::rate m_RateX;	//補間方法X
		Lerp::rate m_RateY;	//補間方法Y
		Lerp::rate m_RateZ;	//補間方法Z
		Vector3 m_Velocity;	//現在の速度
		Impl() :
			m_StartPosition(0, 0, 0),
			m_TargetPosition(0, 0, 0),
			m_RateX(Lerp::Linear),
			m_RateY(Lerp::Linear),
			m_RateZ(Lerp::Linear),
			m_Velocity(0,0,0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveComponent : public ActionComponent ;
	//	用途: 移動アクションの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveComponent::MoveComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		ActionComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	MoveComponent::~MoveComponent(){}
	//アクセサ

	Lerp::rate MoveComponent::GetRateX() const{
		return pImpl->m_RateX;
	}
	Lerp::rate MoveComponent::GetRateY() const{
		return pImpl->m_RateY;
	}
	Lerp::rate MoveComponent::GetRateZ() const{
		return pImpl->m_RateZ;
	}
	void MoveComponent::SetRateX(const Lerp::rate r){
		pImpl->m_RateX = r;
	}
	void MoveComponent::SetRateY(const Lerp::rate r){
		pImpl->m_RateY = r;
	}
	void MoveComponent::SetRateZ(const Lerp::rate r){
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate r){
		pImpl->m_RateX = r;
		pImpl->m_RateY = r;
		pImpl->m_RateZ = r;
	}
	void MoveComponent::SetRateAll(const Lerp::rate rX, const Lerp::rate rY, const Lerp::rate rZ){
		pImpl->m_RateX = rX;
		pImpl->m_RateY = rY;
		pImpl->m_RateZ = rZ;
	}
	
	Vector3 MoveComponent::GetVelocity()const{
		Vector3 Ret = pImpl->m_Velocity;
		if (GetTotalTime() <= 0){
			Ret.Zero();
		}
		else{
			if (!GetRun() || GetArrived()){
				Ret.Zero();
			}
		}
		return Ret;
	}
	


	const Vector3& MoveComponent::GetStartPosition() const{ return pImpl->m_StartPosition; }
	void MoveComponent::SetStartPosition(const Vector3& StartPosition){ pImpl->m_StartPosition = StartPosition; }
	void MoveComponent::SetStartPosition(float x, float y, float z){ pImpl->m_StartPosition = Vector3(x, y, z); }

	const Vector3& MoveComponent::GetTargetPosition() const{ return pImpl->m_TargetPosition; }
	void MoveComponent::SetTargetPosition(const Vector3& TargetPosition){ pImpl->m_TargetPosition = TargetPosition; }
	void MoveComponent::SetTargetPosition(float x, float y, float z){ pImpl->m_TargetPosition = Vector3(x, y, z); }

	Vector3 MoveComponent::CalcVelocitySub(float NowTime){
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
		}
		else{
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				NowTime,
				pImpl->m_RateZ
				);
		}
		return Pos;
	}


	void MoveComponent::CalcVelocity(){
		if (GetTotalTime() <= 0){
			pImpl->m_Velocity = Vector3(0, 0, 0);
			return;
		}
		else{
			if (!GetRun() || GetArrived()){
				pImpl->m_Velocity = Vector3(0, 0, 0);
				return;
			}
			else{
				float NowTime = GetNowTime();
				if (NowTime <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				if (ElapsedTime <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				float BeforeTime = GetNowTime() - App::GetApp()->GetElapsedTime();
				if (BeforeTime <= 0){
					BeforeTime = 0;
				}
				if ((NowTime - BeforeTime) <= 0){
					pImpl->m_Velocity = Vector3(0, 0, 0);
					return;
				}
				Vector3 BeforePos = CalcVelocitySub(BeforeTime);
				Vector3 NowPos = CalcVelocitySub(NowTime);
				pImpl->m_Velocity = (NowPos - BeforePos) / ElapsedTime;
				return;
			}
		}
	}


	//操作
	void MoveComponent::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_StartPosition = TransPtr->GetPosition();
				SetNowTime(0);
				SetRun(true);
				SetArrived(false);
				if (GetTotalTime() <= 0){
					//トータルタイムが0に設定されている
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	void MoveComponent::OnUpdate(){
		if (IsGameObjectActive() && GetRun()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (AdditionalNowTime()){
				Vector3 Pos;
				if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
					Pos = Lerp::CalculateLerp<Vector3>(
						pImpl->m_StartPosition,
						pImpl->m_TargetPosition,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
				}
				else{
					Pos.x = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.x,
						pImpl->m_TargetPosition.x,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateX
						);
					Pos.y = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.y,
						pImpl->m_TargetPosition.y,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateY
						);
					Pos.z = Lerp::CalculateLerp<float>(
						pImpl->m_StartPosition.z,
						pImpl->m_TargetPosition.z,
						0,
						GetTotalTime(),
						GetNowTime(),
						pImpl->m_RateZ
						);
				}
				TransPtr->SetPosition(Pos);
			}
			else{
				if (GetTotalTime() <= 0){
					//トータルタイムが0以下なら、すでに到着していることになる
					TransPtr->SetPosition(pImpl->m_TargetPosition);
					SetTotalTime(0);
					SetNowTime(0);
					SetRun(false);
					SetArrived(true);
				}
			}
			CalcVelocity();
		}
	}

	//現在位置を計算して返す
	//タイムの更新は行わないので
	//Update後に呼ぶべき
	Vector3 MoveComponent::GetNowPosition() const{
		if (GetTotalTime() <= 0){
			return pImpl->m_TargetPosition;
		}
		Vector3 Pos;
		if (pImpl->m_RateX == pImpl->m_RateY && pImpl->m_RateX == pImpl->m_RateZ){
			Pos = Lerp::CalculateLerp<Vector3>(
				pImpl->m_StartPosition,
				pImpl->m_TargetPosition,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
		}
		else{
			Pos.x = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.x,
				pImpl->m_TargetPosition.x,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateX
				);
			Pos.y = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.y,
				pImpl->m_TargetPosition.y,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateY
				);
			Pos.z = Lerp::CalculateLerp<float>(
				pImpl->m_StartPosition.z,
				pImpl->m_TargetPosition.z,
				0,
				GetTotalTime(),
				GetNowTime(),
				pImpl->m_RateZ
				);
		}
		return Pos;
	}



	//--------------------------------------------------------------------------------------
	//	class MoveTo : public  MoveComponent ;
	//	用途: 目的の位置に移動
	//--------------------------------------------------------------------------------------
	MoveTo::MoveTo(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr){}
	MoveTo::~MoveTo(){}

	void MoveTo::SetParams(float TotalTime, const Vector3& TargetPosition, Lerp::rate Rate){
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveTo::SetParams(float TotalTime, const Vector3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		SetTargetPosition(TargetPosition);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}



	//--------------------------------------------------------------------------------------
	//	struct MoveBy::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MoveBy::Impl{
		Vector3 m_LocalVector;	//相対方向
		Impl() :
			m_LocalVector(0, 0, 0)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBy : public  MoveComponent ;
	//	用途: 目的の相対位置に移動
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBy::MoveBy(const shared_ptr<GameObject>& GameObjectPtr) :
		MoveComponent(GameObjectPtr), pImpl(new Impl()){}
	MoveBy::~MoveBy(){}

	//アクセサ
	const Vector3& MoveBy::GetLocalVector() const{ return pImpl->m_LocalVector; }
	void MoveBy::SetLocalVector(const Vector3& LocalVector){ pImpl->m_LocalVector = LocalVector; }
	void MoveBy::SetLocalVector(float x, float y, float z){ pImpl->m_LocalVector = Vector3(x, y, z); }

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate){
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateAll(Rate);
	}

	void MoveBy::SetParams(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		SetLocalVector(LocalVector);
		SetTotalTime(TotalTime);
		SetRateX(RateX);
		SetRateY(RateY);
		SetRateZ(RateZ);
	}

	//操作
	void MoveBy::Run(){
		//m_GameObjectがnullならUpdateしても意味がない
		if (IsGameObjectActive()){
			auto TransPtr = GetGameObject()->GetComponent<Transform>();
			if (TransPtr){
				Vector3 Target = TransPtr->GetPosition() + pImpl->m_LocalVector;
				SetTargetPosition(Target);
				MoveComponent::Run();
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct Action::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Action::Impl{
		bool m_Looped;

		bool m_ScaleArrived;	//m_Loopがfalseのときのみ有効
		bool m_RotateArrived;	//m_Loopがfalseのときのみ有効
		bool m_MoveArrived;	//m_Loopがfalseのときのみ有効


		vector<shared_ptr<ActionComponent> > m_ScaleVec;
		vector<shared_ptr<ActionComponent> > m_RotateVec;
		vector<shared_ptr<ActionComponent> > m_MoveVec;

		size_t m_ScaleActiveIndex;
		size_t m_RotateActiveIndex;
		size_t m_MoveActiveIndex;

		Impl() :
			m_Looped(false),
			m_ScaleArrived(false),
			m_RotateArrived(false),
			m_MoveArrived(false),
			m_ScaleActiveIndex(0),
			m_RotateActiveIndex(0),
			m_MoveActiveIndex(0)
		{}
		~Impl(){}

	};

	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	用途: 自動状態変更コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Action::Action(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Action::~Action(){}

	//アクセサ
	bool Action::IsLooped()const{ return pImpl->m_Looped; }
	bool Action::GetLooped()const{ return pImpl->m_Looped; }
	void Action::SetLooped(bool b){ pImpl->m_Looped = b; }

	bool Action::IsArrived()const{
		bool retScale = true;
		if (pImpl->m_ScaleVec.size() > 0){
			//アクションがあるが終了してない
			if (!pImpl->m_ScaleArrived){
				retScale = false;
			}
		}
		bool retRot = true;
		if (pImpl->m_RotateVec.size() > 0){
			//アクションがあるが終了してない
			if (!pImpl->m_RotateArrived){
				retRot = false;
			}
		}
		bool retMove = true;
		if (pImpl->m_MoveVec.size() > 0){
			//アクションがあるが終了してない
			if (!pImpl->m_MoveArrived){
				retMove = false;
			}
		}
		return
			retScale &&
			retRot &&
			retMove;
	}
	bool Action::GetArrived()const{ return IsArrived(); }


	size_t Action::GetScaleActiveIndex() const {
		if (pImpl->m_ScaleVec.size() <= 0){
			throw BaseException(
				L"スケールアクションが登録されていません",
				L"if (pImpl->m_ScaleVec.size() <= 0)",
				L"Action::GetScaleActiveIndex()"
				);
		}
		return pImpl->m_ScaleActiveIndex;
	}
	size_t Action::GetRotateActiveIndex() const {
		if (pImpl->m_RotateVec.size() <= 0){
			throw BaseException(
				L"回転アクションが登録されていません",
				L"if (pImpl->m_RotateVec.size() <= 0)",
				L"Action::GetRotateActiveIndex()"
				);
		}
		return pImpl->m_RotateActiveIndex;
	}
	size_t Action::GetMoveActiveIndex() const {
		if (pImpl->m_MoveVec.size() <= 0){
			throw BaseException(
				L"移動アクションが登録されていません",
				L"if (pImpl->m_MoveVec.size() <= 0)",
				L"Action::GetMoveActiveIndex()"
				);
		}
		return pImpl->m_MoveActiveIndex;
	}

	//現在の速度
	//Moveコンポーネントがない場合はVector3(0,0,0)を返す
	
	Vector3 Action::GetVelocity()const{
		if (pImpl->m_MoveVec.size() <= 0){
			return Vector3(0, 0, 0);
		}
		else{
			auto Ptr = dynamic_pointer_cast<MoveComponent>(pImpl->m_MoveVec[pImpl->m_MoveActiveIndex]);
			if (Ptr){
				return Ptr->GetVelocity();
			}
			else{
				return Vector3(0, 0, 0);
			}
		}
	}
	


	//操作
	shared_ptr<ScaleTo> Action::AddScaleTo(float TotalTime, const Vector3& TargetScale, Lerp::rate Rate){
		auto Ptr = ObjectFactory::Create<ScaleTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}
	shared_ptr<ScaleBy> Action::AddScaleBy(float TotalTime, const Vector3& LocalScale, Lerp::rate Rate){
		auto Ptr = ObjectFactory::Create<ScaleBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalScale, Rate);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionInterval> Action::AddScaleInterval(float TotalTime){
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_ScaleVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetScaleComponent(size_t TargetIndex)const{
		if (TargetIndex >= pImpl->m_ScaleVec.size()){
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_ScaleVec.size())",
				L"Action::GetScaleComponent()"
				);
		}
		return pImpl->m_ScaleVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetScaleVec() const{
		return pImpl->m_ScaleVec;
	}


	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Vector3& TargetRotate){
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateTo> Action::AddRotateTo(float TotalTime, const Quaternion& TargetQuaternion){
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Vector3& LocalRotate){
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalRotate);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<RotateBy> Action::AddRotateBy(float TotalTime, const Quaternion& LocalQuaternion){
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalQuaternion);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddRotateInterval(float TotalTime){
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_RotateVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetRotateComponent(size_t TargetIndex)const{
		if (TargetIndex >= pImpl->m_RotateVec.size()){
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_RotateVec.size())",
				L"Action::GetRotateComponent()"
				);
		}
		return pImpl->m_RotateVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetRotateVec() const{
		return pImpl->m_RotateVec;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& TargetPosition, Lerp::rate Rate){
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveTo> Action::AddMoveTo(float TotalTime, const Vector3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate Rate){
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, Rate);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<MoveBy> Action::AddMoveBy(float TotalTime, const Vector3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ){
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, RateX, RateY, RateZ);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Action::AddMoveInterval(float TotalTime){
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->m_MoveVec.push_back(Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Action::GetMoveComponent(size_t TargetIndex)const{
		if (TargetIndex >= pImpl->m_MoveVec.size()){
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_MoveVec.size())",
				L"Action::GetMoveComponent()"
				);
		}
		return pImpl->m_MoveVec[TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Action::GetMoveVec() const{
		return pImpl->m_MoveVec;
	}
	//すべてのアクションをクリアする
	void Action::AllActionClear(){
		pImpl->m_ScaleVec.clear();
		pImpl->m_RotateVec.clear();
		pImpl->m_MoveVec.clear();

		pImpl->m_Looped = false;
		pImpl->m_ScaleArrived = false;
		pImpl->m_RotateArrived = false;
		pImpl->m_MoveArrived = false;
		pImpl->m_ScaleActiveIndex = 0;
		pImpl->m_RotateActiveIndex = 0;
		pImpl->m_MoveActiveIndex = 0;
	}



	void Action::RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0){
			TgtIndex = 0;
			TgtVector[TgtIndex]->Run();
		}
	}
	void Action::Run(){
		RunSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		RunSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		RunSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			TgtVector[TgtIndex]->Stop();
		}
	}
	void Action::Stop(){
		StopSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		StopSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		StopSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	void Action::ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			TgtVector[TgtIndex]->ReStart();
		}
	}

	void Action::ReStart(){
		ReStartSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		ReStartSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		ReStartSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	bool Action::UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex){
		bool ret = false;
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()){
			if (TgtVector[TgtIndex]->IsUpdateActive()){
				TgtVector[TgtIndex]->OnUpdate();
				if (TgtVector[TgtIndex]->IsArrived()){
					//到着した
					//インデックスを加算
					TgtIndex++;
					if (TgtIndex >= TgtVector.size()){
						//範囲外なら
						if (pImpl->m_Looped){
							TgtIndex = 0;
							TgtVector[TgtIndex]->Run();
						}
						else{
							TgtIndex = TgtVector.size() - 1;
							//Runは行わない
							//到着した
							ret = true;
						}
					}
					else{
						TgtVector[TgtIndex]->Run();
					}
				}
			}
		}
		return ret;
	}
	//更新
	void Action::OnUpdate(){
		pImpl->m_ScaleArrived = UpdateSub(pImpl->m_ScaleVec, pImpl->m_ScaleActiveIndex);
		pImpl->m_RotateArrived = UpdateSub(pImpl->m_RotateVec, pImpl->m_RotateActiveIndex);
		pImpl->m_MoveArrived = UpdateSub(pImpl->m_MoveVec, pImpl->m_MoveActiveIndex);
	}

	//--------------------------------------------------------------------------------------
	//	struct MultiView::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MultiView::Impl{
		vector< shared_ptr<View> > m_ViewVec;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiView: public Component;
	//	用途: マルチビューコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiView::MultiView(const shared_ptr<GameObject>& GameObjectPtr)
		: Component(GameObjectPtr),
		pImpl(new Impl())
	{
		try{
			auto Ptr = dynamic_pointer_cast<Stage>(GameObjectPtr);
			if (!Ptr){
				throw BaseException(
					L"ゲームオブジェクトがステージではありません",
					L"MultiViewはステージにのみ追加できます",
					L"MultiView::MultiView()"
					);
			}
		}
		catch (...){
			throw;
		}
	}
	MultiView::~MultiView(){}
	//アクセサ
	size_t MultiView::GetViewCount()const{
		return pImpl->m_ViewVec.size(); 
	}
	//ViewVecを直接操作する時用
	vector< shared_ptr<View> >& MultiView::GetViewVec()const{
		return pImpl->m_ViewVec;
	}
	shared_ptr<View> MultiView::GetView(size_t Index) const {
		if (Index >= pImpl->m_ViewVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_ViewVec.size())";
			throw BaseException(
				L"インデックスが範囲外です",
				str,
				L"MultiView::GetView()"
				);
		}
		return pImpl->m_ViewVec[Index];
	}
	//操作
	shared_ptr<View> MultiView::AddView(){
		auto Ptr = make_shared<View>();
		pImpl->m_ViewVec.push_back(Ptr);
		return Ptr;
	}

	void MultiView::OnUpdate(){
		for (auto Ptr : pImpl->m_ViewVec){
			Ptr->OnUpdate();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PlayMusic::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct PlayMusic::Impl{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//リソースキー
		wstring m_ResKey;
		Impl(const wstring& ResKey) :
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl(){
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PlayMusic: public Component;
	//	用途: ミュージックコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PlayMusic::PlayMusic(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey) :
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try{
			auto MusicRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = MusicRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetMusicEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx()),
				L"ミュージック用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx())",
				L"PlayMusic::PlayMusic()"
				);
		}
		catch (...){
			throw;
		}
	}
	PlayMusic::~PlayMusic(){
	}

	shared_ptr<AudioResource> PlayMusic::GetAudioResource() const{
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::GetAudioResource()"
				);
		}
		return pImpl->m_AudioResource.lock();
	}


	void PlayMusic::Start(size_t LoopCount, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//多重定義関数を呼ぶ
		Start(buffer, Volume);
	}

	void PlayMusic::Start(const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"ミュージック用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"ミュージック用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
			);
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
				);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"ミュージックのボリューム設定に失敗しました",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"PlayMusic::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"ミュージックのソースバッファ設定に失敗しました",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"PlayMusic::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"ミュージックサウンドのスタートに失敗しました",
			L"pImpl->m_pSourceVoice->Start()",
			L"PlayMusic::Start()"
			);

	}


	void PlayMusic::Stop(){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"ミュージック用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"ミュージック用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
			);
	}





	//--------------------------------------------------------------------------------------
	//	struct SoundEffect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SoundEffect::Impl{
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//リソースキー
		wstring m_ResKey;
		Impl(const wstring& ResKey):
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl(){
		}
	};



	//--------------------------------------------------------------------------------------
	//	class SoundEffect: public Component;
	//	用途: サウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SoundEffect::SoundEffect(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey):
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try{
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"SoundEffect::SoundEffect()"
				);
		}
		catch (...){
			throw;
		}
	}
	SoundEffect::~SoundEffect(){
	}

	shared_ptr<AudioResource> SoundEffect::GetAudioResource() const{
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::GetAudioResource()"
				);
		}
		return pImpl->m_AudioResource.lock();
	}


	void SoundEffect::Start(size_t LoopCount, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//多重定義関数を呼ぶ
		Start(buffer, Volume);
	}

	void SoundEffect::Start(const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Start()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Start()"
			);
		if (pImpl->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
				);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"SoundEffect::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundEffect::Start()"
			);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pImpl->m_pSourceVoice->Start()",
			L"SoundEffect::Start()"
			);

	}


	void SoundEffect::Stop(){
		if (!pImpl->m_pSourceVoice){
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Stop()"
			);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Stop()"
			);
	}




	//--------------------------------------------------------------------------------------
	//	struct MultiSoundEffect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MultiSoundEffect::Impl{
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl(){
		}
		SoundItem* GetItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//同じ名前が見つかった
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//同じ名前が見つかった
				//Mapから削除
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);

	};

	void MultiSoundEffect::Impl::Start(const SoundItem* pItem,const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Start()"
			);
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiSoundEffect::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiSoundEffect::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiSoundEffect::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiSoundEffect::Impl::Start()"
			);
	}

	void MultiSoundEffect::Impl::Stop(const SoundItem* pItem){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Stop()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Stop()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Stop()"
			);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	用途: マルチサウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSoundEffect::MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	MultiSoundEffect::~MultiSoundEffect(){}

	//アクセサ
	shared_ptr<AudioResource> MultiSoundEffect::AddAudioResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::AddAudioResource()"
					);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem){
				//同じ名前が見つかった
				if (!pItem->m_AudioResource.expired()){
					return pItem->m_AudioResource.lock();
				}
				else{
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
						);
				}
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();
			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiSoundEffect::AddAudioResource()"
				);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...){
			throw;
		}
	}
	void MultiSoundEffect::RemoveAudioResource(const wstring& ResKey){
		if (ResKey == L""){
			throw BaseException(
				L"オーディオリソース名が空白です",
				L"if (ResKey == L\"\")",
				L"MultiSoundEffect::RemoveAudioResource()"
				);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiSoundEffect::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const{
		if (ResKey == L""){
			if (ExceptionActive){
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem){
			//同じ名前が見つかった
			if (!pItem->m_AudioResource.expired()){
				return pItem->m_AudioResource.lock();
			}
			else{
				if (ExceptionActive){
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
						);
				}
				else{
					return nullptr;
				}
			}
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"指定の名前のオーディオリソースがありません",
					ResKey,
					L"MultiSoundEffect::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
	}

	void MultiSoundEffect::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiSoundEffect::Start(const wstring& ResKey, size_t LoopCount, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				ResKey,
				L"MultiSoundEffect::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImplの関数を呼ぶ
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiSoundEffect::Stop(const wstring& ResKey){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Stop()"
				);
		}
		pImpl->Stop(pItem);
	}



}
//endof  basedx11
