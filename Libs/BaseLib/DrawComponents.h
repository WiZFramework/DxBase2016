
/*!
@file DrawComponents.h
@brief コンポーネント（描画系）

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
	//	用途: 描画コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component{
	protected:
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr){}
		virtual ~DrawComponent(){}
	public:
		//各オブジェクトごとにボーンを所持しておくポインタ
		//シャドウマップから親クラスで参照できるように仮想関数にする
		virtual const vector< Bone >* GetVecLocalBonesPtr() const{ return nullptr; }
	};

	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public DrawComponent;
	//	用途: シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	class Shadowmap : public DrawComponent{
	public:
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Shadowmap();
		//アクセサ
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

		//操作
		virtual void OnUpdate()override{}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//	用途: Sprite描画コンポーネントの親
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	public:
		//座標系
		enum class Coordinate {
			m_CenterZeroPlusUpY,		//センター原点でY軸上向きプラス（デフォルト）
			m_LeftBottomZeroPlusUpY,	//左下原点でY軸上向きプラス
		};
	protected:
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr):
			DrawComponent(GameObjectPtr) {}
		virtual ~SpriteBaseDraw() {}
		void CalcSpriteCoordinate(Coordinate cood,Matrix4X4& Retmat);
	};

	//--------------------------------------------------------------------------------------
	//	class PCSpriteDraw : public SpriteBaseDraw;
	//	用途: PCSprite描画コンポーネント
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
		//操作
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PTSpriteDraw : public SpriteBaseDraw;
	//	用途: Dx11PTSprite描画コンポーネント
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
		//操作
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PCTSpriteDraw : public SpriteBaseDraw;
	//	用途: Dx11PCTSprite描画コンポーネント
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
		//操作
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	用途: PNTStatic描画コンポーネント
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

		//操作
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class PNTCollisionDraw : public DrawComponent;
	//	用途: PNTCollision描画コンポーネント
	//--------------------------------------------------------------------------------------
	class PNTCollisionDraw : public DrawComponent {
	public:
		explicit PNTCollisionDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~PNTCollisionDraw();
		Color4 GetEmissive() const;
		void SetEmissive(const Color4& col);
		Color4 GetDiffuse() const;
		void SetDiffuse(const Color4& col);

		//操作
		virtual void OnCreate()override;
		virtual void OnUpdate()override {}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//end basedx11
