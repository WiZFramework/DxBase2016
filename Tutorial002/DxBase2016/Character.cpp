#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{
	}
	Box::~Box(){}

	//������
	void Box::OnCreate(){
		auto PtrTransMatrix = GetComponent<TransformMatrix>();
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			m_StartPos
			);
		PtrTransMatrix->SetWorldMatrix(WorldMat);
		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CYLINDER");

		
		//�`��R���|�[�l���g
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CYLINDER");
		PtrDraw->SetTextureResource(L"TRACE_TX");

		
		//���ߏ���
		SetAlphaActive(true);
	}


}
//endof  basedx11
