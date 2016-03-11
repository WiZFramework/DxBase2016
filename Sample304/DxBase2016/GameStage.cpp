
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class GameStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	//リソースの作成
	void GameStage::CreateResourses(){
		wstring strTexture = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);
		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"bar.jpg";
		App::GetApp()->RegisterTexture(L"BAR_TX", strTexture);
	}

	//ビュー類の作成
	void GameStage::CreateViews(){
		//最初にデフォルトのレンダリングターゲット類を作成する
		//パラメータの2048.0fはシャドウマップのサイズ。大きいほど影が細かくなる（4096.0fなど）
		//影を細かくするとメモリを大量に消費するので注意！
		CreateDefaultRenderTargets(2048.0f);
		//影のビューサイズの設定。この値を小さくすると影が表示される範囲が小さくなる。
		//値が小さいほうが影は細かくなる
		//スタティック変数なので一度設定したらその値はステージを超えて保持される。
		Shadowmap::SetViewSize(32.0f);
		//マルチビューコンポーネントの取得
		auto PtrMultiView = GetComponent<MultiView>();
		//マルチビューにビューの追加
		auto PtrView = PtrMultiView->AddView();
		//ビューの矩形を設定（ゲームサイズの縦半分）
		Rect2D<float> rect(0, 0, (float)640, (float)800);
		//ビューの背景色
		Color4 ViewBkColor(0.0f, 0.125f, 0.3f, 1.0f);
		//最初のビューにパラメータの設定
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, ViewBkColor, 1, 0.2f, 1.0f);
		//最初のビューのビューのライトの設定
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//ビューのカメラの設定
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

		//マルチビューにビューの追加2
		PtrView = PtrMultiView->AddView();
		//ビューの矩形を設定（ゲームサイズ右半分）
		rect = Rect2D<float>(0, 0, (float)640, (float)800);
		rect += Point2D<float>(640.0f, 0);
		PtrView->ResetParamaters<Camera, MultiLight>(rect, ViewBkColor, 1, 0.2f, 1.0f);
		//ビューのビューのライトの設定
		PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//ビューのカメラの設定
		PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}


	//プレートの作成
	void GameStage::CreatePlate(){
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTransMatrix = Ptr->GetComponent<TransformMatrix>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(200.0f, 200.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.0f, 0.0f)
			);
		PtrTransMatrix->SetWorldMatrix(WorldMat);
		
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		
	}

	//固定のボックスの作成
	void GameStage::CreateFixedBox(){
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			},
			{
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(14.0f, 0.25f, 10.0f)
			},

			{
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//上下移動しているボックスの作成
	void GameStage::CreateMoveBox(){
		AddGameObject<MoveBox>(
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}


	//プレイヤーの作成
	void GameStage::CreatePlayer(){
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	//初期化
	void GameStage::OnCreate(){
		try{
			//リソースの作成
			CreateResourses();
			//ビュー類を作成する
			CreateViews();
			//プレートを作成する
			CreatePlate();
			//固定のボックスの作成
			CreateFixedBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//プレーヤーの作成
			CreatePlayer();
			//子供ステージの作成
			AddChileStage<ChildGameStage>();
		}
		catch (...){
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	class ChildGameStage : public Stage;
	//	用途: 子供ゲームステージクラス
	//--------------------------------------------------------------------------------------
	//ビューの作成
	void ChildGameStage::CreateViews(){
		//最初にデフォルトのレンダリングターゲット類を作成する
		//パラメータの2048.0fはシャドウマップのサイズ。大きいほど影が細かくなる（4096.0fなど）
		//影を細かくするとメモリを大量に消費するので注意！
		CreateDefaultRenderTargets(2048.0f);
		//マルチビューコンポーネントの取得
		auto PtrMultiView = GetComponent<MultiView>();
		//マルチビューにビューの追加
		auto PtrView = PtrMultiView->AddView();
		//ビューの矩形を設定（ゲームサイズ全体）
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		//ビューの背景色(透明)
		Color4 ViewBkColor(0.0f, 0.0f, 0.0f, 0.0f);
		//最初のビューにパラメータの設定
		PtrView->ResetParamaters<Camera, MultiLight>(rect, ViewBkColor, 1, 0.0f, 0.2f);
		//最初のビューのビューのライトの設定
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//ビューのカメラの設定
		//配置オブジェクトがスプライトのみの場合は設定しなくてもよいが
		//設定してもスプライトには影響しないので、設定しておく
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 0.0f, -10.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

	}

	//ナンバースプライトの作成
	void ChildGameStage::CreateNumberSprite(){
		//配列の初期化
		vector<Vector3> Vec = {
			{ 64.0f, 400.0f, 0.00 },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<NumberSprite>(v);
		}
	}

	//バースプライトの作成
	void ChildGameStage::CreateBerSprite(){
		AddGameObject<BarSprite>(Vector3(32.0f, 32.0f, 0));
	}

	void ChildGameStage::OnCreate(){
		CreateViews();
		CreateNumberSprite();
		CreateBerSprite();
	}








}
//endof  basedx11
