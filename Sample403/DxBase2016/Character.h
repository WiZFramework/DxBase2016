#pragma once

#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	独自のシェーダで描画するボックス
	//--------------------------------------------------------------------------------------
	class CustomDrawBox : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//経過時間
		float m_TotalTime;
		//コンスタントバッファ構造体
		struct ConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Vector4 Param;	//汎用パラメータ

		};
		//コンスタントバッファ
		shared_ptr<CBuffer<ConstantBuffer>> m_ConstantBuffer;
		//頂点シェーダ
		shared_ptr< VShader<VertexPositionNormalTexture> > m_VirtexShader;
		//ピクセルシェーダー
		shared_ptr<PShader> m_PixelShader;
	public:
		//構築と破棄
		CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawBox();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
