#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SeekObject::SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_BaseY(m_StartPos.y),
		m_StateChangeSize(5.0f)
	{
	}
	SeekObject::~SeekObject(){}

	//������
	void SeekObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(0.125f, 0.25f, 0.25f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Seek����
		auto PtrSeek = AddComponent<SeekSteering>();
		//Arrive����
		auto PtrArrive = AddComponent<ArriveSteering>();
		//Arrive�͖����ɂ��Ă���
		PtrArrive->SetUpdateActive(false);

		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"ObjectGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<SeekObject>());
		//�����s��������
		AddComponent<SeparationSteering>(Group);
		//Obb�̏Փ˔��������
		auto PtrColl = AddComponent<CollisionObb>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"TRACE_TX");
		//��������������
		SetAlphaActive(true);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<SeekObject> >(GetThis<SeekObject>());
		//�ŏ��̃X�e�[�g��SeekFarState�ɐݒ�
		m_StateMachine->ChangeState(FarState::Instance());
	}

	//���[�e�B���e�B�֐��Q
	Vector3 SeekObject::GetPlayerPosition() const{
		//�����v���C���[��������������ĂȂ��ꍇ�ɂ́AVector3(0,m_BaseY,0)��Ԃ�
		Vector3 PlayerPos(0, m_BaseY, 0);
		auto PtrPlayer = GetStage()->GetSharedGameObject<Player>(L"Player", false);
		if (PtrPlayer){
			PlayerPos = PtrPlayer->GetComponent<Transform>()->GetPosition();
			PlayerPos.y = m_BaseY;
		}
		return PlayerPos;
	}
	float SeekObject::GetPlayerLength() const{
		auto MyPos = GetComponent<Transform>()->GetPosition();
		auto LenVec = GetPlayerPosition() - MyPos;
		return LenVec.Length();
	}

	//���[�V��������������֐��Q
	void  SeekObject::SeekStartMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
		PtrSeek->SetTargetPosition(GetPlayerPosition());

	}
	bool  SeekObject::SeekUpdateMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() <= m_StateChangeSize){
			return true;
		}
		return false;
	}
	void  SeekObject::SeekEndMoton(){
		auto PtrSeek = GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	void  SeekObject::ArriveStartMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetUpdateActive(true);
		PtrArrive->SetTargetPosition(GetPlayerPosition());
	}
	bool  SeekObject::ArriveUpdateMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		PtrArrive->SetTargetPosition(GetPlayerPosition());
		if (GetPlayerLength() > m_StateChangeSize){
			//�v���C���[�Ƃ̋��������ȏ�Ȃ�true
			return true;
		}
		return false;
	}
	void  SeekObject::ArriveEndMoton(){
		auto PtrArrive = GetComponent<ArriveSteering>();
		//Arrive�R���|�[�l���g�𖳌��ɂ���
		PtrArrive->SetUpdateActive(false);
	}

	//����
	void SeekObject::OnUpdate(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();
	}
	void SeekObject::OnLastUpdate(){
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//��]�̍X�V
		//Velocity�̒l�ŁA��]��ύX����
		//����Ői�s�����������悤�ɂȂ�
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f){
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//���݂̉�]���擾
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW���ԁi10����1�j
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
		//���y��m_BaseY
		auto Pos = PtrTransform->GetPosition();
		Pos.y = m_BaseY;
		PtrTransform->SetPosition(Pos);
	}
	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<FarState> FarState::Instance(){
		static shared_ptr<FarState> instance(new FarState);
		return instance;
	}
	void FarState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->SeekStartMoton();
	}
	void FarState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->SeekUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(NearState::Instance());
		}
	}
	void FarState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->SeekEndMoton();
	}

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	shared_ptr<NearState> NearState::Instance(){
		static shared_ptr<NearState> instance(new NearState);
		return instance;
	}
	void NearState::Enter(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveStartMoton();
	}
	void NearState::Execute(const shared_ptr<SeekObject>& Obj){
		if (Obj->ArriveUpdateMoton()){
			Obj->GetStateMachine()->ChangeState(FarState::Instance());
		}
	}
	void NearState::Exit(const shared_ptr<SeekObject>& Obj){
		Obj->ArriveEndMoton();
	}



	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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

	//������
	void FixedBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//��������������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//�Փ˔���
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	�p�r: �㉺�ړ�����{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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

	//������
	void MoveBox::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		AddComponent<PNTCollisionDraw>();


		//�A�N�V�����̓o�^
		auto PtrAction = AddComponent<Action>();
		PtrAction->AddMoveBy(5.0f, Vector3(5.0f, 5.0f, 0));
		PtrAction->AddMoveBy(5.0f, Vector3(-5.0f, -5.0f, 0));
		//���[�v����
		PtrAction->SetLooped(true);
		//�A�N�V�����J�n
		PtrAction->Run();



		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetTextureResource(L"SKY_TX");
		PtrDraw->SetOwnShadowActive(true);
	}


	//--------------------------------------------------------------------------------------
	//	class CapsuleObject : public GameObject;
	//	�p�r: ��Q���J�v�Z��
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CapsuleObject::CapsuleObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	CapsuleObject::~CapsuleObject(){}

	//������
	void CapsuleObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Capsule�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionCapsule>();
		CollPtr->SetFixed(true);
		//�R���W�����̃��C�A�t���[���\��
		AddComponent<PNTCollisionDraw>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CAPSULE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CAPSULE");
		PtrDraw->SetTextureResource(L"SKY_TX");

	}


	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	�p�r: ��Q����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	SphereObject::SphereObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}
	SphereObject::~SphereObject(){}
	//������
	void SphereObject::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRegid = AddComponent<Rigidbody>();
		//Sphere�̏Փ˔��������
		auto CollPtr = AddComponent<CollisionSphere>();
		CollPtr->SetFixed(true);
		//�R���W�����̃��C�A�t���[���\��
		AddComponent<PNTCollisionDraw>();

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"SKY_TX");
	}


	//--------------------------------------------------------------------------------------
	//	class HitTestSquare : public GameObject;
	//	�p�r: ���ƏՓ˔��肷��X�N�G�A
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	HitTestSquare::HitTestSquare(const shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos)
	{}
	HitTestSquare::~HitTestSquare(){}

	void HitTestSquare::OnCreate(){
		auto TrancePtr = AddComponent<Transform>();
		TrancePtr->SetScale(m_StartScale);
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(m_StartRotation);
		TrancePtr->SetQuaternion(Qt);
		TrancePtr->SetPosition(m_StartPos);
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetTextureResource(L"SKY_TX");
		DrawComp->SetOwnShadowActive(true);
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SQUARE");


		//�I�u�W�F�N�g�̃O���[�v�𓾂�
		auto Group = GetStage()->GetSharedObjectGroup(L"SquareGroup");
		//�O���[�v�Ɏ������g��ǉ�
		Group->IntoGroup(GetThis<HitTestSquare>());

	}

	PLANE HitTestSquare::GetPLANE() const{
		auto TrasnsPtr = GetComponent<Transform>();
		//�\�ʏ��3�̓_���g����PLANE���쐬
		//1�ڂ̓_�͒��S
		Vector3 Point0 = TrasnsPtr->GetPosition();
		//2�ڂ�-0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point1(-0.5f, -0.5f, 0);
		Point1.Transform(TrasnsPtr->GetWorldMatrix());
		//3�ڂ�0.5,-0.5,0�̓_�����[���h�ϊ���������
		Vector3 Point2(0.5f, -0.5f, 0);
		Point2.Transform(TrasnsPtr->GetWorldMatrix());
		//3�_���g���Ėʂ��쐬
		PLANE ret(Point0, Point1, Point2);
		return ret;
	}

	COLRECT HitTestSquare::GetCOLRECT() const{
		auto TrasnsPtr = GetComponent<Transform>();
		COLRECT rect(1.0f, 1.0f, TrasnsPtr->GetWorldMatrix());
		return rect;
	}


	//�_�Ƃ̍ŋߐړ_��Ԃ�
	void HitTestSquare::ClosestPtPoint(const Vector3& Point, Vector3& Ret){
		COLRECT rect = GetCOLRECT();
		Vector3 d = Point - rect.m_Center;
		Ret = rect.m_Center;
		for (int i = 0; i < 2; i++){
			float dist = Vector3EX::Dot(d, rect.m_Rot[i]);
			if (dist > rect.m_UVec[i]){
				dist = rect.m_UVec[i];
			}
			if (dist < -rect.m_UVec[i]){
				dist = -rect.m_UVec[i];
			}
			Ret += rect.m_Rot[i] * dist;
		}
	}


	//�_�Ƃ̋�����Ԃ��i�߂�l���}�C�i�X�Ȃ痠���j
	float HitTestSquare::GetDistPointPlane(const Vector3& Point) const{
		PLANE pl = GetPLANE();
		return (Vector3EX::Dot(Point, pl.m_Normal) - pl.m_DotValue) / Vector3EX::Dot(pl.m_Normal, pl.m_Normal);
	}

	//�q�b�g�e�X�g�����čŋߐړ_�ƁASquare�̖@����Ԃ�
	bool HitTestSquare::HitTestSphere(const SPHERE& Sp, Vector3& RetVec, Vector3& Normal){
		PLANE pl = GetPLANE();
		Normal = pl.m_Normal;
		//�l�p�`�Ƃ̍ŋߐړ_�𓾂�
		ClosestPtPoint(Sp.m_Center, RetVec);
		//�ŋߐړ_�����a�ȉ��Ȃ�Փ˂��Ă���
		if (Vector3EX::Length(Sp.m_Center - RetVec) <= Sp.m_Radius){
			return true;
		}
		return false;
	}

	//--------------------------------------------------------------------------------------
	//	class SimpleSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	SimpleSprite::SimpleSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos):
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0)
	{}
	SimpleSprite::~SimpleSprite(){}

	void SimpleSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(128.0f, 128.0f), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"WALL_TX");
		//�Z���^�[���_
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_CenterZeroPlusUpY);
		//���ʕ`��
		PtrSprite->SetRasterizerState(RasterizerState::CullNone);
	}

	void SimpleSprite::OnUpdate(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;

		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetScale(cos(m_TotalTime), 1.0f,1.0f);
	}



	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSprite::NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0){
	}
	NumberSprite::~NumberSprite(){}

	//������
	void NumberSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(128.0f,128.0f),Color4(1.0f, 1.0f, 1.0f, 1.0f));
		PtrSprite->SetTextureResource(L"NUMBER_TX");
		//��������
		SetAlphaActive(true);
		//���㌴�_
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftTopZeroPlusDownY);
		//�X�v���C�g�̒��̃��b�V������o�b�N�A�b�v�̎擾
		auto& SpVertexVec = PtrSprite->GetMeshResource()->GetBackupVerteces<VertexPositionColorTexture>();
		//�e�������Ƃ�UV�l���܂ޒ��_�f�[�^��z�񉻂��Ă���
		for (size_t i = 0; i < 10; i++){
			float from = ((float)i) / 10.0f;
			float to = from + (1.0f / 10.0f);
			vector<VertexPositionColorTexture> NumVirtex =
			{
				//���㒸�_
				VertexPositionColorTexture(
				SpVertexVec[0].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 0)
				),
				//�E�㒸�_
				VertexPositionColorTexture(
				SpVertexVec[1].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(to, 0)
				),
				//�������_
				VertexPositionColorTexture(
				SpVertexVec[2].position,
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(from, 1.0f)
				),
				//�E�����_
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
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		size_t Num = (size_t)m_TotalTime;
		Num = Num % 10;
		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto MeshRes = PtrSprite->GetMeshResource();
		//���I��UV�l���ς�钸�_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_NumberVertexVec[Num]);
	}

	//--------------------------------------------------------------------------------------
	//	class NumberSquare : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	NumberSquare::NumberSquare(shared_ptr<Stage>& StagePtr, const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number) :
		GameObject(StagePtr),
		m_SeekObject(SeekObjectPtr),
		m_Number(Number)
	{}
	NumberSquare::~NumberSquare(){}

	//������
	void NumberSquare::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		if (!m_SeekObject.expired()){
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
			PtrTransform->SetQuaternion(SeekTransPtr->GetQuaternion());
			//�ύX�ł���X�N�G�A���\�[�X���쐬

			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateSquare(1.0f, vertices, indices);
			//UV�l�̕ύX
			float from = ((float)m_Number) / 10.0f;
			float to = from + (1.0f / 10.0f);
			//���㒸�_
			vertices[0].textureCoordinate = Vector2(from, 0);
			//�E�㒸�_
			vertices[1].textureCoordinate = Vector2(to, 0);
			//�������_
			vertices[2].textureCoordinate = Vector2(from, 1.0f);
			//�E�����_
			vertices[3].textureCoordinate = Vector2(to, 1.0f);
			//���_�̌^��ς����V�������_���쐬
			vector<VertexPositionColorTexture> new_vertices;
			for (auto& v : vertices){
				VertexPositionColorTexture nv;
				nv.position = v.position;
				nv.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				nv.textureCoordinate = v.textureCoordinate;
				new_vertices.push_back(nv);
			}
			//�V�������_���g���ă��b�V�����\�[�X�̍쐬
			m_SquareMeshResource = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);

			auto DrawComp = AddComponent<PCTStaticDraw>();
			DrawComp->SetMeshResource(m_SquareMeshResource);
			DrawComp->SetTextureResource(L"NUMBER_TX");
			SetAlphaActive(true);
		}
	}


	//�ω�
	void NumberSquare::OnUpdate(){
		if (!m_SeekObject.expired()){
			auto SeekPtr = m_SeekObject.lock();
			auto SeekTransPtr = SeekPtr->GetComponent<Transform>();

			auto PtrTransform = GetComponent<Transform>();
			auto Pos = SeekTransPtr->GetPosition();
			Pos.y += 0.75f;
			PtrTransform->SetPosition(Pos);
			PtrTransform->SetScale(1.0f, 1.0f, 1.0f);

			auto PtrCamera = GetStage()->GetCamera(0);

			Quaternion Qt;
			//�������r���{�[�h�ɂ���
			Qt.Billboard(PtrCamera->GetAt() - PtrCamera->GetEye());

			//�������t�F�C�V���O�ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt.Facing(Pos - PtrCamera->GetEye());
			//�������t�F�C�V���OY�ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt.FacingY(Pos - PtrCamera->GetEye());
			//�������V�[�N�I�u�W�F�N�g�Ɠ����ɂ���ꍇ�͈ȉ��̂悤�ɂ���
			// Qt = SeekTransPtr->GetQuaternion();

			PtrTransform->SetQuaternion(Qt);

		}
	}

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	�p�r: �z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BarSprite::BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr), m_StartPos(StartPos), m_TotalTime(0.0f){
	}
	BarSprite::~BarSprite(){}

	//������
	void BarSprite::OnCreate(){
		auto PtrTransform = AddComponent<Transform>();
		PtrTransform->SetPosition(m_StartPos);
		PtrTransform->SetScale(1.0f, 1.0f, 1.0f);
		PtrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		//�X�v���C�g������
		//�����ɒ��_��ύX���Ă��܂��̂ŁA�T�C�Y�͓K���ł悢
		auto PtrSprite = AddComponent<PCTSpriteDraw>(Vector2(SpriteWidth, SpriteHeight), Color4(1.0f, 1.0f, 1.0f, 1.0f));
		//����
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, SpriteHeight, 0),
				Color4(1.0f, 1.0f, 1.0f, 1.0f),
				Vector2(0, 0))
			);
		//�E��
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, SpriteHeight, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 0))
			);
		//����
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(0.0f, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(0, 1.0f))
			);
		//�E��
		m_BarVirtexVec.push_back(
			VertexPositionColorTexture(
			Vector3(SpriteWidth, 0.0f, 0),
			Color4(1.0f, 1.0f, 1.0f, 1.0f),
			Vector2(1.0f, 1.0f))
			);
		//�X�v���C�g�̒��̃��b�V���̎擾
		auto MeshRes = PtrSprite->GetMeshResource();
		//���_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);

		PtrSprite->SetTextureResource(L"BAR_TX");
		//�������_
		PtrSprite->SetSpriteCoordinate(SpriteCoordinate::m_LeftBottomZeroPlusUpY);
	}

	//�ω�
	void BarSprite::OnUpdate(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 2.0f;
		//sin��-1.0����1.0�܂Ŏ�肤��̂Œ���
		float v = cos(m_TotalTime) * 0.5f + 0.5f;
		float y = SpriteHeight - SpriteHeight * v;

		m_BarVirtexVec[0].position.y = y;
		m_BarVirtexVec[0].textureCoordinate.y = v;
		m_BarVirtexVec[1].position.y = y;
		m_BarVirtexVec[1].textureCoordinate.y = v;

		auto PtrSprite = GetComponent<PCTSpriteDraw>();

		//�X�v���C�g�̒��̃��b�V���̎擾
		auto MeshRes = PtrSprite->GetMeshResource();
		//���_��ݒ肷��
		MeshRes->UpdateVirtexBuffer(m_BarVirtexVec);
	}


}
//endof  basedx11
