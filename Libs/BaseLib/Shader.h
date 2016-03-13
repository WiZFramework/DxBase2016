#pragma once

#include "stdafx.h"

namespace basedx11{



	//--------------------------------------------------------------------------------------
	//	class ShaderResource;
	//	用途: シェーダ関連リソースのインターフェイス
	//--------------------------------------------------------------------------------------
	class ShaderResource{
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	protected:
		ShaderResource();
		virtual ~ShaderResource();
	public:
		static const int m_LightMax{ 3 };
		//--------------------------------------------------------------------------------------
		//	static void ReadBinaryFile(
		//		const wstring& fileName,		//ファイル名
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータの戻り
		//		size_t& CsoSz					//サイズの戻り
		//	);
		//	用途: バイナリファイルを読み込む
		//--------------------------------------------------------------------------------------
		static void ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz);
		//--------------------------------------------------------------------------------------
		//	static void CreateVertexShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11VertexShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータから頂点シェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateInputLayout(
		//		unique_ptr<uint8_t[]>& Data,	//頂点シェーダのバイナリデータ
		//		size_t CsoSz,					//サイズ
		//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//頂点定義
		//		UINT NumElement,							//頂点定義の数
		//		ID3D11InputLayout** pResult			//受け取るレイアウト
		//	);
		//	用途: バイナリデータからインプットレイアウトを作成する
		//--------------------------------------------------------------------------------------
		static void CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreatePixelShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11PixelShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからピクセルシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateGeometryShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11GeometryShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからジオメトリシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult);

		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, 
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride,ID3D11GeometryShader** pResult);

		//--------------------------------------------------------------------------------------
		//	static void CreateComputeShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11ComputeShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからコンピュートシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult);



		//--------------------------------------------------------------------------------------
		//	static void CreateConstantBuffer(
		//		UINT BuffSize,					//サイズ
		//		ID3D11Buffer** pResult			//受け取るシェーダ
		//	);
		//	用途: コンスタントバッファを作成する
		//--------------------------------------------------------------------------------------
		static void CreateConstantBuffer(UINT BuffSize,ID3D11Buffer** pResult);
	protected:



		//ミューテックス
		std::mutex MutexBase;
		//ピクセルシェーダアクセッサ
		ID3D11PixelShader* GetPixelShaderBase(const wstring& Filename, ComPtr<ID3D11PixelShader>& PSPtr){
			return Util::DemandCreate(PSPtr, MutexBase, [&](ID3D11PixelShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreatePixelShader(data, cso_sz, pResult);
			});
		}

		//ジオメトリシェーダアクセッサ
		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename, ComPtr<ID3D11GeometryShader>& GSPtr){
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, pResult);
			});
		}


		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename,
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride,UINT NumStride,
			ComPtr<ID3D11GeometryShader>& GSPtr){
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, SOEntries, Stride, NumStride,pResult);
			});
		}

		//コンピュートシェーダアクセッサ
		ID3D11ComputeShader* GetComputeShaderBase(const wstring& Filename, ComPtr<ID3D11ComputeShader>& CSPtr){
			return Util::DemandCreate(CSPtr, MutexBase, [&](ID3D11ComputeShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateComputeShader(data, cso_sz, pResult);
			});
		}

		//頂点シェーダアクセッサ
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr);
		//インプットレイアウトアクセッサ
		ID3D11InputLayout* GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements);
		//コンスタントバッファアクセッサ
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer);
	private:
		//コピー禁止
		ShaderResource(const ShaderResource&) = delete;
		ShaderResource& operator=(const ShaderResource&) = delete;
		//ムーブ禁止
		ShaderResource(const ShaderResource&&) = delete;
		ShaderResource& operator=(const ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType, typename ConstantType>
	//	class ConstantBuffer : public ShaderResource;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename ConstantType>
	class ConstantBuffer : public ShaderResource{
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11Buffer> m_Buffer;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		ConstantBuffer<ShaderType, ConstantType>() : ShaderResource(){}
		virtual ~ConstantBuffer(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//コピー禁止
		ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&);
		ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&);
	public:
		//公開アクセッサ
		ID3D11Buffer* GetBuffer(){
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ConstantType>
	//	class CBuffer : public ShaderResource;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	template<typename ConstantType>
	class CBuffer : public ShaderResource{
		ComPtr<ID3D11Buffer> m_Buffer;
		//コピー禁止
		CBuffer(CBuffer const&);
		CBuffer& operator=(CBuffer<ConstantType> const&);
	public:
		//構築と破棄
		CBuffer() : ShaderResource(){
			GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		virtual ~CBuffer(){}
		//公開アクセッサ
		ID3D11Buffer* GetBuffer(){
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
	};



	//--------------------------------------------------------------------------------------
	//	class VertexShader : public ShaderResource;
	//	用途: 頂点シェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename VirtexType>
	class VertexShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		VertexShader<ShaderType, VirtexType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~VertexShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//頂点シェーダ公開アクセッサ
		ID3D11VertexShader* GetShader(){
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//インプットレイアウト公開アクセッサ
		ID3D11InputLayout* GetInputLayout(){
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//template<typename VirtexType>
	//	class VShader : public ShaderResource;
	//	用途: シングルトン処理をしない頂点シェーダ（クライアントで直接使用できる）
	//--------------------------------------------------------------------------------------
	template<typename VirtexType>
	class VShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
	public:
		VShader() : ShaderResource(), m_Filename(){}
		VShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		virtual ~VShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_VSPtr = nullptr;
			m_ILPtr = nullptr;
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//頂点シェーダ公開アクセッサ
		ID3D11VertexShader* GetShader(){
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//インプットレイアウト公開アクセッサ
		ID3D11InputLayout* GetInputLayout(){
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class PixelShader : public ShaderResource;
	//	用途: ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class PixelShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11PixelShader> m_PSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		PixelShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~PixelShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//ピクセルシェーダ公開アクセッサ
		ID3D11PixelShader* GetShader(){
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PShader : public ShaderResource;
	//	用途: ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11PixelShader> m_PSPtr;
	public:
		//構築と破棄
		PShader() : ShaderResource(), m_Filename(){}
		PShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		virtual ~PShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_PSPtr = nullptr;
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//ピクセルシェーダ公開アクセッサ
		ID3D11PixelShader* GetShader(){
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
	};



	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class GeometryShader : public ShaderResource;
	//	用途: ジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class GeometryShader : public ShaderResource{
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		GeometryShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~GeometryShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//スロットエントリの登録
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride,UINT NumStride){
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries){
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
	public:
		//ジオメトリシェーダ公開アクセッサ
		ID3D11GeometryShader* GetShader(){
			if (m_SOEntries.size() > 0){
				return GetGeometryShaderBase(m_Filename, m_SOEntries,m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	class GShader : public ShaderResource;
	//	用途: ジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	class GShader : public ShaderResource{
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	public:
		//構築と破棄
		GShader() : ShaderResource(), m_Filename(){}
		GShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~GShader(){}
		//スロットエントリの登録
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride, UINT NumStride){
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries){
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_GSPtr = nullptr;
			if (m_SOEntries.size() > 0){
				GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//ジオメトリシェーダ公開アクセッサ
		ID3D11GeometryShader* GetShader(){
			if (m_SOEntries.size() > 0){
				return GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else{
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class ComputeShader : public ShaderResource;
	//	用途: コンピュートシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class ComputeShader : public ShaderResource{
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11ComputeShader> m_CSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		ComputeShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename){}
		virtual ~ComputeShader(){}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//コンピュートシェーダ公開アクセッサ
		ID3D11ComputeShader* GetShader(){
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr(){
			if (!m_Ptr){
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class CShader : public ShaderResource;
	//	用途: コンピュートシェーダ
	//--------------------------------------------------------------------------------------
	class CShader : public ShaderResource{
		wstring m_Filename;
		ComPtr<ID3D11ComputeShader> m_CSPtr;
	public:
		//構築と破棄
		CShader() : ShaderResource(), m_Filename(){}
		CShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename){
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		virtual ~CShader(){}
		void Create(const wstring& Filename){
			m_Filename = Filename;
			m_CSPtr = nullptr;
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//コンピュートシェーダ公開アクセッサ
		ID3D11ComputeShader* GetShader(){
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
	};

	//これより個別シェーダー
	//コンスタントバッファ宣言用マクロ
#define DECLARE_DX11_CONSTANT_BUFFER(CBName,CBStruct) class CBName : public ConstantBuffer<CBName,CBStruct> { \
public: \
		};

	//シェーダ宣言用マクロ
#define DECLARE_DX11_VERTEX_SHADER(ShaderName,VertexType) class ShaderName : public VertexShader<ShaderName,VertexType>{ \
	public: \
		ShaderName(); \
		};

#define DECLARE_DX11_PIXEL_SHADER(ShaderName) class ShaderName : public PixelShader<ShaderName>{ \
	public: \
		ShaderName(); \
		};

#define DECLARE_DX11_GEOMETRY_SHADER(ShaderName) class ShaderName : public GeometryShader<ShaderName>{ \
	public: \
		ShaderName(); \
	};


	//コンスタントバッファ実体用マクロ
#define IMPLEMENT_DX11_CONSTANT_BUFFER(CBName) unique_ptr<CBName, CBName::Deleter> CBName::m_Ptr;

	//シェーダ実体用マクロ
#define IMPLEMENT_DX11_VERTEX_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	VertexShader(CsoFilename){}

#define IMPLEMENT_DX11_PIXEL_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	PixelShader(CsoFilename){}

#define IMPLEMENT_DX11_GEOMETRY_SHADER(ShaderName,CsoFilename)	unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	GeometryShader(CsoFilename){}




}
//endof  basedx11
