#include "stdafx.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl{
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//リソースビュー
		wstring m_FileName;		//テクスチャファイルへのパス

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl(){}
		//ミューテックス
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"ファイルが指定されていません",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1){
				wstring patherr = m_FileName;
				throw BaseException(
					L"ファイルが存在しません",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
					);
			}
			HRESULT hr;
			//テクスチャ作成
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC"){
				//ファイル拡張子の調査
				wchar_t Drivebuff[_MAX_DRIVE];
				wchar_t Dirbuff[_MAX_DIR];
				wchar_t FileNamebuff[_MAX_FNAME];
				wchar_t Extbuff[_MAX_EXT];

				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));

				_wsplitpath_s(m_FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS"){
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA"){
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else{
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
			}
			else if (TexType == L"DDS"){
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else if (TexType == L"TGA"){
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else{
				throw BaseException(
					L"そのタイプは対応していません",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
					);
			}

			//デバイスとコンテキストインターフェイスの取得
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// 画像からシェーダリソースViewの作成
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"シェーダーリソースビューを作成できません",
					L"if( FAILED( CreateShaderResourceView() ) )",
					L"Texture::Impl::Impl()"
					);
				return hr;
			});


		}
		catch (...){
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	//	用途: テクスチャのラッピングクラス
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	TextureResource(
	//	const wstring& FileName	//ファイル名
	//	);
	//	用途: コンストラクタ
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource(){
	}

	//アクセッサ
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const{
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const{
		return pImpl->m_FileName;
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl{
		WAVEFORMATEX m_WaveFormat;	//ウェブフォーマット
		vector<byte> m_SoundData;	//データ
		wstring m_FileName;		//ファイルへのパス
		Impl(const wstring& FileName) :
			m_FileName(FileName){}
		~Impl(){}
		//ミューテックス
		std::mutex Mutex;

	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	用途: オーディオリソース（wavなど）のラッピングクラス
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName):
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try{

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundationの初期化に失敗しました",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
				);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader),
				L"サウンドリーダーの作成に失敗しました",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
				);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"メディアタイプの作成に失敗しました",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"メディアGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"メディアサブGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"リーダーへのメディアタイプの設定に失敗しました",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
				);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"出力用のメディアタイプの設定に失敗しました",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
				);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"ウェブフォーマットの設定に失敗しました",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
				);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"アトリビュートの設定に失敗しました",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
				);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
				10000000
				);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"サンプラーの読み込みに失敗しました",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
					);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"サンプラーのコンバートに失敗しました",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
						);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"バッファのLockに失敗しました",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
						);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...){
			throw;
		}
	}
	AudioResource::~AudioResource(){}

	const vector<byte>& AudioResource::GetSoundData()const{
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const{
		return &pImpl->m_WaveFormat;
	}



	//--------------------------------------------------------------------------------------
	//	class MeshResource : public MeshResource;
	/*!
	@breaf Dx11プリミティブメッシュクラス.<br />
	プリミティブメッシュは、スタティック関数を使って生成する
	*/
	//--------------------------------------------------------------------------------------
	//構築
	MeshResource::MeshResource() :
		BaseResource()
	{}
	//破棄
	MeshResource::~MeshResource() {}

	//リソース構築

	shared_ptr<MeshResource> MeshResource::CreateSquare(float Size, bool AccessWrite) {
		try {
			float HelfSize = Size / 2.0f;
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)));
			//インデックスを作成するための配列
			vector<uint16_t> indices{
				(uint16_t)0,
				(uint16_t)1,
				(uint16_t)2,
				(uint16_t)1,
				(uint16_t)3,
				(uint16_t)2,
			};
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateCube(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cubeの作成(ヘルパー関数を利用)
			VertexUtil::CreateCube(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Sphereの作成(ヘルパー関数を利用)
			VertexUtil::CreateSphere(Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsuleの作成(ヘルパー関数を利用)
			VertexUtil::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cylinderの作成(ヘルパー関数を利用)
			VertexUtil::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Coneの作成(ヘルパー関数を利用)
			VertexUtil::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Torusの作成(ヘルパー関数を利用)
			VertexUtil::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateTetrahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Tetrahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	//正8面体
	shared_ptr<MeshResource> MeshResource::CreateOctahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Octahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateDodecahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Dodecahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateIcosahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Icosahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}




}

//endof  basedx11
