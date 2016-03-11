
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

	//--------------------------------------------------------------------------------------
	//	class DrawComponent : public Component;
	//	用途: 描画コンポーネントの親クラス
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


		//各オブジェクトごとにボーンを所持しておくポインタ
		//シャドウマップから親クラスで参照できるように仮想関数にする
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const{ return nullptr; }
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
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


		//操作
		virtual void OnUpdate()override{}
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//座標系
	enum class SpriteCoordinate {
		m_CenterZeroPlusUpY,		//センター原点でY軸上向きプラス（デフォルト）
		m_LeftBottomZeroPlusUpY,	//左下原点でY軸上向きプラス
		m_LeftTopZeroPlusDownY,		//左上原点でY軸下向きプラス
	};


	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//	用途: Sprite描画コンポーネントの親
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	protected:
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SpriteBaseDraw();
		void CalcSpriteCoordinate(Matrix4X4& Retmat);
	public:
		void SetSpriteCoordinate(SpriteCoordinate cood);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
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
	//	class PCTStaticDraw : public DrawComponent;
	//	用途: PCTStatic描画コンポーネント
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
	//	class PNTStaticModelDraw : public DrawComponent;
	//	用途: PNTStaticModelDraw描画コンポーネント
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
	//	struct	AnimationData;
	/*!
	アニメーションデータ構造体.
	*/
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		//!	スタートサンプル
		UINT	m_StartSample;
		//!	サンプルの長さ
		UINT	m_SampleLength;
		//!	ループするかどうか
		bool	m_IsLoop;
		//!	アニメが終了したかどうか
		bool	m_IsAnimeEnd;
		//!	1秒当たりのフレーム
		float	m_SamplesParSecond;
		//--------------------------------------------------------------------------------------
		//	AnimationData();
		/*!
		@breaf コンストラクタ.
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		//	AnimationData(
		//		UINT StartSample,	//スタートフレーム
		//		UINT SampleLength,	//フレームの長さ
		//		bool bLoop,			//ループするかどうか
		//		float SamplesParSecond = 30.0f	//1秒あたりのフレーム数
		//	);
		/*!
		@breaf コンストラクタ.
		@param UINT StartSample	スタートフレーム
		@param UINT SampleLength	フレームの長さ
		@param bool bLoop	ループするかどうか
		@param float SamplesParSecond = 30.0f	1秒あたりのフレーム数
		@return	なし
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
	//	用途: PNTBoneModelDraw描画コンポーネント
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

		//補間処理
		void InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out);

		void AddAnimation(const wstring& Name,int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f);

		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f);
		const wstring& GetCurrentAnimation() const;

		bool UpdateAnimation(float ElapsedTime);

		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const;




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
