#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl{
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X

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
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
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
					L"���b�V�����\�[�X��������܂���",
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
					L"���b�V���L�[���󔒂ł�",
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
		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<TransformMatrix>();
			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetMeshResource();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
			auto PtrLight = PtrStage->GetTargetLight(0);
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrMeshResource && PtrLight && PtrCamera){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();


				//���C�g�̎擾
				Matrix4X4 LightView, LightProj;

				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightAt + (LightDir * GetLightHeight());

				//���C�g�̃r���[�Ǝˉe���v�Z
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
							//�{�[���̐ݒ�
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


				//������`�揈��
				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				if (IsSkin){
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
					//�X�g���C�h�ƃI�t�Z�b�g
					UINT stride = sizeof(VertexPositionNormalTextureSkinning);
					UINT offset = 0;
					//���_�o�b�t�@���Z�b�g
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
					//�X�g���C�h�ƃI�t�Z�b�g
					UINT stride = sizeof(VertexPositionNormalTexture);
					if (IsSkinStride){
						stride = sizeof(VertexPositionNormalTextureSkinning);
					}
					UINT offset = 0;
					//���_�o�b�t�@���Z�b�g
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				}



				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//�`��
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}

	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//--------------------------------------------------------------------------------------
	void SpriteBaseDraw::CalcSpriteCoordinate(Coordinate cood, Matrix4X4& Retmat) {
		//�s��̎擾
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
	//	�p�r: PCSpriteDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PCSpriteDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		Vector2 m_SpriteSize;			//�X�v���C�g�̃T�C�Y�i�s�N�Z���j
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;
		Color4 m_Emissive;	//�F
		//���W�n
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
	//	�p�r: Dx11PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	void PCSpriteDraw::CreateMeshResource() {
		Vector2 HalfSize(pImpl->m_SpriteSize.x / 2.0f, pImpl->m_SpriteSize.y / 2.0f);
		//���_���쐬����z��
		vector<VertexPositionColor> verteces = {
			{ Vector3(-HalfSize.x, HalfSize.y, 0) , pImpl->m_LeftTop },
			{ Vector3(HalfSize.x, HalfSize.y, 0.0f), pImpl->m_RightTop },
			{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_LeftBottom },
			{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_RightBottom }
		};
		//�C���f�b�N�X���쐬���邽�߂̔z��
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
					L"�F�z��4�����ł�",
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
		//�s��̎擾
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate,RetWorld);
		//�R���X�^���g�o�b�t�@�̐ݒ�
		SpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�R���X�^���g�o�b�t�@�̐ݒ�
		//�s��̐ݒ�(�X�v���C�g�͓]�u���Ȃ�)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		pImpl->m_DrawContext->DrawIndexed<CBSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionColor), 0);
	}


	//--------------------------------------------------------------------------------------
	//	struct PTSpriteDraw::Impl;
	//	�p�r: PTSpriteDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PTSpriteDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		Vector2 m_SpriteSize;			//�X�v���C�g�̃T�C�Y�i�s�N�Z���j
		Color4 m_Emissive;	//�F
		Color4 m_Diffuse;	//�f�t�B�[�Y���C�g
		//���W�n
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
	//	�p�r: Dx11PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		const Vector2& Size) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl(Size))
	{
		try {
			Vector2 HalfSize(Size.x / 2.0f, Size.y / 2.0f);
			//���_���쐬����z��
			vector<VertexPositionTexture> verteces = {
				{ Vector3(-HalfSize.x, HalfSize.y, 0) , Vector2(0.0f, 0.0f) },
				{ Vector3(HalfSize.x, HalfSize.y, 0.0f), Vector2(1.0f, 0.0f) },
				{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), Vector2(0.0f, 1.0f) },
				{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), Vector2(1.0f, 1.0f) }
			};
			//�C���f�b�N�X���쐬���邽�߂̔z��
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
		//�e�N�X�`�����Ȃ����null��Ԃ�
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
		//�s��̎擾
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate, RetWorld);
		//�R���X�^���g�o�b�t�@�̐ݒ�
		DiffuseSpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�X�v���C�g�͓]�u���Ȃ�)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		cb1.Diffuse = pImpl->m_Diffuse;
		pImpl->m_DrawContext->DrawIndexed<CBDiffuseSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionTexture), 0);
	}

	//--------------------------------------------------------------------------------------
	//	struct PCTSpriteDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PCTSpriteDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<DrawContext> m_DrawContext;
		shared_ptr<MeshResource> m_MeshResource;
		Vector2 m_SpriteSize;			//�X�v���C�g�̃T�C�Y�i�s�N�Z���j
		Color4 m_LeftTop;
		Color4 m_RightTop;
		Color4 m_LeftBottom;
		Color4 m_RightBottom;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		Color4 m_Emissive;	//�F
		SpriteBaseDraw::Coordinate m_Coordinate;	//���W�n
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
	//	�p�r: Dx11PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	void PCTSpriteDraw::CreateMeshResource() {
		Vector2 HalfSize(pImpl->m_SpriteSize.x / 2.0f, pImpl->m_SpriteSize.y / 2.0f);
		//���_���쐬����z��
		vector<VertexPositionColorTexture> verteces = {
			{ Vector3(-HalfSize.x, HalfSize.y, 0) , pImpl->m_LeftTop, Vector2(0.0f, 0.0f) },
			{ Vector3(HalfSize.x, HalfSize.y, 0.0f), pImpl->m_RightTop, Vector2(1.0f, 0.0f) },
			{ Vector3(-HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_LeftBottom, Vector2(0.0f, 1.0f) },
			{ Vector3(HalfSize.x, -HalfSize.y, 0.0f), pImpl->m_RightBottom, Vector2(1.0f, 1.0f) }
		};
		//�C���f�b�N�X���쐬���邽�߂̔z��
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
					L"�F�z��4�����ł�",
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
		//�e�N�X�`�����Ȃ����null��Ԃ�
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
		//�s��̎擾
		Matrix4X4 RetWorld;
		CalcSpriteCoordinate(pImpl->m_Coordinate, RetWorld);
		//�R���X�^���g�o�b�t�@�̐ݒ�
		SpriteConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�X�v���C�g�͓]�u���Ȃ�)
		cb1.World = RetWorld;
		cb1.Emissive = pImpl->m_Emissive;
		pImpl->m_DrawContext->DrawIndexed<CBSprite>(GetGameObject(),
			&cb1, GetMeshResource(), sizeof(VertexPositionColorTexture), 0);
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<DrawContext> m_DrawContext;
		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		weak_ptr<TextureResource> m_TextureResource; //�e�N�X�`��
		Color4 m_Emissive;	//�F
		Color4 m_Diffuse;	//�f�t�B�[�Y���C�g
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
	//	�p�r: PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
		pImpl->m_DrawContext = ObjectFactory::Create<DrawContext>();
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//���b�V�����Ȃ���΃��^�[��
		if (pImpl->m_MeshResource.expired()) {
			throw BaseException(
				L"���b�V�����ݒ肳��Ă܂���",
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
		//�e�N�X�`�����Ȃ����null��Ԃ�
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
			//�V���h�E�}�b�v������Ύ��ȉe�h�~�p�̃s�N�Z���V�F�[�_
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow2>(true);
			IsShadowmap = true;
		}
		else {
			pImpl->m_DrawContext->SetVertexShader<VSPNTStaticShadow>(true);
			pImpl->m_DrawContext->SetPixelShader<PSPNTStaticShadow>(true);
		}

		//�X�e�[�W����J���������o��
		auto PtrCamera = PtrStage->GetTargetCamera();
		//�X�e�[�W���烉�C�g�����o��
		auto PtrLight = PtrStage->GetTargetLight(0);
		//�s��̎擾
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		PNTStaticShadowConstantBuffer cb1;

		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�]�u����)
		//�R���X�^���g�o�b�t�@�̐ݒ�
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
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		cb1.LightView = Matrix4X4EX::Transpose(LightView);
		cb1.LightProjection = Matrix4X4EX::Transpose(LightProj);

		//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
		//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
		auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();

		if (PtrTextureResource){
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::LinearClamp, PtrTextureResource->GetShaderResourceView().Get());
		}
		else{
			pImpl->m_DrawContext->AddSamplerAndSrv(0, SamplerState::SamplerNone, nullptr);
		}
		//���ȉe
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


		//�X�e�[�W����J���������o��
		auto PtrCamera = GetGameObject()->GetStage()->GetTargetCamera();
		//�X�e�[�W���烉�C�g�����o��
		auto PtrLight = GetGameObject()->GetStage()->GetTargetLight(0);
		//�s��̎擾
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�]�u����)
		//�R���X�^���g�o�b�t�@�̐ݒ�
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
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PNTCollisionDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<DrawContext> m_DrawContext;
		Color4 m_Emissive;	//�F
		Color4 m_Diffuse;	//�f�t�B�[�Y���C�g
		Impl() :
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0f),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class PNTCollisionDraw : public DrawComponent;
	//	�p�r: PNTCollision�`��R���|�[�l���g
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
		//���C�A�t���[��
		pImpl->m_DrawContext->SetRasterizerState(RasterizerState::Wireframe);

		auto PtrColl = GetGameObject()->GetComponent<Collision>();
		auto PtrMesh = PtrColl->GetMeshResource();

		//�X�e�[�W����J���������o��
		auto PtrCamera = GetGameObject()->GetStage()->GetTargetCamera();
		//�X�e�[�W���烉�C�g�����o��
		auto PtrLight = GetGameObject()->GetStage()->GetTargetLight(0);
		//�s��̎擾
		Matrix4X4 World = GetGameObject()->GetComponent<TransformMatrix>()->GetWorldMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		PNTStaticConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�]�u����)
		//�R���X�^���g�o�b�t�@�̐ݒ�
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
