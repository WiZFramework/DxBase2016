#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	�Ǝ��̃��b�V�����\�[�X
	//--------------------------------------------------------------------------------------
	//���b�V�����\�[�X�̍쐬
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//Face����6
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
		//�P�ӂ̒����͂̔�����0.5
		float size = 0.5f;
		for (int i = 0; i < FaceCount; i++)
		{
			//�@��
			auto normal = faceNormals[i];
			auto basis = (i >= 4) ? Vector3(0, 0, 1) : Vector3(0, 1, 0);
			auto side1 = Vector3EX::Cross(normal, basis);
			auto side2 = Vector3EX::Cross(normal, side1);
			//�C���f�b�N�X�̓o�^
			size_t vbase = vertices.size();
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 1);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 3);
			//���_�̓o�^
			vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
			vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
		}
		//RH����LH�ɕύX
		if ((indices.size() % 3) != 0){
			throw BaseException(
				L"�C���f�b�N�X�̐��������܂���",
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
		//���_�ƃC���f�b�N�X�̔z�񂩂烁�b�V�����\�[�X���쐬�i���_��ύX�ł���j
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, true);
	}

	/*
	//��L�̊֐��͈ȉ��̂悤�ɂ�������
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		VertexUtil::CreateCube(1.0f, vertices, indices);
		//���_�ƃC���f�b�N�X�̔z�񂩂烁�b�V�����\�[�X���쐬�i���_��ύX�ł���j
		m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, true);
	}
	*/


	//���_�̕ύX
	void CustomDrawBox::UpdateMeshResource(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = m_MeshResource->GetVertexBuffer().Get();
		//�o�b�N�A�b�v�̒��_�����o��
		auto& BacukVertices = m_MeshResource->GetBackupVerteces<VertexPositionNormalTexture>();

		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawBox::UpdateMeshResource()"
				);
		}
		//���_�̕ύX
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;

		for (size_t i = 0; i < m_MeshResource->GetNumVertices(); i++){
			Vector3 Pos = BacukVertices[i].position;
			if (Pos.x > 0 && Pos.y > 0 && Pos.z > 0){
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y += sin(m_TotalTime);
			}
			else if (Pos.x < 0 && Pos.y > 0 && Pos.z < 0){
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y -= sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalTexture(
				Pos,
				BacukVertices[i].normal,
				BacukVertices[i].textureCoordinate
				);

		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



	//�\�z�Ɣj��
	CustomDrawBox::CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawBox::~CustomDrawBox(){
	}
	//������
	void CustomDrawBox::OnCreate(){
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		CreateMeshResource();
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(m_MeshResource);
		//�`��R���|�[�l���g�̒ǉ�
		auto DrawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		DrawComp->SetMeshResource(m_MeshResource);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		DrawComp->SetTextureResource(L"TRACE_TX");
		//���������i�`�揇����̂��߁j
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
