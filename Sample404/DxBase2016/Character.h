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
		FixedBox(shared_ptr<Stage>& StagePtr,
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
	//	class FixedBoxManager : public GameObject;
	//	用途: 固定のボックスのマネージャ（描画する）
	//--------------------------------------------------------------------------------------
	class FixedBoxManager : public GameObject{
		//頂点とインデックスを行列と、ナンバーに合わせて変更する
		void ChangeVertex(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, const Matrix4X4& Mat, size_t Num);
		//リソース
		shared_ptr<MeshResource> m_BoxesRes;
	public:
		//構築と破棄
		FixedBoxManager(shared_ptr<Stage>& StagePtr);
		virtual ~FixedBoxManager();
		//初期化
		virtual void OnCreate() override;
	};

}
//endof  basedx11
