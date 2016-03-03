#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct ShaderResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ShaderResource::Impl{
		Impl(){}
		~Impl(){}
		//ミューテックス
		std::mutex Mutex;
		//コンスタントバッファアクセッサ
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer){
			return Util::DemandCreate(Buffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				ShaderResource::CreateConstantBuffer(BuffSize, pResult);
			});
		}
		//頂点シェーダアクセッサ
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr){
			//ラムダ式利用
			return Util::DemandCreate(VSPtr, Mutex, [&](ID3D11VertexShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateVertexShader(data, cso_sz, pResult);
			});
		}
		//インプットレイアウトアクセッサ
		ID3D11InputLayout* GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements){
			return Util::DemandCreate(ILPtr, Mutex, [&](ID3D11InputLayout** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateInputLayout(
					data,
					cso_sz,
					pElem,
					NumElements,
					pResult);
			});
		}


	};



	ShaderResource::ShaderResource() :pImpl(new Impl()){}
	ShaderResource::~ShaderResource(){}


	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::ReadBinaryFile(
	//		const wstring& fileName,		//ファイル名
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータの戻り
	//		size_t& CsoSz					//サイズの戻り
	//	);
	//	用途: バイナリファイルを読み込む
	//--------------------------------------------------------------------------------------
	void ShaderResource::ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz){
		try{
			if (fileName == L""){
				throw BaseException(
					L"ファイルが指定されていません",
					L"if(fileName == L\"\")",
					L"ShaderResource::ReadBinaryFile()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(fileName.c_str());
			if (RetCode == -1){
				throw BaseException(
					L"ファイルが存在しません",
					fileName,
					L"ShaderResource::ReadBinaryFile()"
					);
			}

			ThrowIfFailed(
				BinaryReader::ReadEntireFile(fileName, Data, &CsoSz),
				L"ファイルの読み込みに失敗しました。",
				fileName,
				L"ShaderResource::ReadBinaryFile()"
				);

		}
		catch (...){
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateVertexShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11VertexShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータから頂点シェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//頂点シェーダーの作成
			ThrowIfFailed(
				pDx11Device->CreateVertexShader(Data.get(),CsoSz,nullptr,pResult),
				L"頂点シェーダの作成に失敗しました。",
				L"if( FAILED( pDx11Device->CreateVertexShader() ) )",
				L"ShaderResource::CreateVertexShader()"
				);

		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateInputLayout(
	//		unique_ptr<uint8_t[]>& Data,	//頂点シェーダのバイナリデータ
	//		size_t CsoSz,					//サイズ
	//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//頂点定義
	//		UINT NumElement,							//頂点定義の数
	//		ID3D11InputLayout** pResult			//受け取るレイアウト
	//	);
	//	用途: バイナリデータからインプットレイアウトを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//インプットレイアウトの作成
			HRESULT hr = pDx11Device->CreateInputLayout(
				pElement,	//頂点定義
				NumElement,		//頂点定義の数
				Data.get(),
				CsoSz,
				pResult
				);
			//失敗したら
			if (FAILED(hr))
			{
				throw basedx11::BaseException(
					L"頂点シェーダのインプットレイアウトの作成に失敗しました。",
					L"if( FAILED( pDx11Device->CreateInputLayout() ) )",
					L"ShaderResource::CreateInputLayout()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreatePixelShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11PixelShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからピクセルシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ピクセルシェーダ作成
			HRESULT hr = pDx11Device->CreatePixelShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"ピクセルシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreatePixelShader() ) )",
					L"ShaderResource::CreatePixelShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateGeometryShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11GeometryShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからジオメトリシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ジオメトリシェーダ作成
			HRESULT hr = pDx11Device->CreateGeometryShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"ジオメトリシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			//ジオメトリシェーダ作成
			UINT StridBuff[1] = { Stride };
			HRESULT hr = pDx11Device->CreateGeometryShaderWithStreamOutput(
				Data.get(),
				CsoSz,
				SOEntries.data(),
				SOEntries.size(),
				StridBuff,
				NumStride,
				0,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"ジオメトリシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateComputeShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11ComputeShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからコンピュートシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ピクセルシェーダ作成
			HRESULT hr = pDx11Device->CreateComputeShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"コンピュートシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateComputeShader() ) )",
					L"ShaderResource::CreateComputeShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}






	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateConstantBuffer(
	//		UINT BuffSize,					//サイズ
	//		ID3D11Buffer** pResult			//受け取るシェーダ
	//	);
	//	用途: コンスタントバッファを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult){
		try{
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//コンスタントバッファの作成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = BuffSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			HRESULT hr = pDx11Device->CreateBuffer(&bd, NULL, pResult);
			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"コンスタントバッファ作成に失敗しました。",
					L"if( FAILED( hr ) )",
					L"ShaderResource::CreateConstantBuffer()"
					);
			}
		}
		catch (...){
			throw;
		}
	}


	//コンスタントバッファアクセッサ
	ID3D11Buffer* ShaderResource::GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer){
		return pImpl->GetConstantBufferBase(BuffSize, Buffer);
	}
	//頂点シェーダアクセッサ
	ID3D11VertexShader* ShaderResource::GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr){
		return pImpl->GetVertexShaderBase(Filename,VSPtr);
	}
	//インプットレイアウトアクセッサ
	ID3D11InputLayout* ShaderResource::GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
		const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements){
		return pImpl->GetInputLayoutBase2(Filename, ILPtr,pElem, NumElements);
	}


}
//endof  basedx11
