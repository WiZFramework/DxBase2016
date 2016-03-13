#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	独自のメッシュリソース
	//--------------------------------------------------------------------------------------
	class CustomDrawBox : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
		//メッシュリソース
		shared_ptr<MeshResource> m_MeshResource;
		//メッシュリソースの作成
		void CreateMeshResource();
		//頂点の変更
		void UpdateMeshResource();
		//経過時間
		float m_TotalTime;
	public:
		//構築と破棄
		CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~CustomDrawBox();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//endof  basedx11
