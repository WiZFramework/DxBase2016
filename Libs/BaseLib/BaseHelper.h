/*!
@file BaseHelper.h
@brief 例外処理、ヘルパー関数等

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//! スマートポインタのヘルパー
	struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
	typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
	inline HANDLE safe_handle(HANDLE h) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }

	//--------------------------------------------------------------------------------------
	//	class BaseException : public exception
	/*!
		例外クラス
	*/
	//--------------------------------------------------------------------------------------
	class BaseException : public exception
	{
		//メッセージ変数
		wstring m_Message;
	public:
		//--------------------------------------------------------------------------------------
		//	BaseException(
		//	const wstring& m1,  //第1メッセージ（警告文）
		//	const wstring& m2,   //第2メッセージ（問題となったデータ）
		//	const wstring& m3   //第3メッセージ（発生した位置）
		//	);
		/*!
		@breaf コンストラクタ
		@param const wstring& m1  第1メッセージ（警告文）
		@param const wstring& m2  第2メッセージ（問題となったデータ）
		@param const wstring& m3  第3メッセージ（発生した位置）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		BaseException(const wstring&  m1, const wstring&  m2, const wstring& m3){
			m_Message = m1;
			m_Message += L"\r\n";
			m_Message += m2;
			m_Message += L"\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		//	const wstring& what_w() const throw()
		/*!
		@breaf メッセージの取得
		@return	メッセージ文字列の参照
		*/
		//--------------------------------------------------------------------------------------
		const wstring& what_w() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class BaseMBException : public exception
	/*!
	例外クラス（マルチバイト版）
	*/
	//--------------------------------------------------------------------------------------
	class BaseMBException : public exception{
		//メッセージ変数
		string m_Message;
	public:
		//--------------------------------------------------------------------------------------
		//	BaseMBException(
		//	const string& m1,  //第1メッセージ（警告文）
		//	const string& m2,   //第2メッセージ（問題となったデータ）
		//	const string& m3   //第3メッセージ（発生した位置）
		//	);
		/*!
		@breaf コンストラクタ
		@param const string& m1  第1メッセージ（警告文）
		@param const string& m2  第2メッセージ（問題となったデータ）
		@param const string& m3  第3メッセージ（発生した位置）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		BaseMBException(const string& m1, const string& m2, const string& m3)
		{
			m_Message = m1;
			m_Message += "\r\n";
			m_Message += m2;
			m_Message += "\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		//	const wstring& what_m() const throw()
		/*!
		@breaf メッセージの取得（マルチバイト）
		@return	メッセージ文字列の参照
		*/
		//--------------------------------------------------------------------------------------
		const string& what_m() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	//	inline HRESULT ThrowIfFailed(
	//		HRESULT hr,		//インターフェイス処理の戻り値
	//		const wstring& Str1,	//警告
	//		const wstring& Str2,	//問題となったデータ
	//		const wstring& Str3	//発生個所
	//	);
	/*!
	@breaf インターフェイスの例外
	@param const wstring& Str1  第1メッセージ（警告文）
	@param const wstring& Str2  第2メッセージ（問題となったデータ）
	@param const wstring& Str3  第3メッセージ（発生した位置）
	@return	HRESULT
	*/
	//--------------------------------------------------------------------------------------
	inline HRESULT ThrowIfFailed(HRESULT hr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (FAILED(hr))
		{
			throw BaseException(
				Str1,
				Str2,
				Str3
				);
		}
		return hr;
	}

	//--------------------------------------------------------------------------------------
	//	class BinaryReader
	/*!
	バイナリデータ読み込み。DirectXTKカスタマイズ
	*/
	//--------------------------------------------------------------------------------------
	class BinaryReader
	{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit BinaryReader(
		//	const wstring& fileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ
		@param const wstring& fileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit BinaryReader(const wstring& fileName);
		BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize);


		// Reads a single value.
		template<typename T> T const& Read()
		{
			return *ReadArray<T>(1);
		}


		// Reads an array of values.
		template<typename T> T const* ReadArray(size_t elementCount)
		{
			static_assert(std::is_pod<T>::value, "Can only read plain-old-data types");

			uint8_t const* newPos = mPos + sizeof(T)* elementCount;

			if (newPos > mEnd)
				throw std::exception("End of file");

			auto result = reinterpret_cast<T const*>(mPos);

			mPos = newPos;

			return result;
		}


		// Lower level helper reads directly from the filesystem into memory.
		static HRESULT ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);


	private:
		// The data currently being read.
		uint8_t const* mPos;
		uint8_t const* mEnd;

		std::unique_ptr<uint8_t[]> mOwnedData;

		//コピー禁止
		BinaryReader(const BinaryReader&) = delete;
		BinaryReader& operator=(const BinaryReader&) = delete;
		//ムーブ禁止
		BinaryReader(const BinaryReader&&) = delete;
		BinaryReader& operator=(const BinaryReader&&) = delete;
	};




	//--------------------------------------------------------------------------------------
	//	class CsvFile;
	/*!
	Csvファイルアクセスクラス
	*/
	//--------------------------------------------------------------------------------------
	class CsvFile{
	public:
		//--------------------------------------------------------------------------------------
		//	CsvFile();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		CsvFile();
		//--------------------------------------------------------------------------------------
		//	explicit CsvFile(
		//	const wstring& FileName	//CSVファイル名
		//	);
		/*!
		@breaf コンストラクタ
		@param const wstring& FileName	CSVファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit CsvFile(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~CsvFile();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CsvFile();
		//--------------------------------------------------------------------------------------
		//	const wstring& GetFileName() const;
		/*!
		@breaf ファイル名を得る
		@param なし
		@return	csvファイル名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFileName() const;
		//--------------------------------------------------------------------------------------
		//	void SetFileName(
		//	const wstring& FileName	//CSVファイル名
		//	);
		/*!
		@breaf ファイル名をセットする
		@param const wstring& FileName	CSVファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFileName(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	vector< wstring >& GetCsvVec()const;
		/*!
		@breaf CSV配列を得る
		@param なし
		@return	CSV配列
		*/
		//--------------------------------------------------------------------------------------
		vector< wstring >& GetCsvVec()const;
		//--------------------------------------------------------------------------------------
		//	bool ReadCsv();
		/*!
		@breaf
		Csvを読み込む.
		＊すでに持ってるデータは破棄される<br/>
		＊Csvファイルが存在しない場合は初期化したうえでfalseを返す。<br />
		＊存在しても読み込みに失敗したら例外
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		bool ReadCsv();
		//--------------------------------------------------------------------------------------
		//	void SaveCsv(
		//	bool Exp = true	//例外処理するかどうか
		//	);
		/*!
		@breaf
		Csvを書き出す. 
		＊Csvファイルが存在しなければ作成する、存在していたら、データは初期化される<br/>
		＊アクセスに失敗し、例外処理フラグが立ってたら例外<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる<br />
		@param bool Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SaveCsv(bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	size_t AddRow(
		//	vector<wstring>& Row	//Csvの1行（各セルは配列化されている必要がある）
		//	);
		/*!
		@breaf
		1行最後尾に追加する.
		@param vector<wstring>& Row	Csvの1行（各セルは配列化されている必要がある）
		@return	追加した行番号
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		//	size_t AddRow(
		//	wstring& Row	//Csvの1行（文字列は、「,」で区切られている必要がある）
		//	);
		/*!
		@breaf
		1行最後尾に追加する.
		@param wstring& Row	Csvの1行（文字列は、「,」で区切られている必要がある）
		@return	追加した行番号
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(wstring& Row);
		//--------------------------------------------------------------------------------------
		//	void UpdateRow(
		//	size_t RowNum,			//変更したい行
		//	vector<wstring>& Row.	//Csvの1行（各セルは配列化されている必要がある）
		//	bool Exp = true			//例外処理するかどうか
		//	);
		/*!
		@breaf
		指定した1行を変更する.
		＊必ずしも更新前と列の数は同じでなくてもよい。<br />
		＊指定した行が存在しない場合、例外を発生する<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる
		@param size_t RowNum,	変更したい行
		@param vector<wstring>& Row.	Csvの1行（各セルは配列化されている必要がある）
		@param bool Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	void UpdateRow(
		//	size_t RowNum,	//変更したい行
		//	wstring& Row,	//Csvの1行（文字列は、「,」で区切られている必要がある）
		//	bool Exp = true	//例外処理するかどうか
		//	);
		/*!
		@breaf
		指定した1行を変更する.
		＊Rowは「,」によって区切られ配列化され保存される<br />
		＊必ずしも更新前と列の数は同じでなくてもよい。<br />
		＊指定した行が存在しない場合、例外を発生する<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる
		@param size_t RowNum,	変更したい行
		@param wstring& Row,	Csvの1行（文字列は、「,」で区切られている必要がある）
		@param bool Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, wstring& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	void UpdateCell(
		//	size_t RowNum,	//変更行
		//	size_t ColNum,	//変更列
		//	wstring& Cell,	//差し替える文字列
		//	bool Exp = true			//例外処理するかどうか
		//	);
		/*!
		@breaf
		1つのセルを設定する.
		@param size_t RowNum,	変更行
		@param size_t ColNum,	変更列
		@param wstring& Cell,	差し替える文字列
		@param bool Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp = true);
		//--------------------------------------------------------------------------------------
		//	size_t GetRowCount() const;
		/*!
		@breaf
		行数を得る.
		@param なし
		@return	行数
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRowCount() const;
		//--------------------------------------------------------------------------------------
		//	void GetRowVec(
		//	size_t RowNum,			//受け取りたい行
		//	vector<wstring>& Row	//1行を受け取る文字列の配列
		//	);
		/*!
		@breaf
		1行分の配列を得る.
		＊RowNumが配列の上限を超えた場合は例外<br />
		＊受け取る文字列の配列が空白の状態のこともある
		@param size_t RowNum,	受け取りたい行
		@param vector<wstring>& Row	1行を受け取る文字列の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetRowVec(size_t RowNum, vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		//	wstring GetCell(
		//	size_t RowNum,	//受け取りたい行
		//	size_t ColNum	//受け取りたい列
		//	);
		/*!
		@breaf
		1つのセルを得る.
		＊RowNumまたはColNumが配列の上限を超えた場合は例外<br/>
		＊受け取る文字列が空白の状態のこともある
		@param size_t RowNum,	受け取りたい行
		@param size_t ColNum	受け取りたい列
		@return	セルの中身
		*/
		//--------------------------------------------------------------------------------------
		wstring GetCell(size_t RowNum, size_t ColNum);
		//--------------------------------------------------------------------------------------
		//	void GetSelect(
		//	vector< wstring >& RetVec	//結果を受け取る文字列の配列
		//	size_t ColNum,			//条件を検査する列
		//	const wstring& Key,		//条件キー
		//	);
		/*!
		@breaf
		検査する列が条件キーと同じ行を抜出し文字列の配列に返す.
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@param vector< wstring >& RetVec	結果を受け取る文字列の配列
		@param size_t ColNum,	条件を検査する列
		@param const wstring& Key,	条件キー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec, size_t ColNum, const wstring& Key);
		//--------------------------------------------------------------------------------------
		//	void GetSelect(
		//	vector< wstring >& RetVec,		//結果を受け取る文字列の配列
		//	bool (Func)(const wstring&),	//条件を検査するコールバック関数
		//	);
		/*!
		@breaf
		コールバック関数を呼び出し、trueが返った行を抜出し文字列の配列に返す.
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@param vector< wstring >& RetVec	結果を受け取る文字列の配列
		@param bool (Func)(const wstring&),	条件を検査するコールバック関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec,bool (Func)(const wstring&));
		//--------------------------------------------------------------------------------------
		//	template<typename Fct>
		//	void GetSelect2(
		//	vector< wstring >& RetVec,	//結果を受け取る文字列の配列
		//	Fct f						//条件を検査するラムダ式（キャプチャ使用可能）
		//	);
		/*!
		@breaf
		ラムダ式を呼び出し、trueが返った行を抜出し文字列の配列に返す.
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@param vector< wstring >& RetVec	結果を受け取る文字列の配列
		@param Fct f,	条件を検査するラムダ式
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename Fct>
		void GetSelect2(vector< wstring >& RetVec, Fct f){
			//渡された配列に値が入ってる場合があるのでクリア
			RetVec.clear();
			auto& CsvVec = GetCsvVec();
			size_t sz = CsvVec.size();
			for (size_t i = 0; i < sz; i++){
				//コールバック関数を呼び出す
				if (f(CsvVec[i])){
					//条件があれば追加
					RetVec.push_back(CsvVec[i]);
				}
			}
		}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		CsvFile(const CsvFile&) = delete;
		CsvFile& operator=(const CsvFile&) = delete;
		//ムーブ禁止
		CsvFile(const CsvFile&&) = delete;
		CsvFile& operator=(const CsvFile&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	wstring MakeRangeErr(
	//		const wchar_t* key1,	//キー1名
	//		T val1,					//値１
	//		const wchar_t* key2,	//キー２名
	//		T val2					//値2
	//	)
	/*!
	@breaf
	比較のエラーの文字列を作成する.
	@param const wchar_t* key1,	キー1名
	@param T val1,	値１
	@param const wchar_t* key2,	キー２名
	@param T val2	値2
	@return	エラー文字列
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	wstring MakeRangeErr(const wchar_t* key1, T val1, const wchar_t* key2, T val2){
		//返す文字列
		wstring str;
		//書式を整えるストリーム
		wostringstream stream;
		stream << key1 << L"==" << val1 << L"," << key2 << L"==" << val2;
		str = stream.str();
		return str;
	}

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeDelete(
	//	T*& p   // T型のポインタの参照
	//	)
	/*!
	@breaf
	安全にポインタをdeleteする.
	@param T*& p	T型のポインタの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDelete(T*& p){
		if (p){
			delete p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeDeleteArr(
	//	T*& p   // T型の配列の参照
	//	)
	/*!
	@breaf
	安全に配列をdeleteする.
	@param T*& p	T型の配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDeleteArr(T*& p){
		if (p){
			delete[] p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	inline void SafeRelease(
	//	T*& p   // T型のインターフェイスのポインタの参照
	//	)
	/*!
	@breaf
	安全にインターフェイスをreleaseする.
	@param T*& p  T型のインターフェイスのポインタの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeRelease(T*& p){
		if (p){
			p->Release();
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SefeDeletePointerList(
	//	list<C*>& List	//C型のポインタのリストの参照
	//	);
	/*!
	@breaf
	ポインタのリストを安全にクリアする.
	@param list<C*>& List	C型のポインタのリストの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SefeDeletePointerList(list<C*>& List){
		list<C*>::iterator it;
		for (it = List.begin(); it != List.end(); ++it){
			delete (*it);
		}
		List.clear();
	}
	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SafeDeletePointerVector(
	//	vector<C*>& vec	//C型のポインタの配列の参照
	//	);
	/*!
	@breaf
	ポインタの配列（vector）を安全にクリアする.
	@param vector<C*>& vec	//C型のポインタの配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SafeDeletePointerVector(vector<C*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			delete Vec[i];
		}
		Vec.clear();
	}

	//--------------------------------------------------------------------------------------
	//	template<class C>
	//	void SafeReleasePointerVector(
	//	vector<C*>& vec	//C型インターフェイスのポインタ配列の参照
	//	);
	/*!
	@breaf
	インターフェイスの配列（vector）を安全にリリースする.
	@param vector<C*>& vec	C型インターフェイスのポインタ配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<class C>
	void SafeReleasePointerVector(vector<C*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			if (Vec[i]){
				Vec[i]->Release();
			}
			Vec[i] = nullptr;
		}
		Vec.clear();
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	shared_ptr<void> SharedToVoid(
	//		const shared_ptr<T>& SrcPtr //変換元
	//	);
	/*!
	@breaf
	shared_ptrをvoid型に変換する.
	@param const shared_ptr<T>& SrcPtr　変換元
	@return	void型のshared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<void> SharedToVoid(const shared_ptr<T>& SrcPtr){
		shared_ptr<void> RetPtr = static_pointer_cast<void>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	shared_ptr<T> VoidToShared(
	//		const shared_ptr<void>& SrcPtr //変換元
	//	);
	/*!
	@breaf
	void型のshared_ptrをT型のshared_ptrに変換する.
	@param const shared_ptr<void>& SrcPt　変換元
	@return	T型のshared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> VoidToShared(const shared_ptr<void>& SrcPtr){
		shared_ptr<T> RetPtr = static_pointer_cast<T>(SrcPtr);
		return RetPtr;
	}

	//--------------------------------------------------------------------------------------
	//	struct Util;
	/*!
	その他のユーティリティ。static呼び出しをする
	*/
	//--------------------------------------------------------------------------------------
	struct Util{
		//--------------------------------------------------------------------------------------
		//	template<class T>
		//	T Maximum(
		//	const T& v1,	//T型の値1の参照
		//	const T& v2		//T型の値2の参照
		//	);
		/*!
		@breaf
		大きい方を求める.
		@param const T& v1	T型の値1
		@param const T& v2	T型の値2
		@return	T型の大きい方
		*/
		//--------------------------------------------------------------------------------------
		template<class T>
		static T Maximum(const T& v1, const T& v2){
			return v1 > v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		//	template<class T>
		//	T Minimum(
		//	const T& v1,	//T型の値1の参照
		//	const T& v2		//T型の値2の参照
		//	);
		/*!
		@breaf
		小さい方を求める.
		@param const T& v1	T型の値1
		@param const T& v2	T型の値2
		@return	T型の小さい方
		*/
		//--------------------------------------------------------------------------------------
		template<class T>
		static T Minimum(const T& v1, const T& v2){
			return v1 < v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		//	template<typename T, typename TCreateFunc>
		//	static T* DemandCreate(
		//	ComPtr<T>& comPtr,	//COMポインタ
		//	std::mutex& mutex,	//ミューテックス
		//	TCreateFunc createFunc	//作成関数
		//	);
		/*!
		@breaf
		安全にCOMインターフェイスのポインタを取得するヘルパー関数.
		@param ComPtr<T>& comPtr	COMポインタ
		@param std::mutex& mutex	ミューテックス
		@param TCreateFunc createFunc	作成関数
		@return　COMが無効の場合は新しく作成して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();

			// Double-checked lock pattern.
			MemoryBarrier();

			if (!result)
			{
				std::lock_guard<std::mutex> lock(mutex);

				result = comPtr.Get();

				if (!result)
				{
					createFunc(&result);

					MemoryBarrier();

					comPtr.Attach(result);
				}
			}
			return result;
		}

		//--------------------------------------------------------------------------------------
		//	template<typename T>
		//	static T* GetComPtr(
		//	ComPtr<T>& comPtr,//COMポインタ
		//	);
		/*!
		@breaf
		安全にCOMインターフェイスのポインタを取得するヘルパー関数.
		無効の場合は例外を出す
		@param ComPtr<T>& comPtr	COMポインタ
		@return　COMが無効の場合は例外を出して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T* GetComPtr(ComPtr<T>& comPtr){
			T* result = comPtr.Get();
			// Double-checked lock pattern.
			MemoryBarrier();
			if (!result)
			{
				//失敗
				throw BaseException(
					L"このコンポーネントを取得できません",
					L"if (!result)",
					L"Util::GetComPtr()"
					);
			}
			return result;
		}
		//--------------------------------------------------------------------------------------
		//	static void WStrTrim(
		//	wstring& wstr,			//もとになる文字列（リターンにも使用される）
		//	const wchar_t* trimCharacterList = L" \t\v\r\n"	//取り除く文字
		//	)
		/*!
		@breaf
		両端から空白を取り除く<br/>
		元になるい文字列から取り除く
		@param wstring& wstr	もとになる文字列（リターンにも使用される）
		@param const wchar_t* trimCharacterList = L" \t\v\r\n"	取り除く文字
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrTrim(wstring& wstr, const wchar_t* TrimCharList = L" \t\v\r\n"){
			wstring result(L"");
			if (wstr.size() <= 0){
				wstr = result;
				return;
			}
			wstring::size_type left = wstr.find_first_not_of(TrimCharList);
			if (left != wstring::npos){
				wstring::size_type right = wstr.find_last_not_of(TrimCharList);
				result = wstr.substr(left, right - left + 1);
			}
			wstr = result;
		}

		//--------------------------------------------------------------------------------------
		//	static void WStoMB(
		//	const wstring& src,	//変換する文字列（ワイドキャラ）
		//	string& dest			//変換後の文字列（マルチバイト）
		//	);
		/*!
		@breaf
		ワイド文字列からマルチバイト文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param const wstring& src,	変換する文字列（ワイドキャラ）
		@param string& dest			変換後の文字列（マルチバイト）
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStoMB(const wstring& src, string& dest){
			size_t i;
			char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs_s(
				&i,
				pMBstr,
				src.length() * MB_CUR_MAX + 1,
				src.c_str(),
				_TRUNCATE	//すべて変換できなかったら切り捨て
				);
			dest = pMBstr;
			delete[] pMBstr;
		}

		//--------------------------------------------------------------------------------------
		//	static void ConvertWstringtoUtf8(
		//	const wstring& src,	//変換する文字列（ワイドキャラ）
		//	string& dest			//変換後の文字列（マルチバイトUTF8）
		//	);
		/*!
		@breaf
		ワイド文字列からマルチバイトUTF8文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param const wstring& src,	変換する文字列（ワイドキャラ）
		@param string& dest			変換後の文字列（マルチバイト）
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertWstringtoUtf8(const wstring& src, string& dest){
			INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
			char* Temp = new char[bufsize + 1];
			::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize, NULL, NULL);
			dest = Temp;
			delete[] Temp;
		}

		//--------------------------------------------------------------------------------------
		//	static void ConvertUtf8toWstring(
		//	const string& src				//変換する文字列（マルチバイトUTF8）
		//	wstring& dest,	//変換後文字列（ワイドキャラ）
		//	);
		/*!
		@breaf
		マルチバイトUTF8文字からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param const string& src　変換する文字列（マルチバイトUTF8）
		@param wstring& dest,	//変換後文字列（ワイドキャラ）
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertUtf8toWstring(const string& src, wstring& dest){
			INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (wchar_t *)NULL, 0);
			wchar_t* Temp = (wchar_t*)new wchar_t[bufsize];
			::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize);
			dest = Temp;
			delete[] Temp;
		}
		//--------------------------------------------------------------------------------------
		//	static  void MBtoWS(
		//	const string& src,	//変換する文字列（マルチバイト）
		//	wstring& dest		//変換後の文字列（ワイドキャラ）
		//	);
		/*!
		@breaf
		マルチバイト文字列からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param const string& src,	変換する文字列（マルチバイト）
		@param wstring& dest		変換後の文字列（ワイドキャラ）
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void MBtoWS(const string& src, wstring& dest){
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //すべて変換できなかったら切り捨て
				);
			dest = WCstr;
			delete[] WCstr;
		}

		//--------------------------------------------------------------------------------------
		//	static void WStrToTokenVector(
		//	vector<wstring>& wstrvec,	//保存される配列
		//	const wstring& line,		//元となる文字列
		//	wchar_t delimiter			//デリミタ
		//	)
		/*!
		@breaf
		文字列をデリミタ文字により区切って配列に保存
		@param vector<wstring>& wstrvec,	保存される配列
		@param const wstring& line,		元となる文字列
		@param wchar_t delimiter		デリミタ
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrToTokenVector(vector<wstring>& wstrvec, const wstring& line, wchar_t delimiter){
			wstring::size_type i = 0;
			wstring wks(L"");
			for (i = 0; i < line.size(); i++){
				if (line[i] == delimiter){
					if (wks != L""){
						wstrvec.push_back(wks);
					}
					wks = L"";
				}
				else{
					wks += line[i];
				}
			}
			wstrvec.push_back(wks);
		}

		enum FloatModify{
			Default = 0,
			Fixed,
			Scientific,
		};

		//--------------------------------------------------------------------------------------
		//	static wstring FloatToWStr(
		//	float Val,				//浮動小数点値
		//	streamsize Precision = 0,	//精度0でデフォルト
		//	FloatModify Modify = FloatModify::Default	//表示形式
		//	)
		/*!
		@breaf
		浮動小数点を文字列に変換する（wstring版）
		@param float Val,				浮動小数点値
		@param streamsize Precision = 0,	精度0でデフォルト
		@param FloatModify Modify = FloatModify::Default	表示形式
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring FloatToWStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static string FloatToStr(
		//	float Val,				//浮動小数点値
		//	streamsize Precision = 0,	//精度0でデフォルト
		//	FloatModify Modify = FloatModify::Default	//表示形式
		//	)
		/*!
		@breaf
		浮動小数点を文字列に変換する（string版）
		@param float Val,				浮動小数点値
		@param streamsize Precision = 0,	精度0でデフォルト
		@param FloatModify Modify = FloatModify::Default	表示形式
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string FloatToStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}


		enum NumModify{
			Dec = 0,
			Hex,
			Oct,
		};
		//--------------------------------------------------------------------------------------
		//	static wstring UintToWStr(
		//	UINT num,	//数値
		//	NumModify Modify = NumModify::Dec	//進数
		//	)
		/*!
		@breaf
		unsigned数値を文字列に変換する（wstring版）
		@param UINT num,				unsigned数値
		@param NumModify Modify = NumModify::Dec	進数
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring UintToWStr(UINT num, NumModify Modify = NumModify::Dec){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		//	static string UintToStr(
		//	UINT num,
		//	NumModify Modify = NumModify::Dec
		//	)
		/*!
		@breaf
		unsigned数値を文字列に変換する（string版）
		@param UINT num,				unsigned数値
		@param NumModify Modify = NumModify::Dec	進数
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string UintToStr(UINT num, NumModify Modify = NumModify::Dec){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static wstring IntToWStr(
		//	int num,	//signed数値
		//	NumModify Modify = NumModify::Dec	//進数
		//	)
		/*!
		@breaf
		signed数値を文字列に変換する（wstring版）
		@param int num,		signed数値
		@param NumModify Modify = NumModify::Dec	進数
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring IntToWStr(int num, NumModify Modify = NumModify::Dec){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static string IntToStr(
		//	int num,	//signed数値
		//	NumModify Modify = NumModify::Dec	//進数
		//	)
		/*!
		@breaf
		signed数値を文字列に変換する（string版）
		@param int num,		signed数値
		@param NumModify Modify = NumModify::Dec	進数
		@return　変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string IntToStr(int num, NumModify Modify = NumModify::Dec){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		//	static bool DivProbability(
		//	UINT Probability	//確率。何分の1かを指定する
		//	);
		/*!
		@breaf
		Probability分の1の確率で、trueを返す<br />
		Probabilityは0より大きくなければならない（0除算はできない）
		@param UINT Probability	確率。何分の1かを指定する
		@return　Probability分の1の確率になればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool DivProbability(UINT Probability){
			if (Probability <= 0){
				return false;
			}
			double Prob = (double)rand() / (double)RAND_MAX;
			double Seed = 1.0 / (double)Probability;
			if (Prob <= Seed){
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		//	static float RandZeroToOne(
		//	bool ZeroOK = false
		//	);
		/*!
		@breaf
		0から1.0fの間の乱数を返す<br />
		ZeroOKがfalseの場合、0が返る可能性はない（0.00001fになる）
		@param bool ZeroOK = false	0が返っていいかどうかの指定
		@return　0から1.0fの間の乱数
		*/
		//--------------------------------------------------------------------------------------
		static float RandZeroToOne(bool ZeroOK = false){
			double Prob = (double)rand() / (double)RAND_MAX;
			if (Prob <= 0){
				if (!ZeroOK){
					Prob = 0.00001;
				}
				else{
					Prob = 0.0;
				}
			}
			return (float)Prob;
		}

	};

	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;


	//--------------------------------------------------------------------------------------
	//	class StepTimer;
	/*!
	ステップタイマー
	*/
	//--------------------------------------------------------------------------------------
	class StepTimer{
	public:
		//--------------------------------------------------------------------------------------
		//	StepTimer();
		/*!
		@breaf コンストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw BaseException(
					L"パフォーマンスカウンタの周波数を取得できません。",
					L"if (!QueryPerformanceFrequency(&m_qpcFrequency))",
					L"StepTimer::StepTimer()"
					);
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::StepTimer()"
					);
			}

			// 最大デルタを 1 秒の 1/10 に初期化します。
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		//--------------------------------------------------------------------------------------
		//	uint64 GetElapsedTicks() const;
		/*!
		@breaf 前の Update 呼び出しから経過したカウンタを取得.
		@param なし
		@return	経過カウンタ
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetElapsedTicks() const						{ return m_elapsedTicks; }
		//--------------------------------------------------------------------------------------
		//	double GetElapsedSeconds() const;
		/*!
		@breaf 前の Update 呼び出しから経過した時間を取得.
		@param なし
		@return	経過時間
		*/
		//--------------------------------------------------------------------------------------
		double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }
		//--------------------------------------------------------------------------------------
		//	uint64 GetTotalTicks() const;
		/*!
		@breaf プログラム開始から経過した合計カウンタを取得.
		@param なし
		@return	合計カウンタ
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetTotalTicks() const						{ return m_totalTicks; }
		//--------------------------------------------------------------------------------------
		//	double GetTotalSeconds() const;
		/*!
		@breaf プログラム開始から経過した合計時間を取得.
		@param なし
		@return	合計時間
		*/
		//--------------------------------------------------------------------------------------
		double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }
		//--------------------------------------------------------------------------------------
		//	uint32 GetFrameCount() const;
		/*!
		@breaf プログラム開始からの合計更新回数を取得.
		@param なし
		@return	合計更新回数
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFrameCount() const						{ return m_frameCount; }

		//--------------------------------------------------------------------------------------
		//	uint32 GetFramesPerSecond() const;
		/*!
		@breaf 現在のフレーム レートを取得.
		@param なし
		@return	現在のフレーム レート
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFramesPerSecond() const					{ return m_framesPerSecond; }

		//--------------------------------------------------------------------------------------
		//	void SetFixedTimeStep(
		//		bool isFixedTimestep	//固定モードにするかどうか
		//	);
		/*!
		@breaf 固定または可変のどちらのタイムステップ モードを使用するかを設定（デフォルトは可変）.
		@param bool isFixedTimestep	固定モードにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }

		//--------------------------------------------------------------------------------------
		//	void SetTargetElapsedTicks(
		//		uint64 targetElapsed	//呼び出し頻度（カウンタ）
		//	);
		/*!
		@breaf 固定タイムステップ モードで、Update の呼び出し頻度を設定.
		@param uint64 targetElapsed		呼び出し頻度（カウンタ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedTicks(uint64 targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
		//--------------------------------------------------------------------------------------
		//	void SetTargetElapsedSeconds(
		//		double targetElapsed	//呼び出し頻度（時間）
		//	);
		/*!
		@breaf 固定タイムステップ モードで、Update の呼び出し頻度を設定.
		@param double targetElapsed		呼び出し頻度（時間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// 整数形式は 1 秒あたり 10,000,000 ティックを使用して時間を表します。
		static const uint64 TicksPerSecond = 10000000;

		//--------------------------------------------------------------------------------------
		//	static double TicksToSeconds(
		//		uint64 ticks	//チックカウンタ
		//	);
		/*!
		@breaf チックカウンタか時間に換算.
		@param uint64 ticks		チックカウンタ
		@return	換算された時間
		*/
		//--------------------------------------------------------------------------------------
		static double TicksToSeconds(uint64 ticks)			{ return static_cast<double>(ticks) / TicksPerSecond; }
		//--------------------------------------------------------------------------------------
		//	static uint64 SecondsToTicks(
		//		double seconds	//時間
		//	);
		/*!
		@breaf 時間をチックカウンタに換算.
		@param double seconds	時間
		@return	換算されたチックカウンタ
		*/
		//--------------------------------------------------------------------------------------
		static uint64 SecondsToTicks(double seconds)		{ return static_cast<uint64>(seconds * TicksPerSecond); }
		//--------------------------------------------------------------------------------------
		//	void ResetElapsedTime();
		/*!
		@breaf 呼び出しの更新<br />
		意図的なタイミングの不連続性の後 (ブロック IO 操作など)<br />
		これを呼び出すと、固定タイムステップ ロジックによって一連のキャッチアップが試行されるのを回避できる
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		//--------------------------------------------------------------------------------------
		//	template<typename TUpdate>
		//	void Tick(
		//		const TUpdate& update	//呼び出すべきUpdate関数
		//	)
		/*!
		@breaf タイマー状態を更新し、指定の Update 関数を適切な回数だけ呼び出す
		@param 呼び出すべきUpdate関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// 現在の時刻をクエリします。
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// 極端に大きな時間差 (デバッガーで一時停止した後など)  をクランプします。
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// QPC 単位を標準の目盛り形式に変換します。前のクランプが発生したため、この変換ではオーバーフローが不可能です。
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// 固定タイムステップ更新ロジック

				// クランプしたターゲット経過時間 (1/4 ミリ秒以内) に接近して、アプリケーションが実行されている場合
				// ターゲット値に正確に一致させるクロック。これにより、小さい無関係なエラーの発生を防ぎます。
				// 一定期間内に蓄積された。このクランプなしで、60 fps を要求したゲームは
				// 固定された更新 (59.94 NTSC ディスプレイ上で有効な vsync で実行) は最終的には、
				// フレームをドロップするのに十分な小さいエラーを蓄積します。丸めることをお勧めします
				// わずかな偏差をゼロに持っていき、スムーズに進行するようにします。

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// 可変タイムステップ更新ロジック。
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// 現在のフレーム レートを追跡します。
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// ソース タイミング データでは QPC 単位を使用します。
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// 派生タイミング データでは、標準の目盛り形式を使用します。
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// フレーム レートの追跡用メンバー。
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// 固定タイムステップ モードの構成用メンバー。
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;

		//コピー禁止
		StepTimer(const StepTimer&) = delete;
		StepTimer& operator=(const StepTimer&) = delete;
		//ムーブ禁止
		StepTimer(const StepTimer&&) = delete;
		StepTimer& operator=(const StepTimer&&) = delete;

	};





}

//end basedx11
