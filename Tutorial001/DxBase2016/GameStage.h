#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage{
		//ビューの作成
		void CreateViews();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void OnCreate()override;
	};
}
//endof  basedx11
