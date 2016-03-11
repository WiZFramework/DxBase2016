
/*!
@file DrawComponents.h
@brief �R���|�[�l���g�i�`��n�j

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	class TextureResource;
	class MeshResource;
	class FbxMeshResource;

	//--------------------------------------------------------------------------------------
	//	class DrawComponent : public Component;
	//	�p�r: �`��R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component{
	protected:
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~DrawComponent();
	public:
		BlendState GetBlendState() const;
		DepthStencilState GetDepthStencilState() const;
		RasterizerState GetRasterizerState() const;
		SamplerState GetSamplerState() const;

		void SetBlendState(const BlendState state);
		void SetDepthStencilState(const DepthStencilState state);
		void SetRasterizerState(const RasterizerState state);
		void SetSamplerState(const SamplerState state);

		const Matrix4X4& GetMeshToTransformMatrix() const;
		void SetMeshToTransformMatrix(const Matrix4X4& Mat);


		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		//�V���h�E�}�b�v����e�N���X�ŎQ�Ƃł���悤�ɉ��z�֐��ɂ���
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const{ return nullptr; }
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public DrawComponent;
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	class Shadowmap : public DrawComponent{
	public:
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Shadowmap();
		//�A�N�Z�T
		static float GetLightHeight();
		static float GetLightNear();
		static float GetLightFar();
		static float GetViewWidth();
		static float GetViewHeight();

		static void SetLightHeight(float f);
		static void SetLightNear(float f);
		static void SetLightFar(float f);
		static void SetViewWidth(float f);
		static void SetViewHeight(float f);
		static void SetViewSize(float f);


		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		void SetMeshResource(const wstring& ResKey);
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);


		//����
		virtual void OnUpdate()override{}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//���W�n
	enum class SpriteCoordinate {
		m_CenterZeroPlusUpY,		//�Z���^�[���_��Y��������v���X�i�f�t�H���g�j
		m_LeftBottomZeroPlusUpY,	//�������_��Y��������v���X
		m_LeftTopZeroPlusDownY,		//���㌴�_��Y���������v���X
	};


	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//	�p�r: Sprite�`��R���|�[�l���g�̐e
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	protected:
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SpriteBaseDraw();
		void CalcSpriteCoordinate(Matrix4X4& Retmat);
	public:
		void SetSpriteCoordinate(SpriteCoordinate cood);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PCSpriteDraw : public SpriteBaseDraw;
	//	�p�r: PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCSpriteDraw : public SpriteBaseDraw {
		void CreateMeshResource();
	public:
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const Vector2& Size,const Color4& Col);
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const Vector2& Size, const vector<Color4>& ColorVec);
		virtual ~PCSpriteDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PTSpriteDraw : public SpriteBaseDraw;
	//	�p�r: Dx11PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public SpriteBaseDraw {
	public:
		explicit PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const Vector2& Size);
		virtual ~PTSpriteDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetTextureResource() const;
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		Color4 GetDiffuse() const;
		void SetDiffuse(const Color4& col);
		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PCTSpriteDraw : public SpriteBaseDraw;
	//	�p�r: Dx11PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCTSpriteDraw : public SpriteBaseDraw {
		void CreateMeshResource();
	public:
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const Vector2& Size, const Color4& Col);
		virtual ~PCTSpriteDraw();
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			const Vector2& Size, const vector<Color4>& ColorVec);
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetTextureResource() const;
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class PCTStaticDraw : public DrawComponent;
	//	�p�r: PCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCTStaticDraw : public DrawComponent {
	public:
		explicit PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PCTStaticDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		void SetMeshResource(const wstring& MeshKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetTextureResource() const;
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);

		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	�p�r: PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public DrawComponent {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		explicit PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTStaticDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		void SetMeshResource(const wstring& MeshKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		void SetTextureResource(const wstring& TextureKey);
		shared_ptr<TextureResource> GetTextureResource() const;
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		Color4 GetDiffuse() const;
		void SetDiffuse(const Color4& col);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PNTStaticModelDraw : public DrawComponent;
	//	�p�r: PNTStaticModelDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticModelDraw : public DrawComponent {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		explicit PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTStaticModelDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		void SetMeshResource(const wstring& MeshKey);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	struct	AnimationData;
	/*!
	�A�j���[�V�����f�[�^�\����.
	*/
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		//!	�X�^�[�g�T���v��
		UINT	m_StartSample;
		//!	�T���v���̒���
		UINT	m_SampleLength;
		//!	���[�v���邩�ǂ���
		bool	m_IsLoop;
		//!	�A�j�����I���������ǂ���
		bool	m_IsAnimeEnd;
		//!	1�b������̃t���[��
		float	m_SamplesParSecond;
		//--------------------------------------------------------------------------------------
		//	AnimationData();
		/*!
		@breaf �R���X�g���N�^.
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		//	AnimationData(
		//		UINT StartSample,	//�X�^�[�g�t���[��
		//		UINT SampleLength,	//�t���[���̒���
		//		bool bLoop,			//���[�v���邩�ǂ���
		//		float SamplesParSecond = 30.0f	//1�b������̃t���[����
		//	);
		/*!
		@breaf �R���X�g���N�^.
		@param UINT StartSample	�X�^�[�g�t���[��
		@param UINT SampleLength	�t���[���̒���
		@param bool bLoop	���[�v���邩�ǂ���
		@param float SamplesParSecond = 30.0f	1�b������̃t���[����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartSample, UINT SampleLength, bool bLoop,
			float SamplesParSecond = 30.0f) :
			m_StartSample{ StartSample },
			m_SampleLength{ SampleLength },
			m_IsLoop{ bLoop },
			m_IsAnimeEnd{ false },
			m_SamplesParSecond{ SamplesParSecond }
		{}
	};


	//--------------------------------------------------------------------------------------
	//	class PNTBoneModelDraw : public DrawComponent;
	//	�p�r: PNTBoneModelDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTBoneModelDraw : public DrawComponent {
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		explicit PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTBoneModelDraw();
		shared_ptr<MeshResource> GetMeshResource() const;
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		void SetMeshResource(const wstring& MeshKey);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		//��ԏ���
		void InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out);

		void AddAnimation(const wstring& Name,int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f);

		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f);
		const wstring& GetCurrentAnimation() const;

		bool UpdateAnimation(float ElapsedTime);

		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const;




		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class PNTCollisionDraw : public DrawComponent;
	//	�p�r: PNTCollision�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTCollisionDraw : public DrawComponent {
	public:
		explicit PNTCollisionDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTCollisionDraw();
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		Color4 GetDiffuse() const;
		void SetDiffuse(const Color4& col);

		//����
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//end basedx11
