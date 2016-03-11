#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	用途: メニューステージクラス
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage{
		//ビューの作成
		void CreateViews();
		//文字列の作成
		void CreateString();
	public:
		//構築と破棄
		MenuStage() :Stage(){}
		virtual ~MenuStage(){}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate() override;
	};

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
		//固定のボックスの作成
		void CreateFixedBox();
		//上下移動しているボックスの作成
		void CreateMoveBox();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage(){}
		virtual ~GameStage(){}
		//初期化
		virtual void OnCreate()override;
	};
}
//endof  basedx11
