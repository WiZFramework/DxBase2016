#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Box::Box(const shared_ptr<Stage>& StagePtr, const Vector3& Scale, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Posision(StartPos)
	{
	}
	Box::~Box(){}

	//初期化
	void Box::OnCreate(){
		auto PtrTransformMatrix = GetComponent<TransformMatrix>();
		Matrix4X4 mat;
		mat.DefTransformation(
			m_Scale,
			Vector3(0.0f, 0.0f, 0.0f),
			m_Posision
			);

		PtrTransformMatrix->SetWorldMatrix(mat);
		
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明有効
		SetAlphaActive(true);
	}

	//更新
	void Box::OnUpdate(){
	}


	//--------------------------------------------------------------------------------------
	//	class MoveSphere : public GameObject;
	//	用途: 移動する球体
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveSphere::MoveSphere(const shared_ptr<Stage>& StagePtr, const Vector3& Scale, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_StartPosision(StartPos),
		m_Posision(StartPos),
		m_Velocity(-1.0f,0,0)
	{}
	MoveSphere::~MoveSphere(){}

	//再帰的に衝突判定をする
	bool MoveSphere::RecTestSphereObb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
		const OBB& DestObb,
		float StartTime, float EndTime, float& HitTime){
		const float EPSILON = 0.002f;
		SPHERE SrcSp2;
		float mid = (StartTime + EndTime) * 0.5f;
		SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
		SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
		Vector3 RetVec;
		if (!HitTest::SPHERE_OBB(SrcSp2, DestObb, RetVec)){
			return false;
		}
		if (EndTime - StartTime < EPSILON){
			HitTime = StartTime;
			return true;
		}
		if (RecTestSphereObb(SrcSp, SrcVelocity, DestObb, StartTime, mid, HitTime)){
			return true;
		}
		return RecTestSphereObb(SrcSp, SrcVelocity, DestObb, mid, EndTime, HitTime);
	}


	//初期化
	void MoveSphere::OnCreate(){
		auto PtrTransformMatrix = GetComponent<TransformMatrix>();
		Matrix4X4 mat;
		mat.DefTransformation(
			m_Scale,
			Vector3(0.0f, 0.0f, 0.0f),
			m_Posision
			);

		PtrTransformMatrix->SetWorldMatrix(mat);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//透明有効
		SetAlphaActive(true);

	}
	//更新
	void MoveSphere::OnUpdate(){
		//行き過ぎた場合の対応
		if (m_Posision.x > m_StartPosision.x){
			m_Posision = m_StartPosision;
			//速度を速くする
			m_Velocity.x = -50.0f;
		}
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//Boxを取得
		auto BoxPtr = GetStage()->GetSharedGameObject<Box>(L"Box");
		auto BoxMat = BoxPtr->GetComponent<TransformMatrix>()->GetWorldMatrix();
		OBB BoxObb(Vector3(1.0f, 1.0f, 1.0f), BoxMat);
		SPHERE Sp(m_Posision, m_Scale.x / 2.0f);
		float HitTime;
		bool Hit = false;
		//衝突判定
		//実際に動かす前に判定する
		if (RecTestSphereObb(Sp, m_Velocity, BoxObb, 0, ElapsedTime, HitTime)){
			//衝突した
			ElapsedTime = HitTime;
			Hit = true;
		}
		//判定後に動かす
		auto Span = m_Velocity * ElapsedTime;
		m_Posision += Span;
		if (Hit){
			//衝突してたら反転移動（ゆっくり）
			m_Velocity.x = 1.0f;
		}
		auto PtrTransformMatrix = GetComponent<TransformMatrix>();
		Matrix4X4 mat;
		mat.DefTransformation(
			m_Scale,
			Vector3(0.0f, 0.0f, 0.0f),
			m_Posision
			);
		PtrTransformMatrix->SetWorldMatrix(mat);
	}


}
//endof  basedx11
