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
		BaseResource()
	{}
	//�j��
	MeshResource::~MeshResource() {}

	//���\�[�X�\�z

	shared_ptr<MeshResource> MeshResource::CreateSquare(float Size, bool AccessWrite) {
		try {
			float HelfSize = Size / 2.0f;
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)));
			//�C���f�b�N�X���쐬���邽�߂̔z��
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




}

//endof  basedx11
