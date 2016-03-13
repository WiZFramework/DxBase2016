#include "stdafx.h"
#include "Project.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	独自のシェーダで描画するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	CustomDrawBox::CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos)
	{}
	CustomDrawBox::~CustomDrawBox(){
	}
	//初期化
	void CustomDrawBox::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//シェーダの作成
		//コンスタントバッファ
		m_ConstantBuffer = make_shared<CBuffer<ConstantBuffer>>();
		//頂点シェーダ
		m_VirtexShader = make_shared<VShader<VertexPositionNormalTexture>>(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso");
		//ピクセルシェーダー
		m_PixelShader = make_shared<PShader>(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso");
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//透明処理（描画順制御のため）
		SetAlphaActive(true);
	}
	void CustomDrawBox::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}
	void CustomDrawBox::OnDraw(){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = GetStage()->GetRenderState();

		auto PtrT = GetComponent<Transform>();
		//ステージからカメラを取り出す
		auto PtrCamera = GetStage()->GetTargetCamera();
		//カメラの取得
		Matrix4X4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		ConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//行列の設定(転置する)
		cb1.World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());;
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//ライトの設定
		//ステージから0番目のライトを取り出す
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(m_ConstantBuffer->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		auto PtrMeshResource = App::GetApp()->GetResource<MeshResource>(L"DEFAULT_CUBE");
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//テクスチャを取得
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//テクスチャを設定
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(L"TRACE_TX");
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//半透明処理
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);

		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(m_VirtexShader->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(m_PixelShader->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(m_VirtexShader->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = m_ConstantBuffer->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);


	}



}
//endof  basedx11
