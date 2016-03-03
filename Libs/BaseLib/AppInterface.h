#pragma once
#include "stdafx.h"

namespace basedx11{

	class ObjectFactory;

	//--------------------------------------------------------------------------------------
	//	class ObjectInterface;
	/*!
	CreateとPreCreateを持ち、Thisスマートポインタがとれるインターフェイス
	*/
	//--------------------------------------------------------------------------------------
	class ObjectInterface : public std::enable_shared_from_this<ObjectInterface> {
		friend class ObjectFactory;
		//クリエイト済みかどうか
		//Create関数が呼び出し後にtrueになる
		bool m_Created{ false };
		void SetCreated(bool b) {
			m_Created = b;
		}
	protected:
		//構築
		ObjectInterface(){}
		//破棄
		virtual ~ObjectInterface() {}
	public:
		//thisポインタ取得
		template<typename T>
		std::shared_ptr<T> GetThis() {
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				throw BaseException(
					L"thisをT型にキャストできません",
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"ObjectInterface::GetThis()"
					);
			}
			return nullptr;
		}
		//初期化を行う（仮想関数）
		//＊thisポインタが必要なオブジェクトはこの関数を多重定義して、構築する
		virtual void OnPreCreate() = 0;
		virtual void OnCreate() = 0;
		//クリエイト済みかどうか
		bool IsCreated() {
			return m_Created;
		}
	private:
		//コピー禁止
		ObjectInterface(const ObjectInterface&) = delete;
		ObjectInterface& operator=(const ObjectInterface&) = delete;
		//ムーブ禁止
		ObjectInterface(const ObjectInterface&&) = delete;
		ObjectInterface& operator=(const ObjectInterface&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class ObjectFactory;
	/*!
	Objectインターフェイスの派生クラスを構築する
	*/
	//--------------------------------------------------------------------------------------
	class Stage;
	class ObjectFactory {
	public:
		//オブジェクト作成（static関数）
		template<typename T, typename... Ts>
		static shared_ptr<T> Create(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}

		//オブジェクト作成（static関数）
		//コンストラクタではなく、CreateWithParam()関数でパラメータを渡す場合
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateWithParam(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T());
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}

		template<typename T, typename... Ts>
		static shared_ptr<T> CreateGameObjectWithParam(const shared_ptr<Stage>& StagePtr, Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(StagePtr));
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	class ShapeInterface;
	//	用途: UpdateとDrawのインターフェイス
	//--------------------------------------------------------------------------------------
	class ShapeInterface {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	ShapeInterface();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface() {}
		//--------------------------------------------------------------------------------------
		//	virtual ~ShapeInterface();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface() {}
		//操作
		//--------------------------------------------------------------------------------------
		//	virtual void OnUpdate() = 0;
		/*!
		@breaf OnUpdate処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void OnLastUpdate();
		/*!
		@breaf OnLastUpdate処理.<br />
		＊最終アップデート
		＊純粋仮想関数にしない
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate() {}
		//--------------------------------------------------------------------------------------
		//	virtual void OnDraw() = 0;
		/*!
		@breaf OnDraw処理
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() = 0;
	private:
		//コピー禁止
		ShapeInterface(const ShapeInterface&) = delete;
		ShapeInterface& operator=(const ShapeInterface&) = delete;
		//ムーブ禁止
		ShapeInterface(const ShapeInterface&&) = delete;
		ShapeInterface& operator=(const ShapeInterface&&) = delete;
	};

}
// end namespace basedx11


