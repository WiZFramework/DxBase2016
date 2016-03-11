#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//初期化
	void FixedBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//反発を実装する場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}

	MoveBox::~MoveBox(){}

	//初期化
	void MoveBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();


		//アクションの登録
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//ループする
		PtrAction->SetLooped(true);
		//アクション開始
		PtrAction->Run();



		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	NumberSprite::NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0){
	}
	NumberSprite::~NumberSprite(){}

	//初期化
	void NumberSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//スプライトをつける
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(128.0f, 128.0f), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"NUMBER_TX");
		//透明処理
		SetAlphaActive(true);
		//左上原点
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftTopZeroPlusDownY);
		//スプライトの中のメッシュからバックアップの取得
		auto& SpVertexVec = PtrSprite->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		//各数字ごとにUV値を含む頂点データを配列化しておく
		for (size_t i = 0; i < 10; i++){
			float from = ((float)i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//左上頂点
				VertexPositionColorTexture(
				SpVertexVec[0].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 0)
				),
				//右上頂点
				VertexPositionColorTexture(
				SpVertexVec[1].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 0)
				),
				//左下頂点
				VertexPositionColorTexture(
				SpVertexVec[2].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 1.0f)
				),
				//右下頂点
				VertexPositionColorTexture(
				SpVertexVec[3].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 1.0f)
				),
			};
			m_NumberVertexVec.push_back(NumVirtex);
		}
	}

	void NumberSprite::OnUpdate(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		size_t Num = (size_t)m_TotalTime;
		Num = Num % 10;
		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto MeshRes = PtrSprite->GetMeshResource();
		//動的にUV値が変わる頂点を設定する
		MeshRes->UpdateVirtexBuffer(m_NumberVertexVec[Num]);
	}
	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	BarSprite::BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0.0f){
	}
	BarSprite::~BarSprite(){}

	//初期化
	void BarSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//スプライトをつける
		//すぐに頂点を変更してしまうので、サイズは適当でよい
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(SpriteWidth, SpriteHeight), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		//左上
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, SpriteHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 0))
			);
		//右上
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, SpriteHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 0))
			);
		//左下
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f))
			);
		//右下
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f))
			);
		//スプライトの中のメッシュの取得
		auto MeshRes = PtrSprite->GetMeshResource();
		//頂点を設定する
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);

		PtrSprite->SetTextureResource(L"BAR_TX");
		//左下原点
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftBottomZeroPlusUpY);
	}

	//変化
	void BarSprite::OnUpdate(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 2.0f;
		//sinは-1.0から1.0まで取りうるので調整
		float v = cos(m_TotalTime) * 0.5f + 0.5f;
		float y = SpriteHeight - SpriteHeight * v;

		m_BarVirtexVec[0].position.y = y;
		m_BarVirtexVec[0].textureCoordinate.y = v;
		m_BarVirtexVec[1].position.y = y;
		m_BarVirtexVec[1].textureCoordinate.y = v;

		auto PtrSprite = GetComponent<PCTSpriteDraw>();

		//スプライトの中のメッシュの取得
		auto MeshRes = PtrSprite->GetMeshResource();
		//頂点を設定する
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);
	}


}
//endof  basedx11
