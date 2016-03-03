
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

	struct	Bone;

	//--------------------------------------------------------------------------------------
	//	class DrawComponent : public Component;
	//	�p�r: �`��R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component{
	protected:
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr){}
		virtual ~DrawComponent(){}
	public:
		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		//�V���h�E�}�b�v����e�N���X�ŎQ�Ƃł���悤�ɉ��z�֐��ɂ���
		virtual const vector< Bone >* GetVecLocalBonesPtr() const{ return nullptr; }
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

		const Matrix4X4& GetMeshToTransform() const;
		void SetMeshToTransform(const Matrix4X4& Mat);

		//����
		virtual void OnUpdate()override{}
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//	�p�r: Sprite�`��R���|�[�l���g�̐e
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	public:
		//���W�n
		enum class Coordinate {
			m_CenterZeroPlusUpY,		//�Z���^�[���_��Y��������v���X�i�f�t�H���g�j
			m_LeftBottomZeroPlusUpY,	//�������_��Y��������v���X
		};
	protected:
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr):
			DrawComponent(GameObjectPtr) {}
		virtual ~SpriteBaseDraw() {}
		void CalcSpriteCoordinate(Coordinate cood,Matrix4X4& Retmat);
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
