#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct SplitBoxData;
	//	用途:分割されたオブジェクトのデータ
	//--------------------------------------------------------------------------------------
	struct SplitBoxData{
		bool m_Enabled;	//有効かどうか
		float m_Time;	//経過時間
		Vector3 m_Scale;	//現在のスケール
		Quaternion m_Qt;	//現在の回転
		Vector3 m_Pos;	//現在の位置
		Vector3 m_Velocity;	//速度
		Quaternion m_QtVelocity;	//回転速度
		Vector3 m_GravityVelocity;	//重力による速度
		SplitBoxData(const Vector3&scale, const Vector3&rot, const Vector3&pos, const Vector3&velo, const Vector3&rotvelo) :
			m_Enabled(true),
			m_Time(0),
			m_Scale(scale),
			m_Qt(),
			m_Pos(pos),
			m_Velocity(velo),
			m_QtVelocity(),
			m_GravityVelocity(0.0f, 0.0f, 0.0f)
		{
			m_Qt.RotationRollPitchYawFromVector(rot);
			m_QtVelocity.RotationRollPitchYawFromVector(rotvelo);
		}
		void UpdateData(float timespan){
			if (m_Enabled){
				m_Time += timespan;
				m_Pos += m_Velocity * timespan;
				m_GravityVelocity += Vector3(0, -9.8f, 0) * timespan;
				m_Pos += m_GravityVelocity * timespan;
				//回転の追加
				m_Qt *= m_QtVelocity;
				//m_PosのYが0になるまで更新する
				//場合によってはm_PosのYが0ではないケースも設定できる
				//砕けた状態を少し見せたい場合は、m_Time条件で消す形にもできる
				//その場合、インスタンスのMAXに引っかからないよう注意
				if (m_Pos.y <= 0.0f || m_Time >= 1.0f){
					m_Enabled = false;
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		shared_ptr< StateMachine<FixedBox> >  m_StateMachine;	//ステートマシーン
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		//分割状態の時のデータの配列
		vector<SplitBoxData> m_SplitBoxDataVec;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//アクセサ
		shared_ptr< StateMachine<FixedBox> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		virtual void OnLastUpdate() override;
		//モーション系
		//プレイヤーがヒットしたか
		bool IsPlayerHit() const;
		//分割用の行列配列の作成
		void CreateSplitVec(size_t splitcount);
		//分割用の行列配列の更新
		void UpdateSplitVec();
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxDefaultState : public ObjState<FixedBox>;
	//	用途: 通常
	//--------------------------------------------------------------------------------------
	class FixedBoxDefaultState : public ObjState<FixedBox>
	{
		FixedBoxDefaultState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<FixedBoxDefaultState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxSplitState : public ObjState<FixedBox>;
	//	用途: 分割状態
	//--------------------------------------------------------------------------------------
	class  FixedBoxSplitState : public ObjState<FixedBox>
	{
		FixedBoxSplitState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr< FixedBoxSplitState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	struct InstanceConstantBuffer;
	//	用途: インスタンスシャドウマップ用コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct InstanceShadowmapConstantBuffer
	{
		XMMATRIX mView;
		XMMATRIX mProj;
		InstanceShadowmapConstantBuffer() {
			memset(this, 0, sizeof(InstanceShadowmapConstantBuffer));
		};
	};
	//シェーダ宣言(マクロ使用)
	DECLARE_DX11_CONSTANT_BUFFER(CBInstanceShadowmap, InstanceShadowmapConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSInstanceShadowmap, VertexPositionNormalTextureMatrix)

	//--------------------------------------------------------------------------------------
	//	struct InstanceConstantBuffer;
	//	用途: インスタンス描画用コンスタントバッファ構造体
	//--------------------------------------------------------------------------------------
	struct InstanceConstantBuffer
	{
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//汎用パラメータ
		Vector4 LPos;
		Vector4 EyePos;
		Matrix4X4 LView;
		Matrix4X4 LProjection;
		InstanceConstantBuffer() {
			memset(this, 0, sizeof(InstanceConstantBuffer));
		};
	};
	//シェーダ宣言(マクロ使用)
	DECLARE_DX11_CONSTANT_BUFFER(CBInstance, InstanceConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSInstance, VertexPositionNormalTextureMatrix)
	DECLARE_DX11_PIXEL_SHADER(PSInstance)


	//--------------------------------------------------------------------------------------
	//	class InstanceShadowmap : public Shadowmap;
	//	用途: インスタンスシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	class InstanceShadowmap : public Shadowmap{
	public:
		explicit InstanceShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~InstanceShadowmap();
		//操作
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	//	class DrawBoxManager : public GameObject;
	//	用途: 固定のボックスの描画マネージャ
	//--------------------------------------------------------------------------------------
	class DrawBoxManager : public GameObject{
		//インスタンスの最大値
		const size_t m_MaxInstance;
		wstring m_MeshKey;
		wstring m_TextureKey;
		vector<Matrix4X4> m_InstanceVec;
		//行列用の頂点バッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//行列用の頂点バッファの作成
		void CreateMatrixBuffer();
	public:
		//構築と破棄
		DrawBoxManager(const shared_ptr<Stage>& StagePtr, size_t MaxInstance, const wstring& MeshKey, const wstring& TextureKey);
		virtual ~DrawBoxManager();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		const ComPtr<ID3D11Buffer>& GetMatrixBuffer() const{
			return m_MatrixBuffer;
		}
		const vector<Matrix4X4>& GetInstanceVec(){
			return m_InstanceVec;
		}
		//操作
		//スケール、回転、位置で追加
		void AddInstanceVectors(const Vector3& Scale, const Vector3& Rot, const Vector3& Pos);
		//行列で追加
		void AddInstanceMatrix(const Matrix4X4& Mat);
		//仮想関数
		virtual void OnLastUpdate() override;
		virtual void OnDraw() override;
	};

}
//endof  basedx11
