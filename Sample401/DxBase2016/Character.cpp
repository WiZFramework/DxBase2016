#include "stdafx.h"
#include "Project.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class TriangleObject : public GameObject;
	//	用途: 三角形オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	TriangleObject::TriangleObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_VertexBuffer(),
		m_NumVertices(0)
	{}
	TriangleObject::~TriangleObject(){}

	//初期化
	void TriangleObject::OnCreate(){
		//頂点を作成するための配列
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(0.0f, 0.5f, 0.5f)) },
			{ VertexPosition(Vector3(0.5f, -0.5f, 0.5f)) },
			{ VertexPosition(Vector3(-0.5f, -0.5f, 0.5f)) },
		};
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());

	}
	//描画
	void TriangleObject::OnDraw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//コンスタントバッファの準備
		PDirectConstantBuffer sb;
		sb.Diffuse = Color4(1.0f, 1.0f, 0, 1.0f);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPDirect::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダには渡さない（nullptrを渡す）
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPDirect::GetPtr()->GetInputLayout());

		//描画
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//後始末
		//ステータスのポインタ
		auto RenderStatePtr = GetStage()->GetRenderState();
		Dev->InitializeStates(RenderStatePtr);
	}


}
//endof  basedx11
