#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	Box::~Box(){}

	//初期化
	void Box::OnCreate(){
		auto PtrTransMatrix = GetComponent<TransformMatrix>();
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			m_StartPos
			);
		PtrTransMatrix->SetWorldMatrix(WorldMat);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");

		
		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CYLINDER");
		PtrDraw->SetTextureResource(L"TRACE_TX");

		
		//透過処理
		SetAlphaActive(true);
	}


}
//endof  basedx11
