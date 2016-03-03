#pragma once
#include "stdafx.h"

namespace basedx11{

	struct Event;
	class App;



	//--------------------------------------------------------------------------------------
	//	class Object public ObjectInterface;
	//	用途: すべてのオブジェクトの基底クラス（抽象クラス）
	//--------------------------------------------------------------------------------------
	class Object : public ObjectInterface{
	protected:
		//構築と破棄
		Object(){}
		virtual ~Object(){}
	public:
		//初期化を行う（仮想関数）
		//＊thisポインタが必要なオブジェクトはこの関数を多重定義して、構築する
		virtual void OnPreCreate() override {}
		virtual void OnCreate() override {}
		// イベントのPOST（キューに入れる）
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//指定のダイナミッククラスに送る
		template<typename T>
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					PostEvent(DispatchTime, Sender, TgtPtr, MsgStr, Info);
				}
			}
		}
		// イベントのSEND（キューに入れずにそのまま送る）
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//指定のダイナミッククラスに送る
		template<typename T>
		void SendEvent(const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					SendEvent(Sender, TgtPtr, MsgStr, Info);
				}
			}
		}

		//イベントを受け取る（仮想関数）
		//＊オブジェクトはイベントを受け取る場合にこの関数を多重定義する
		virtual void OnEvent(const shared_ptr<Event>& event){}
	private:
		//コピー禁止
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		//ムーブ禁止
		Object(const Object&&) = delete;
		Object& operator=(const Object&&) = delete;

	};

	//--------------------------------------------------------------------------------------
	//	struct Event;
	//	用途: イベント構造体
	//--------------------------------------------------------------------------------------
	struct Event{
		//遅延時間（SendEventの場合は常に0）
		float m_DispatchTime;
		//このメッセージを送ったオブジェクト
		weak_ptr<Object> m_Sender;
		//受け取るオブジェクト（nullptrの場合はアクティブステージ内すべてもしくはキーワードで識別するオブジェクト）
		weak_ptr<Object> m_Receiver;
		//受け取るオブジェクトの参照名もしくは配列名
		wstring m_ReceiverKey;
		//メッセージ文字列
		wstring m_MsgStr;
		//追加情報をもつオブジェクトのポインタ
		shared_ptr<void> m_Info;
		//構築と破棄
		Event(float DispatchTime,const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(Receiver),
			m_ReceiverKey(L""),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		Event(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(),
			m_ReceiverKey(ReceiverKey),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		~Event(){}
	};

	class Stage;
	class SceneBase;

	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	用途: イベント配送クラス
	//--------------------------------------------------------------------------------------
	class EventDispatcher{
	public:
		//構築と破棄
		explicit EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr);
		virtual ~EventDispatcher();

		//イベントのPOST（キューに入れる）
		void PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//イベントのSEND（キューに入れずにそのまま送る）
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		//	void DispatchDelayedEvwnt();
		//	用途: POSTイベントの送信(メインループで呼ばれる)
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void DispatchDelayedEvwnt();

		//--------------------------------------------------------------------------------------
		//	void ClearEventQ();
		//	用途:キューにたまっているメッセージを削除する
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void ClearEventQ();

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
		//ムーブ禁止
		EventDispatcher(const EventDispatcher&&) = delete;
		EventDispatcher& operator=(const EventDispatcher&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	用途: カメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Camera : public Object, public ShapeInterface{
	public:
		//構築と破棄
		explicit Camera();
		virtual ~Camera();
		//アクセサ
		const Vector3& GetEye() const;
		void SetEye(const Vector3& Eye);
		void SetEye(float x, float y, float z);

		const Vector3& GetAt() const;
		void SetAt(const Vector3& At);
		void SetAt(float x, float y, float z);

		const Vector3& GetUp() const;
		void SetUp(const Vector3& Up);
		void SetUp(float x, float y, float z);

		bool IsPers()const;
		bool GetPers()const;
		void SetPers(bool b);

		float GetFovY() const;
		void SetFovY(float f);

		float GetAspect() const;
		void SetAspect(float f);

		float GetWidth() const;
		void SetWidth(float f);

		float GetHeight() const;
		void SetHeight(float f);

		float GetNear() const;
		void SetNear(float f);

		float GetFar() const;
		void SetFar(float f);

		shared_ptr<GameObject> GetCameraObject() const;
		void SetCameraObject(const shared_ptr<GameObject>& Obj);
		void ClearCameraObject(const shared_ptr<GameObject>& Obj);


		void SetViewPort(const D3D11_VIEWPORT& v);

		const Matrix4X4& GetViewMatrix() const;
		const Matrix4X4& GetProjMatrix() const;
		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	用途: LookAtカメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class LookAtCamera : public Camera{
	public:
		//構築と破棄
		explicit LookAtCamera();
		virtual ~LookAtCamera();
		//アクセサ
		shared_ptr<GameObject> GetTargetObject() const;
		void SetTargetObject(const shared_ptr<GameObject>& Obj);

		float GetToTargetLerp() const;
		void SetToTargetLerp(float f);

		//操作
		virtual void OnUpdate() override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public ShapeInterface ;
	//	用途: ライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class Light : public Object, public ShapeInterface{
	public:
		explicit Light();
		Light(
			const Vector3& Directional,
			const Color4& DiffuseColor,
			const Color4& SpecularColor
			);

		virtual ~Light();
		//アクセサ
		const Vector3& GetDirectional() const;
		void SetDirectional(const Vector3& Directional);
		void SetDirectional(float x, float y, float z);
		void SetPositionToDirectional(const Vector3& Position);
		void SetPositionToDirectional(float x, float y, float z);

		const Color4& GetDiffuseColor()const;
		void SetDiffuseColor(const Color4& col);
		void SetDiffuseColor(float r, float g, float b, float a);

		const Color4& GetSpecularColor() const;
		void SetSpecularColor(const Color4& col);
		void SetSpecularColor(float r, float g, float b, float a);

		virtual void OnUpdate()override{}
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public ShapeInterface ;
	//	用途: マルチライト（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class MultiLight : public Object, public ShapeInterface{
	public:
		//構築と破棄
		explicit MultiLight();
		virtual ~MultiLight();
		//アクセサ
		//Lightの数
		size_t GetLightCount() const;
		//Lightの取得
		shared_ptr<Light> GetLight(size_t Index)const;
		//Lightのセット
		void SetLight(size_t Index, shared_ptr<Light>& Ptr);
		//配列の参照
		vector< shared_ptr<Light> >& GetLightVec();
		//Lightの追加
		shared_ptr<Light> AddLight();
		//Lightの削除
		void RemoveLight(size_t Index);
		//デフォルトのライティングの設定
		void SetDefaultLighting();

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public ShapeInterface;
	//	用途: ビュークラス（ビューポートとライト、カメラを管理、コンポーネントではない）
	//--------------------------------------------------------------------------------------
	class View : public Object, public ShapeInterface{
		//テンプレートから呼ばれるサブ関数
		void ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr);
	public:
		//構築と破棄
		View();
		virtual ~View();
		//アクセサ
		const D3D11_VIEWPORT& GetViewPort()const;
		const D3D11_VIEWPORT* GetViewPortPtr()const;
		void SetViewPort(const D3D11_VIEWPORT& v);
		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& c);
		shared_ptr<Camera> GetCamera() const;
		void SetCamera(shared_ptr<Camera>& Ptr);

		shared_ptr<MultiLight> GetMultiLight() const;
		void SetMultiLight(shared_ptr<MultiLight>& Lights);

		shared_ptr<MultiLight> SetDefaultLighting();


		//操作
		shared_ptr<Light> AddLight();

		template<typename CameraType, typename LightType>
		void ResetParamaters(const Rect2D<float>& ViewPortSize, Color4& Col, size_t LightCount, float MinDepth = 0.0f, float MaxDepth = 1.0f){
			D3D11_VIEWPORT vp;
			ZeroMemory(&vp, sizeof(vp));
			vp.MinDepth = MinDepth;
			vp.MaxDepth = MaxDepth;
			vp.TopLeftX = ViewPortSize.left;
			vp.TopLeftY = ViewPortSize.top;
			vp.Width = ViewPortSize.Width();
			vp.Height = ViewPortSize.Height();
			//カメラを差し替える
			auto CameraPtr = ObjectFactory::Create<CameraType>();
			//ライトを差し替える
			auto MultiLightPtr = ObjectFactory::Create<LightType>();
			for (size_t i = 0; i < LightCount; i++){
				MultiLightPtr->AddLight();
			}
			//サブ関数を使って設定
			ResetParamatersSub(vp, Col,
				dynamic_pointer_cast<Camera>(CameraPtr),
				dynamic_pointer_cast<MultiLight>(MultiLightPtr));
		}
		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override{}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}
//end basedx11
