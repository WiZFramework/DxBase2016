#pragma once

#include "stdafx.h"

namespace basedx11{

	struct BallConstantBuffer
	{
		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Proj;
		Vector3  m_LightDir;
		float m_Explode;
		BallConstantBuffer() {
			memset(this, 0, sizeof(BallConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBBall, BallConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSBall, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSBall)
	DECLARE_DX11_GEOMETRY_SHADER(GSBall)



	//--------------------------------------------------------------------------------------
	//	class Ball : public GameObject;
	//	用途: 頂点が変更されるボール
	//--------------------------------------------------------------------------------------
	class Ball : public GameObject{
		Vector3 m_StartPos;
		//飛び出す係数
		float m_Explode;
	public:
		//構築と破棄
		Ball(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Ball(){}
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
		//描画
		virtual void OnDraw() override;
	};

}
//endof  basedx11
