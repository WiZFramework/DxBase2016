#pragma once

#include "stdafx.h"

namespace basedx11{

	//カスタムシャドウマップ用コンスタントバッファ構造体
	struct CustomShadowmapConstantBuffer
	{
		XMMATRIX mWorld[3];
		XMMATRIX mView;
		XMMATRIX mProj;
		CustomShadowmapConstantBuffer() {
			memset(this, 0, sizeof(CustomShadowmapConstantBuffer));
		};
	};
	//シェーダ宣言（マクロ使用）
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomShadowmap, CustomShadowmapConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomShadowmap, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomShadowmap)
	//カスタム描画コンスタントバッファ構造体
	struct CustomDrawConstantBuffer
	{
		Matrix4X4 World[3];
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//汎用パラメータ
		CustomDrawConstantBuffer() {
			memset(this, 0, sizeof(CustomDrawConstantBuffer));
		};
	};
	//シェーダ宣言（マクロ使用）
	DECLARE_DX11_CONSTANT_BUFFER(CBCustomDraw, CustomDrawConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSCustomDraw, VertexPositionNormalColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSCustomDraw)
	DECLARE_DX11_PIXEL_SHADER(PSCustomDraw)



	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	用途: カスタムシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	class CustomShadowmap : public Shadowmap{
	public:
		explicit CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~CustomShadowmap();
		//操作
		virtual void OnDraw()override;

	};



	//--------------------------------------------------------------------------------------
	//	class CustomDrawOctahedron : public GameObject;
	//	独自のシェーダで描画する8面体
	//--------------------------------------------------------------------------------------
	class CustomDrawOctahedron : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//インデックスバッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//インデックスの数
		UINT m_NumIndicis;
		//バックアップ用の頂点配列
		vector<VertexPositionNormalColor> m_BackupVirtex;
		//メッシュの作成
		void CreateCustomMesh();
		//頂点の変更
		void UpdateCustomMesh();
		//経過時間
		float m_TotalTime;
	public:
		//構築と破棄
		CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawOctahedron();
		//アクセサ
		const ComPtr<ID3D11Buffer>& GetVertexBuffer() const{
			return m_VertexBuffer;
		}
		const ComPtr<ID3D11Buffer>& GetIndexBuffer() const{
			return m_IndexBuffer;
		}
		UINT GetNumVertices() const{
			return m_NumVertices;
		}
		UINT GetNumIndicis() const{
			return m_NumIndicis;
		}

		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
