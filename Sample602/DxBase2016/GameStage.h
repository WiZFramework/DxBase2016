#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViews();
		//プレートの作成
		void CreatePlate();
		//ボックスの作成
		void CreateBox();
		//動く球体の作成
		void CreateMoveSphere();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void OnCreate()override;
	};
}
//endof  basedx11
