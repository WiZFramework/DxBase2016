#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	用途: 動くオブジェクト
	//--------------------------------------------------------------------------------------
	class MoveObject : public GameObject{
	public:
		//構築と破棄
		MoveObject(const shared_ptr<Stage>& StagePtr);
		virtual ~MoveObject();
		virtual void OnCreate() override{}
		//初期化
		void OnCreateWithParam(const Vector3& StartPos,
			const Vector3& StartVelocity, bool Type);
		//ターンの最終更新時
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		//Vector3 m_Scale;
		//Vector3 m_Rotation;
		//Vector3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override{}
		void OnCreateWithParam(const Vector3& Scale,
			const Vector3& Rotation,const Vector3& Position);
	};

}
//endof  basedx11
