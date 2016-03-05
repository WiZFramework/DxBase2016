#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class SeekCamera : public GameObject;
	//	用途: 追いかけるカメラオブジェクト
	//--------------------------------------------------------------------------------------
	class SeekCamera : public GameObject{
		Vector3 m_StartPos;
		shared_ptr< StateMachine<SeekCamera> >  m_StateMachine;	//ステートマシーン
	public:
		//構築と破棄
		SeekCamera(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekCamera();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr< StateMachine<SeekCamera> > GetStateMachine() const{
			return m_StateMachine;
		}
		//モーションを実装する関数群
		Vector3 GetCameraTargetMotion(bool IsFar);
		float GetToPlayerLemngth();

		void  SeekStartMoton();
		bool  SeekUpdateMoton();
		void  SeekEndMoton();

		void  ArriveStartMoton();
		bool  ArriveUpdateMoton();
		void  ArriveEndMoton();
		//操作
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class SeekCameraFarState : public ObjState<SeekCamera>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class SeekCameraFarState : public ObjState<SeekCamera>
	{
		SeekCameraFarState(){}
	public:
		static shared_ptr<SeekCameraFarState> Instance();
		virtual void Enter(const shared_ptr<SeekCamera>& Obj)override;
		virtual void Execute(const shared_ptr<SeekCamera>& Obj)override;
		virtual void Exit(const shared_ptr<SeekCamera>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class SeekCameraNearState : public ObjState<SeekCamera>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class SeekCameraNearState : public ObjState<SeekCamera>
	{
		SeekCameraNearState(){}
	public:
		static shared_ptr<SeekCameraNearState> Instance();
		virtual void Enter(const shared_ptr<SeekCamera>& Obj)override;
		virtual void Execute(const shared_ptr<SeekCamera>& Obj)override;
		virtual void Exit(const shared_ptr<SeekCamera>& Obj)override;
	};





	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject{
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//ステートマシーン
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//ユーティリティ関数群
		//プレイヤーの位置を返す
		Vector3 GetPlayerPosition() const;
		//プレイヤーまでの距離を返す
		float GetPlayerLength() const;
	public:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const{
			return m_StateMachine;
		}
		//モーションを実装する関数群
		void  SeekStartMoton();
		bool  SeekUpdateMoton();
		void  SeekEndMoton();

		void  ArriveStartMoton();
		bool  ArriveUpdateMoton();
		void  ArriveEndMoton();

		//操作
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState(){}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState(){}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


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
		FixedBox(const shared_ptr<Stage>& StagePtr,
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
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject{
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
	};


	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	用途: 障害物カプセル
	//--------------------------------------------------------------------------------------
	class CapsuleObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//構築と破棄
		CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~CapsuleObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject{
		Vector3 m_StartPos;
	public:
		//構築と破棄
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SphereObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//衝突判定に使用するRECT
	struct COLRECT{
		Vector3 m_Center;     //中心点の座標
		Vector3 m_Rot[2];  //XY の各座標軸の傾きを表す方向ベクトル
		float m_UVec[2];     //XY座標軸に沿った長さの半分（中心点から面までの長さ）
		COLRECT(){}
		//横の基本大きさ、縦の基本大きさをと変換行列を与えてRECTを完成させる
		COLRECT(float BaseXSize, float BaseYSize, const Matrix4X4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11, Matrix._12, Matrix._13);
			Vector3 VecY(Matrix._21, Matrix._22, Matrix._23);
			m_UVec[0] = BaseXSize * VecX.Length() * 0.5f;
			m_UVec[1] = BaseYSize * VecY.Length() * 0.5f;
			//回転を得る
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
		}
	};


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	用途: 球と衝突判定するスクエア
	//--------------------------------------------------------------------------------------
	class HitTestSquare : public GameObject{
		Vector3 m_StartScale;
		Vector3 m_StartRotation;
		Vector3 m_StartPos;
	public:
		//構築と破棄
		HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos);
		virtual ~HitTestSquare();
		//初期化
		virtual void OnCreate() override;
		//平面を返す
		PLANE GetPLANE() const;
		//判定用のRECTを返す
		COLRECT GetCOLRECT() const;

		//点との最近接点を返す
		void ClosestPtPoint(const Vector3& Point, Vector3& Ret);

		//点との距離を返す（戻り値がマイナスなら裏側）
		float GetDistPointPlane(const Vector3& Point) const;
		//ヒットテストをして最近接点と、Squareの法線を返す
		bool HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal);

		//操作
		virtual void OnUpdate() override{}
	};




}
//endof  basedx11
