#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl{
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		Matrix4X4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix(){}
	};

	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent(){}


	BlendState DrawComponent::GetBlendState() const{
		return pImpl->m_BlendState;
	}
	DepthStencilState DrawComponent::GetDepthStencilState() const{
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const{
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const{
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state){
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDepthStencilState(const DepthStencilState state){
		pImpl->m_DepthStencilState = state;

	}
	void DrawComponent::SetRasterizerState(const RasterizerState state){
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetSamplerState(const SamplerState state){
		pImpl->m_SamplerState = state;
	}

	const Matrix4X4& DrawComponent::GetMeshToTransformMatrix() const{
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const Matrix4X4& Mat){
		pImpl->m_MeshToTransformMatrix = Mat;
	}








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


		Impl()
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

				Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();


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
					auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
					if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()){
						if (pLocalBoneVec){
							//ボーンの設定
							size_t BoneSz = pLocalBoneVec->size();
							UINT cb_count = 0;
							for (size_t b = 0; b < BoneSz; b++){
								Matrix4X4 mat = pLocalBoneVec->at(b);
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
	//	struct SpriteBaseDraw::Impl;
	//	用途: SpriteBaseDrawイディオム
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl{
		//座標系
		SpriteCoordinate m_Coordinate;
		Impl():
			m_Coordinate(SpriteCoordinate::m_CenterZeroPlusUpY)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//--------------------------------------------------------------------------------------

	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの２D 
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	SpriteBaseDraw::~SpriteBaseDraw(){}

	void SpriteBaseDraw::CalcSpriteCoordinate(Matrix4X4& Retmat) {
		//行列の取得
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();
		float ViewWidth = GetGameObject()->GetStage()->GetTargetViewPort().Width;
		float ViewHeight = GetGameObject()->GetStage()->GetTargetViewPort().Height;

		Retmat = World;
		switch (pImpl->m_Coordinate) {
		case SpriteCoordinate::m_CenterZeroPlusUpY:
			Retmat *= Matrix4X4EX::LookAtLH(Vector3(0, 0, 0.0f), Vector3(0, 0, 1.0f), Vector3(0, 1.0f, 0));
			Retmat *= Matrix4X4EX::OrthographicLH(ViewWidth, ViewHeight, 0, 1.0f);
			break;
		case SpriteCoordinate::m_LeftBottomZeroPlusUpY:
			Retmat *= Matrix4X4EX::LookAtLH(Vector3(0, 0, 0.0f), Vector3(0, 0, 1.0f), Vector3(0, 1.0f, 0));
			Retmat *= Matrix4X4EX::OrthographicOffCenterLH(0, ViewWidth, 0, ViewHeight, 0, 1.0f);
			break;
		case SpriteCoordinate::m_LeftTopZeroPlusDownY:
			Retmat._42 *= -1.0f;
			Retmat *= Matrix4X4EX::LookAtLH(Vector3(0, 0, 0.0f), Vector3(0, 0, 1.0f), Vector3(0, 1.0f, 0));
			Retmat *= Matrix4X4EX::OrthographicOffCenterLH(0, ViewWidth, -ViewHeight, 0, 0, 1.0f);
			break;

		}

	}

	void SpriteBaseDraw::SetSpriteCoordinate(SpriteCoordinate cood){
		pImpl->m_Coordinate = cood;
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
		Impl(const Vector2& Size,const Color4& Col):
			m_SpriteSize(Size),
			m_LeftTop(Col),
			m_RightTop(Col),
			m_LeftBottom(Col),
			m_RightBottom(Col),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_LeftTop(Color4(0,0,0,0)),
			m_RightTop(Color4(0, 0, 0, 0)),
			m_LeftBottom(Color4(0, 0, 0, 0)),
			m_RightBottom(Color4(0, 0, 0, 0)),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f)
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
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);
		pImpl->m_DrawContext->SetVertexShader<VSPCSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPCSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(RetWorld);
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
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_Emissive(0.0f,0.0f,0.0f,0.0f),
			m_Diffuse(1.0f,1.0f,1.0f,1.0f)
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
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		pImpl->m_DrawContext->SetVertexShader<VSPTSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPTSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(RetWorld);
		//コンスタントバッファの設定
		DiffuseSpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(スプライトは転置しない)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;

		auto PtrTextureResource = GetTextureResource();
		if (!PtrTextureResource){
			throw BaseException(
				L"スプライトにテクスチャが設定されていません",
				L"if (!PtrTextureResource)",
				L"PTSpriteDraw::OnDraw()"
				);

		}
		pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());

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
		Impl(const Vector2& Size, const Color4& Col) :
			m_SpriteSize(Size),
			m_LeftTop(Col),
			m_RightTop(Col),
			m_LeftBottom(Col),
			m_RightBottom(Col),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		Impl(const Vector2& Size) :
			m_SpriteSize(Size),
			m_LeftTop(Color4(0, 0, 0, 0)),
			m_RightTop(Color4(0, 0, 0, 0)),
			m_LeftBottom(Color4(0, 0, 0, 0)),
			m_RightBottom(Color4(0, 0, 0, 0)),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f)
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
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		pImpl->m_DrawContext->SetVertexShader<VSPCTSprite>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPCTSprite>(true);
		//行列の取得
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(RetWorld);
		//コンスタントバッファの設定
		SpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(スプライトは転置しない)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;

		auto PtrTextureResource = GetTextureResource();
		if (!PtrTextureResource){
			throw BaseException(
				L"スプライトにテクスチャが設定されていません",
				L"if (!PtrTextureResource)",
				L"PCTSpriteDraw::OnDraw()"
				);

		}

		pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());

		pImpl->m_DrawContext->DrawIndexed<CBSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionColorTexture), 0);
	}


	//--------------------------------------------------------------------------------------
	//	struct PCTStaticDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PCTStaticDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		weak_ptr<TextureResource> m_TextureResource; //テクスチャ
		Color4 m_Emissive;	//色
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class PCTStaticDraw : public DrawComponent;
	//	用途: PCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTStaticDraw::~PCTStaticDraw() {}

	void PCTStaticDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	shared_ptr<MeshResource> PCTStaticDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"PCTStaticDraw::GetMeshResource()"
				);
		}
		return pImpl->m_MeshResource.lock();
	}

	void PCTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PCTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PCTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}
	void PCTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}
	shared_ptr<TextureResource> PCTStaticDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		if (pImpl->m_TextureResource.expired()) {
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	Color4 PCTStaticDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void PCTStaticDraw::SetEmissive(const Color4& col) {
		pImpl->m_Emissive = col;
	}

	void PCTStaticDraw::OnDraw(){
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrTextureResource = GetTextureResource();
		if (!PtrTextureResource){
			throw BaseException(
				L"描画コンポーネントににテクスチャが設定されていません",
				L"if (!PtrTextureResource)",
				L"PCTStaticDraw::OnDraw()"
				);
		}

		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		pImpl->m_DrawContext->SetVertexShader<VSPCTStatic>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPCTStatic>(true);


		//ステージからカメラを取り出す
		auto PtrCamera = GetGameObject()->GetStage()->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = GetGameObject()->GetStage()->GetTargetLight(0);
		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PCTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.Emissive = pImpl->m_Emissive;

		pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());
		pImpl->m_DrawContext->DrawIndexed<CBPNTStatic>(GetGameObject(),
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
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

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
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

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
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
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
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 1.0f;
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

			pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());
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

		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		Desc.m_RasterizerState = GetRasterizerState();
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

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
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;

		if (PtrTextureResource){
			pImpl->m_DrawContext->AddSamplerAndSrv(0, GetSamplerState(), PtrTextureResource->GetShaderResourceView().Get());
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
	//	struct PNTStaticModelDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTStaticModelDraw::Impl {
		//描画コンテキスト
		shared_ptr<DrawContext> m_DrawContext;
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTStaticModelDraw : public DrawComponent;
	//	用途: PNTStaticModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticModelDraw::PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticModelDraw::~PNTStaticModelDraw() {}

	void PNTStaticModelDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	shared_ptr<MeshResource> PNTStaticModelDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"PNTStaticModelDraw::GetMeshResource()"
				);
		}
		return pImpl->m_MeshResource.lock();
	}

	void PNTStaticModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticModelDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}

	bool PNTStaticModelDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticModelDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticModelDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	void PNTStaticModelDraw::DrawWithShadow(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);
		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrGameObject->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		Vector3 CalcLightDir = -1.0 * PtrLight->GetDirectional();
		Vector3 LightAt = PtrCamera->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 1.0f;
		//モデルはテクスチャ必須
		cb1.ActiveFlg.x = 1;

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		bool IsShadowmap = false;
		if (PtrGameObject->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pID3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);

			IsShadowmap = true;
		}
		else {
			pID3D11DeviceContext->VSSetShader(VSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
		}

		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTStaticShadow::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//サンプラー

		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//影のシェーダリソース
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		pSampler = RenderStatePtr->GetComparisonLinear();
		//影のサンプラー
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pSampler);

		for (auto& m : MatVec){
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTStaticShadow::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe){
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else{
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (PtrGameObject->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates(RenderStatePtr);
	}

	void PNTStaticModelDraw::DrawNotShadow(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);
		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrGameObject->GetComponent<TransformMatrix>()->GetWorldMatrix();
		//コンスタントバッファの設定
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		for (auto& m : MatVec){
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe){
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else{
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (PtrGameObject->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates(RenderStatePtr);
	}
	void PNTStaticModelDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct PNTBoneModelDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PNTBoneModelDraw::Impl {
		weak_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		bool m_OwnShadowActive;
		//シェーダに渡すボーン行列
		vector<Matrix4X4> m_LocalBonesMatrix;
		map<wstring, AnimationData> m_AnimationMap;
		wstring m_CurrentAnimeName;
		float m_CurrentAnimeTime;
		Impl() :
			m_OwnShadowActive(false),
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTBoneModelDraw : public DrawComponent;
	//	用途: PNTBoneModelDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTBoneModelDraw::PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTBoneModelDraw::~PNTBoneModelDraw() {}

	void PNTBoneModelDraw::OnCreate() {

	}

	shared_ptr<MeshResource> PNTBoneModelDraw::GetMeshResource() const {
		//メッシュがなければリターン
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"メッシュが設定されてません",
				L"if (pImpl->m_MeshResource.expired())",
				L"PNTStaticModelDraw::GetMeshResource()"
				);
		}
		return pImpl->m_MeshResource.lock();
	}

	void PNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0){
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"PNTBoneModelDraw::SetMeshResource()"
				);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++){
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(MeshKey);
		if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0){
			throw BaseException(
				L"メッシュがボーンメッシュではありません",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"PNTBoneModelDraw::SetMeshResource()"
				);

		}
		//先頭のボーン数の行列で初期化
		pImpl->m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
		auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
		for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++){
			pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[i];
		}
		pImpl->m_MeshResource = MeshRes;
	}

	bool PNTBoneModelDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTBoneModelDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTBoneModelDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}

	const vector< Matrix4X4 >* PNTBoneModelDraw::GetVecLocalBonesPtr() const{
		return &pImpl->m_LocalBonesMatrix;
	}


	void PNTBoneModelDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond){
		if (Name == L""){
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (Name == L\"\")",
				L"PNTBoneModelDraw::AddAnimation()"
				);
		}
		if (StartSample < 0 || SampleLength < 0){
			throw BaseException(
				L"開始サンプルかサンプル数が0未満です",
				L"if (StartSample < 0 || SampleLength < 0)",
				L"PNTBoneModelDraw::AddAnimation()"
				);
		}
		if (SamplesParSecond <= 0.0f){
			throw BaseException(
				L"サンプル毎秒が0以下です",
				L"if (SamplesParSecond <= 0.0f)",
				L"PNTBoneModelDraw::AddAnimation()"
				);
		}
		//重複キーがあれば差し替える
		AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
		pImpl->m_AnimationMap[Name] = Data;
		if (pImpl->m_AnimationMap.size() == 1){
			//1つしか登録がなかったら、カレントアニメは該当アニメとなる
			ChangeCurrentAnimation(Name, 0);
		}


	}

	void PNTBoneModelDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime){
		if (AnemationName == L""){
			throw BaseException(
				L"アニメーション名が空白です",
				L"if (AnemationName == L\"\")",
				L"PNTBoneModelDraw::SetCurrentAnimation()"
				);
		}
		auto it = pImpl->m_AnimationMap.find(AnemationName);
		if (it != pImpl->m_AnimationMap.end()) {
			//指定の名前が見つかった
			pImpl->m_CurrentAnimeName = AnemationName;
			pImpl->m_CurrentAnimeTime = StartTime;
			//アニメーションは終了していない
			it->second.m_IsAnimeEnd = false;
		}
		else{
			//見つからない
			throw BaseException(
				L"指定のアニメーションは登録されてません",
				AnemationName,
				L"PNTBoneModelDraw::SetCurrentAnimation()"
				);
		}
	}

	const wstring& PNTBoneModelDraw::GetCurrentAnimation() const{
		return pImpl->m_CurrentAnimeName;
	}

	bool PNTBoneModelDraw::UpdateAnimation(float ElapsedTime){
		if (ElapsedTime < 0.0f){
			throw BaseException(
				L"アニメーション更新にマイナスは設定できません",
				L"if (ElapsedTime < 0.0f)",
				L"PNTBoneModelDraw::UpdateAnimation()"
				);
		}
		if (pImpl->m_CurrentAnimeName == L""){
			//見つからない
			throw BaseException(
				L"カレントアニメーションが設定されてません",
				L"if (pImpl->m_CurrentAnimeName == L\"\")",
				L"PNTBoneModelDraw::UpdateAnimation()"
				);
		}
		auto PtrMesh = GetMeshResource();
		UINT SampleCount = PtrMesh->GetSampleCount();
		auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
		UINT BoneCount = PtrMesh->GetBoneCount();
		auto& TgtAnimeData = pImpl->m_AnimationMap[pImpl->m_CurrentAnimeName];
		if (TgtAnimeData.m_StartSample >= SampleCount){
			//スタートのサンプルが最後のサンプル以降だった
			TgtAnimeData.m_StartSample = SampleCount - 1;
			TgtAnimeData.m_SampleLength = 0;
			UINT UITgtSample = TgtAnimeData.m_StartSample;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++){
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			pImpl->m_CurrentAnimeTime = 0;
			if (TgtAnimeData.m_IsLoop){
				TgtAnimeData.m_IsAnimeEnd = false;
				return false;
			}
			else{
				TgtAnimeData.m_IsAnimeEnd = true;
				return true;
			}
		}
		//すでにアニメが終了している
		if (TgtAnimeData.m_IsAnimeEnd){
			//現在のローカル行列を使用
			return true;
		}
		//カレントタイムを更新
		pImpl->m_CurrentAnimeTime += ElapsedTime;
		//スタート位置を計算
		auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample +  pImpl->m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
		UINT UITgtSample = (UINT)FLOATTgtSample;
		UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
		if (UILastSample >= SampleCount){
			UILastSample = SampleCount - 1;
		}
		if (UITgtSample >= UILastSample){
			UITgtSample = UILastSample - 1;
			//最後のサンプルを表示
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++){
				pImpl->m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
			}
			if (TgtAnimeData.m_IsLoop){
				TgtAnimeData.m_IsAnimeEnd = false;
				//ループするのでカレントタイムを0にする
				pImpl->m_CurrentAnimeTime = 0;
				return false;
			}
			else{
				pImpl->m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
				TgtAnimeData.m_IsAnimeEnd = true;
				return true;
			}
		}
		else{
			//サンプルとサンプルの間の割合を計算
			FLOATTgtSample -= (float)UITgtSample;
			UINT UINextSample = UITgtSample + 1;
			for (UINT i = 0; i < pImpl->m_LocalBonesMatrix.size(); i++){
				InterpolationMatrix(
					SampleMatrixVec[BoneCount * UITgtSample + i],
					SampleMatrixVec[BoneCount * UINextSample + i],
					FLOATTgtSample, pImpl->m_LocalBonesMatrix[i]);
			}
			//アニメは終わってない
			return false;
		}
	}


	void PNTBoneModelDraw::InterpolationMatrix(const Matrix4X4& m1, const Matrix4X4& m2, float t, Matrix4X4& out){
		Vector3 Scale1, Pos1;
		Quaternion Qt1;
		m1.Decompose(Scale1, Qt1, Pos1);
		Qt1.Normalize();

		Vector3 Scale2, Pos2;
		Quaternion Qt2;

		m2.Decompose(Scale2, Qt2, Pos2);
		Qt2.Normalize();

		Vector3 ScaleOut, PosOut;
		Quaternion QtOut;

		ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
		PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
		QtOut = QuaternionEX::Slerp(Qt1, Qt2, t);
		out.DefTransformation(ScaleOut, QtOut, PosOut);
	}


	void PNTBoneModelDraw::DrawWithShadow(){
		
		auto PtrGameObject = GetGameObject();
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);
		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrGameObject->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//コンスタントバッファの設定
		PNTBoneShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		Vector3 CalcLightDir = -1.0 * PtrLight->GetDirectional();
		Vector3 LightAt = PtrCamera->GetAt();
		Vector3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		cb1.LightPos = LightEye;
		cb1.LightPos.w = 1.0f;

		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 1.0f;
		//モデルはテクスチャ必須
		cb1.ActiveFlg.x = 1;

		Matrix4X4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//ボーンの設定
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++){
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


		//シャドウマップのリソースビューを取得
		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		bool IsShadowmap = false;
		if (PtrGameObject->GetComponent<Shadowmap>(false)) {
			//シャドウマップがあれば自己影防止用のピクセルシェーダ
			pID3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow2::GetPtr()->GetShader(), nullptr, 0);

			IsShadowmap = true;
		}
		else {
			pID3D11DeviceContext->VSSetShader(VSPNTBoneShadow::GetPtr()->GetShader(), nullptr, 0);
			pID3D11DeviceContext->PSSetShader(PSPNTStaticShadow::GetPtr()->GetShader(), nullptr, 0);
		}

		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTBoneShadow::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//サンプラー

		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//影のシェーダリソース
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		pSampler = RenderStatePtr->GetComparisonLinear();
		//影のサンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pSampler);

		for (auto& m : MatVec){
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTBoneShadow::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe){
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else{
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (PtrGameObject->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates(RenderStatePtr);
		
	}

	void PNTBoneModelDraw::DrawNotShadow(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage){
			return;
		}
		//ステージからカメラを取り出す
		auto PtrCamera = PtrStage->GetTargetCamera();
		//ステージからライトを取り出す
		auto PtrLight = PtrStage->GetTargetLight(0);
		//行列の取得
		Matrix4X4 RealWorldMatrix = GetMeshToTransformMatrix() * PtrGameObject->GetComponent<TransformMatrix>()->GetWorldMatrix();
		//コンスタントバッファの設定
		PNTBoneConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		//コンスタントバッファの設定
		cb1.World = Matrix4X4EX::Transpose(RealWorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(PtrCamera->GetViewMatrix());
		cb1.Projection = Matrix4X4EX::Transpose(PtrCamera->GetProjMatrix());
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//ボーンの設定
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		UINT cb_count = 0;
		for (size_t b = 0; b < BoneSz; b++){
			Matrix4X4 mat = pImpl->m_LocalBonesMatrix[b];
			mat.Transpose();
			cb1.Bones[cb_count] = ((XMMATRIX)mat).r[0];
			cb1.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
			cb1.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
			cb_count += 3;
		}


		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = PtrStage->GetRenderState();

		auto MeshPtr = GetMeshResource();
		auto& MatVec = MeshPtr->GetMaterialExVec();

		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSPNTBone::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPNTBone::GetPtr()->GetInputLayout());

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTextureSkinning);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, MeshPtr->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshPtr->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//デプスステンシル
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}
		//サンプラー
		//nullに初期化
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//サンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		for (auto& m : MatVec){
			cb1.Emissive = m.m_Emissive;
			cb1.Diffuse = m.m_Diffuse;
			//コンスタントバッファの更新
			ID3D11Buffer* pConstantBuffer = CBPNTBone::GetPtr()->GetBuffer();
			pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb1, 0, 0);
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
			if (GetRasterizerState() == RasterizerState::Wireframe){
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetWireframe());
				//描画
				pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else{
				//ブレンドステートとラスタライザを設定して描画
				//もし、透明描画ならAlphaBlendExに設定し、そうでなければ、指定に従う。
				if (PtrGameObject->GetAlphaActive()) {
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					//ラスタライザステート
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					switch (GetBlendState()) {
					case BlendState::Opaque:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						break;
					case BlendState::AlphaBlend:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						break;
					case BlendState::Additive:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
						break;
					case BlendState::NonPremultiplied:
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
						break;
					}
					switch (GetRasterizerState()) {
					case RasterizerState::CullBack:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						break;
					case RasterizerState::CullFront:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						break;
					case RasterizerState::CullNone:
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
						break;
					}
					//描画
					pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
			}
		}
		Dev->InitializeStates(RenderStatePtr);
	}
	void PNTBoneModelDraw::OnDraw() {
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
		pImpl(new Impl()) 
	{
		//パイプラインステートをワイアフレーム
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::Wireframe);
	}

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
		PipeLineDesc Desc;
		Desc.m_BlendState = GetBlendState();
		Desc.m_DepthStencilState = GetDepthStencilState();
		//ワイアフレーム
		Desc.m_RasterizerState = RasterizerState::Wireframe;
		pImpl->m_DrawContext->SetPipeLineDesc(Desc);

		pImpl->m_DrawContext->SetVertexShader<VSPNTStatic>(true);
		pImpl->m_DrawContext->SetPixelShader<PSPNTStaticNoTexture>(true);

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
