#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~MoveBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NumberSprite : public GameObject{
		Vector3 m_StartPos;
		//頂点の配列の配列（2次元配列）
		vector< vector<VertexPositionColorTexture> > m_NumberVertexVec;
		float m_TotalTime;
	public:
		//構築と破棄
		NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~NumberSprite();
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class BarSprite : public GameObject{
		Vector3 m_StartPos;
		float m_TotalTime;
		//頂点のバックアップ
		vector<VertexPositionColorTexture> m_BarVirtexVec;
		//スプライトの幅と高さ
		const float SpriteHeight = 256.0f;
		const float SpriteWidth = 64.0f;
	public:
		//構築と破棄
		BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BarSprite();
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
	};



}
//endof  basedx11
