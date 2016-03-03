#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	用途: 回転するトーラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RollingTorus::RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_YRot(0), m_RotationSpeed(20.0f)
	{}
	RollingTorus::~RollingTorus(){}
	//初期化
	void RollingTorus::OnCreate(){
		//Transformだけは追加しなくても取得できる
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);
		Ptr->SetPosition(m_StartPos);
		//影の作成
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形状
		ShadowPtr->SetMeshResource(L"DEFAULT_TORUS");

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		//メッシュの登録
		PtrDraw->SetMeshResource(L"DEFAULT_TORUS");

		PtrDraw->SetDiffuse(Color4(1.0f, 1.0f, 0, 1.0f));


	}
	void RollingTorus::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_YRot += (m_RotationSpeed * ElapsedTime);
		if (m_RotationSpeed > 0){
			m_RotationSpeed -= 0.05f;
		}
		auto Ptr = GetComponent<Transform>();
		Ptr->SetRotation(XM_PIDIV2, m_YRot, 0.0f);

	}


}
//endof  basedx11
