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

	//--------------------------------------------------------------------------------------
	//	class ChildGameStage : public Stage;
	//	用途: 子供ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class ChildGameStage : public Stage{
		//ビューの作成
		void CreateViews();
		//ナンバースプライトの作成
		void CreateNumberSprite();
		//バースプライトの作成
		void CreateBerSprite();
	public:
		//構築と破棄
		ChildGameStage() :Stage(){}
		virtual ~ChildGameStage(){}
		//初期化
		virtual void OnCreate()override;
	};

}
//endof  basedx11
