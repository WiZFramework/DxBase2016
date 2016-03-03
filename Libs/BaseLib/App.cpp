#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct AudioManager::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct AudioManager::Impl{
		bool m_audioAvailable;
		ComPtr<IXAudio2>    m_musicEngine;
		ComPtr<IXAudio2>    m_soundEffectEngine;
		IXAudio2MasteringVoice* m_musicMasteringVoice;
		IXAudio2MasteringVoice* m_soundEffectMasteringVoice;
		Impl() :
			m_audioAvailable{false}
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	//--------------------------------------------------------------------------------------
	AudioManager::AudioManager() :
		pImpl(new Impl)
	{
	}
	AudioManager::~AudioManager(){}

	void AudioManager::CreateDeviceIndependentResources()
	{
		UINT32 flags = 0;

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_musicEngine, flags),
			L"音楽用オーディオエンジンの初期化に失敗しました",
			L"XAudio2Create(&m_musicEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);

#if defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
		debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
		debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
		pImpl->m_musicEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		HRESULT hr = pImpl->m_musicEngine->CreateMasteringVoice(&pImpl->m_musicMasteringVoice);
		if (FAILED(hr))
		{
			throw BaseException(
				L"音楽のマスタリングボイスの初期化に失敗しました",
				L"m_musicEngine->CreateMasteringVoice(&m_musicMasteringVoice)",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
			// Unable to create an audio device
			pImpl->m_audioAvailable = false;
			return;
		}

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_soundEffectEngine, flags),
			L"サウンド用オーディオエンジンの初期化に失敗しました",
			L"XAudio2Create(&m_soundEffectEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);
