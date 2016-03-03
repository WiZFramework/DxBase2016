#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	—p“r: ‰ñ“]‚·‚éƒg[ƒ‰ƒX
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		Vector3 m_StartPos;
		float m_YRot;
		float m_RotationSpeed;
	public:
		//\’z‚Æ”jŠü
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//‰Šú‰»
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//endof  basedx11
