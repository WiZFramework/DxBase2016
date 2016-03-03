#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//アクセサ
		enum TextAlignment{
			m_Left,		//左合わせ
			m_Center,	//中央合わせ
			m_Right		//右合わせ
		};
		//アクセサ
		bool GetKeyInput() const;
		bool IsKeyInput() const;
		void SetKeyInput(bool b);

		bool IsFocus();
		void SetFocus(bool b);

		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);
		void InsertText(const wstring& str);
		void InsertText(const wstring& str, size_t CaretIndex);


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



		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//操作
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//end basedx11
