#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	独自のメッシュリソース
	//--------------------------------------------------------------------------------------
	//メッシュリソースの作成
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//Face数は6
		const int FaceCount = 6;
		static const Vector3 faceNormals[FaceCount] =
		{
			{ 0, 0, 1 },
			{ 0, 0, -1 },
			{ 1, 0, 0 },
			{ -1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, -1, 0 },
		};
		static const Vector2 textureCoordinates[4] =
		{
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			{ 0, 0 },
		};
		//１辺の長さはの半分は0.5
		float size = 0.5f;
		for (int i = 0; i < FaceCount; i++)
		{
			//法線
			auto normal = faceNormals[i];
			auto basis = (i >= 4) ? Vector3(0, 0, 1) : Vector3(0, 1, 0);
			auto side1 = Vector3EX::Cross(normal, basis);
			auto side2 = Vector3EX::Cross(normal, side1);
			//インデックスの登録
			size_t vbase = vertices.size();
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 1);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 3);
			//頂点の登録
			vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
			vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
		}
		//RHからLHに変更
		if ((indices.size() % 3) != 0){
			throw BaseException(
				L"インデックスの数が合いません",
				L"if((indices.size() % 3) == 0)",
				L"CustomDrawBox::CreateMeshResource()"
				);
		}
		for (auto it = indices.begin(); it != indices.end(); it += 3)
		{
			std::swap(*it, *(it + 2));
		}
		for (auto it = vertices.begin(); it != vertices.end(); ++it)
		{
			it->textureCoordinate.x = (1.f - it->textureCoordinate.x);
		}
		//頂点とインデックスの配列からメッシュリソースを作成（頂点を変更できる）
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, true);
	}

	/*
	//上記の関数は以下のようにも書ける
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		VertexUtil::CreateCube(1.0f, vertices, indices);
		//頂点とインデックスの配列からメッシュリソースを作成（頂点を変更できる）
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, true);
	}
	*/


	//頂点の変更
	void CustomDrawBox::UpdateMeshResource(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = m_MeshResource->GetVertexBuffer().Get();
		//バックアップの頂点を取り出す
		auto& BacukVertices = m_MeshResource->GetBackupVerteces<VertexPositionNormalTexture>();

		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawBox::UpdateMeshResource()"
				);
		}
		//頂点の変更
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;

		for (size_t i = 0; i < m_MeshResource->GetNumVertices(); i++){
			Vector3 Pos = BacukVertices[i].position;
			if (Pos.x > 0 && Pos.y > 0 && Pos.z > 0){
				//サインを使っていったり来たりするようにする
				Pos.y += sin(m_TotalTime);
			}
			else if (Pos.x < 0 && Pos.y > 0 && Pos.z < 0){
				//サインを使っていったり来たりするようにする
				Pos.y -= sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalTexture(
				Pos,
				BacukVertices[i].normal,
				BacukVertices[i].textureCoordinate
				);

		}
		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



	//構築と破棄
	CustomDrawBox::CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawBox::~CustomDrawBox(){
	}
	//初期化
	void CustomDrawBox::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//頂点バッファとインデックスバッファを作成
		CreateMeshResource();
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(m_MeshResource);
		//描画コンポーネントの追加
		auto DrawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(m_MeshResource);
		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"TRACE_TX");
		//透明処理（描画順制御のため）
		SetAlphaActive(true);
	}
	void CustomDrawBox::OnUpdate(){
		UpdateMeshResource();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}

}
//endof  basedx11
