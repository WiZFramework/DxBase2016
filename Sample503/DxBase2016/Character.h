#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	class Jump
	//	用途: ジャンプの計算を返す関数オブジェクトテンプレート
	//--------------------------------------------------------------------------------------
	template<typename T>
	class Jump{
		T m_T;
	public:
		float operator()(){
			return m_T();
		}
	};

	//--------------------------------------------------------------------------------------
	//	class ShortType;
	//	用途: ショートジャンプの計算を返す関数オブジェクト
	//--------------------------------------------------------------------------------------
	class ShortType{
	public:
		float operator()(){
			return 2.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class MediumType;
	//	用途: ミディアムジャンプの計算を返す関数オブジェクト
	//--------------------------------------------------------------------------------------
	class MediumType{
	public:
		float operator()(){
			return 4.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class class LongType;
	//	用途: ロングジャンプの計算を返す関数オブジェクト
	//--------------------------------------------------------------------------------------
	class LongType{
	public:
		float operator()(){
			return 6.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class LongLongType;
	//	用途: ロングロングジャンプの計算を返す関数オブジェクト
	//--------------------------------------------------------------------------------------
	class LongLongType{
	public:
		float operator()(){
			return 8.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
	public:
		//構築と破棄
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		Vector3 m_StartPos;
		float m_YRot;
		float m_RotationSpeed;
	public:
		//構築と破棄
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};


}
//endof  basedx11
