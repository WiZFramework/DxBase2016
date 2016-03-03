#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class ActionObject : public GameObject;
	//	用途: アクションする配置オブジェクト
	//--------------------------------------------------------------------------------------
	class ActionObject : public GameObject{
	public:
		//構築と破棄
		ActionObject(const shared_ptr<Stage>& StagePtr);
		virtual ~ActionObject();
		//初期化
		virtual void OnCreate() override{}
		void OnCreateWithParam(const Vector3& StartPos);
		//衝突時
		virtual void OnCollision(const shared_ptr<GameObject>& other)override;
	};
}
//endof  basedx11
