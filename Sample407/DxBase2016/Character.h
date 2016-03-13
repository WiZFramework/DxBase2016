#pragma once

#include "stdafx.h"

namespace basedx11{

	//カスタム描画コンスタントバッファ構造体
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 Param;	//汎用パラメータ
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//シェーダ宣言（マクロ使用）
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)

	//--------------------------------------------------------------------------------------
	//	class CustomDrawVortex : public GameObject;
	//	独自のシェーダで描画する渦巻き
	//--------------------------------------------------------------------------------------
	class CustomDrawVortex : public GameObject{
		Vector3 m_StartPos;
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//バックアップ用の頂点配列
		vector<VertexPositionColor> m_BackupVirtex;
		//メッシュの作成
		void CreateCustomMesh();
		//頂点の変更
		void UpdateCustomMesh();
		//経過時間
		float m_TotalTime;
	public:
		//構築と破棄
		CustomDrawVortex(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CustomDrawVortex();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
