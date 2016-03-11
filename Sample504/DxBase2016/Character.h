#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle{
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//--------------------------------------------------------------------------------------
		//	void InsertSpark(
		//	const Vector3& Pos,	//放出位置（エミッター位置）
		//	const Color4& Col	//色
		//	);
		//用途: スパークを放出する
		//戻り値: なし。
		//--------------------------------------------------------------------------------------
		void InsertSpark(const Vector3& Pos, const Color4& Col);
	};
	//プレイヤーの宣言
	class Player;
	//--------------------------------------------------------------------------------------
	//	class AssignInterface;
	//	用途: 振り分けするインターフェイス
	//--------------------------------------------------------------------------------------
	class AssignInterface{
	protected:
		AssignInterface(){}
		virtual ~AssignInterface(){}
	public:
		virtual void AssignAction(const shared_ptr<Player>& ply) = 0;

	};


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject,public AssignInterface;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject, public AssignInterface{
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
		virtual void AssignAction(const shared_ptr<Player>& ply)override;
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject,public AssignInterface;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject, public AssignInterface{
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
		virtual void AssignAction(const shared_ptr<Player>& ply)override;
	};

}
//endof  basedx11