#if defined(_DEBUG)
		pImpl->m_soundEffectEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		ThrowIfFailed(
			pImpl->m_soundEffectEngine->CreateMasteringVoice(&pImpl->m_soundEffectMasteringVoice),
			L"サウンド用マスタリングボイスの初期化に失敗しました",
			L"m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice)",
			L"AudioManager::CreateDeviceIndependentResources()"
			);
		pImpl->m_audioAvailable = true;
	}

	IXAudio2* AudioManager::GetMusicEngine()const
	{
		return pImpl->m_musicEngine.Get();
	}

	IXAudio2* AudioManager::GetSoundEffectEngine()const
	{
		return pImpl->m_soundEffectEngine.Get();
	}

	void AudioManager::SuspendAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			pImpl->m_musicEngine->StopEngine();
			pImpl->m_soundEffectEngine->StopEngine();
		}
	}

	void AudioManager::ResumeAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			ThrowIfFailed(
				pImpl->m_musicEngine->StartEngine(),
				L"音楽用エンジンのスタートに失敗しました",
				L"m_musicEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
			ThrowIfFailed(
				pImpl->m_soundEffectEngine->StartEngine(),
				L"サウンド用エンジンのスタートに失敗しました",
				L"m_soundEffectEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
				);
		}
	}




	//--------------------------------------------------------------------------------------
	//	class App;
	//	用途: アプリケーションクラス
	//--------------------------------------------------------------------------------------
	//static変数実体
	unique_ptr<App, App::AppDeleter> App::m_App;
	//構築
	App::App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height) :
		m_hInstance{ hInstance },
		m_hWnd{ hWnd },
		m_FullScreen{ FullScreen },
		m_GameWidth{ Width },
		m_GameHeight{ Height },
		m_Timer()
	{
		try{
			//デバイスリソースの構築
			m_DeviceResources = shared_ptr<DeviceResources>(new DeviceResources(hWnd, FullScreen, Width, Height));
			//基準ディレクトリの設定
			//相対パスにすると、ファイルダイアログでカレントパスが狂うので
			//絶対パス指定
			wchar_t Modulebuff[MAX_PATH];
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];

			::ZeroMemory(Modulebuff, sizeof(Modulebuff));
			::ZeroMemory(Drivebuff, sizeof(Drivebuff));
			::ZeroMemory(Dirbuff, sizeof(Dirbuff));
			::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
			::ZeroMemory(Extbuff, sizeof(Extbuff));

			//モジュール名（プログラムファイル名）を得る
			if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))){
				throw BaseException(
					L"モジュールが取得できません。",
					L"if(!::GetModuleFileName())",
					L"App::App()"
					);
			}
			m_wstrModulePath = Modulebuff;
			//モジュール名から、各ブロックに分ける
			_wsplitpath_s(Modulebuff,
				Drivebuff, _MAX_DRIVE,
				Dirbuff, _MAX_DIR,
				FileNamebuff, _MAX_FNAME,
				Extbuff, _MAX_EXT);

			//ドライブ名の取得
			m_wstrDir = Drivebuff;
			//ディレクトリ名の取得
			m_wstrDir += Dirbuff;
			//mediaディレクトリを探す
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"media";
			//まず、実行ファイルと同じディレクトリを探す
			DWORD RetCode;
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF){
				//失敗した
				m_wstrDataPath = m_wstrDir;
				m_wstrDataPath += L"..\\media";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF){
					//再び失敗した
					throw BaseException(
						L"mediaディレクトリを確認できません。",
						L"if(RetCode == 0xFFFFFFFF)",
						L"App::App()"
						);
				}
				else{
					m_wstrDataPath += L"\\";
					//相対パスの設定
					m_wstrRelativeDataPath = L"..\\media\\";
				}
			}
			else{
				m_wstrDataPath += L"\\";
				//相対パスの設定
				m_wstrRelativeDataPath = L"media\\";
			}
			m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
			m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";

			//オーディオマネージャの取得
			GetAudioManager();

			//乱数の初期化
			srand((unsigned)time(nullptr));


		}
		catch (...){
			throw;
		}
	}

	//シングルトン構築とアクセサ
	unique_ptr<App, App::AppDeleter>& App::GetApp(HINSTANCE hInstance, HWND hWnd,
		bool FullScreen, UINT Width, UINT Height){
		try{
			if (m_App.get() == 0){
				//自分自身の構築
				m_App.reset(new App(hInstance, hWnd, FullScreen, Width, Height));
			}
			return m_App;
		}
		catch (...){
			throw;
		}

	}
	//強制破棄
	void App::DeleteApp(){
		if (m_App.get()){
			m_App.reset();
		}
	}
	// オーディオマネージャの取得
	unique_ptr<AudioManager>& App::GetAudioManager(){
		try{
			if (m_AudioManager.get() == 0){
				m_AudioManager.reset(new AudioManager);
				m_AudioManager->CreateDeviceIndependentResources();

			}
			return m_AudioManager;
		}
		catch (...){
			throw;
		}
	}

	void App::SetSceneBase(const shared_ptr<SceneBase>& ptr){
		if (m_SceneBase.get()){
			m_SceneBase.reset();
		}
		m_SceneBase = ptr;
	}

	void App::RegisterResource(const wstring& Key,const shared_ptr<BaseResource>& ResObj){
		try{
			if (Key == L""){
				throw BaseException(
					L"キーが空白です",
					L"if(Key == L\"\")",
					L"App::RegisterResource()"
					);
			}
			if (!ResObj){
				throw BaseException(
					L"リソースが不定です",
					L"if(!pResObj)",
					L"App::RegisterResource()"
					);
			}
			map<wstring, shared_ptr<BaseResource> >::iterator it;
			//重複ポインタの検査
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++){
				if (it->second == ResObj){
					//ポインタが重複していても、キーが同じなら
					//すでに登録されているのでリターン
					if (it->first == Key){
						return;
					}
					wstring keyerr = Key;
					throw BaseException(
						L"すでにそのリソースは登録されています",
						keyerr,
						L"App::RegisterResource()"
						);
				}
			}
			//重複キーの検査
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//指定の名前が見つかった
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"すでにそのキーは登録されています",
					keyerr,
					L"App::RegisterResource()"
					);
			}
			else{
				//見つからない
				//リソースペアの追加
				m_ResMap[Key] = ResObj;
			}
		}
		catch (...){
			throw;
		}
	}

	//テクスチャの登録(同じキーのテクスチャがなければファイル名で作成し、登録)
	//同じ名前のテクスチャがあればそのポインタを返す
	shared_ptr<TextureResource> App::RegisterTexture(const wstring& Key, const wstring& TextureFileName, const wstring& TexType){
		if (CheckResource<TextureResource>(Key)){
			return GetResource<TextureResource>(Key);
		}
		//
		auto PtrTexture = ObjectFactory::Create<TextureResource>(TextureFileName, TexType);
		RegisterResource(Key, PtrTexture);
		return PtrTexture;
	}

	//--------------------------------------------------------------------------------------
	//	shared_ptr<AudioResource> RegisterWav(
	//		const wstring& Key, // リソースキー
	//		const wstring& WavFileName	//Wavファイル名
	//	);
	shared_ptr<AudioResource> App::RegisterWav(const wstring& Key, const wstring& WavFileName){
		if (CheckResource<AudioResource>(Key)){
			return GetResource<AudioResource>(Key);
		}
		//
		auto PtrWav = ObjectFactory::Create<AudioResource>(WavFileName);
		RegisterResource(Key, PtrWav);
		return PtrWav;
	}
	void App::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		m_SceneBase->OnMessage(message,wParam, lParam);
	}
	//操作
	void App::Update(){
		//入力機器状態をリセット
		m_InputDevice.ResetControlerState();
		// シーン オブジェクトを更新します。
		m_Timer.Tick([&]()
		{
			m_SceneBase->OnUpdate();
		});
	}
	bool App::Draw(){
		// 初回更新前にレンダリングは行わない。
		if (m_Timer.GetFrameCount() == 0)
		{
			return false;
		}
		m_SceneBase->OnDraw();
		return true;
	}
	void App::Present(UINT SyncInterval, UINT Flags){
		// バックバッファからフロントバッファに転送
		GetDeviceResources()->GetSwapChain()->Present(SyncInterval, Flags);
	}




}



//end basedx11
