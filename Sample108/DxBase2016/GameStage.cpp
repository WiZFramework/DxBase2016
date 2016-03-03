
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
		//ビューの矩形を設定（ゲームサイズ全体）
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		//ビューの背景色
		Color4 ViewBkColor(0.0f, 0.125f, 0.3f, 1.0f);
		//最初のビューにパラメータの設定
		PtrView->ResetParamaters<LookAtCamera, MultiLight>(rect, ViewBkColor, 1, 0.0f, 1.0f);
		//最初のビューのビューのライトの設定
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
		//ビューのカメラの設定
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
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

		//徘徊範囲がわかるようにもう一つプレートを付ける
		Ptr = AddGameObject<GameObject>();
		PtrTransMatrix = Ptr->GetComponent<TransformMatrix>();
		//下のプレートと混じってしまうので少し上にあげる
		WorldMat.DefTransformation(
			Vector3(40.0f, 40.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.01f, 0.0f)
			);
		PtrTransMatrix->SetWorldMatrix(WorldMat);
		DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"WALL_TX");
		DrawComp->SetOwnShadowActive(true);

	}

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject(){
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<SeekObject>(v);
		}
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
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//プレイヤーの作成
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}




}
//endof  basedx11
