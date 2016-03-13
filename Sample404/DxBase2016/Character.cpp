#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(shared_ptr<Stage>& StagePtr,
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

		//操舵系のコンポーネントをつける場合はRigidbodyをつける
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//各ボックスはDrawしない
		//マネージャによって一つのメッシュにまとめられ描画される
		SetDrawActive(false);

		//オブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"FIXED_BOX");
		//グループに自分自身を追加
		Group->IntoGroup(GetThis<FixedBox>());

	}


	//--------------------------------------------------------------------------------------
	//	class FixedBoxManager : public GameObject;
	//	用途: 固定のボックスのマネージャ（描画する）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBoxManager::FixedBoxManager(shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	FixedBoxManager::~FixedBoxManager(){}

	//頂点とインデックスを行列と、ナンバーに合わせて変更する
	void FixedBoxManager::ChangeVertex(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, const Matrix4X4& Mat, size_t Num){
		auto GStage = dynamic_pointer_cast<GameStage>(GetStage());

		float XScale = Mat.ScaleInMatrix().x / GStage->GetPiece().x;
		float ZScale = Mat.ScaleInMatrix().z / GStage->GetPiece().z;
		for (auto& V : vertices){
			//頂点を行列により変更する
			V.position = Vector3EX::Transform(V.position, Mat);
			//UV値の変更
			//タイリングの実装
			if (V.normal.x == 0.0f){
				if (V.textureCoordinate.x != 0){
					V.textureCoordinate.x = XScale;
				}
				if (V.textureCoordinate.y != 0){
					if (V.normal.z != 0.0f){
						V.textureCoordinate.y = 1.0f;
					}
					else{
						V.textureCoordinate.y = ZScale;
					}
				}
			}
			else{
				if (V.textureCoordinate.x != 0){
					V.textureCoordinate.x = ZScale;
				}
				if (V.textureCoordinate.y != 0){
					V.textureCoordinate.y = 1.0f;
				}
			}
		}
		//頂点の数を得る
		size_t sz = vertices.size();
		for (auto& I : indices){
			//インデックスをNum（ボックスのナンバー）に合わせて変更
			I = (Num * sz) + I;
		}
	}


	//初期化
	void FixedBoxManager::OnCreate(){
		//FixBoxのグループを一つのメッシュにまとめる
		//こうすることでマップのDrawは、一回の描画命令で描画される
		//トランスフォームは原点に置く
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		PtrTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		PtrTransform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		//新しく作成する頂点の配列
		vector<VertexPositionNormalTexture> NewCubeVerteces;
		//新しく作成するインデックスの配列
		vector<uint16_t> NewCubeIndices;
		//FIXEDBOXオブジェクトのグループを得る
		auto Group = GetStage()->GetSharedObjectGroup(L"FIXED_BOX");
		auto Vec = Group->GetGroupVector();
		for (size_t Index = 0; Index < Vec.size(); Index++){

			if (!Vec[Index].expired()){
				//グループ配列はweakptrなのでshared_ptrにキャスト
				auto ShPtr = Vec[Index].lock();
				//FIXEDボックスのそれぞれの行列を得る
				auto Mat = ShPtr->GetComponent<Transform>()->GetWorldMatrix();
				vector<VertexPositionNormalTexture> CubeVerteces;
				vector<uint16_t> CubeIndices;
				//基準となるCUBEの作成(デフォルトと同じ大きさ)
				VertexUtil::CreateCube(1.0f, CubeVerteces, CubeIndices);
				//ボックスのインデックスにより頂点情報を変更
				ChangeVertex(CubeVerteces, CubeIndices, Mat, Index);
				//頂点とインデックスを新しい配列に追加
				for (auto V : CubeVerteces){
					NewCubeVerteces.push_back(V);
				}
				for (auto I : CubeIndices){
					NewCubeIndices.push_back(I);
				}
			}
		}
		//メッシュリソースの作成
		m_BoxesRes = MeshResource::CreateMeshResource(NewCubeVerteces, NewCubeIndices,false);
		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(m_BoxesRes);

		//描画コンポーネント
		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(m_BoxesRes);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//ステータスのポインタ
		auto RenderStatePtr = GetStage()->GetRenderState();
		//タイリングの設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetOwnShadowActive(true);
	}


}
//endof  basedx11
