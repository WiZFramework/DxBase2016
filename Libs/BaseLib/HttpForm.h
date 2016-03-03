/*!
@file HttpForm.h
@brief HttpのGET/POST、Formオブジェクト

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class StaticText : public GameObject;
	//	用途: 表示用テキスト（StringSpriteのコンテナ）
	//--------------------------------------------------------------------------------------
	class StaticText : public GameObject{
	public:
		//構築と破棄
		explicit StaticText(const shared_ptr<Stage>& StagePtr);
		virtual ~StaticText();
		//アクセサ
		void SetFont(const wstring& FontName, float FontSize);
		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);

		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);


		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);

		//初期化
		virtual void OnCreate() override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class InputObject : public GameObject;
	//	用途: 入力オブジェクトの親クラス
	//--------------------------------------------------------------------------------------
	class InputObject : public GameObject{
	protected:
		//構築と破棄
		InputObject(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputObject();
		const wstring& GetAttribute(const wstring& Key) const;
		void SetAttribute(const wstring& Key, const wstring& Val);
	public:
		//アクセサ(仮想関数)
		virtual const wstring& GetName();
		virtual void SetName(const wstring& Str);
		virtual const wstring& GetValue();
		virtual void SetValue(const wstring& Str);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class InputHidden : public InputObject;
	//	用途: 表示なしテキスト
	//--------------------------------------------------------------------------------------
	class InputHidden : public InputObject{
	public:
		//構築と破棄
		explicit InputHidden(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputHidden();
	};


	//--------------------------------------------------------------------------------------
	//	class InputText : public InputObject;
	//	用途: 入力テキスト
	//--------------------------------------------------------------------------------------
	class InputText : public InputObject{
	public:
		//構築と破棄
		explicit InputText(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputText();
		//アクセサ
		bool IsFocus()const;
		bool GetFocus()const;
		void SetFocus(bool b);

		void SetFont(const wstring& FontName, float FontSize);
		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);

		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);


		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);


		const Color4& GetCaretColor() const;
		void SetCaretColor(const Color4& Col);
		float GetCaretSpeed() const;
		void SetCaretSpeed(float f);


		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//アクセサ(仮想関数)
		virtual const wstring& GetValue();
		virtual void SetValue(const wstring& Str);
		//初期化
		virtual void OnCreate() override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class HttpForm : public GameObject;
	//	用途: HttpFormクラス
	//--------------------------------------------------------------------------------------
	class HttpForm : public GameObject{
		void SessionCreate();
		void Request(const wstring& Url, bool Post);
	protected:
		//構築と消滅
		explicit HttpForm(const shared_ptr<Stage>& StagePtr, const wstring& GroupName);
		virtual ~HttpForm();
	public:
		//アクセサ
		const wstring& GetGroupName() const;
		//操作
		shared_ptr<InputText> AddInputText(const wstring& Name, const wstring& Value);
		shared_ptr<InputHidden> AddInputHidden(const wstring& Name, const wstring& Value);
		void MoveFocusNextInput();

		void GetRequest(const wstring& Url);
		void PostRequest(const wstring& Url);

		void OnCallback(DWORD InternetStatus, LPVOID pStatusInformation, DWORD StatusInformationLength);
		void GetResponseData(wstring& Ret,bool ClearFlg = true);
		//ウィンドウメッセージ
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam){}
		//レスポンス失敗（純粋仮想関数）
		virtual void OnResponseFailed(const wstring& MessageStr) = 0;
		//レスポンス成功（純粋仮想関数）
		virtual void OnResponseSuccess() = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

}
//end basedx11
