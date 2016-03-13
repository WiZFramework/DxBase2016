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
	//	�p�r: ���_���ύX�����{�[��
	//--------------------------------------------------------------------------------------
	class Ball : public GameObject{
		Vector3 m_StartPos;
		//��яo���W��
		float m_Explode;
	public:
		//�\�z�Ɣj��
		Ball(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Ball(){}
		//������
		virtual void OnCreate() override;
		//�ω�
		virtual void OnUpdate() override;
		//�`��
		virtual void OnDraw() override;
	};

}
//endof  basedx11
