#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	用途: ボックス
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Posision;
	public:
		//構築と破棄
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& Scale,const Vector3& StartPos);
		virtual ~Box();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveSphere : public GameObject;
	//	用途: 移動する球体
	//--------------------------------------------------------------------------------------
	class MoveSphere : public GameObject{
		Vector3 m_Scale;
		Vector3 m_StartPosision;
		Vector3 m_Posision;
		//速度
		Vector3 m_Velocity;
		//再帰的に衝突判定をする
		bool RecTestSphereObb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime);
	public:
		//構築と破棄
		MoveSphere(const shared_ptr<Stage>& StagePtr, const Vector3& Scale, const Vector3& StartPos);
		virtual ~MoveSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
	};


}
//endof  basedx11
