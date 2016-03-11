#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	用途: シーンクラス
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	public:
		//構築と破棄
		Scene(){}
		~Scene(){}
		//アクセサ
		//操作
		virtual void OnCreate()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;

	};
}
//end basedx11
