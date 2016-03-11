#include "stdafx.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl{
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//���\�[�X�r���[
		wstring m_FileName;		//�e�N�X�`���t�@�C���ւ̃p�X

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1){
				wstring patherr = m_FileName;
				throw BaseException(
					L"�t�@�C�������݂��܂���",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
					);
			}
			HRESULT hr;
			//�e�N�X�`���쐬
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC"){
				//�t�@�C���g���q�̒���
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
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA"){
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else{
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
			}
			else if (TexType == L"DDS"){
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else if (TexType == L"TGA"){
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else{
				throw BaseException(
					L"���̃^�C�v�͑Ή����Ă��܂���",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
					);
			}

			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// �摜����V�F�[�_���\�[�XView�̍쐬
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
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
	//	�p�r: �e�N�X�`���̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	TextureResource(
	//	const wstring& FileName	//�t�@�C����
	//	);
	//	�p�r: �R���X�g���N�^
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource(){
	}

	//�A�N�Z�b�T
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const{
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const{
		return pImpl->m_FileName;
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl{
		WAVEFORMATEX m_WaveFormat;	//�E�F�u�t�H�[�}�b�g
		vector<byte> m_SoundData;	//�f�[�^
		wstring m_FileName;		//�t�@�C���ւ̃p�X
		Impl(const wstring& FileName) :
			m_FileName(FileName){}
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	�p�r: �I�[�f�B�I���\�[�X�iwav�Ȃǁj�̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName):
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try{

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundation�̏������Ɏ��s���܂���",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
				);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader),
				L"�T�E���h���[�_�[�̍쐬�Ɏ��s���܂���",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
				);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"���f�B�A�^�C�v�̍쐬�Ɏ��s���܂���",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"���f�B�AGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"���f�B�A�T�uGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"���[�_�[�ւ̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
				);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"�o�͗p�̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
				);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"�E�F�u�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
				);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"�A�g���r���[�g�̐ݒ�Ɏ��s���܂���",
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
					L"�T���v���[�̓ǂݍ��݂Ɏ��s���܂���",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
					);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"�T���v���[�̃R���o�[�g�Ɏ��s���܂���",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
						);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"�o�b�t�@��Lock�Ɏ��s���܂���",
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
	@breaf Dx11�v���~�e�B�u���b�V���N���X.<br />
	�v���~�e�B�u���b�V���́A�X�^�e�B�b�N�֐����g���Đ�������
	*/
	//--------------------------------------------------------------------------------------
	//�\�z
	MeshResource::MeshResource() :
		BaseResource(),
		m_IsSkining(false),
		m_BoneCount(0),
		m_SampleCount(0)
	{}
	//�j��
	MeshResource::~MeshResource() {}

	//���\�[�X�\�z

	shared_ptr<MeshResource> MeshResource::CreateSquare(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateSquare(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateCube(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cube�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCube(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Sphere�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateSphere(Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsule�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cylinder�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cone�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Torus�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateTetrahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Tetrahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	//��8�ʑ�
	shared_ptr<MeshResource> MeshResource::CreateOctahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Octahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateDodecahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Dodecahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateIcosahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Icosahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	void MeshResource::ReadBaseData(const wstring& BinDataDir,const wstring& BinDataFile,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials){
		vertices.clear();
		indices.clear();
		materials.clear();
		wstring DataFile = BinDataDir + BinDataFile;
		BinaryReader Reader(DataFile);
		//�w�b�_�̓ǂݍ���
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0"){
			throw BaseException(
				L"�f�[�^�`�����Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}
		//���_�̓ǂݍ���
		auto blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Vertex){
			throw BaseException(
				L"���_�̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}
		auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTexturePOD);
		auto pVertex = Reader.ReadArray<VertexPositionNormalTexturePOD>((size_t)VerTexSize);
		for (UINT i = 0; i < VerTexSize; i++){
			VertexPositionNormalTexture v;
			v.position.x = pVertex[i].position[0];
			v.position.y = pVertex[i].position[1];
			v.position.z = pVertex[i].position[2];
			v.normal.x = pVertex[i].normal[0];
			v.normal.y = pVertex[i].normal[1];
			v.normal.z = pVertex[i].normal[2];
			v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
			v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
			vertices.push_back(v);
		}

		//�C���f�b�N�X�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index){
			throw BaseException(
				L"�C���f�b�N�X�̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++){
			indices.push_back(pIndex[i]);
		}

		//�}�e���A���̓ǂݍ���
		//�}�e���A�����̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount){
			throw BaseException(
				L"�}�e���A�����̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++){
			//�e�N�X�`���t�@�C�������ϒ��Ȃ̂Œ��ӁB
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material){
				throw BaseException(
					L"�}�e���A���̃w�b�_���Ⴂ�܂�",
					DataFile,
					L"MeshResource::ReadBaseData()"
					);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!�J�n�C���f�b�N�X
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!�`��C���f�b�N�X�J�E���g
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! �f�t�B�[�Y�i���̂̐F�j
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! �X�y�L�����[�i���ˌ��j
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! �A���r�G���g�i���F�j
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! �G�~�b�V�u�i���ˌ��j
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			TextureFileStr = BinDataDir + TextureFileStr;
			ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);
			materials.push_back(ToM);
		}

		//End�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End){
			throw BaseException(
				L"End�w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}
	}

	void MeshResource::ReadBaseBoneData(const wstring& BinDataDir, const wstring& BinDataFile,
		vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Matrix4X4>& bonematrix, UINT& BoneCount, UINT& SampleCount){
		vertices.clear();
		indices.clear();
		materials.clear();
		bonematrix.clear();

		wstring DataFile = BinDataDir + BinDataFile;
		BinaryReader Reader(DataFile);
		//�w�b�_�̓ǂݍ���
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0"){
			throw BaseException(
				L"�f�[�^�`�����Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}
		//���_�̓ǂݍ���
		auto blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::SkinedVertex){
			throw BaseException(
				L"���_(�X�L������)�̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}
		auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTextureSkinningPOD);
		auto pVertex = Reader.ReadArray<VertexPositionNormalTextureSkinningPOD>((size_t)VerTexSize);
		for (UINT i = 0; i < VerTexSize; i++){
			VertexPositionNormalTextureSkinning v;
			v.position.x = pVertex[i].position[0];
			v.position.y = pVertex[i].position[1];
			v.position.z = pVertex[i].position[2];
			v.normal.x = pVertex[i].normal[0];
			v.normal.y = pVertex[i].normal[1];
			v.normal.z = pVertex[i].normal[2];
			v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
			v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
			for (int j = 0; j < 4; j++){
				v.indices[j] = pVertex[i].indices[j];
				v.weights[j] = pVertex[i].weights[j];
			}
			vertices.push_back(v);
		}

		//�C���f�b�N�X�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index){
			throw BaseException(
				L"�C���f�b�N�X�̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++){
			indices.push_back(pIndex[i]);
		}

		//�}�e���A���̓ǂݍ���
		//�}�e���A�����̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount){
			throw BaseException(
				L"�}�e���A�����̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseData()"
				);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++){
			//�e�N�X�`���t�@�C�������ϒ��Ȃ̂Œ��ӁB
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material){
				throw BaseException(
					L"�}�e���A���̃w�b�_���Ⴂ�܂�",
					DataFile,
					L"MeshResource::ReadBaseBoneData()"
					);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!�J�n�C���f�b�N�X
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!�`��C���f�b�N�X�J�E���g
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! �f�t�B�[�Y�i���̂̐F�j
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! �X�y�L�����[�i���ˌ��j
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! �A���r�G���g�i���F�j
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! �G�~�b�V�u�i���ˌ��j
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			TextureFileStr = BinDataDir + TextureFileStr;
			ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);
			materials.push_back(ToM);
		}

		//�{�[����
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::BoneCount){
			throw BaseException(
				L"�{�[�����̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}
		BoneCount = blockHeader.m_Size;
		//�{�[���A�j���[�V�����s��
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::AnimeMatrix){
			throw BaseException(
				L"�A�j���[�V�����s��̃w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}
		auto MatrixSize = blockHeader.m_Size / sizeof(MatrixPOD);
		auto pAnimeMatrix = Reader.ReadArray<MatrixPOD>((size_t)MatrixSize);
		for (UINT i = 0; i < MatrixSize; i++){
			//�{�[���P�ʂł͂Ȃ��s��P�ʂœǂݍ���
			Matrix4X4 mat;
			for (int u = 0; u < 4; u++){
				for (int v = 0; v < 4; v++){
					mat(u, v) = pAnimeMatrix->m_Mat[u][v];
				}
			}
			bonematrix.push_back(mat);
			pAnimeMatrix++;
		}
		SampleCount = MatrixSize / BoneCount;

		//End
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End){
			throw BaseException(
				L"�I���w�b�_���Ⴂ�܂�",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
				);
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateStaticModelMesh(const wstring& BinDataDir, const wstring& BinDataFile, bool AccessWrite){
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			ReadBaseData(BinDataDir, BinDataFile,vertices, indices, Materials);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials){
				Ptr->m_MaterialExVec.push_back(v);
			}
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}


	shared_ptr<MeshResource> MeshResource::CreateBoneModelMesh(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite){
		try {
			//���_�z��
			vector<VertexPositionNormalTextureSkinning> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			//�T���v�����O�s��
			vector<Matrix4X4> SampleMatrices;
			//�{�[����
			UINT BoneCount;
			//�T���v����
			UINT SampleCount;
			ReadBaseBoneData(BinDataDir, BinDataFile,vertices, indices, Materials,
				SampleMatrices, BoneCount, SampleCount);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTextureSkinning>(vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials){
				Ptr->m_MaterialExVec.push_back(v);
			}
			Ptr->m_SampleMatrixVec.clear();
			for (auto& v : SampleMatrices){
				Ptr->m_SampleMatrixVec.push_back(v);
			}
			Ptr->m_BoneCount = BoneCount;
			Ptr->m_SampleCount = SampleCount;
			Ptr->m_IsSkining = true;
			return Ptr;
		}
		catch (...) {
			throw;
		}

	}





}

//endof  basedx11
