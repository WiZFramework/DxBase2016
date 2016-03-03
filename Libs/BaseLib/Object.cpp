#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Object;
	//	用途: すべてのオブジェクトの基底クラス（抽象クラス）
	//--------------------------------------------------------------------------------------
	void Object::PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void Object::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetSceneBase()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}



	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	用途: Impl構造体
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl{
		weak_ptr<SceneBase> m_SceneBase;
		//イベントのキュー
		list< shared_ptr<Event> > m_PriorityQ;
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//イベント
		//	);
		//用途: イベントの送信
		//戻り値: なし
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl(const shared_ptr<SceneBase>& SceneBasePtr) :m_SceneBase(SceneBasePtr){}
		~Impl(){}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event){
		if (m_SceneBase.expired()){
			//シーンが無効
			m_PriorityQ.clear();
			return;
		}
		auto SceneBasePtr = m_SceneBase.lock();
		if (event->m_ReceiverKey != L""){
			//キーがあった処理
			if (auto Vec = SceneBasePtr->GetActiveStage()->GetSharedObjectGroup(event->m_ReceiverKey, false)){
				//配列があった
				for (size_t i = 0; i < Vec->size(); i++){
					Vec->at(i)->OnEvent(event);
				}
			}
			//続いてシェアオブジェクトも同じキーがあれば送る
			if (auto Ptr = SceneBasePtr->GetActiveStage()->GetSharedObject(event->m_ReceiverKey, false)){
				Ptr->OnEvent(event);
			}
		}
		else if (!event->m_Receiver.expired()){
			//受け手が有効
			auto RePtr = event->m_Receiver.lock();
			RePtr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	用途: イベント配送クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	EventDispatcher::EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr):
		pImpl(new Impl(SceneBasePtr))
	{}
	EventDispatcher::~EventDispatcher(){}


	//イベントのPOST（キューに入れる）
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//イベントの作成 
		auto Ptr = make_shared<Event>(Delay,Sender, Receiver, MsgStr, Info);
		//キューにためる
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	//イベントのSEND（キューに入れずにそのまま送る）
	void EventDispatcher::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//イベントの作成 
		auto Ptr = make_shared<Event>(0.0f,Sender, Receiver, MsgStr, Info);
		//送信
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::DispatchDelayedEvwnt(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()){
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f){
				(*it)->m_DispatchTime = 0.0f;
				//メッセージの送信
				pImpl->Discharge(*it);
				//キューから削除
				it = pImpl->m_PriorityQ.erase(it);
				//削除後のイテレータが「最後」の
				//ときはループを抜ける
				if (it == pImpl->m_PriorityQ.end()){
					break;
				}
			}
			else{
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ(){
		pImpl->m_PriorityQ.clear();
	}


	//--------------------------------------------------------------------------------------
	//	struct Camera::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct Camera::Impl{
		Vector3 m_Eye;	//カメラ位置
		Vector3 m_At;	//注目点
		Vector3 m_Up;   //カメラの傾き（アップ）
		bool m_Pers;	//遠近法を使うかどうか
		weak_ptr<GameObject> m_CameraObject;

		D3D11_VIEWPORT m_ViewPort;

		float m_FovY;   //射影角度
		float m_Aspect;	//アスペクト比

		float m_Width;	//遠近法を使わない場合（正射影）の幅
		float m_Height;	//遠近法を使わない場合（正射影）の高さ
		float m_Near;	//手前の最小距離
		float m_Far;	//奥の最大距離

		Matrix4X4 m_ViewMatrix;
		Matrix4X4 m_ProjMatrix;
		Impl() :
			m_Eye(0, 10.0f, -20.0f),	//デフォルトは後方斜め上
			m_At(0, 0, 0),
			m_Up(0, 1.0f, 0),
			m_Pers(true),
			m_FovY(XM_PIDIV4),
			m_Aspect(0),
			m_Width(0),
			m_Height(0),
			m_Near(0.5f),
			m_Far(1000.0f),
			m_ViewMatrix(),
			m_ProjMatrix()
		{
			try{
				m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
				m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
				m_ViewPort.MinDepth = 0.0f;
				m_ViewPort.MaxDepth = 1.0f;
				m_ViewPort.TopLeftX = 0;
				m_ViewPort.TopLeftY = 0;

				m_Width = m_ViewPort.Width;
				m_Height = m_ViewPort.Height;
				m_Aspect = m_Width / m_Height;
			}
			catch (...){
				throw;
			}
		}
		~Impl(){}





	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	用途: カメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	Camera::Camera()
		:Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{

	}
	Camera::~Camera(){}

	//アクセサ
	const Vector3& Camera::GetEye() const{ return pImpl->m_Eye; }
	void Camera::SetEye(const Vector3& Eye){ pImpl->m_Eye = Eye; }
	void Camera::SetEye(float x, float y, float z){ pImpl->m_Eye = Vector3(x, y, z); }

	const Vector3& Camera::GetAt() const{ return pImpl->m_At; }
	void Camera::SetAt(const Vector3& At){ pImpl->m_At = At; }
	void Camera::SetAt(float x, float y, float z){ pImpl->m_At = Vector3(x, y, z); }

	const Vector3& Camera::GetUp() const{ return pImpl->m_Up; }
	void Camera::SetUp(const Vector3& Up){ pImpl->m_Up = Up; }
	void Camera::SetUp(float x, float y, float z){ pImpl->m_Up = Vector3(x, y, z); }

	bool Camera::IsPers()const{ return pImpl->m_Pers; }
	bool Camera::GetPers()const{ return pImpl->m_Pers; }
	void Camera::SetPers(bool b){ pImpl->m_Pers = b; }

	float Camera::GetFovY() const{ return pImpl->m_FovY; }
	void Camera::SetFovY(float f){ pImpl->m_FovY = f; }

	float Camera::GetAspect() const{ return pImpl->m_Aspect; }
	void Camera::SetAspect(float f){ pImpl->m_Aspect = f; }

	float Camera::GetWidth() const{ return pImpl->m_Width; }
	void Camera::SetWidth(float f){ pImpl->m_Width = f; }

	float Camera::GetHeight() const{ return pImpl->m_Height; }
	void Camera::SetHeight(float f){ pImpl->m_Height = f; }

	float Camera::GetNear() const{ return pImpl->m_Near; }
	void Camera::SetNear(float f){ pImpl->m_Near = f; }

	float Camera::GetFar() const{ return pImpl->m_Far; }
	void Camera::SetFar(float f){ pImpl->m_Far = f; }

	shared_ptr<GameObject> Camera::GetCameraObject() const{
		if (pImpl->m_CameraObject.expired()){
			return nullptr;
		}
		else{
			return pImpl->m_CameraObject.lock();
		}
	}
	void Camera::SetCameraObject(const shared_ptr<GameObject>& Obj){
		if (!Obj){
			pImpl->m_CameraObject.reset();
		}
		else{
			pImpl->m_CameraObject = Obj;
		}
	}
	void Camera::ClearCameraObject(const shared_ptr<GameObject>& Obj){
		SetCameraObject(nullptr);
	}

	void Camera::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
		pImpl->m_Width = pImpl->m_ViewPort.Width;
		pImpl->m_Height = pImpl->m_ViewPort.Height;
		pImpl->m_Aspect = pImpl->m_Width / pImpl->m_Height;
	}


	const Matrix4X4& Camera::GetViewMatrix() const{ return pImpl->m_ViewMatrix; }
	const Matrix4X4& Camera::GetProjMatrix() const{ return pImpl->m_ProjMatrix; }

	//操作
	void Camera::OnUpdate(){
		if (!pImpl->m_CameraObject.expired()){
			auto ShPtr = pImpl->m_CameraObject.lock();
			auto TransPtr = ShPtr->GetComponent<Transform>();
			if (TransPtr){
				pImpl->m_Eye = TransPtr->GetPosition();
				pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
				if (pImpl->m_Pers){
					pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
				}
				else{
					pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
				}
			}
		}
		else{
			pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
			if (pImpl->m_Pers){
				pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
			}
			else{
				pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct LookAtCamera::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct LookAtCamera::Impl{
		weak_ptr<GameObject> m_TargetObject;	//目標となるオブジェクト
		float m_ToTargetLerp;	//目標を追いかける際の補間値
		Impl() :
			m_ToTargetLerp(1.0f)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	用途: LookAtカメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	LookAtCamera::LookAtCamera() :
		Camera(),
		pImpl(new Impl())
	{
	}
	LookAtCamera::~LookAtCamera(){}
	//アクセサ
	shared_ptr<GameObject> LookAtCamera::GetTargetObject() const{
		if (!pImpl->m_TargetObject.expired()){
			return pImpl->m_TargetObject.lock();
		}
		return nullptr;
	}

	void LookAtCamera::SetTargetObject(const shared_ptr<GameObject>& Obj){
		pImpl->m_TargetObject = Obj;
	}

	float LookAtCamera::GetToTargetLerp() const{
		return pImpl->m_ToTargetLerp;
	}
	void LookAtCamera::SetToTargetLerp(float f){
		pImpl->m_ToTargetLerp = f;
	}


	void LookAtCamera::OnUpdate(){
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (CntlVec[0].bConnected){
			Vector3 NewAt(0, 0, 0);
			auto TargetPtr = GetTargetObject();
			if (TargetPtr){
				//目指したい場所
				Vector3 ToAt = TargetPtr->GetComponent<Transform>()->GetPosition();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, pImpl->m_ToTargetLerp, Lerp::Linear);
			}
			//ステップ1、注視点と位置の変更
			Vector3 Span = GetAt() - GetEye();
			Vector3 NewEye = NewAt - Span;
			SetAt(NewAt);
			SetEye(NewEye);
			//ステップ２、ズームの変更
			//カメラ位置と注視点の間のベクトルを算出
			Span = GetAt() - GetEye();
			//正規化
			Span.Normalize();
			//変化値の決定
			Span = Span * ElapsedTime * 10.0f;

			Vector3 NewArm = GetAt() - GetEye();
			//Dパッド下
			//カメラを引く
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN){
				//カメラ位置を引く
				NewEye = NewEye - Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() > (GetFar() * 0.1f)){
					NewEye = NewEye + Span;
					NewArm = NewAt - NewEye;
				}
			}
			//Dパッド上
			//カメラを寄る
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP){
				//カメラ位置を寄る
				NewEye = NewEye + Span;
				NewArm = NewAt - NewEye;
				if (NewArm.Length() < GetNear() * 2.0f){
					NewEye = NewEye - Span;
					NewArm = NewAt - NewEye;
				}
			}
			SetAt(NewAt);
			SetEye(NewEye);
			//ステップ3角度の変更
			//現在のAtとEyeの角度を得る
			Vector3 ArmInv = GetEye() - GetAt();
			//右スティックX方向
			FLOAT AngleY = 0;
			//右スティックY方向
			FLOAT AngleX = 0;
			FLOAT AngleZ = 0;
			if (CntlVec[0].fThumbRX != 0){
				AngleY = -CntlVec[0].fThumbRX * ElapsedTime;
			}
			if (CntlVec[0].fThumbRY != 0){
				AngleX = CntlVec[0].fThumbRY * ElapsedTime;
				AngleZ = CntlVec[0].fThumbRY * ElapsedTime;
			}
			if (ArmInv.z > 0){
				AngleX *= -1.0f;
			}
			if (ArmInv.x < 0){
				AngleZ *= -1.0f;
			}
			Quaternion QtSpan(AngleX, AngleY, AngleZ);
			QtSpan.Normalize();
			//回転先計算の行列を作成
			Matrix4X4 Mat, Mat2;
			Mat.STRTransformation(
				Vector3(1.0f, 1.0f, 1.0f),
				ArmInv,
				QtSpan);
			Mat2.TranslationFromVector(GetAt());
			Mat *= Mat2;
			NewEye = Mat.PosInMatrix();
			if (NewEye.y < 0.5f){
				NewEye.y = 0.5f;
			}
			//カメラが一定以上、上から視線にならなように調整
			ArmInv = NewEye - GetAt();
			ArmInv.Normalize();
			float y2 = ArmInv.y * ArmInv.y;
			float x2 = ArmInv.x * ArmInv.x;
			float z2 = ArmInv.z * ArmInv.z;
			if (y2 <= (x2 + z2)){
				SetEye(NewEye);
			}

		}
		Camera::OnUpdate();
	}


	//--------------------------------------------------------------------------------------
	//	struct Light::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct Light::Impl{
		Vector3 m_Directional;	//ライトの向き
		Color4 m_DiffuseColor;	//ディフィーズ色
		Color4 m_SpecularColor;	//スペキュラー色
		Impl() :
			m_Directional(0, 0, 0),
			m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
		{
		}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public ShapeInterface ;
	//	用途: ライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Light::Light() :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
		SetPositionToDirectional(Vector3(-1.0f, 1.0f, -1.0f));
	}
	Light::Light(
		const Vector3& Directional,
		const Color4& DiffuseColor,
		const Color4& SpecularColor
		) :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
		pImpl->m_Directional = Directional;
		pImpl->m_DiffuseColor = DiffuseColor;
		pImpl->m_SpecularColor = SpecularColor;
	}

	Light::~Light(){}

	//アクセサ
	const Vector3& Light::GetDirectional() const{
		return pImpl->m_Directional;
	}
	void Light::SetDirectional(const Vector3& Directional){
		pImpl->m_Directional = Directional;
		pImpl->m_Directional.Normalize();
	}
	void Light::SetDirectional(float x, float y, float z){
		pImpl->m_Directional = Vector3(x, y, z);
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(const Vector3& Position){
		pImpl->m_Directional = Position;
		pImpl->m_Directional *= -1.0f;
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(float x, float y, float z){
		pImpl->m_Directional = Vector3(x, y, z);
		pImpl->m_Directional *= -1.0f;
		pImpl->m_Directional.Normalize();
	}

	const Color4& Light::GetDiffuseColor() const{
		return pImpl->m_DiffuseColor;
	}
	void Light::SetDiffuseColor(const Color4& col){
		pImpl->m_DiffuseColor = col;
	}
	void Light::SetDiffuseColor(float r, float g, float b, float a){
		pImpl->m_DiffuseColor = Color4(r, g, b, a);
	}

	const Color4& Light::GetSpecularColor() const{
		return pImpl->m_SpecularColor;
	}
	void Light::SetSpecularColor(const Color4& col){
		pImpl->m_SpecularColor = col;
	}
	void Light::SetSpecularColor(float r, float g, float b, float a){
		pImpl->m_SpecularColor = Color4(r, g, b, a);
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiLight::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct MultiLight::Impl{
		vector< shared_ptr<Light> > m_LightVec;
		Impl(){}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public ShapeInterface ;
	//	用途: マルチライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiLight::MultiLight() :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
	}
	MultiLight::~MultiLight(){
	}
	//Lightの数
	size_t MultiLight::GetLightCount() const{
		return pImpl->m_LightVec.size();
	}
	//Lightの取得
	shared_ptr<Light> MultiLight::GetLight(size_t Index)const{
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"インデックスが範囲外です",
				str,
				L"MultiLight::GetLight()"
				);
		}
		return pImpl->m_LightVec[Index];
	}
	//Lightのセット
	void MultiLight::SetLight(size_t Index, shared_ptr<Light>& Ptr){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"インデックスが範囲外です",
				str,
				L"MultiLight::SetLight()"
				);
		}
		pImpl->m_LightVec[Index] = Ptr;
	}
	//配列の参照
	vector< shared_ptr<Light> >& MultiLight::GetLightVec(){
		return pImpl->m_LightVec;
	}
	//Lightの追加
	shared_ptr<Light> MultiLight::AddLight(){
		auto Ptr = ObjectFactory::Create<Light>();
		pImpl->m_LightVec.push_back(Ptr);
		return Ptr;
	}
	//Lightの削除
	void MultiLight::RemoveLight(size_t Index){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"インデックスが範囲外です",
				str,
				L"MultiLight::RemoveLight()"
				);
		}
		vector< shared_ptr<Light> >::iterator it;
		size_t count = 0;
		for (it = pImpl->m_LightVec.begin(); it != pImpl->m_LightVec.end(); it++){
			if (count == Index){
				pImpl->m_LightVec.erase(it);
				return;
			}
			count++;
		}
	}

	//デフォルトのライティングの設定
	void MultiLight::SetDefaultLighting(){
		size_t NouCount = pImpl->m_LightVec.size();
		pImpl->m_LightVec.clear();
		//最低一つはライトを置く
		auto Ptr = ObjectFactory::Create<Light>(
			Vector3(0.5265408f, -0.5735765f, 0.6275069f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
			);
		pImpl->m_LightVec.push_back(Ptr);
		if (NouCount >= 2){

			Ptr = ObjectFactory::Create<Light>(
				Vector3(-0.7198464f, 0.3420201f, -0.6040227f),
				Color4(0.9647059f, 0.7607844f, 0.4078432f, 1.0f),
				Color4(0.0000000f, 0.0000000f, 0.0000000f, 0.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
		if (NouCount >= 3){
			Ptr = ObjectFactory::Create<Light>(
				Vector3(0.4545195f, -0.7660444f, 0.4545195f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
	}


	//操作
	void MultiLight::OnUpdate(){
		for (auto Ptr : pImpl->m_LightVec){
			Ptr->OnUpdate();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct View::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct View::Impl{
		D3D11_VIEWPORT m_ViewPort;	//ビューポート
		Color4 m_BackColor;			//背景色
		shared_ptr<Camera> m_Camera;	//カメラ
		shared_ptr<MultiLight> m_MultiLight;	//ライト
		explicit Impl() :
			m_BackColor(0.0f, 0.125f, 0.3f, 1.0f)
		{
			//ビューポートのセットアップ(クライアント領域全体)
			ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
			m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			m_ViewPort.MinDepth = 0.0f;
			m_ViewPort.MaxDepth = 1.0f;
			m_ViewPort.TopLeftX = 0;
			m_ViewPort.TopLeftY = 0;
			//カメラを作成しておく
			m_Camera = make_shared<Camera>();
			//ライトを作成しておく
			m_MultiLight = make_shared<MultiLight>();
		}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public ShapeInterface;
	//	用途: ビュークラス（ビューポートとライト、カメラを管理、コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//テンプレートから呼ばれるサブ関数
	void View::ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr){
		pImpl->m_ViewPort = vp;
		pImpl->m_BackColor = Col;
		pImpl->m_Camera = CameraPtr;
		pImpl->m_Camera->SetViewPort(vp);
		pImpl->m_MultiLight = MultiLightPtr;
	}

	//構築と破棄
	View::View() :
		Object(),
		ShapeInterface(),
		pImpl(new Impl())
	{
	}
	View::~View(){}

	//アクセサ
	const D3D11_VIEWPORT& View::GetViewPort()const {
		return pImpl->m_ViewPort;
	}
	const D3D11_VIEWPORT* View::GetViewPortPtr()const {
		return &pImpl->m_ViewPort;
	}
	void View::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
	}
	const Color4& View::GetBackColor() const {
		return pImpl->m_BackColor;
	}
	void View::SetBackColor(const Color4& c){
		pImpl->m_BackColor = c;
	}
	shared_ptr<Camera> View::GetCamera() const {
		return pImpl->m_Camera;
	}
	void View::SetCamera(shared_ptr<Camera>& Ptr){
		pImpl->m_Camera = Ptr;
	}

	shared_ptr<MultiLight> View::GetMultiLight() const {
		return pImpl->m_MultiLight;
	}
	void View::SetMultiLight(shared_ptr<MultiLight>& Lights){
		pImpl->m_MultiLight = Lights;
	}

	shared_ptr<MultiLight> View::SetDefaultLighting(){
		pImpl->m_MultiLight->SetDefaultLighting();
		return pImpl->m_MultiLight;
	}

	shared_ptr<Light> View::AddLight(){
		return pImpl->m_MultiLight->AddLight();
	}



	//操作
	void View::OnUpdate(){
		if (pImpl->m_Camera){
			pImpl->m_Camera->OnUpdate();
		}
		if (pImpl->m_MultiLight){
			pImpl->m_MultiLight->OnUpdate();
		}
	}




}
//endof  basedx11

