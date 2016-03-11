#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class JumpBase;
	//	用途: ジャンプのアルゴリズムのベースクラス
	//--------------------------------------------------------------------------------------
	class JumpBase{
	protected:
		JumpBase(){}
	public:
		virtual ~JumpBase(){}
		virtual float JumpMethod() = 0;
	};

	//--------------------------------------------------------------------------------------
	//	class JumpShort : public JumpBase;
	//	用途: 短いジャンプ
	//--------------------------------------------------------------------------------------
	class JumpShort : public JumpBase{
	public:
		JumpShort() :JumpBase(){}
		virtual ~JumpShort(){}
		virtual float JumpMethod()override{
			return 2.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpMiddle : public JumpBase;
	//	用途: 通常のジャンプ
	//--------------------------------------------------------------------------------------
	class JumpMiddle : public JumpBase{
	public:
		JumpMiddle() :JumpBase(){}
		virtual ~JumpMiddle(){}
		virtual float JumpMethod()override{
			return 4.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpLong : public JumpBase;
	//	用途: 長いのジャンプ
	//--------------------------------------------------------------------------------------
	class JumpLong : public JumpBase{
	public:
		JumpLong() :JumpBase(){}
		virtual ~JumpLong(){}
		virtual float JumpMethod()override{
			return 6.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpLongLong : public JumpBase;
	//	用途:　長い長いジャンプ
	//--------------------------------------------------------------------------------------
	class JumpLongLong : public JumpBase{
	public:
		JumpLongLong() :JumpBase(){}
		virtual ~JumpLongLong(){}
		virtual float JumpMethod()override{
			return 8.0f;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
		unique_ptr<JumpBase> m_JumpStrategy;
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
		unique_ptr<JumpBase> m_JumpStrategy;
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
