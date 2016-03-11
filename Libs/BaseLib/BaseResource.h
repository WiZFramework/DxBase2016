/*!
@file BaseResource.h
@brief メモリ上に保持するリソース（テクスチャ、メッシュ、オーディオなど）

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/


#pragma once
#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class BaseResource : public Object;
	/*!
	メモリ上に保持するリソースの親クラス
	*/
	//--------------------------------------------------------------------------------------
	class BaseResource : public Object{
	protected:
		//--------------------------------------------------------------------------------------
		//	BaseResource();
		/*!
		@breaf プロテクトコンスラトクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		BaseResource(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~BaseResource();
		/*!
		@breaf プロテクトデストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource(){}
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	/*!
	テクスチャリソース
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit TextureResource(
		//	const wstring& FileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ
		@param const wstring& FileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit TextureResource(const wstring& FileName,const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		//	virtual ~TextureResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		/*!
		@breaf シェーダーリソースビューの取得
		@param なし
		@return	このテクスチャのシェーダーリソースビュー
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		//	const wstring& GetTextureFileName() const;
		/*!
		@breaf テクスチャのファイル名の取得
		@param なし
		@return	このテクスチャのファイル名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureFileName() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	/*!
	オーディオリソース（wav）クラス
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ<br />
		ウェブファイル名を渡す
		@param const wstring& FileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf サウンドデータの取得
		@param なし
		@return	このウェブリソースのサウンドデータ
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf ウェブフォーマットの取得
		@param なし
		@return	このウェブリソースのWAVEFORMATEXウェブフォーマットのポインタ
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	struct BackupDataBase {
		BackupDataBase() {}
		virtual ~BackupDataBase() {}

	};
	template<typename T>
	struct BackupData : public BackupDataBase {
		vector<T> m_Vertices;
	};

	struct VertexPositionNormalTexturePOD{
		float position[3];
		float normal[3];
		float textureCoordinate[2];
	};

	struct VertexPositionNormalTextureSkinningPOD{
		float position[3];
		float normal[3];
		float textureCoordinate[2];
		uint32_t indices[4];
		float weights[4];
	};


	struct MaterialEx{
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		Color4 m_Diffuse;
		//! スペキュラー（反射光）
		Color4 m_Specular;
		//! アンビエント（環境色）
		Color4 m_Ambient;
		//! エミッシブ（放射光）
		Color4 m_Emissive;
		//シェーダリソースビュー（テクスチャリソース）
		shared_ptr<TextureResource> m_TextureResource;
	};

	struct MaterialExPOD{
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		float m_Diffuse[4];
		//! スペキュラー（反射光）
		float m_Specular[4];
		//! アンビエント（環境色）
		float m_Ambient[4];
		//! エミッシブ（放射光）
		float m_Emissive[4];
	};

	struct	MatrixPOD
	{
		float	m_Mat[4][4];
	};



	enum class BlockType{
		Vertex,
		Index,
		Material,
		MaterialCount,
		SkinedVertex,
		BoneCount,
		AnimeMatrix,
		End = 100
	};

	struct BlockHeader{
		BlockType m_Type;
		UINT m_Size;
	};


	//--------------------------------------------------------------------------------------
	//	class MeshResource : public BaseResource;
	/*!
	@breaf Dx11プリミティブメッシュクラス.<br />
	プリミティブメッシュは、スタティック関数を使って生成する
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
		shared_ptr<BackupDataBase> m_BackUpData;
		vector<MaterialEx> m_MaterialExVec;	//マテリアルの配列（モデルで使用）
		//以下、ボーン用
		bool m_IsSkining;
		UINT m_BoneCount;	//ボーンの数
		UINT m_SampleCount;	//サンプリング数
		vector<Matrix4X4> m_SampleMatrixVec;	//サンプリングされたボーン行列
	protected:
		//派生クラスからのみアクセスできるアクセサ
		//--------------------------------------------------------------------------------------
		//	void SetVertexBuffer(
		//	ComPtr<ID3D11Buffer>& VertexBuffer	//頂点バッファ
		//	);
		/*!
		@breaf 頂点バッファの設定
		@param ComPtr<ID3D11Buffer>& VertexBuffer	頂点バッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer) {
			m_VertexBuffer = VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	void SetNumVertices(
		//		UINT NumVertices	//頂点数
		//	);
		/*!
		@breaf 頂点数の設定<br />
		頂点バッファの数と合わせなければならないので、慎重に使用する
		@param UINT NumVertices	頂点数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices) {
			m_NumVertices = NumVertices;
		}
		//--------------------------------------------------------------------------------------
		//	void SetIndexBuffer(
		//		ComPtr<ID3D11Buffer>& IndexBuffer	//インデックスバッファ
		//	);
		/*!
		@breaf インデックスバッファの設定
		@param ComPtr<ID3D11Buffer>& IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer) {
			m_IndexBuffer = IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	void SetNumIndicis(
		//		UINT NumIndicis	//インデックス数
		//	);
		/*!
		@breaf インデックス数の設定<br />
		インデックスバッファの数と合わせなければならないので、慎重に使用する
		@param ComPtr<ID3D11Buffer>& IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis) {
			m_NumIndicis = NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		//	MeshResource();
		/*!
		@breaf プロテクトコンストラクタ<br />
		構築はスタティック関数を利用する
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		MeshResource();
	public:
		//--------------------------------------------------------------------------------------
		//	virtual ~MeshResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer> GetVertexBuffer() const;
		/*!
		@breaf 頂点バッファの取得
		@param なし
		@return	頂点バッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetVertexBuffer() const {
			return m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer> GetIndexBuffer() const;
		/*!
		@breaf インデックスバッファの取得
		@param なし
		@return	インデックスバッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetIndexBuffer() const {
			return m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetNumVertices() const;
		/*!
		@breaf 頂点数の取得
		@param なし
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetNumIndicis() const;
		/*!
		@breaf インデックス数の取得
		@param なし
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		//	const vvector<MaterialEx>& GetMaterialExVec()const;
		/*!
		@breaf マテリアル配列の取得
		@param なし
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<MaterialEx>& GetMaterialExVec()const{
			return m_MaterialExVec;
		}
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@param なし
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const { return m_IsSkining; }
		//--------------------------------------------------------------------------------------
		//	UINT GetBoneCount() const;
		/*!
		@breaf ボーン数を得る
		@param なし
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetBoneCount() const{
			return m_BoneCount;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetSampleCount() const;
		/*!
		@breaf サンプリング数を得る
		@param なし
		@return	サンプリング数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetSampleCount() const{
			return m_SampleCount;
		}

		const vector<Matrix4X4>& GetSampleMatrixVec() const{
			return m_SampleMatrixVec;
		}

		//リソース構築
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<Dx11CommonMes> CreateSquare(
		//		float Size,		//1辺のサイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 4角形平面の作成
		@param	float size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSquare(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCube(
		//		float Size,		//1辺のサイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf キューブ（立方体）の作成
		@param	float size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCube(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateSphere(
		//		float Diameter,		//直径
		//		size_t Tessellation,	//分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 球体の作成
		@param	float Diameter		直径
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCapsule(
		//		float Diameter,		//直径
		//		float Height,		//高さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf カプセルの作成
		@param	float Diameter		直径
		@param	float Height,		高さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCylinder(
		//		float Height,	//高さ
		//		float Diameter, //直径
		//		size_t Tessellation,	//分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf シリンダーの作成
		@param	float Height,		高さ
		@param	float Diameter		直径
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCone(
		//		float Diameter, //直径
		//		float Height, //高さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf コーンの作成
		@param	float Diameter		直径
		@param	float Height,		高さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateTorus(
		//		float Diameter, //直径
		//		float Thickness, //ドーナッツの太さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf トーラスの作成
		@param	float Diameter		直径
		@param	float Thickness		ドーナッツの太さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateTetrahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正4面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateOctahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正8面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateDodecahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正12面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateIcosahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正20面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false);

		//--------------------------------------------------------------------------------------
		// static void ReadBaseData(
		//		const wstring& BinDataDir, //基準ディレクトリ
		//		const wstring& BinDataFile,	//データファイル名
		//		vector<VertexPositionNormalTexture>& vertices,	//頂点の参照 
		//		vector<uint16_t>& indices,	//インデックスの参照
		//		vector<MaterialEx>& materials	//マテリアルの参照
		//	);
		/*!
		@breaf オリジナルメッシュデータの読み込み（スタティックメッシュ）
		@param	const wstring& BinDataDir, 基準ディレクトリ
		@param	const wstring& BinDataFile,	データファイル名
		@param	vector<VertexPositionNormalTexture>& vertices,	頂点の参照
		@param	vector<uint16_t>& indices,	インデックスの参照
		@param	vector<MaterialEx>& materials	マテリアルの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBaseData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials);

		static void ReadBaseBoneData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Matrix4X4>& bonematrix, UINT& BoneCount, UINT& SampleCount);


		//--------------------------------------------------------------------------------------
		// static shared_ptr<MeshResource> CreateStaticModelMesh(
		//		const wstring& BinDataDir,//基準ディレクトリ
		//		const wstring& BinDataFile,//データファイル名
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf オリジナルメッシュの作成（スタティックメッシュ）
		@param	const wstring& BinDataDir, 基準ディレクトリ
		@param	const wstring& BinDataFile,	データファイル名
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateStaticModelMesh(const wstring& BinDataDir, 
			const wstring& BinDataFile, bool AccessWrite = false);

		static shared_ptr<MeshResource> CreateBoneModelMesh(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);



		//--------------------------------------------------------------------------------------
		//
		//	template<typename T>
		//	static  shared_ptr<MeshResource> CreateMeshResource(
		//		const vector<T>& vertices,	//頂点の配列
		//		bool AccessWrite			//上書き可能かどうか
		//	);
		/*!
		@breaf メッシュリソースの作成
		@param	const vector<T>& vertices,	頂点の配列
		@param	bool AccessWrite			上書き可能かどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			std::mutex Mutex;
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources<Dx11DeviceResources>();
			auto pDx11Device = Dev->GetD3DDevice();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//バッファの作成
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//頂点バッファの作成
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		//
		//	template<typename T>
		//	static  shared_ptr<MeshResource> CreateMeshResource(
		//		const vector<T>& vertices,	//頂点の配列
		//		const vector<uint16_t>& indices,	//インデックスの配列
		//		bool AccessWrite			//上書き可能かどうか
		//	);
		/*!
		@breaf メッシュリソースの作成
		@param	const vector<T>& vertices,	頂点の配列
		@param	const vector<uint16_t>& indices,	インデックスの配列
		@param	bool AccessWrite			上書き可能かどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			std::mutex Mutex;
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//バッファの作成
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//頂点バッファの作成
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//インデックスの作成
			Util::DemandCreate(Ptr->m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//インデックスバッファの作成
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//インデックス数の設定
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		// template<typename T>
		//	vector<T>& GetBackupVerteces() const;
		/*!
		@breaf バックアップデータの取得<br />
		AccessWriteがtrueで作成されたリソースは、頂点の配列によって頂点を変更できる。<br />
		AccessWriteがtrueでない場合は、バックアップは空である。
		@param	なし
		@return	バックアップデータの配列。
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		vector<T>& GetBackupVerteces() const {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseMBException(
					"バックアップをT型にキャストできません",
					typeid(T).name(),
					"MeshResource::GetBackupVerteces<T>()"
					);
			}
			return Ptr->m_Vertices;
		}
		//--------------------------------------------------------------------------------------
		// template<typename T>
		//	void UpdateVirtexBuffer(
		//		const vector<T>& NewBuffer	//頂点の配列
		//	);
		/*!
		@breaf 頂点の変更.<br />
		AccessWriteがtrueで作成されたリソースは、頂点の配列によって頂点を変更する。
		@param	const vector<T>& NewBuffer	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtexBuffer(const vector<T>& NewBuffer) {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseMBException(
					"バックアップをT型にキャストできません",
					typeid(T).name(),
					"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}

			if (NewBuffer.size() != Ptr->m_Vertices.size()) {
				// Map失敗
				throw BaseException(
					L"変更する頂点の数がバックアップと違います",
					L"if (NewBuffer.size() != Ptr->m_Vertices.size())",
					L"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}
			//座標を変更する
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//頂点バッファをリソースから取り出す
			auto pVertexBuffer = GetVertexBuffer().Get();

			//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//頂点のマップ
			if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))) {
				// Map失敗
				throw BaseException(
					L"頂点のMapに失敗しました。",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}
			//頂点の変更
			T* vertices = (T*)mappedBuffer.pData;
			for (size_t i = 0; i < NewBuffer.size(); i++) {
				vertices[i] = NewBuffer[i];
			}
			//アンマップ
			pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

		}
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};



}

//endof  basedx11
