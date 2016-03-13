#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//シェーダ定義（マクロ使用）
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBCustomDraw)
	IMPLEMENT_DX11_VERTEX_SHADER(VSCustomDraw, App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSCustomDraw, App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso")

	//--------------------------------------------------------------------------------------
	//	class CustomDrawVortex : public GameObject;
	//	独自のシェーダで描画する渦巻き
	//--------------------------------------------------------------------------------------
	//メッシュの作成
	void CustomDrawVortex::CreateCustomMesh(){
		m_BackupVirtex.clear();
		float rot = 0.0f;
		float Min = 1.0f / 50.0f;
		for (size_t i = 0; i < 50; i++){
			m_BackupVirtex.push_back(
				VertexPositionColor(
				Vector3(sin(rot), (float)i * 0.05f, cos(rot)),	//渦巻き状になるよう初期化
				Color4(1.0, Min * i, 0.0, 1.0)
				)
				);
			rot += 0.5f;
		}
		//頂点バッファの作成（頂点を変更できる）
		VertexUtil::CreateDynamicVertexBuffer(m_VertexBuffer, m_BackupVirtex);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(m_BackupVirtex.size());
	}

	//頂点の変更
	void CustomDrawVortex::UpdateCustomMesh(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 10.0f;
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(m_VertexBuffer.Get(), 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawOctahedron::UpdateCustomMesh()"
				);
		}
		//頂点の変更
		auto* vertices = (VertexPositionColor*)mappedBuffer.pData;

		float rot = m_TotalTime;
		for (size_t i = 0; i < m_NumVertices; i++){
			Vector3 Pos = m_BackupVirtex[i].position;
			Pos = Vector3(sin(rot), (float)i * 0.05f, cos(rot));
			vertices[i] = VertexPositionColor(
				Pos,
				m_BackupVirtex[i].color
				);
			rot += 0.5f;
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	}



	//構築と破棄
	CustomDrawVortex::CustomDrawVortex(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawVortex::~CustomDrawVortex(){
	}
	//初期化
	void CustomDrawVortex::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0, 0, 0);
		Ptr->SetPosition(m_StartPos);
		//頂点バッファとインデックスバッファを作成
		CreateCustomMesh();
	}
	void CustomDrawVortex::OnUpdate(){
		UpdateCustomMesh();
	}
	void CustomDrawVortex::OnDraw(){
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
		CustomDrawConstantBuffer cb1;
		//行列の設定(転置する)
		cb1.World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());;
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//描画方法
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		//ステータスのポインタ
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//デプスステンシルは使用しない
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSCustomDraw::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBCustomDraw::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
		//描画
		pID3D11DeviceContext->Draw(m_NumVertices, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);

	}

}
//endof  basedx11
