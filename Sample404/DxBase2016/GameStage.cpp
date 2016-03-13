
#include "stdafx.h"
#include "Project.h"


namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	Labyrinth 定義部
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//各定数
	//--------------------------------------------------------------------------------------
	int Labyrinth::m_Dirtable[24][4] = {
		0, 1, 2, 3, 0, 1, 3, 2, 0, 2, 1, 3, 0, 2, 3, 1, 0, 3, 1, 2, 0, 3, 2, 1,
		1, 0, 2, 3, 1, 0, 3, 2, 1, 2, 0, 3, 1, 2, 3, 0, 1, 3, 0, 2, 1, 3, 2, 0,
		2, 0, 1, 3, 2, 0, 3, 1, 2, 1, 0, 3, 2, 1, 3, 0, 2, 3, 0, 1, 2, 3, 1, 0,
		3, 0, 1, 2, 3, 0, 2, 1, 3, 1, 0, 2, 3, 1, 2, 0, 3, 2, 0, 1, 3, 2, 1, 0 };

	int Labyrinth::m_Dx[4] = { 2, 0, -2, 0 };
	int Labyrinth::m_Dy[4] = { 0, 2, 0, -2 };

	//--------------------------------------------------------------------------------------
	//	void Labyrinth::Add(
	//	int i,		//X軸
	//	int j		//Y軸
	//	);
	//	用途: サブ関数、サイトに加える
	//	戻り値: なし。
	//--------------------------------------------------------------------------------------
	void Labyrinth::Add(int i, int j){
		m_XX[m_Nsite] = i;
		m_YY[m_Nsite] = j;
		m_Nsite++;
	}

	//--------------------------------------------------------------------------------------
	//	int Labyrinth::Select(
	//	int* i,	//X軸
	//	int* j		//Y軸
	//	);
	//	用途: サブ関数、サイトを乱数で選ぶ
	//	戻り値: サイトの番号。サイトが尽きたら0
	//--------------------------------------------------------------------------------------
	int Labyrinth::Select(int* i, int* j){
		int r;
		if (m_Nsite == 0)
			return 0;
		m_Nsite--;
		r = (int)(m_Nsite * (rand() / (RAND_MAX + 1.0)));
		*i = m_XX[r];
		m_XX[r] = m_XX[m_Nsite];
		*j = m_YY[r];
		m_YY[r] = m_YY[m_Nsite];
		return 1;
	}

	//--------------------------------------------------------------------------------------
	//	Labyrinth::Labyrinth(
	//	DWORD MaxX,		//横軸の大きさ（偶数）
	//	DWORD MaxY		//縦軸の大きさ（偶数）
	//	);
	//	用途: コンストラクタ。迷路を作成する
	//	戻り値: なし（失敗時は例外をthrow）
	//--------------------------------------------------------------------------------------
	Labyrinth::Labyrinth(DWORD  MaxX, DWORD  MaxY)
		:
		m_MaxX(MaxX), m_MaxY(MaxY),
		m_MaxSite(MaxX *MaxY / 4),
		m_Map(MaxX + 1, vector<int>(MaxY + 1)),	//二次元配列のイニシャライザ
		m_XX(m_MaxSite), m_YY(m_MaxSite), //1次元配列のイニシャライザ
		m_Nsite(0){
		try{
			int i, j, i1, j1, d, t, *tt;
			for (i = 0; i <= m_MaxX; i++){
				for (j = 0; j <= m_MaxY; j++){
					m_Map[i][j] = 1;
				}
			}
			for (i = 3; i <= m_MaxX - 3; i++){
				for (j = 3; j <= m_MaxY - 3; j++){
					m_Map[i][j] = 0;
				}
			}
			m_Map[2][3] = 0;
			m_Map[m_MaxX - 2][m_MaxY - 3] = 0;
			for (i = 4; i <= m_MaxX - 4; i += 2){
				Add(i, 2);
				Add(i, m_MaxY - 2);
			}
			for (j = 4; i <= m_MaxY - 4; j += 2){
				Add(2, j);
				Add(m_MaxX - 2, j);
			}
			while (Select(&i, &j)){
				for (;;){
					tt = m_Dirtable[(int)(24 * (rand() / (RAND_MAX * 1.0)))];
					for (d = 3; d >= 0; d--){
						t = tt[d];
						i1 = i + m_Dx[t];
						j1 = j + m_Dy[t];
						if (m_Map[i1][j1] == 0){
							break;
						}
					}
					if (d < 0){
						break;
					}
					m_Map[(i + i1) / 2][(j + j1) / 2] = 1;
					i = i1;
					j = j1;
					m_Map[i][j] = 1;
					Add(i, j);
				}
			}
		}
		catch (...){
			//再スロー
			throw;
		}
	}

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

		
	}

	//データマップをXZ入れ替えする
	void GameStage::ChangeVecXZ(const vector< vector<int> >& SrcMap, vector< vector<int> >& DestMap){
		DestMap.clear();
		size_t count = 0;
		size_t maxx = SrcMap[0].size();
		while (count < maxx){
			vector<int> ZLine(SrcMap.size());
			for (size_t i = 0; i < SrcMap.size(); i++){
				ZLine[i] = SrcMap[i][count];
			}
			DestMap.push_back(ZLine);
			count++;
		}
	}

	//固定ボックスをAddする
	void GameStage::SetFixedBox(vector< vector<int> >& DataMap, bool XLine){
		for (size_t i = 0; i < DataMap.size(); i++){
			auto& line = DataMap[i];
			int Size = 0;
			int Start = 0;
			int MiniSize = 0;
			if (XLine){
				MiniSize = 1;
			}
			for (size_t j = 0; j < line.size(); j++){
				if (line[j]){
					if (Size == 0){
						Start = j;
					}
					Size++;
				}
				else{
					if (Size > MiniSize){
						if (XLine){
							Vector3 Pos = Vector3(Start + (((float)Size) / 2.0f) - 0.5f, 0.5f, i * 1.0f);
							Pos.x *= m_Piece.x;
							Pos.y *= m_Piece.y;
							Pos.z *= m_Piece.z;

							Pos -= Vector3(((float)line.size() - 1) / 2.0f * m_Piece.x, 0, ((float)DataMap.size() - 1) / 2.0f * m_Piece.z);

							AddGameObject<FixedBox>(
								Vector3((float)Size * m_Piece.x, m_Piece.y, 1.0f * m_Piece.z),
								Vector3(0.0f, 0.0f, 0.0f),
								Pos
								);
							for (int k = 0; k < Size; k++){
								line[Start + k] = 0;
							}
						}
						else{
							Vector3 Pos = Vector3(i * 1.0f, 0.5f, Start + (((float)Size) / 2.0f) - 0.5f);
							Pos.x *= m_Piece.x;
							Pos.y *= m_Piece.y;
							Pos.z *= m_Piece.z;
							Pos -= Vector3(((float)DataMap.size() - 1) / 2.0f * m_Piece.x, 0, ((float)line.size() - 1) / 2.0f * m_Piece.z);

							AddGameObject<FixedBox>(
								Vector3(1.0f * m_Piece.x, m_Piece.y, (float)Size * m_Piece.z),
								Vector3(0.0f, 0.0f, 0.0f),
								Pos
								);
						}
					}
					Size = 0;
				}
			}
			if (Size > MiniSize){
				if (XLine){
					Vector3 Pos = Vector3(Start + (((float)Size) / 2.0f) - 0.5f, 0.5f, (i)* 1.0f);
					Pos.x *= m_Piece.x;
					Pos.y *= m_Piece.y;
					Pos.z *= m_Piece.z;
					Pos -= Vector3(((float)line.size() - 1) / 2.0f * m_Piece.x, 0, ((float)DataMap.size() - 1) / 2.0f * m_Piece.z);
					AddGameObject<FixedBox>(
						Vector3((float)Size * m_Piece.x, m_Piece.y, 1.0f * m_Piece.z),
						Vector3(0.0f, 0.0f, 0.0f),
						Pos
						);
					for (int k = 0; k < Size; k++){
						line[Start + k] = 0;
					}
				}
				Size = 0;
			}
		}
	}


	//固定のボックスの作成
	void GameStage::CreateFixedBox(){
		//マネージャから参照できるようにグループを作成
		CreateSharedObjectGroup(L"FIXED_BOX");
		//迷路の作成
		Labyrinth labyrinth(m_MapSizeX, m_MapSizeZ);
		//迷路マップの取得
		auto& LabMap = labyrinth.GetMap();
		//横軸
		vector< vector<int> > DataMap;
		ChangeVecXZ(LabMap, DataMap);
		//一番外の壁に接している通路を作成
		for (size_t i = 1; i < DataMap.size() - 1; i++){
			auto line = DataMap[i];
			for (size_t j = 1; j < line.size() - 1; j++){
				if (i == 1 || i == (DataMap.size() - 2)){
					DataMap[i][j] = 0;
				}
				if (j == 1 || j == (line.size() - 2)){
					DataMap[i][j] = 0;
				}

			}
		}
		//横軸のセット
		SetFixedBox(DataMap, true);
		//縦軸
		vector< vector<int> > DataMap2;
		ChangeVecXZ(DataMap, DataMap2);
		//縦軸のセット
		SetFixedBox(DataMap2, false);
		//ボックス描画マネージャの作成
		AddGameObject<FixedBoxManager>();
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
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...){
			throw;
		}
	}




}
//endof  basedx11
