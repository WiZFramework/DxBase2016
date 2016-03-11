
#include "stdafx.h"
#include "Project.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class MenuStage : public Stage;
	//	用途: ゲームステージクラス
	//--------------------------------------------------------------------------------------
	//リソースの作成
	void MenuStage::CreateResourses(){
		wstring CursorWav = App::GetApp()->m_wstrRelativeDataPath + L"Sounds\\cursor.wav";
		App::GetApp()->RegisterWav(L"Cursor", CursorWav);
		wstring DecideSE = App::GetApp()->m_wstrRelativeDataPath + L"Sounds\\decide.wav";
		App::GetApp()->RegisterWav(L"Decide", DecideSE);
	}

	//ビュー類の作成
	void MenuStage::CreateViews(){
		//最初にデフォルトのレンダリングターゲット類を作成する
		CreateDefaultRenderTargets(2048.0f);
		//マルチビューコンポーネントの取得
		auto PtrMultiView = GetComponent<MultiView>();
		//マルチビューにビューの追加
		auto PtrView = PtrMultiView->AddView();
		//ビューの矩形を設定（ゲームサイズ全体）
		Rect2D<float> rect(0, 0, 640, 360);
		Point2D<float> offset(0, 70);
		//最初のビューにパラメータの設定
		PtrView->ResetParamaters<Camera, MultiLight>(rect + offset, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 0.0f, -1.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		auto PtrLight = PtrView->GetMultiLight()->GetLight(0);
		PtrLight->SetDirectional(0.0f, 0.0f, 0.5f);
	}

	//メニュースプライトアニメーションの作成
	void MenuStage::CreateSSMenu(){
		AddGameObject<StageSelectAnimation>( App::GetApp()->m_wstrRelativeDataPath + L"StageSelect\\", L"Opening" ) ;
	}

	//初期化
	void MenuStage::OnCreate(){
		try{
			//リソースの作成
			CreateResourses();
			//ビュー類を作成する
			CreateViews();
			//メニュースプライトアニメーションの作成
			CreateSSMenu();
		}
		catch (...){
			throw;
		}
	}

	//操作
	void MenuStage::OnUpdate(){
		//	: コントローラ情報の取得.
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();

		//	: Bボタンが押されたら.
		if ( CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B ) {
			PostEvent( 0.0f, GetThis<MenuStage>(), App::GetApp()->GetSceneBase(), L"ToGame" ) ;
			
		}
	}

}
//endof  basedx11