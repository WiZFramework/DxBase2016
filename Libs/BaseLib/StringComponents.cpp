#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// �e�L�X�g �����_�����O�Ɋ֘A���郊�\�[�X�B
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		Color4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAlignment m_TextAlignment;
		bool m_KeyInput;	//�L�[���͂��邩�ǂ���
		size_t m_Caret;	//�L�����b�g�ʒu
		float m_CaretCounter;	//�L�����b�g�̃J�E���^
		float m_CaretSpeed;		//�L�����b�g�̓_�ő��x(0�œ_�ł��Ȃ�)
		bool m_CaretState;		//�L�����b�g��`�悷�邩�ǂ����i�_�ł̕\����ԁj
		Color4 m_CaretColor;	//�L�����b�g�̐F
		Color4 m_BackColor;		//�w�i�F
		Point2D<float> m_BackTextMargin;	//�w�i�F��h��Ԃ��e�L�X�g�̃}�[�W��(���E�Ə㉺)



		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_CaretBrush;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;


		Impl():
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(128.0f),
			m_TextBlockHeight(32.0f),
			m_FontName(L"�l�r�S�V�b�N"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{16.0f,16.0f},
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_KeyInput(false),
			m_Caret(0),
			m_CaretCounter(0),
			m_CaretSpeed(0.5f),
			m_CaretState(true),
			m_CaretColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_BackColor(Color4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f,0.0f)
			{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	�p�r: StringSprite�R���|�[�l���g
	//	������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()){
		try{
			ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
				pImpl->m_FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				pImpl->m_FoneSize,
				L"ja",
				&pImpl->m_textFormat
				),
				L"�t�H���g�쐬�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
				);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_Brush
				),
				L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_CaretBrush
				),
				L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_BackBrush
				),
				L"�o�b�N�O���E���h�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);


		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}
	//�A�N�Z�T
	void StringSprite::SetFont(const wstring& FontName, float FontSize){
		ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize,
			L"ja",
			&pImpl->m_textFormat
			),
			L"�t�H���g�쐬�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);
		//�t�H���g�̍쐬�ɐ��������̂Œl��ݒ�
		pImpl->m_FontName = FontName;
		pImpl->m_FoneSize = FontSize;

		ThrowIfFailed(
			pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
			L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"StringSprite::SetFont()"
			);


	}
	StringSprite::TextAlignment StringSprite::GetTextAlignment() const{
		return pImpl->m_TextAlignment;
	}
	void StringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment){
		pImpl->m_TextAlignment = Alignment;
	}


	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}


	//�Ō���ɒǉ�
	void StringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		if (IsKeyInput()){
			pImpl->m_Caret = TempText.size();
		}
		SetText(TempText);
	}
	void StringSprite::InsertText(const wstring& str){
		if (!IsKeyInput()){
			return;
		}
		wstring TempText = GetText();
		if (pImpl->m_Caret >= TempText.size()){
			pImpl->m_Caret = TempText.size();
			AddText(str);
		}
		else{
			TempText.insert(pImpl->m_Caret, str);
			pImpl->m_Caret += str.size();
			SetText(TempText);
		}
	}
	void StringSprite::InsertText(const wstring& str, size_t CaretIndex){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_Caret = CaretIndex;
		InsertText(str);
	}


	const Color4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFontColor(const Color4& Col){
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_Brush
			),
			L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetFontColor()"
			);

	}

	const Color4& StringSprite::GetBackColor() const{
		return pImpl->m_BackColor;
	}
	void StringSprite::SetBackColor(const Color4& Col){
		pImpl->m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_BackBrush
			),
			L"�o�b�N�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	Point2D<float> StringSprite::GetBackTextMargin() const{
		return pImpl->m_BackTextMargin;
	}
	void StringSprite::SetBackTextMargin(Point2D<float> p){
		pImpl->m_BackTextMargin = p;
	}

	const Color4& StringSprite::GetCaretColor() const{
		return pImpl->m_CaretColor;
	}
	void StringSprite::SetCaretColor(const Color4& Col){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_CaretBrush
			),
			L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	float StringSprite::GetCaretSpeed() const{
		if (!IsKeyInput()){
			return 0.0f;
		}
		return pImpl->m_CaretSpeed;
	}
	void StringSprite::SetCaretSpeed(float f){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretSpeed = f;
	}


	const wstring& StringSprite::GetFontName() const{
		return pImpl->m_FontName;
	}
	float StringSprite::GetFontSize() const{
		return pImpl->m_FoneSize;
	}

	float StringSprite::GetTextBlockWidth() const{
		return pImpl->m_TextBlockWidth;
	}
	void StringSprite::SetTextBlockWidth(float f){
		pImpl->m_TextBlockWidth = f;
	}
	float StringSprite::GetTextBlockHeight() const{
		return pImpl->m_TextBlockHeight;
	}
	void StringSprite::SetTextBlockHeight(float f){
		pImpl->m_TextBlockHeight = f;
	}

	bool StringSprite::GetKeyInput() const{
		return pImpl->m_KeyInput;
	}
	bool StringSprite::IsKeyInput() const{
		return pImpl->m_KeyInput;

	}
	void StringSprite::SetKeyInput(bool b){
		pImpl->m_KeyInput = b;
	}

	bool StringSprite::IsFocus(){
		if (!IsKeyInput()){
			return false;
		}
		shared_ptr<InputTextManager> Manager = GetStage()->GetInputTextManager();
		if (Manager){
			auto Ptr = GetThis<StringSprite>();
			if (Manager->GetFocusInputString() == Ptr){
				return true;
			}
		}
		return false;
	}
	void StringSprite::SetFocus(bool b){
		if (!IsKeyInput()){
			return;
		}
		shared_ptr<InputTextManager> Manager = GetStage()->GetInputTextManager();
		if (Manager){
			if (b){
				Manager->SetFocusInputString(GetThis<StringSprite>());
			}
			else{
				Manager->SetFocusInputString(nullptr);
			}
		}
	}



	ComPtr<IDWriteTextLayout>& StringSprite::GetTextLayout()const{
		return pImpl->m_textLayout;
	}
	ComPtr<IDWriteTextFormat>&	StringSprite::GetTextFormat()const{
		return pImpl->m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringSprite::GetDriteTextMetrics() const{
		return pImpl->m_textMetrics;
	}

	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}

	Rect2D<float> StringSprite::GetTextRect() const{
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringSprite::SetTextRect(const Rect2D<float>& rect){
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}



	void StringSprite::OnKeyDown(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring TempText = GetText();
		switch (wParam){
		case VK_BACK:
			if (pImpl->m_Caret > 0 && TempText.size() > 0){
				pImpl->m_Caret--;
				wstring LeftStr = TempText.substr(0, pImpl->m_Caret);
				wstring RightStr = TempText.substr(pImpl->m_Caret + 1);
				TempText = LeftStr + RightStr;
				SetText(TempText);
			}
			break;
		case VK_RIGHT:
			if (pImpl->m_Caret < TempText.size()){
				pImpl->m_Caret++;
			}
			break;
		case VK_LEFT:
			if (pImpl->m_Caret > 0){
				pImpl->m_Caret--;
			}
			break;
		default:
			break;
		}

	}
	void StringSprite::OnChar(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring str(L"");
		switch (wParam){
		case VK_RIGHT:
		case VK_LEFT:
		case VK_BACK:
			break;
		default:
			str += (wchar_t)wParam;
			InsertText(str);
			break;
		}
	}


	void StringSprite::OnUpdate(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
			pImpl->m_text.c_str(),
			(uint32)pImpl->m_text.length(),
			pImpl->m_textFormat.Get(),
			pImpl->m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
			pImpl->m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
			&pImpl->m_textLayout
			),
			L"�t�H���g���C�A�E�g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextLayout()",
			L"StringSprite::Update()"
			);
		ThrowIfFailed(
			pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
			L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringSprite::Update()"
			);

	}



	void StringSprite::OnDraw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2D1_RECT_F BackRectangle = D2D1::RectF(
			pImpl->m_StartPosition.x, pImpl->m_StartPosition.y,
			pImpl->m_StartPosition.x + pImpl->m_TextBlockWidth,
			pImpl->m_StartPosition.y + pImpl->m_TextBlockHeight
			);
		BackRectangle.left -= pImpl->m_BackTextMargin.x;
		BackRectangle.top -= pImpl->m_BackTextMargin.y;
		BackRectangle.right += pImpl->m_BackTextMargin.x;
		BackRectangle.bottom += pImpl->m_BackTextMargin.y;

		D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//�o�b�N�O���E���h�̕`��
		D2DDeviceContext->FillRectangle(&BackRectangle,pImpl->m_BackBrush.Get());


		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			pImpl->m_StartPosition.x,
			pImpl->m_StartPosition.y
			);

		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (pImpl->m_TextAlignment){
		case TextAlignment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAlignment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAlignment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			pImpl->m_textFormat->SetTextAlignment(Alignment),
			L"�e�L�X�g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringSprite::Draw()"
			);

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			pImpl->m_textLayout.Get(),
			pImpl->m_Brush.Get()
			);

		//���͂ł��ăt�H�[�J�X������΁A�L�����b�g�\��
		if (IsKeyInput() && IsFocus()){
			pImpl->m_CaretCounter += ElapsedTime;
			if (pImpl->m_CaretSpeed == 0.0f){
				pImpl->m_CaretCounter = 0;
				pImpl->m_CaretState = true;
			}
			else{
				if (pImpl->m_CaretCounter >= pImpl->m_CaretSpeed){
					pImpl->m_CaretCounter = 0;
					if (pImpl->m_CaretState){
						pImpl->m_CaretState = false;
					}
					else{
						pImpl->m_CaretState = true;
					}
				}
			}
			if (pImpl->m_CaretState){
				float x, y;
				DWRITE_HIT_TEST_METRICS m;
				pImpl->m_textLayout->HitTestTextPosition(pImpl->m_Caret, false, &x, &y, &m);

				ComPtr<IDWriteTextLayout>       m_CaretLayout;
				ThrowIfFailed(
					DWriteFactory->CreateTextLayout(
					L"|",
					(uint32)1,
					pImpl->m_textFormat.Get(),
					pImpl->m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
					pImpl->m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
					&m_CaretLayout
					),
					L"�L�����b�g���C�A�E�g�ݒ�Ɏ��s���܂����B",
					L"DWriteFactory->CreateTextLayout()",
					L"StringSprite::Draw()"
					);
				ThrowIfFailed(
					pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
					L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
					L"pImpl->m_textLayout->GetMetrics()",
					L"StringSprite::Draw()"
					);

				D2DDeviceContext->DrawTextLayout(
					D2D1::Point2F(x, y),
					m_CaretLayout.Get(),
					pImpl->m_CaretBrush.Get()
					);
			}

		}
		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)){
				throw BaseException(
					L"�������\���ł��܂���ł���",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"StringSprite::Draw()"
					);
			}
		}
		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());
	}


}

//end basedx11
