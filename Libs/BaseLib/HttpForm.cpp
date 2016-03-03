#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct StaticText::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct StaticText::Impl{
		Impl()
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class StaticText : public InputObject;
	//	用途: 表示用テキスト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	StaticText::StaticText(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	StaticText::~StaticText(){}

	//初期化
	void StaticText::OnCreate(){
		//表示用テキストの作成
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetStartPosition(Point2D<float>(0, 0));
		PtrString->SetText(L"");
		PtrString->SetKeyInput(false);
	}

	void StaticText::SetFont(const wstring& FontName, float FontSize){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFont(FontName, FontSize);
	}
	StringSprite::TextAlignment StaticText::GetTextAlignment() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextAlignment();
	}
	void StaticText::SetTextAlignment(StringSprite::TextAlignment Alignment){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextAlignment(Alignment);
	}

	const Color4& StaticText::GetFontColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontColor();
	}
	void StaticText::SetFontColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFontColor(Col);
	}
	const wstring& StaticText::GetFontName() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontName();

	}
	float StaticText::GetFontSize() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontSize();
	}

	const Color4& StaticText::GetBackColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackColor();
	}
	void StaticText::SetBackColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackColor(Col);
	}

	Point2D<float> StaticText::GetBackTextMargin() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackTextMargin();
	}
	void StaticText::SetBackTextMargin(Point2D<float> p){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackTextMargin(p);
	}

	Rect2D<float> StaticText::GetTextRect() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextRect();
	}
	void StaticText::SetTextRect(const Rect2D<float>& rect){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextRect(rect);
	}

	const wstring& StaticText::GetText() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetText();
	}
	void StaticText::SetText(const wstring& str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetText(str);
	}
	void StaticText::AddText(const wstring& str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->AddText(str);
	}


	//--------------------------------------------------------------------------------------
	//	struct InputObject::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct InputObject::Impl{
		wstring m_Name;
		wstring m_Value;
		//アトリビュートマップ
		map<wstring, wstring> m_AttributeMap;
		Impl():
			m_Name(L""),
			m_Value(L"")
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class InputObject : public Object;
	//	用途: 入力オブジェクトの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	InputObject::InputObject(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{
		pImpl->m_AttributeMap[L"Name"] = Name;
		pImpl->m_AttributeMap[L"Value"] = Value;
	}
	InputObject::~InputObject(){}

	const wstring& InputObject::GetAttribute(const wstring& Key) const{
		return pImpl->m_AttributeMap[Key];
	}
	void InputObject::SetAttribute(const wstring& Key, const wstring& Val){
		pImpl->m_AttributeMap[Key] = Val;
	}



	const wstring& InputObject::GetName(){
		return pImpl->m_AttributeMap[L"Name"];
	}
	void InputObject::SetName(const wstring& Str){
		pImpl->m_AttributeMap[L"Name"] = Str;
	}
	const wstring& InputObject::GetValue(){
		return pImpl->m_AttributeMap[L"Value"];
	}
	void InputObject::SetValue(const wstring& Str){
		pImpl->m_AttributeMap[L"Value"] = Str;
	}


	//--------------------------------------------------------------------------------------
	//	class InputHidden : public InputObject;
	//	用途: 表示なしテキスト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	InputHidden::InputHidden(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		InputObject(StagePtr, Name, Value)
	{}
	InputHidden::~InputHidden(){}




	//--------------------------------------------------------------------------------------
	//	struct InputText::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct InputText::Impl{
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class InputText : public InputObject;
	//	用途: 入力テキスト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	InputText::InputText(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		InputObject(StagePtr, Name, Value),
		pImpl(new Impl())
	{}
	InputText::~InputText(){}

	bool InputText::IsFocus()const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->IsFocus();
	}
	bool InputText::GetFocus()const{
		return IsFocus();
	}

	void InputText::SetFocus(bool b){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFocus(true);
	}

	void InputText::SetFont(const wstring& FontName, float FontSize){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFont(FontName, FontSize);
	}
	StringSprite::TextAlignment InputText::GetTextAlignment() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextAlignment();
	}
	void InputText::SetTextAlignment(StringSprite::TextAlignment Alignment){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextAlignment(Alignment);
	}

	const Color4& InputText::GetFontColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontColor();
	}
	void InputText::SetFontColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFontColor(Col);
	}
	const wstring& InputText::GetFontName() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontName();

	}
	float InputText::GetFontSize() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontSize();
	}

	const Color4& InputText::GetBackColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackColor();
	}
	void InputText::SetBackColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackColor(Col);
	}

	Point2D<float> InputText::GetBackTextMargin() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackTextMargin();
	}
	void InputText::SetBackTextMargin(Point2D<float> p){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackTextMargin(p);
	}



	const Color4& InputText::GetCaretColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetCaretColor();
	}
	void InputText::SetCaretColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetCaretColor(Col);
	}
	float InputText::GetCaretSpeed() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetCaretSpeed();
	}
	void InputText::SetCaretSpeed(float f){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetCaretSpeed(f);
	}


	Rect2D<float> InputText::GetTextRect() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextRect();
	}
	void InputText::SetTextRect(const Rect2D<float>& rect){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextRect(rect);
	}

	const wstring& InputText::GetValue(){
		auto Ptr = GetComponent<StringSprite>();
		//入力テキストをValueにセット
		InputObject::SetValue(Ptr->GetText());
		return InputObject::GetValue();
	}
	void InputText::SetValue(const wstring& Str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetText(Str);
		//入力テキストと同じものをValueにセット
		InputObject::SetValue(Str);
	}


	//初期化
	void InputText::OnCreate(){
		//入力テキストの作成
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetStartPosition(Point2D<float>(240.0f, 320.0f));
		PtrString->SetText(InputObject::GetValue());
		PtrString->SetKeyInput(true);
	}


	//--------------------------------------------------------------------------------------
	//	struct HttpForm::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct HttpForm::Impl{
		HINTERNET m_hSession;	//セッションハンドル
		wstring m_SessionName;	//セッション名
		HINTERNET m_hConnect;	//コネクションハンドル
		HINTERNET m_hRequest;	//リクエストハンドル
		stringstream m_ReadData;	//読み込み済みデータ

		string m_StrBody;	//POST用に使用される文字列

		wstring m_GroupName;	//グループ登録時の名前
		bool m_SubmitLock;	//サブミットのロック
		Impl() :
			m_hSession(NULL),
			m_SessionName(L"DxBaseSession"),
			m_hConnect(NULL),
			m_hRequest(NULL),
			m_SubmitLock(false)
		{}
		~Impl(){}
		//ユーティリティ関数
		bool IsOkChar(char c) {
			if (isalnum(c) || c == '.' || c == '-' || c == '_' || c == '*'){
				return true;
			}
			return false;
		}
		void EncodeHex(char c, char* Buff) {
			Buff[1] = "0123456789ABCDEF"[(c & 0xF0) >> 4];
			Buff[2] = "0123456789ABCDEF"[c & 0x0F];
		}
		void UrlEnc(const char* pC, string& Ret){
			//初期化
			Ret = "";
			char Buff[] = { '%', 0, 0, 0 };
			while (*pC != '\0'){
				if (IsOkChar(*pC)){
					Ret += *pC;
				}
				else if (*pC == ' '){
					Ret += '+';
				}
				else{
					EncodeHex(*pC, Buff);
					Ret += Buff;
				}
				pC++;
			}
		}
		void MakeParam(const wstring& Key, const wstring& Value, wstring& Ret,bool FirstFlg,bool Get = true){
			string Utf8Str;
			string UrlEncStr;
			wstring WUrlParam;
			Ret = L"";
			if (FirstFlg ){
				if (Get){
					Ret += L"?";
				}
			}
			else{
				Ret += L"&";
			}
			Util::ConvertWstringtoUtf8(Key, Utf8Str);
			UrlEnc(Utf8Str.c_str(), UrlEncStr);
			Util::MBtoWS(UrlEncStr, WUrlParam);
			Ret += WUrlParam;
			Ret += L"=";
			Util::ConvertWstringtoUtf8(Value, Utf8Str);
			UrlEnc(Utf8Str.c_str(), UrlEncStr);
			Util::MBtoWS(UrlEncStr, WUrlParam);
			Ret += WUrlParam;
		}
		void ReadDataClear(){
			m_ReadData.str("");
			m_ReadData.clear(stringstream::goodbit);
		}
		void Clear(){
			if (m_hRequest){
				WinHttpCloseHandle(m_hRequest);
				m_hRequest = NULL;
			}
			if (m_hConnect){
				WinHttpCloseHandle(m_hConnect);
				m_hConnect = NULL;
			}
			m_SubmitLock = false;
		}
		void SessionClear(){
			if (m_hSession){
				//セッションのクローズ
				WinHttpCloseHandle(m_hSession);
				m_hSession = NULL;
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	class HttpForm : public GameObject;
	//	用途: HttpFormクラス
	//--------------------------------------------------------------------------------------
	void HttpForm::SessionCreate(){
		try{
			// セッションオブジェクトの作成
			WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ProxyConfig;
			ZeroMemory(&ProxyConfig, sizeof(ProxyConfig));
			WinHttpGetIEProxyConfigForCurrentUser(&ProxyConfig);
			if (ProxyConfig.lpszProxy){
				// InternetExplorerのプロキシを使用してセッションオブジェクト作成
				pImpl->m_hSession = WinHttpOpen(
					pImpl->m_SessionName.c_str(),	//ユーザーエージェント名 
					WINHTTP_ACCESS_TYPE_NAMED_PROXY,//プロキシを設定する
					ProxyConfig.lpszProxy,	// プロキシ名称
					ProxyConfig.lpszProxyBypass ?
					ProxyConfig.lpszProxyBypass : WINHTTP_NO_PROXY_BYPASS,	//プロキシバイパスの設定
					WINHTTP_FLAG_ASYNC
					);
			}
			else{
				pImpl->m_hSession = WinHttpOpen(
					pImpl->m_SessionName.c_str(),	//ユーザーエージェント名 
					WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,	//デフォルトのプロキシ
					WINHTTP_NO_PROXY_NAME,	// プロキシ名称
					WINHTTP_NO_PROXY_BYPASS,	// プロキシバイパスの設定
					WINHTTP_FLAG_ASYNC
					);
			}
			if (!pImpl->m_hSession){
				throw BaseException(
					L"Httpのセッションが確立できませんでした",
					L"if (!pImpl->m_hSession)",
					L"HttpForm::Create()"
					);
			}
		}
		catch (...){
			throw;
		}
	}


	//構築と消滅
	HttpForm::HttpForm(const shared_ptr<Stage>& StagePtr,const wstring& GroupName) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{
		pImpl->m_GroupName = GroupName;
		StagePtr->CreateSharedObjectGroup(pImpl->m_GroupName);
	}
	HttpForm::~HttpForm(){
		pImpl->Clear();
		pImpl->ReadDataClear();
		pImpl->SessionClear();
	}

	//アクセサ
	const wstring& HttpForm::GetGroupName() const{
		return pImpl->m_GroupName;
	}

	//操作
	void HttpForm::MoveFocusNextInput(){
		vector<shared_ptr<InputText>> InputTextVec;
		//タブキーを押されたら、フォーカスを変更する
		auto Group = GetStage()->GetSharedObjectGroup(GetGroupName());
		auto GroupVec = Group->GetGroupVector();
		for (size_t i = 0; i < GroupVec.size(); i++){
			if (!GroupVec[i].expired()){
				auto ShPtr = dynamic_pointer_cast<InputText>(GroupVec[i].lock());
				if (ShPtr){
					InputTextVec.push_back(ShPtr);
				}
			}
		}
		if (InputTextVec.size() > 0){
			bool FocusActive = false;
			for (size_t i = 0; i < InputTextVec.size(); i++){
				if (InputTextVec[i]->IsFocus()){
					FocusActive = true;
					//どこかにフォーカスがあった
					if ((i + 1) < InputTextVec.size()){
						InputTextVec[i + 1]->SetFocus(true);
						break;
					}
					else{
						InputTextVec[0]->SetFocus(true);
						break;
					}
				}
			}
			if (!FocusActive){
				//どこにもフォーカスがない
				InputTextVec[0]->SetFocus(true);
			}
		}
	}


	shared_ptr<InputText> HttpForm::AddInputText(const wstring& Name, const wstring& Value){
		auto Ptr = GetStage()->AddGameObject<InputText>(Name, Value);
		Ptr->SetFocus(true);
		//背景は白
		Ptr->SetBackColor(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		//フォントは黒
		Ptr->SetFontColor(Color4(0.0f, 0.0f, 0.0f, 1.0f));
		//キャレットも黒
		Ptr->SetCaretColor(Color4(0.0f, 0.0f, 0.0f, 1.0f));
		auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
		Group->IntoGroup(Ptr);
		return Ptr;
	}

	shared_ptr<InputHidden> HttpForm::AddInputHidden(const wstring& Name, const wstring& Value){
		auto Ptr = GetStage()->AddGameObject<InputHidden>(Name, Value);
		auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
		Group->IntoGroup(Ptr);
		return Ptr;
	}


	//コールバック関数
	void CALLBACK HttpFormRequestCallBack(
		HINTERNET hInternet,
		DWORD_PTR PtrContext,
		DWORD InternetStatus,
		LPVOID pStatusInformation,
		DWORD StatusInformationLength
		){
		if (PtrContext == 0){
			return;
		}
		//インスタンスのコールバックメッセージを呼ぶ
		HttpForm* pHttpForm = reinterpret_cast<HttpForm*>(PtrContext);
		pHttpForm->OnCallback(InternetStatus,pStatusInformation,StatusInformationLength);
	}


	void HttpForm::GetRequest(const wstring& Url){
		Request(Url,false);
	}
	void HttpForm::PostRequest(const wstring& Url){
		Request(Url, true);
	}



	void HttpForm::Request(const wstring& Url, bool Post){
		if (!pImpl->m_hSession){
			SessionCreate();
		}
		const size_t UrlBufSize = 4096;
		if (!pImpl->m_SubmitLock && pImpl->m_hSession){
			pImpl->m_SubmitLock = true;
			auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
			wstring Param;
			wstring AllParams;
			auto GroupVec = Group->GetGroupVector();
			bool FirstFlg = true;
			for (auto Obj : GroupVec){
				if (!Obj.expired()){
					auto ShPtr = dynamic_pointer_cast<InputObject>(Obj.lock());
					if (ShPtr){
						//パラメータに追加
						if (Post){
							pImpl->MakeParam(ShPtr->GetName(), ShPtr->GetValue(), Param, FirstFlg,false);
						}
						else{
							pImpl->MakeParam(ShPtr->GetName(), ShPtr->GetValue(), Param, FirstFlg);
						}
						AllParams += Param;
						FirstFlg = false;
					}
				}
			}

			//URLを解析する
			URL_COMPONENTS UrlComponents;
			ZeroMemory(&UrlComponents, sizeof(UrlComponents));
			UrlComponents.dwStructSize = sizeof(UrlComponents);
			wchar_t HostName[UrlBufSize];
			wchar_t UrlPath[UrlBufSize];
			UrlComponents.lpszHostName = HostName;
			UrlComponents.lpszUrlPath = UrlPath;
			UrlComponents.dwHostNameLength = UrlBufSize;
			UrlComponents.dwUrlPathLength = UrlBufSize;
			if (!WinHttpCrackUrl(Url.c_str(),
				0,
				0,
				&UrlComponents)){
				pImpl->Clear();
				throw BaseException(
					L"URL解析に失敗しました",
					Url.c_str(),
					L"HttpForm::Request()"
					);
			}

			wstring StrHost = UrlComponents.lpszHostName;
			wstring StrUrlPath = UrlComponents.lpszUrlPath;
			INTERNET_PORT NumPort = UrlComponents.nPort;
			// HTTPかHTTPSかそれ以外か
			DWORD OpenRequestFlag = 0;
			if (UrlComponents.nScheme == INTERNET_SCHEME_HTTPS){
				OpenRequestFlag = WINHTTP_FLAG_SECURE;
			}
			wstring StrMethod;
			wstring StrHeader;
			//POSTボディ
			pImpl->m_StrBody = "";
			if (Post){
				StrMethod = L"POST";
				StrHeader = L"Content-Type: application/x-www-form-urlencoded\r\n";
				if (AllParams.size() > 0){
					Util::WStoMB(AllParams, pImpl->m_StrBody);
				}
			}
			else{
				StrMethod = L"GET";
				if (AllParams.size() > 0){
					StrUrlPath += AllParams;
				}
			}

			//HTTP接続
			pImpl->m_hConnect = WinHttpConnect(
				pImpl->m_hSession,
				StrHost.c_str(),
				NumPort,
				0);
			if (pImpl->m_hConnect == NULL){
				pImpl->Clear();
				throw BaseException(
					L"HTTP接続に失敗しました",
					StrHost.c_str(),
					L"HttpForm::Request()"
					);
			}
			//HTTPリクエストオープン
			pImpl->m_hRequest = WinHttpOpenRequest(
				pImpl->m_hConnect,
				StrMethod.c_str(),
				StrUrlPath.c_str(),
				NULL,
				WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				OpenRequestFlag
				);
			if (pImpl->m_hRequest == NULL){
				pImpl->Clear();
				throw BaseException(
					L"HTTPリクエストオープンに失敗しました",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}

			//コールバック関数の指定
			if ( 
				WinHttpSetStatusCallback(
					pImpl->m_hRequest,
					(WINHTTP_STATUS_CALLBACK)HttpFormRequestCallBack,
					WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS | WINHTTP_CALLBACK_FLAG_REDIRECT,
					NULL
				) == WINHTTP_INVALID_STATUS_CALLBACK)
			{
				pImpl->Clear();
				throw BaseException(
					L"コールバック指定に失敗しました",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}
			///HTTPリクエストの送信
			if(
				!WinHttpSendRequest(
					pImpl->m_hRequest,
					StrHeader.c_str(),
					StrHeader.length(),
					(LPVOID)pImpl->m_StrBody.c_str(),
					pImpl->m_StrBody.length() * sizeof(char),
					pImpl->m_StrBody.length() * sizeof(char),
					(DWORD_PTR)this
				)
			)
			{
				pImpl->Clear();
				throw BaseException(
					L"HTTPリクエストの送信に失敗しました",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}
		}
	}

	void HttpForm::OnCallback(DWORD InternetStatus, LPVOID pStatusInformation, DWORD StatusInformationLength){
		switch (InternetStatus)
		{
		case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
			if(!WinHttpReceiveResponse(pImpl->m_hRequest, NULL)){
				//セッション以外のクリア
				pImpl->Clear();
				OnResponseFailed(L"リクエスト送信完了できませんでした。");
				return;
			}
			break;
		case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
		{
			//ヘッダの取得
			DWORD dwStatusCode = 0;
			DWORD dwStatusCodeSize = sizeof(DWORD);
			if (!WinHttpQueryHeaders(pImpl->m_hRequest,
				WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,	// ステータスコードをDWORD型で取得する。
				WINHTTP_HEADER_NAME_BY_INDEX,	// ヘッダー名のポインタ
				&dwStatusCode,				// バッファー
				&dwStatusCodeSize,			// バッファーサイズ
				WINHTTP_NO_HEADER_INDEX))		// 最初に発生したヘッダーのみ取り出す
			{
				//セッション以外のクリア
				pImpl->Clear();
				OnResponseFailed(L"ヘッダの取得に失敗しました。");
				return;
			}
			if (HTTP_STATUS_OK != dwStatusCode)
			{
				//セッション以外のクリア
				pImpl->Clear();
				OnResponseFailed(L"ヘッダのステータスが不正でした。");
				return;
			}
			// レスポンスデータデータ問い合わせ
			if (!WinHttpQueryDataAvailable(pImpl->m_hRequest, NULL))
			{
				//セッション以外のクリア
				pImpl->Clear();
				OnResponseFailed(L"ヘッダのデータ問い合わせに失敗しました。");
				return;
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		{
			// データの取得可能
			DWORD dwSize = *((LPDWORD)pStatusInformation);
			if (dwSize == 0)
			{
				//セッション以外のクリア
				pImpl->Clear();
				// 読み込み終了->コールバック終了
				OnResponseSuccess();
				return;
			}

			// レスポンスデータ読み込み
			DWORD dwLength = dwSize + 1;
			char* pBuffer = new char[dwLength * sizeof(char)];
			if (!WinHttpReadData(pImpl->m_hRequest,
				pBuffer,		// バッファー
				dwSize,			// 読み込むバイト数
				NULL))
			{
				delete[](pBuffer);
				pBuffer = nullptr;
				//セッション以外のクリア
				pImpl->Clear();
				OnResponseFailed(L"データの読み込みに失敗しました。");
				return;
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		{
			//読み込み
			if (pStatusInformation && StatusInformationLength)
			{
				char* pBuf = (char*)pStatusInformation;
				//最後に\0を入れる
				pBuf[StatusInformationLength] = '\0';
				pImpl->m_ReadData << pBuf;
				delete[](pBuf);
				// レスポンスデータデータ問い合わせ
				if (!WinHttpQueryDataAvailable(pImpl->m_hRequest, NULL))
				{
					//セッション以外のクリア
					pImpl->Clear();
					OnResponseFailed(L"データブロックの読み込み完了に失敗しました。");
					return;
				}
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_REDIRECT:
			//リダイレクト（他ページに推移）
			break;
		case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
			OnResponseFailed(L"リクエスト送信に失敗しました。");
			return;
		case WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE:
			//セッション以外のクリア
			pImpl->Clear();
			OnResponseFailed(L"レスポンス受信に失敗しました。");
			break;
		default:
			//セッション以外のクリア
			pImpl->Clear();
			OnResponseFailed(L"不定なエラーです。");
			return;
		}
	}

	void HttpForm::GetResponseData(wstring& Ret, bool ClearFlg){
		if (pImpl->m_ReadData.str().size() > 0){
			Util::ConvertUtf8toWstring(pImpl->m_ReadData.str(),Ret);
			if (ClearFlg){
				pImpl->ReadDataClear();
			}
		}
		else{
			Ret = L"";
		}
	}


}
//end basedx11
