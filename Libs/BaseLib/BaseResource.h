/*!
@file BaseResource.h
@brief ��������ɕێ����郊�\�[�X�i�e�N�X�`���A���b�V���A�I�[�f�B�I�Ȃǁj

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/


#pragma once
#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class BaseResource : public Object;
	/*!
	��������ɕێ����郊�\�[�X�̐e�N���X
	*/
	//--------------------------------------------------------------------------------------
	class BaseResource : public Object{
	protected:
		//--------------------------------------------------------------------------------------
		//	BaseResource();
		/*!
		@breaf �v���e�N�g�R���X���g�N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		BaseResource(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~BaseResource();
		/*!
		@breaf �v���e�N�g�f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource(){}
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	/*!
	�e�N�X�`�����\�[�X
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit TextureResource(
		//	const wstring& FileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^
		@param const wstring& FileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		explicit TextureResource(const wstring& FileName,const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		//	virtual ~TextureResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		/*!
		@breaf �V�F�[�_�[���\�[�X�r���[�̎擾
		@param �Ȃ�
		@return	���̃e�N�X�`���̃V�F�[�_�[���\�[�X�r���[
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		//	const wstring& GetTextureFileName() const;
		/*!
		@breaf �e�N�X�`���̃t�@�C�����̎擾
		@param �Ȃ�
		@return	���̃e�N�X�`���̃t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureFileName() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	/*!
	�I�[�f�B�I���\�[�X�iwav�j�N���X
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^<br />
		�E�F�u�t�@�C������n��
		@param const wstring& FileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf �T�E���h�f�[�^�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X�̃T�E���h�f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf �E�F�u�t�H�[�}�b�g�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X��WAVEFORMATEX�E�F�u�t�H�[�}�b�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImpl�C�f�B�I��
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
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Color4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Color4 m_Specular;
		//! �A���r�G���g�i���F�j
		Color4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Color4 m_Emissive;
		//�V�F�[�_���\�[�X�r���[�i�e�N�X�`�����\�[�X�j
		shared_ptr<TextureResource> m_TextureResource;
	};

	struct MaterialExPOD{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		float m_Diffuse[4];
		//! �X�y�L�����[�i���ˌ��j
		float m_Specular[4];
		//! �A���r�G���g�i���F�j
		float m_Ambient[4];
		//! �G�~�b�V�u�i���ˌ��j
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
	@breaf Dx11�v���~�e�B�u���b�V���N���X.<br />
	�v���~�e�B�u���b�V���́A�X�^�e�B�b�N�֐����g���Đ�������
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
		shared_ptr<BackupDataBase> m_BackUpData;
		vector<MaterialEx> m_MaterialExVec;	//�}�e���A���̔z��i���f���Ŏg�p�j
		//�ȉ��A�{�[���p
		bool m_IsSkining;
		UINT m_BoneCount;	//�{�[���̐�
		UINT m_SampleCount;	//�T���v�����O��
		vector<Matrix4X4> m_SampleMatrixVec;	//�T���v�����O���ꂽ�{�[���s��
	protected:
		//�h���N���X����̂݃A�N�Z�X�ł���A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	void SetVertexBuffer(
		//	ComPtr<ID3D11Buffer>& VertexBuffer	//���_�o�b�t�@
		//	);
		/*!
		@breaf ���_�o�b�t�@�̐ݒ�
		@param ComPtr<ID3D11Buffer>& VertexBuffer	���_�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer) {
			m_VertexBuffer = VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	void SetNumVertices(
		//		UINT NumVertices	//���_��
		//	);
		/*!
		@breaf ���_���̐ݒ�<br />
		���_�o�b�t�@�̐��ƍ��킹�Ȃ���΂Ȃ�Ȃ��̂ŁA�T�d�Ɏg�p����
		@param UINT NumVertices	���_��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices) {
			m_NumVertices = NumVertices;
		}
		//--------------------------------------------------------------------------------------
		//	void SetIndexBuffer(
		//		ComPtr<ID3D11Buffer>& IndexBuffer	//�C���f�b�N�X�o�b�t�@
		//	);
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�̐ݒ�
		@param ComPtr<ID3D11Buffer>& IndexBuffer	�C���f�b�N�X�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer) {
			m_IndexBuffer = IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	void SetNumIndicis(
		//		UINT NumIndicis	//�C���f�b�N�X��
		//	);
		/*!
		@breaf �C���f�b�N�X���̐ݒ�<br />
		�C���f�b�N�X�o�b�t�@�̐��ƍ��킹�Ȃ���΂Ȃ�Ȃ��̂ŁA�T�d�Ɏg�p����
		@param ComPtr<ID3D11Buffer>& IndexBuffer	�C���f�b�N�X�o�b�t�@
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis) {
			m_NumIndicis = NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		//	MeshResource();
		/*!
		@breaf �v���e�N�g�R���X�g���N�^<br />
		�\�z�̓X�^�e�B�b�N�֐��𗘗p����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		MeshResource();
	public:
		//--------------------------------------------------------------------------------------
		//	virtual ~MeshResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer> GetVertexBuffer() const;
		/*!
		@breaf ���_�o�b�t�@�̎擾
		@param �Ȃ�
		@return	���_�o�b�t�@
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetVertexBuffer() const {
			return m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer> GetIndexBuffer() const;
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�̎擾
		@param �Ȃ�
		@return	�C���f�b�N�X�o�b�t�@
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetIndexBuffer() const {
			return m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetNumVertices() const;
		/*!
		@breaf ���_���̎擾
		@param �Ȃ�
		@return	���_��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetNumIndicis() const;
		/*!
		@breaf �C���f�b�N�X���̎擾
		@param �Ȃ�
		@return	�C���f�b�N�X��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		//	const vvector<MaterialEx>& GetMaterialExVec()const;
		/*!
		@breaf �}�e���A���z��̎擾
		@param �Ȃ�
		@return	�}�e���A���̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<MaterialEx>& GetMaterialExVec()const{
			return m_MaterialExVec;
		}
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf �X�L�j���O���邩�ǂ���.<br />
		���z�֐��Ȃ̂ŁA�h���N���X�œƎ��ɑ��d��`����
		@param �Ȃ�
		@return	�X�L�j���O����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const { return m_IsSkining; }
		//--------------------------------------------------------------------------------------
		//	UINT GetBoneCount() const;
		/*!
		@breaf �{�[�����𓾂�
		@param �Ȃ�
		@return	�{�[����
		*/
		//--------------------------------------------------------------------------------------
		UINT GetBoneCount() const{
			return m_BoneCount;
		}
		//--------------------------------------------------------------------------------------
		//	UINT GetSampleCount() const;
		/*!
		@breaf �T���v�����O���𓾂�
		@param �Ȃ�
		@return	�T���v�����O��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetSampleCount() const{
			return m_SampleCount;
		}

		const vector<Matrix4X4>& GetSampleMatrixVec() const{
			return m_SampleMatrixVec;
		}

		//���\�[�X�\�z
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<Dx11CommonMes> CreateSquare(
		//		float Size,		//1�ӂ̃T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf 4�p�`���ʂ̍쐬
		@param	float size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSquare(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCube(
		//		float Size,		//1�ӂ̃T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �L���[�u�i�����́j�̍쐬
		@param	float size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCube(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateSphere(
		//		float Diameter,		//���a
		//		size_t Tessellation,	//������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ���̂̍쐬
		@param	float Diameter		���a
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCapsule(
		//		float Diameter,		//���a
		//		float Height,		//����
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �J�v�Z���̍쐬
		@param	float Diameter		���a
		@param	float Height,		����
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCylinder(
		//		float Height,	//����
		//		float Diameter, //���a
		//		size_t Tessellation,	//������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �V�����_�[�̍쐬
		@param	float Height,		����
		@param	float Diameter		���a
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateCone(
		//		float Diameter, //���a
		//		float Height, //����
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �R�[���̍쐬
		@param	float Diameter		���a
		@param	float Height,		����
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateTorus(
		//		float Diameter, //���a
		//		float Thickness, //�h�[�i�b�c�̑���
		//		size_t Tessellation, //������
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �g�[���X�̍쐬
		@param	float Diameter		���a
		@param	float Thickness		�h�[�i�b�c�̑���
		@param	size_t Tessellation	������
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateTetrahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��4�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateOctahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��8�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateDodecahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��12�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<MeshResource> CreateIcosahedron(
		//		float Size,		//�T�C�Y
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf ��20�ʑ̂̍쐬
		@param	float Size		1�ӂ̃T�C�Y
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false);

		//--------------------------------------------------------------------------------------
		// static void ReadBaseData(
		//		const wstring& BinDataDir, //��f�B���N�g��
		//		const wstring& BinDataFile,	//�f�[�^�t�@�C����
		//		vector<VertexPositionNormalTexture>& vertices,	//���_�̎Q�� 
		//		vector<uint16_t>& indices,	//�C���f�b�N�X�̎Q��
		//		vector<MaterialEx>& materials	//�}�e���A���̎Q��
		//	);
		/*!
		@breaf �I���W�i�����b�V���f�[�^�̓ǂݍ��݁i�X�^�e�B�b�N���b�V���j
		@param	const wstring& BinDataDir, ��f�B���N�g��
		@param	const wstring& BinDataFile,	�f�[�^�t�@�C����
		@param	vector<VertexPositionNormalTexture>& vertices,	���_�̎Q��
		@param	vector<uint16_t>& indices,	�C���f�b�N�X�̎Q��
		@param	vector<MaterialEx>& materials	�}�e���A���̎Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBaseData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials);

		static void ReadBaseBoneData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Matrix4X4>& bonematrix, UINT& BoneCount, UINT& SampleCount);


		//--------------------------------------------------------------------------------------
		// static shared_ptr<MeshResource> CreateStaticModelMesh(
		//		const wstring& BinDataDir,//��f�B���N�g��
		//		const wstring& BinDataFile,//�f�[�^�t�@�C����
		//		bool AccessWrite = false	//���_��ύX�ł��邩�ǂ���
		//	);
		/*!
		@breaf �I���W�i�����b�V���̍쐬�i�X�^�e�B�b�N���b�V���j
		@param	const wstring& BinDataDir, ��f�B���N�g��
		@param	const wstring& BinDataFile,	�f�[�^�t�@�C����
		@param	bool AccessWrite = false	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
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
		//		const vector<T>& vertices,	//���_�̔z��
		//		bool AccessWrite			//�㏑���\���ǂ���
		//	);
		/*!
		@breaf ���b�V�����\�[�X�̍쐬
		@param	const vector<T>& vertices,	���_�̔z��
		@param	bool AccessWrite			�㏑���\���ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			std::mutex Mutex;
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources<Dx11DeviceResources>();
			auto pDx11Device = Dev->GetD3DDevice();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//�o�b�t�@�̍쐬
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		//
		//	template<typename T>
		//	static  shared_ptr<MeshResource> CreateMeshResource(
		//		const vector<T>& vertices,	//���_�̔z��
		//		const vector<uint16_t>& indices,	//�C���f�b�N�X�̔z��
		//		bool AccessWrite			//�㏑���\���ǂ���
		//	);
		/*!
		@breaf ���b�V�����\�[�X�̍쐬
		@param	const vector<T>& vertices,	���_�̔z��
		@param	const vector<uint16_t>& indices,	�C���f�b�N�X�̔z��
		@param	bool AccessWrite			�㏑���\���ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			std::mutex Mutex;
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//�o�b�t�@�̍쐬
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//�C���f�b�N�X�̍쐬
			Util::DemandCreate(Ptr->m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//�C���f�b�N�X�o�b�t�@�̍쐬
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//�C���f�b�N�X���̐ݒ�
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		// template<typename T>
		//	vector<T>& GetBackupVerteces() const;
		/*!
		@breaf �o�b�N�A�b�v�f�[�^�̎擾<br />
		AccessWrite��true�ō쐬���ꂽ���\�[�X�́A���_�̔z��ɂ���Ē��_��ύX�ł���B<br />
		AccessWrite��true�łȂ��ꍇ�́A�o�b�N�A�b�v�͋�ł���B
		@param	�Ȃ�
		@return	�o�b�N�A�b�v�f�[�^�̔z��B
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		vector<T>& GetBackupVerteces() const {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseMBException(
					"�o�b�N�A�b�v��T�^�ɃL���X�g�ł��܂���",
					typeid(T).name(),
					"MeshResource::GetBackupVerteces<T>()"
					);
			}
			return Ptr->m_Vertices;
		}
		//--------------------------------------------------------------------------------------
		// template<typename T>
		//	void UpdateVirtexBuffer(
		//		const vector<T>& NewBuffer	//���_�̔z��
		//	);
		/*!
		@breaf ���_�̕ύX.<br />
		AccessWrite��true�ō쐬���ꂽ���\�[�X�́A���_�̔z��ɂ���Ē��_��ύX����B
		@param	const vector<T>& NewBuffer	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtexBuffer(const vector<T>& NewBuffer) {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseMBException(
					"�o�b�N�A�b�v��T�^�ɃL���X�g�ł��܂���",
					typeid(T).name(),
					"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}

			if (NewBuffer.size() != Ptr->m_Vertices.size()) {
				// Map���s
				throw BaseException(
					L"�ύX���钸�_�̐����o�b�N�A�b�v�ƈႢ�܂�",
					L"if (NewBuffer.size() != Ptr->m_Vertices.size())",
					L"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}
			//���W��ύX����
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//���_�o�b�t�@�����\�[�X������o��
			auto pVertexBuffer = GetVertexBuffer().Get();

			//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//���_�̃}�b�v
			if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))) {
				// Map���s
				throw BaseException(
					L"���_��Map�Ɏ��s���܂����B",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"MeshResource::UpdateVirtexBuffer<T>()"
					);
			}
			//���_�̕ύX
			T* vertices = (T*)mappedBuffer.pData;
			for (size_t i = 0; i < NewBuffer.size(); i++) {
				vertices[i] = NewBuffer[i];
			}
			//�A���}�b�v
			pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

		}
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};



}

//endof  basedx11
