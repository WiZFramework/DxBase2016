#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl{
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース

		Matrix4X4 m_MeshToTransform;

		Impl():
			m_MeshToTransform()
		{}
		~Impl(){}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);


	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	用途: シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap(){}

	float Shadowmap::GetLightHeight(){ return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear(){ return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar(){ return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth(){ return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight(){ return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f){ Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f){ Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f){ Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f){ Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f){ Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f){ Impl::m_ViewWidth = Impl::m_ViewHeight = f; }



	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"メッシュリソースが見つかりません",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"メッシュキーが空白です",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	const Matrix4X4& Shadowmap::GetMeshToTransform() const{
		return pImpl->m_MeshToTransform;
	}
	void Shadowmap::SetMeshToTransform(const Matrix4X4& Mat){
		pImpl->m_MeshToTransform = Mat;
	}



	void Shadowmap::OnDraw(){
		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<TransformMatrix>();
			//メッシュリソースの取得
			auto PtrMeshResource = GetMeshResource();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrMeshResource && PtrLight && PtrCamera){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();


				//ライトの取得
				Matrix4X4 LightView, LightProj;

				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightAt + (LightDir * GetLightHeight());

				//ライトのビューと射影を計算
				LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
				LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

				ShadowConstantBuffer Cb;

				Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				bool IsSkin = false;
				bool IsSkinStride = false;
				if (PtrMeshResource->IsSkining()){
					/*
					auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
					if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()){
						if (pLocalBoneVec){
							//ボーンの設定
							size_t BoneSz = pLocalBoneVec->size();
							UINT cb_count = 0;
							for (size_t b = 0; b < BoneSz; b++){
								Matrix4X4 mat = (*pLocalBoneVec)[b].m_ConbinedPose;
								mat.Transpose();
								Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
								Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
								Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
								cb_count += 3;
							}
							IsSkin = true;
						}
					}
					IsSkinStride = true;
					*/
				}


				//これより描画処理
				//コンスタントバッファの更新
				pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				if (IsSkin){
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
					//ストライドとオフセット
					UINT stride = sizeof(VertexPositionNormalTextureSkinning);
					UINT offset = 0;
					//頂点バッファをセット
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
					//ストライドとオフセット
					UINT stride = sizeof(VertexPositionNormalTexture);
					if (IsSkinStride){
						stride = sizeof(VertexPositionNormalTextureSkinning);
					}
					UINT offset = 0;
					//頂点バッファをセット
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				}



				//インデックスバッファのセット
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//ピクセルシェーダはセットしない！
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//ジオメトリシェーダの設定（使用しない）
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//コンスタントバッファをピクセルシェーダにセット
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//描画
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}

	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//--------------------------------------------------------------------------------------
	void SpriteBaseDraw::CalcSpriteCoordinate(Coordinate cood, Matrix4X4& Retmat) {
		//行列の取得
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();
		float ViewWidth = GetGameObject()->GetStage()->GetTargetViewPort().Width;
		float ViewHeight = GetGameObject()->GetStage()->GetTargetViewPort().Height;

		Retmat = World;
		Retmat *= Matrix4X4EX::LookAtLH(Vector3(0,0,0.0f), Vector3(0, 0, 1.0f), Vector3(0, 1.0f, 0));
		switch (cood) {
		case Coordinate::m_CenterZeroPlusUpY:
			Retmat *= Matrix4X4EX::OrthographicLH(ViewWidth, ViewHeight, 0, 1.0f);
			break;
		case Coordinate::m_LeftBottomZeroPlusUpY:
			Retmat *= Matrix4X4EX::OrthographicOffCenterLH(0, ViewWidth,0, ViewHeight,0,1.0f);
			break;
		}

	}


	//--------------------------------------------------------------------------------------
	//	struct PCSpriteDraw::Impl;
	//	用途: PCSpriteDrawイディオム
	//--------------------------------------------------------------------------------------
	struct PCSpriteDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		Vector2 m_SpriteSize;			//スプライトのサイズ（ピクセル）
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;
		Color4 m_Emissive;	//色
		//座標系
		SpriteBaseDraw::Coordinate m_Coordinate;
		Impl(const Vector2& Size,const Color4& Col):
			m_SpriteSize(Size),
			m_LeftTop(Col),
			m_RightTop(Col),
			m_LeftBottom(Col),
			m_RightBottom(Col),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Coordinate(SpriteBaseDraw::Coordinate::m_CenterZeroPlusUpY)
		{}
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_LeftTop(Color4(0,0,0,0)),
			m_RightTop(Color4(0, 0, 0, 0)),
			m_LeftBottom(Color4(0, 0, 0, 0)),
			m_RightBottom(Color4(0, 0, 0, 0)),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Coordinate(SpriteBaseDraw::Coordinate::m_CenterZeroPlusUpY)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PCSpriteDraw : public DrawComponent;
	//	用途: Dx11PCSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	void PCSpriteDraw::CreateMeshResource() {
		Vector2 HalfSize(pImpl->m_SpriteSize.x / 2.0f, pImpl->m_SpriteSize.y / 2.0f);
		//頂点を作成する配列
		vector<VertexPositionColor> verteces = {
			{ Vector3(-HalfSize.x, HalfSize.y, 0) , pImpl->m_LeftTop },
			{ Vector3(HalfSize.x, HalfSize.y, 0.0f), pImpl->m_RightTop },
			{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_LeftBottom },
			{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_RightBottom }
		};
		//インデックスを作成するための配列
		vector<uint16_t> indices = {
			(uint16_t)0,
			(uint16_t)1,
			(uint16_t)2,
			(uint16_t)1,
			(uint16_t)3,
			(uint16_t)2
		};
		pImpl->m_MeshResource = MeshResource::CreateMeshResource(verteces, indices, true);

	}

	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size, const Color4& Col) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size, Col)) 
	{
		try {
			CreateMeshResource();
		}
		catch (...) {
			throw;
		}
	}
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size, const vector<Color4>& ColorVec) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size))
	{
		try {
			if (ColorVec.size() < 4) {
				throw BaseException(
					L"色配列が4つ未満です",
					L"if (ColorVec.size() < 4)",
					L"PCSpriteDraw::PCSpriteDraw()"
					);
			}
			pImpl->m_LeftTop = ColorVec[0];
			pImpl->m_RightTop = ColorVec[1];
			pImpl->m_LeftBottom = ColorVec[2];
			pImpl->m_RightBottom = ColorVec[3];
			CreateMeshResource();
		}
		catch (...) {
			throw;
		}
	}


	PCSpriteDraw::~PCSpriteDraw() {}

	shared_ptr<MeshResource> PCSpriteDraw::GetMeshResource() const {
		return pImpl->m_MeshResource;
	}

	Color4 PCSpriteDraw::GetEmissive() const {
		return pImpl->m_Emissive;

	}
	void PCSpriteDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}


	void PCSpriteDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	void PCSpriteDraw::OnDraw() {
		pImpl->m_DrawContext->SetDefault2DPipeline();
		pImpl->m_DrawContext->SetVertexShader<VSPCSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPCSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate,RetWorld);
		//コンスタントバッファの設定
		SpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//コンスタントバッファの設定
		//行列の設定(スプライトは転置しない)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		pImpl->m_DrawContext->DrawIndexed<CBSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionColor), 0);
	}


	//--------------------------------------------------------------------------------------
	//	struct PTSpriteDraw::Impl;
	//	用途: PTSpriteDrawイディオム
	//--------------------------------------------------------------------------------------
	struct PTSpriteDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		//テクスチャ
		weak_ptr<TextureResource> m_TextureResource;
		Vector2 m_SpriteSize;			//スプライトのサイズ（ピクセル）
		Color4 m_Emissive;	//色
		Color4 m_Diffuse;	//デフィーズライト
		//座標系
		SpriteBaseDraw::Coordinate m_Coordinate;
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_Emissive(0.0f,0.0f,0.0f,0.0f),
			m_Diffuse(1.0f,1.0f,1.0f,1.0f),
			m_Coordinate(SpriteBaseDraw::Coordinate::m_CenterZeroPlusUpY)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PTSpriteDraw : public SpriteBaseDraw;
	//	用途: Dx11PTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size))
	{
		try {
			Vector2 HalfSize(Size.x / 2.0f, Size.y / 2.0f);
			//頂点を作成する配列
			vector<VertexPositionTexture> verteces = {
				{ Vector3(-HalfSize.x, HalfSize.y, 0) , Vector2(0.0f, 0.0f) },
				{ Vector3(HalfSize.x, HalfSize.y, 0.0f), Vector2(1.0f, 0.0f) },
				{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), Vector2(0.0f, 1.0f) },
				{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), Vector2(1.0f, 1.0f) }
			};
			//インデックスを作成するための配列
			vector<uint16_t> indices = {
				(uint16_t)0,
				(uint16_t)1,
				(uint16_t)2,
				(uint16_t)1,
				(uint16_t)3,
				(uint16_t)2
			};
			pImpl->m_MeshResource = MeshResource::CreateMeshResource(verteces, indices, true);
		}
		catch (...) {
			throw;
		}
	}
	PTSpriteDraw::~PTSpriteDraw() {}

	shared_ptr<MeshResource> PTSpriteDraw::GetMeshResource() const {
		return pImpl->m_MeshResource;
	}

	void PTSpriteDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}
	void PTSpriteDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}

	shared_ptr<TextureResource> PTSpriteDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		if (pImpl->m_TextureResource.expired()) {
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	Color4 PTSpriteDraw::GetEmissive() const {
		return pImpl->m_Emissive;

	}
	void PTSpriteDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	Color4 PTSpriteDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void PTSpriteDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}

	void PTSpriteDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}
	void PTSpriteDraw::OnDraw() {
		pImpl->m_DrawContext->SetDefault2DPipeline();
		pImpl->m_DrawContext->SetVertexShader<VSPTSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPTSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate, RetWorld);
		//コンスタントバッファの設定
		DiffuseSpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(スプライトは転置しない)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;
		pImpl->m_DrawContext->DrawIndexed<CBDiffuseSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionTexture), 0);
	}

	//--------------------------------------------------------------------------------------
	//	struct PCTSpriteDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PCTSpriteDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		Vector2 m_SpriteSize;			//スプライトのサイズ（ピクセル）
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;
		//テクスチャ
		weak_ptr<TextureResource> m_TextureResource;
		Color4 m_Emissive;	//色
		SpriteBaseDraw::Coordinate m_Coordinate;	//座標系
		Impl(const Vector2& Size, const Color4& Col) :
			m_SpriteSize(Size),
			m_LeftTop(Col),
			m_RightTop(Col),
			m_LeftBottom(Col),
			m_RightBottom(Col),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Coordinate(SpriteBaseDraw::Coordinate::m_CenterZeroPlusUpY)
		{}
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_LeftTop(Color4(0, 0, 0, 0)),
			m_RightTop(Color4(0, 0, 0, 0)),
			m_LeftBottom(Color4(0, 0, 0, 0)),
			m_RightBottom(Color4(0, 0, 0, 0)),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Coordinate(SpriteBaseDraw::Coordinate::m_CenterZeroPlusUpY)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class PCTSpriteDraw : public SpriteBaseDraw;
	//	用途: Dx11PCTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	void PCTSpriteDraw::CreateMeshResource() {
		Vector2 HalfSize(pImpl->m_SpriteSize.x / 2.0f, pImpl->m_SpriteSize.y / 2.0f);
		//頂点を作成する配列
		vector<VertexPositionColorTexture> verteces = {
			{ Vector3(-HalfSize.x, HalfSize.y, 0) , pImpl->m_LeftTop, Vector2(0.0f, 0.0f) },
			{ Vector3(HalfSize.x, HalfSize.y, 0.0f), pImpl->m_RightTop, Vector2(1.0f, 0.0f) },
			{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_LeftBottom, Vector2(0.0f, 1.0f) },
			{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_RightBottom, Vector2(1.0f, 1.0f) }
		};
		//インデックスを作成するための配列
		vector<uint16_t> indices = {
			(uint16_t)0,
			(uint16_t)1,
			(uint16_t)2,
			(uint16_t)1,
			(uint16_t)3,
			(uint16_t)2
		};
		pImpl->m_MeshResource = MeshResource::CreateMeshResource(verteces, indices, true);

	}


	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size, const Color4& Col) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size, Col))
	{
		try {
			CreateMeshResource();
		}
		catch (...) {
			throw;
		}
	}

	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size, const vector<Color4>& ColorVec):
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size))
	{
		try {
			if (ColorVec.size() < 4) {
				throw BaseException(
					L"色配列が4つ未満です",
					L"if (ColorVec.size() < 4)",
					L"PCTSpriteDraw::PCTSpriteDraw()"
					);
			}
			pImpl->m_LeftTop = ColorVec[0];
			pImpl->m_RightTop = ColorVec[1];
			pImpl->m_LeftBottom = ColorVec[2];
			pImpl->m_RightBottom = ColorVec[3];
			CreateMeshResource();
		}
		catch (...) {
			throw;
		}
	}

	PCTSpriteDraw::~PCTSpriteDraw() {}

	shared_ptr<MeshResource> PCTSpriteDraw::GetMeshResource() const {
		return pImpl->m_MeshResource;
	}

	void PCTSpriteDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}

	void PCTSpriteDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}

	shared_ptr<TextureResource> PCTSpriteDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		if (pImpl->m_TextureResource.expired()) {
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}


	Color4 PCTSpriteDraw::GetEmissive() const {
		return pImpl->m_Emissive;

	}
	void PCTSpriteDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}


	void PCTSpriteDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}
	void PCTSpriteDraw::OnDraw() {
		pImpl->m_DrawContext->SetDefault2DPipeline();
		pImpl->m_DrawContext->SetVertexShader<VSPCTSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPCTSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate, RetWorld);
		//コンスタントバッファの設定
		SpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(スプライトは転置しない)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		pImpl->m_DrawContext->DrawIndexed<CBSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionColorTexture), 0);
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<TextureResource> m_TextureResource; //テクスチャ
		Color4 m_Emissive;	//色
		Color4 m_Diffuse;	//デフィーズライト
		bool m_OwnShadowActive;
		Impl():
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Diffuse(0.7f, 0.7f, 0.7f, 1.0f),
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	用途: PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"PNTStaticDraw::GetMeshResource()"
				);
		}
		return pImpl->m_MeshResource.lock();
	}

	void PNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}
	void PNTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		if (pImpl->m_TextureResource.expired()) {
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	Color4 PNTStaticDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void PNTStaticDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	Color4 PNTStaticDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void PNTStaticDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}

	bool PNTStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}


	void PNTStaticDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrTextureResource = GetTextureResource();
		pImpl->m_DrawContext->SetDefault3DPipeline();
		bool IsShadowmap = false;
		if (GetGameObject()->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow2>(true);
			IsShadowmap = true;
		}
		else {
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow>(true);
		}

		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);
		//行列の取得
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(World);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;

		Vector3 CalcLightDir = -1.0 * PtrLight->GetDirectional();
		Vector3 LightAt = PtrCamera->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 0.0f;

		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 0.0f;
		if (PtrTextureResource){
			cb1.ActiveFlg.x = 1;
		}
		else{
			cb1.ActiveFlg.x = 0;
		}


		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		if (PtrTextureResource){
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::LinearClamp, PtrTextureResource->GetShaderResourceView().Get());
		}
		else{
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::SamplerNone, nullptr);
		}
		//自己影
		pImpl->m_DrawContext->AddSamplerAndSrv(1, SamplerState::ComparisonLinear, ShadoumapPtr->GetShaderResourceView());
		pImpl->m_DrawContext->DrawIndexed<CBPNTStaticShadow>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionNormalTexture), 0);

	}
	void PNTStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}

		auto PtrTextureResource = GetTextureResource();

		pImpl->m_DrawContext->SetDefault3DPipeline();
		pImpl->m_DrawContext->SetVertexShader<VSPNTStatic>(true);
		if (PtrTextureResource){
			pImpl->m_DrawContext->SetPixelShader<PSPNTStatic>(true);
		}
		else{
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticNoTexture>(true);
		}


		//ステージからカメラを取り出す
		auto PtrCamera = GetGameObject()->GetStage()->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = GetGameObject()->GetStage()->GetTargetLight(0);
		//行列の取得
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(World);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;

		if (PtrTextureResource){
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::LinearClamp, PtrTextureResource->GetShaderResourceView().Get());
		}
		else{
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::SamplerNone, nullptr);
		}
		pImpl->m_DrawContext->DrawIndexed<CBPNTStatic>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionNormalTexture), 0);
	}



	void PNTStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTCollisionDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTCollisionDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		Color4 m_Emissive;	//色
		Color4 m_Diffuse;	//デフィーズライト
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTCollisionDraw : public DrawComponent;
	//	用途: PNTCollision描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTCollisionDraw::PNTCollisionDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {}

	PNTCollisionDraw::~PNTCollisionDraw() {}

	void PNTCollisionDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	Color4 PNTCollisionDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void PNTCollisionDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}


	Color4 PNTCollisionDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void PNTCollisionDraw::SetDiffuse(const Color4& col) {
		pImpl->m_Diffuse = col;
	}

	void PNTCollisionDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		pImpl->m_DrawContext->SetDefault3DPipeline();
		pImpl->m_DrawContext->SetVertexShader<VSPNTStatic>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPNTStaticNoTexture>(true);
		//ワイアフレーム
		pImpl->m_DrawContext->SetRasterizerState(RasterizerState::Wireframe);

		auto PtrColl = GetGameObject()->GetComponent<Collision>();
		auto PtrMesh = PtrColl->GetMeshResource();

		//ステージからカメラを取り出す
		auto PtrCamera = GetGameObject()->GetStage()->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = GetGameObject()->GetStage()->GetTargetLight(0);
		//行列の取得
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(World);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;

		pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::SamplerNone, nullptr);
		pImpl->m_DrawContext->DrawIndexed<CBPNTStatic>(GetGameObject(),
			&cb1, PtrMesh, sizeof(VertexPositionNormalTexture), 0);

	}





}

//end basedx11
