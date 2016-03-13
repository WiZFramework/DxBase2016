#include "stdafx.h"
#include "Project.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//初期化
	void FixedBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<FixedBox> >(GetThis<FixedBox>());
		//最初のステートをFixedBoxDefaultStateに設定
		m_StateMachine->SetCurrentState(FixedBoxDefaultState::Instance());
		//DefaultStateの初期化実行を行う
		m_StateMachine->GetCurrentState()->Enter(GetThis<FixedBox>());

	}

	void FixedBox::OnUpdate(){
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();

	}


	void FixedBox::OnLastUpdate(){
		if (m_StateMachine->GetCurrentState() == FixedBoxDefaultState::Instance()){
			//マネージャに自分自身の行列を追加する
			auto ManagerPtr = GetStage()->GetSharedGameObject<DrawBoxManager>(L"DrawBoxManager");
			auto PtrTransform = GetComponent<Transform>();
			ManagerPtr->AddInstanceMatrix(PtrTransform->GetWorldMatrix());
		}
		else{
			//分割用マネージャに自分自身の分割行列を追加する
			auto ManagerPtr = GetStage()->GetSharedGameObject<DrawBoxManager>(L"DrawSplitBoxManager");
			for (auto& data : m_SplitBoxDataVec){
				if (data.m_Enabled){
					Matrix4X4 m;
					m.DefTransformation(data.m_Scale, data.m_Qt, data.m_Pos);
					ManagerPtr->AddInstanceMatrix(m);
				}
			}
		}
	}

	//モーション系
	//プレイヤーがヒットしたか
	bool FixedBox::IsPlayerHit() const{
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PtrPlayerTransform = PlayerPtr->GetComponent<Transform>();
		auto PtrTransform = GetComponent<Transform>();
		auto Len = Vector3EX::Length(PtrPlayerTransform->GetPosition() - PtrTransform->GetPosition());
		if (Len < 0.5f){
			return true;
		}
		return false;
	}

	//分割用の行列配列の作成
	void FixedBox::CreateSplitVec(size_t splitcount){
		m_SplitBoxDataVec.clear();
		if (splitcount <= 0){
			//分割数が0ならリターン（例外でもよい）
			return;
		}
		auto PtrTransform = GetComponent<Transform>();
		Vector3 HalfBaseScale = PtrTransform->GetScale() / 2.0f;
		Vector3 Scale = PtrTransform->GetScale() / (float)splitcount;
		for (size_t x = 0; x < splitcount; x++){
			for (size_t y = 0; y < splitcount; y++){
				for (size_t z = 0; z < splitcount; z++){
					auto BasePos = PtrTransform->GetPosition();
					float PosX = (BasePos.x - HalfBaseScale.x + Scale.x * 0.5f) + x * Scale.x;
					float PosY = (BasePos.y - HalfBaseScale.y + Scale.y * 0.5f) + y * Scale.y;
					float PosZ = (BasePos.z - HalfBaseScale.z + Scale.z * 0.5f) + z * Scale.z;

					Vector3 Velo = Vector3(PosX, PosY, PosZ) - PtrTransform->GetPosition();
					float rv = Util::RandZeroToOne() * 4.0f;
					Velo *= rv;
					Velo.y = rv;
					float rx = (Util::RandZeroToOne() - 0.5f) / 2.0f;
					float ry = (Util::RandZeroToOne() - 0.5f) / 2.0f;
					float rz = (Util::RandZeroToOne() - 0.5f) / 2.0f;

					m_SplitBoxDataVec.push_back(
						SplitBoxData(Scale, Vector3(0, 0, 0), Vector3(PosX, PosY, PosZ), Velo, Vector3(rx, ry, rz))
						);

				}
			}
		}
	}

	//分割用の行列配列の更新
	void FixedBox::UpdateSplitVec(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& data : m_SplitBoxDataVec){
			data.UpdateData(ElapsedTime);
		}
	}





	//--------------------------------------------------------------------------------------
	//	class FixedBoxDefaultState : public ObjState<FixedBox>;
	//	用途: 通常
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<FixedBoxDefaultState> FixedBoxDefaultState::Instance(){
		static shared_ptr<FixedBoxDefaultState> instance;
		if (!instance){
			instance = shared_ptr<FixedBoxDefaultState>(new FixedBoxDefaultState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void FixedBoxDefaultState::Enter(const shared_ptr<FixedBox>& Obj){
		//何もしない
	}
	//ステート実行中に毎ターン呼ばれる関数
	void FixedBoxDefaultState::Execute(const shared_ptr<FixedBox>& Obj){
		if (Obj->IsPlayerHit()){
			//ステート変更
			Obj->GetStateMachine()->ChangeState(FixedBoxSplitState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void FixedBoxDefaultState::Exit(const shared_ptr<FixedBox>& Obj){
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	//	class  FixedBoxSplitState : public ObjState<FixedBox>;
	//	用途: 分割状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr< FixedBoxSplitState>  FixedBoxSplitState::Instance(){
		static shared_ptr< FixedBoxSplitState> instance;
		if (!instance){
			instance = shared_ptr< FixedBoxSplitState>(new  FixedBoxSplitState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void  FixedBoxSplitState::Enter(const shared_ptr<FixedBox>& Obj){
		//分割の行列の作成
		Obj->CreateSplitVec(4);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void  FixedBoxSplitState::Execute(const shared_ptr<FixedBox>& Obj){
		Obj->UpdateSplitVec();
	}
	//ステートにから抜けるときに呼ばれる関数
	void  FixedBoxSplitState::Exit(const shared_ptr<FixedBox>& Obj){
		//何もしない
	}


	//シェーダー実体（マクロ使用）
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBInstanceShadowmap)
	IMPLEMENT_DX11_VERTEX_SHADER(VSInstanceShadowmap,App::GetApp()->m_wstrRelativeShadersPath + L"VSInstanceShadowmap.cso")
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBInstance)
	IMPLEMENT_DX11_VERTEX_SHADER(VSInstance, App::GetApp()->m_wstrRelativeShadersPath + L"VSInstance.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSInstance, App::GetApp()->m_wstrRelativeShadersPath + L"PSInstance.cso")



	//--------------------------------------------------------------------------------------
	//	class InstanceShadowmap : public Shadowmap;
	//	用途: インスタンスシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	InstanceShadowmap::InstanceShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr){}
	InstanceShadowmap::~InstanceShadowmap(){}

	//操作
	void InstanceShadowmap::OnDraw(){
		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrLight && PtrCamera){

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

				InstanceShadowmapConstantBuffer Cb;
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				//これより描画処理
				//コンスタントバッファの更新
				pID3D11DeviceContext->UpdateSubresource(CBInstanceShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSInstanceShadowmap::GetPtr()->GetInputLayout());
				//マネージャを取得
				auto GameObj = dynamic_pointer_cast<DrawBoxManager>(GetGameObject());
				if (GameObj->GetInstanceVec().size() <= 0){
					return;
				}
				//形状の頂点バッファと行列バッファを設定
				UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(Matrix4X4) };
				UINT offset[2] = { 0, 0 };
				ID3D11Buffer* pBuf[2] = { GetMeshResource()->GetVertexBuffer().Get(), GameObj->GetMatrixBuffer().Get() };
				//頂点バッファをセット
				pID3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSInstanceShadowmap::GetPtr()->GetShader(), nullptr, 0);
				//インデックスバッファのセット
				pID3D11DeviceContext->IASetIndexBuffer(GetMeshResource()->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//ピクセルシェーダはセットしない！
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//ジオメトリシェーダの設定（使用しない）
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBInstanceShadowmap::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//コンスタントバッファをピクセルシェーダにセット
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//描画
				//インスタンス描画(インスタンス配列の数だけ描画)
				pID3D11DeviceContext->DrawIndexedInstanced(GetMeshResource()->GetNumIndicis(), GameObj->GetInstanceVec().size(), 0, 0, 0);
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}



	//--------------------------------------------------------------------------------------
	//	class DrawBoxManager : public GameObject;
	//	用途: 固定のボックスの描画マネージャ
	//--------------------------------------------------------------------------------------
	//行列用の頂点バッファの作成
	void DrawBoxManager::CreateMatrixBuffer(){
		//行列バッファの作成
		//Max値で作成する
		vector<Matrix4X4> matrices(m_MaxInstance);
		for (auto& m : matrices){
			m = Matrix4X4();
		}
		VertexUtil::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
	}

	//構築と破棄
	DrawBoxManager::DrawBoxManager(const shared_ptr<Stage>& StagePtr,
		size_t MaxInstance, const wstring& MeshKey, const wstring& TextureKey) :
		GameObject(StagePtr),
		m_MaxInstance(MaxInstance),
		m_MeshKey(MeshKey),
		m_TextureKey(TextureKey)
	{}
	DrawBoxManager::~DrawBoxManager(){}

	void DrawBoxManager::OnCreate(){
		AddComponent<Transform>();
		//行列バッファの作成
		CreateMatrixBuffer();
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<InstanceShadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(m_MeshKey);

	}
	//操作
	//スケール、回転、位置で追加
	void DrawBoxManager::AddInstanceVectors(const Vector3& Scale, const Vector3& Rot, const Vector3& Pos){
		if (m_InstanceVec.size() >= m_MaxInstance){
			wstring strsz = Util::UintToWStr(m_InstanceVec.size());
			wstring strmax = Util::UintToWStr(m_MaxInstance);
			strsz += L">=";
			strsz += strmax;
			throw BaseException(
				L"これ以上オブジェクトを追加できません。",
				strsz,
				L"DrawBoxManager::AddInstanceVectors()"
				);
		}
		Matrix4X4 mat;
		mat.DefTransformation(Scale, Rot, Pos);
		m_InstanceVec.push_back(mat);
	}
	//行列で追加
	void DrawBoxManager::AddInstanceMatrix(const Matrix4X4& Mat){
		if (m_InstanceVec.size() >= m_MaxInstance){
			wstring strsz = Util::UintToWStr(m_InstanceVec.size());
			wstring strmax = Util::UintToWStr(m_MaxInstance);
			strsz += L">=";
			strsz += strmax;
			throw BaseException(
				L"これ以上オブジェクトを追加できません。",
				strsz,
				L"DrawBoxManager::AddInstanceMatrix()"
				);
		}
		m_InstanceVec.push_back(Mat);
	}
	//仮想関数
	void DrawBoxManager::OnLastUpdate(){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();

		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pID3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"DrawBoxManager::Update3()"
				);
		}
		//行列の変更（インスタンス配列に登録されている数だけ設定）
		auto* matrices = (Matrix4X4*)mappedBuffer.pData;
		for (size_t i = 0; i < m_InstanceVec.size(); i++){
			matrices[i] = Matrix4X4EX::Transpose(m_InstanceVec[i]);
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);

	}


	void DrawBoxManager::OnDraw(){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = GetStage()->GetRenderState();

		//シャドウマップのレンダラーターゲット
		auto ShadoumapPtr = GetStage()->GetShadowMapRenderTarget();

		auto PtrT = GetComponent<Transform>();
		//ステージからカメラを取り出す
		auto PtrCamera = GetStage()->GetTargetCamera();
		//カメラの取得
		Matrix4X4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//描画するメッシュリソースを取得
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(m_MeshKey);

		//コンスタントバッファの設定
		InstanceConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//ライトの設定
		//ステージから0番目のライトを取り出す
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		Matrix4X4 LightView, LightProj, LightViewProj;
		Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
		Vector3 LightAt = PtrCamera->GetAt();
		Vector3 LightEye = LightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		//ライトのビューと射影を計算
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		LightViewProj = LightView * LightProj;
		Matrix4X4 LWMatrix = PtrT->GetWorldMatrix() * LightViewProj;
		//コンスタントバッファに設定
		cb1.LPos = LightEye;
		cb1.LPos.w = 0;
		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 0;
		cb1.LView = Matrix4X4EX::Transpose(LightView);
		cb1.LProjection = Matrix4X4EX::Transpose(LightProj);

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBInstance::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(Matrix4X4) };
		UINT offset[2] = { 0, 0 };
		ID3D11Buffer* pBuf[2] = { MeshRes->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pID3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//テクスチャを取得
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//テクスチャを設定
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(m_TextureKey);
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);


		//シャドウマップのリソースビューを取得
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		//シャドウ用にリニアサンプラーを取得
		ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
		//シャドウ用リニアサンプラーを設定
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);


		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSInstance::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSInstance::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSInstance::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBInstance::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//インスタンス描画(インスタンス配列の数だけ描画)
		pID3D11DeviceContext->DrawIndexedInstanced(MeshRes->GetNumIndicis(), m_InstanceVec.size(), 0, 0, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);
		//インスタンス配列が増減する可能性があるので配列をクリア
		m_InstanceVec.clear();
	}


}
//endof  basedx11
