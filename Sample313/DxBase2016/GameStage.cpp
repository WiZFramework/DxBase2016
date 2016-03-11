
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
		//パラメータは4096.0fにしている
		//影を細かくするとメモリを大量に消費するので注意！
		CreateDefaultRenderTargets(4096.0f);
		//影のビューサイズの設定。この値を小さくすると影が表示される範囲が小さくなる。
		//値が小さいほうが影は細かくなる
		//スタティック変数なので一度設定したらその値はステージを超えて保持される。
		Shadowmap::SetViewSize(16.0f);
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

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject(){


		wstring MediaPath = App::GetApp()->m_wstrRelativeDataPath;
		//CSVファイルの決定
		wstring CsvFilename = MediaPath + L"GameStage.csv";
		//ローカル上にCSVファイルクラスを作成
		CsvFile GameStageCsv(CsvFilename);
		if (!GameStageCsv.ReadCsv()){
			//ファイルは存在しなかった
			// 初期化失敗
			throw BaseException(
				L"CSVファイルがありません。",
				CsvFilename,
				L"GameStage::CreateSeekObject()"
				);
		}
		//追い掛けるオブジェクトデータを読み込み
		//ラムダ式を利用
		//わかりやすいようにインラインでは書かない
		//以下は、wstringを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [](const wstring& str)->bool{
			//1行がコンマ区切りの文字列になっているので分解する
			vector< wstring > StrVec;
			Util::WStrToTokenVector(StrVec, str, L',');
			if (StrVec[0] == L"SeekObject" && StrVec[1] == L"1" && StrVec[2] == L"Release"){
				return true;
			}
			else{
				return false;
			}
		};
		//--------------------------------------------------------
		//ワーク用の文字列の配列
		vector<wstring> SeekObjectVec;
		//csvの中から、条件（ラムダ式で検査）で抽出
		GameStageCsv.GetSelect(SeekObjectVec, func);

		//上記の構文は、以下のようにインラインで書ける
		/*
		GameStageCsv.GetSelect(SeekObjectVec,
		[](const wstring& str)->bool{
		//1行がコンマ区切りの文字列になっているので分解する
		vector< wstring > StrVec;
		Util::WStrToTokenVector(StrVec, str, L',');
		if (StrVec[0] == L"SeekObject" && StrVec[1] == L"1" && StrVec[2] == L"Release"){
		return true;
		}
		else{
		return false;
		}
		}
		);
		*/

		/*
		//キャプチャつきのラムダ式として以下のようにも書ける

		wstring StageStr = L"1";
		wstring ReleaseStr = L"Release";
		auto func2 = [&](const wstring& str)->bool{
		//1行がコンマ区切りの文字列になっているので分解する
		vector< wstring > StrVec;
		Util::WStrToTokenVector(StrVec, str, L',');
		if (StrVec[0] == L"SeekObject" && StrVec[1] == StageStr && StrVec[2] == ReleaseStr){
		return true;
		}
		else{
		return false;
		}
		};
		//キャプチャつきラムダ式を使ったselect
		GameStageCsv.GetSelect2(SeekObjectVec, func2);
		*/
		/*
		//キャプチャつきのラムダ式を一度に書く場合

		wstring StageStr = L"1";
		wstring ReleaseStr = L"Release";
		//キャプチャつきラムダ式を使ったselect
		GameStageCsv.GetSelect2(SeekObjectVec,
		[&](const wstring& str)->bool{
		//1行がコンマ区切りの文字列になっているので分解する
		vector< wstring > StrVec;
		Util::WStrToTokenVector(StrVec, str, L',');
		if (StrVec[0] == L"SeekObject" && StrVec[1] == StageStr && StrVec[2] == ReleaseStr){
		return true;
		}
		else{
		return false;
		}
		}
		);
		*/

		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配置オブジェクトの作成
		for (auto LineStr : SeekObjectVec){
			//1行がコンマ区切りの文字列になっているので分解する
			vector< wstring > LineVec;
			Util::WStrToTokenVector(LineVec, LineStr, L',');
			//分解した配列からデータを取り出す
			AddGameObject<SeekObject>(Vector3(LineVec[3], LineVec[4], LineVec[5]));
		}

	}

	//プレイヤーの作成
	void GameStage::CreatePlayer(){
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//カプセルの作成
	void GameStage::CreateCapsule(){
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 20.0f },
			{ 20.0f, 0, -20.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<CapsuleObject>(v);
		}
	}

	//ヒットする球体の作成
	void GameStage::CreateSphere(){
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, -25.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec){
			AddGameObject<SphereObject>(v);
		}
	}



	//衝突するスクエアの作成
	void GameStage::CreateHitTestSquare(){
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"SquareGroup");
		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 1.0f, 1.0f),
			Vector3(0, 0.0f, 0.0f),
			Vector3(0.0f, 0.5f, 10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -15.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(10.0f, 0.4f, -10.0f)
			);

		AddGameObject< HitTestSquare >(
			Vector3(10.0f, 5.0f, 1.0f),
			Vector3(XM_PIDIV2, 0, 0.0f),
			Vector3(20.0f, 0.4f, -15.0f)
			);
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
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//カプセル作成
			CreateCapsule();
			//球体作成
			CreateSphere();
			//衝突するスクエアの作成
			CreateHitTestSquare();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}




}
//endof  basedx11
