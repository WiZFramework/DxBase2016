#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	用途: メニューステージクラス
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage{
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//メニュースプライトアニメーションの作成
		void CreateSSMenu();
	public:
		//構築と破棄
		MenuStage() :Stage(){}
		virtual ~MenuStage(){}
		//初期化
		virtual void OnCreate()override;
		//操作
		virtual void OnUpdate()override;
	};
}
//endof  basedx11
