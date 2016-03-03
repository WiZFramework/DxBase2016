
#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct GameObject::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct GameObject::Impl{
		bool m_UpdateActive;	//updateするかどうか
		bool m_DrawActive;		//Drawするかどうか
		bool m_AlphaActive;		//透明かどうか
		//半透明処理をするかどうか（m_AlphaActiveがtrueでなければ意味がない）
		bool m_AlphaExActive;

		bool m_SpriteDraw;
		int m_DrawLayer;	//描画レイヤー

		weak_ptr<Stage> m_Stage;	//所属ステージ
		map<type_index, shared_ptr<Component> > m_CompMap;
		shared_ptr<Rigidbody> m_Rigidbody;	//Rigidbodyは別にする
		shared_ptr<Gravity> m_Gravity;	//Gravityは別にする
		shared_ptr<TransformMatrix> m_TransformMatrix;	//TransformMatrixも別にする
		shared_ptr<Collision> m_Collision;	//Collisionも別にする

		list<type_index> m_CompOrder;	//コンポーネント実行順番



		Impl(const shared_ptr<Stage>& StagePtr) :
			m_UpdateActive(true),
			m_DrawActive(true),
			m_AlphaActive(false),
			m_AlphaExActive(false),
			m_SpriteDraw(false),
			m_DrawLayer(0),
			m_Stage(StagePtr)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObject :public Object, public ShapeInterface;
	//	用途: ゲーム配置オブジェクト親クラス
	//--------------------------------------------------------------------------------------
	//privateメンバ
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const{
		auto it = pImpl->m_CompMap.find(TypeIndex);
		if (it != pImpl->m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}
	shared_ptr<Rigidbody> GameObject::GetRigidbody()const{
		return pImpl->m_Rigidbody;
	}
	shared_ptr<Gravity> GameObject::GetGravity()const{
		return pImpl->m_Gravity;
	}

	shared_ptr<TransformMatrix> GameObject::GetTransformMatrix()const {
		return pImpl->m_TransformMatrix;

	}
	shared_ptr<Transform> GameObject::GetTransform()const {
		return dynamic_pointer_cast<Transform>(pImpl->m_TransformMatrix);
	}
	shared_ptr<Collision> GameObject::GetCollision()const{
		return pImpl->m_Collision;
	}
	shared_ptr<CollisionSphere> GameObject::GetCollisionSphere()const{
		return dynamic_pointer_cast<CollisionSphere>(pImpl->m_Collision);
	}

	shared_ptr<CollisionCapsule> GameObject::GetCollisionCapsule()const{
		return dynamic_pointer_cast<CollisionCapsule>(pImpl->m_Collision);
	}

	shared_ptr<CollisionObb> GameObject::GetCollisionObb()const{
		return dynamic_pointer_cast<CollisionObb>(pImpl->m_Collision);
	}


	void GameObject::SetRigidbody(const shared_ptr<Rigidbody>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Rigidbody = Ptr;
	}
	void GameObject::SetGravity(const shared_ptr<Gravity>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Gravity = Ptr;
	}
	void GameObject::SetCollision(const shared_ptr<Collision>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}
	void GameObject::SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionObb(const shared_ptr<CollisionObb>& Ptr){
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetTransformMatrix(const shared_ptr<TransformMatrix>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_TransformMatrix = Ptr;
	}

	void GameObject::SetTransform(const shared_ptr<Transform>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_TransformMatrix = Ptr;
	}
	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr){
		if (!SearchComponent(TypeIndex)){
			//そのコンポーネントがまだなければ新規登録
			pImpl->m_CompOrder.push_back(TypeIndex);
		}
		//mapに追加もしくは更新
		pImpl->m_CompMap[TypeIndex] = Ptr;
		Ptr->AttachGameObject(GetThis<GameObject>());
	}
	map<type_index, shared_ptr<Component> >& GameObject::GetCompoMap()const {
		return pImpl->m_CompMap;
	}

	void GameObject::RemoveTgtComponent(type_index TypeIndex){
		//順番リストを検証して削除
		auto it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			if (*it == TypeIndex){
				auto it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//指定の型のコンポーネントが見つかった
					//mapデータを削除
					pImpl->m_CompMap.erase(it2);
				}
				pImpl->m_CompOrder.erase(it);
				return;
			}
			it++;
		}
	}




	//構築と破棄

	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		Object(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}
	GameObject::~GameObject(){}
	//アクセサ
	bool GameObject::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool GameObject::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void GameObject::SetUpdateActive(bool b){ pImpl->m_UpdateActive = b; }

	bool GameObject::IsDrawActive() const { return pImpl->m_DrawActive; }
	bool GameObject::GetDrawActive() const { return pImpl->m_DrawActive; }
	void GameObject::SetDrawActive(bool b){ pImpl->m_DrawActive = b; }

	bool GameObject::IsAlphaActive() const { return pImpl->m_AlphaActive; }
	bool GameObject::GetAlphaActive() const { return pImpl->m_AlphaActive; }
	void GameObject::SetAlphaActive(bool b){ pImpl->m_AlphaActive = b; }

	bool GameObject::IsAlphaExActive() const{
		return pImpl->m_AlphaExActive;
	}
	bool GameObject::GetAlphaExActive() const{
		return pImpl->m_AlphaExActive;
	}
	void GameObject::SetAlphaExActive(bool b){
		pImpl->m_AlphaExActive = b;
	}


	//スプライトとしてDrawするかどうか
	bool  GameObject::IsSpriteDraw() const{
		return pImpl->m_SpriteDraw;

	}
	bool  GameObject::GetSpriteDraw() const{
		return pImpl->m_SpriteDraw;
	}
	void  GameObject::SetSpriteDraw(bool b){
		pImpl->m_SpriteDraw = b;
	}
	//描画レイヤーの取得と設定
	int GameObject::GetDrawLayer() const{
		return pImpl->m_DrawLayer;
	}
	void  GameObject::SetDrawLayer(int l){
		pImpl->m_DrawLayer = l;
	}

	shared_ptr<Stage> GameObject::GetStage() const {
		if (!pImpl->m_Stage.expired()){
			return pImpl->m_Stage.lock();
		}
		return nullptr;
	}
	void GameObject::SetStage(const shared_ptr<Stage>& stage){ pImpl->m_Stage = stage; }

	void GameObject::OnPreCreate(){
		//TransformMatrix必須
		AddComponent<TransformMatrix>();
	}

	void GameObject::ComponentUpdate(){
		//Transformかどうかを検証
		auto Tptr = GetComponent<Transform>(false);
		if (Tptr) {
			auto RigidPtr = GetComponent<Rigidbody>(false);
			auto GravityPtr = GetComponent<Gravity>(false);
			if (RigidPtr) {
				//Rigidbodyがあればフォースを初期化
				RigidPtr->SetForce(0, 0, 0);
			}
			//マップを検証してUpdate
			list<type_index>::iterator it = pImpl->m_CompOrder.begin();
			while (it != pImpl->m_CompOrder.end()) {
				map<type_index, shared_ptr<Component> >::const_iterator it2;
				it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//指定の型のコンポーネントが見つかった
					if (it2->second->IsUpdateActive()) {
						it2->second->OnUpdate();
					}
				}
				it++;
			}
			if (RigidPtr && RigidPtr->IsUpdateActive()) {
				//RigidbodyがあればUpdate()
				RigidPtr->OnUpdate();
			}
			if (GravityPtr && GravityPtr->IsUpdateActive()) {
				//GravityPtrがあればUpdate()
				GravityPtr->OnUpdate();
			}
			//TransformのUpdate
			if (Tptr->IsUpdateActive()) {
				Tptr->OnUpdate();
			}
			auto CollisionPtr = GetComponent<Collision>(false);
			if (CollisionPtr && CollisionPtr->IsUpdateActive()) {
				//CollisionがあればUpdate()
				CollisionPtr->OnUpdate();
			}
		}
		else {
			auto TMptr = GetComponent<TransformMatrix>();
			//マップを検証してUpdate
			list<type_index>::iterator it = pImpl->m_CompOrder.begin();
			while (it != pImpl->m_CompOrder.end()) {
				map<type_index, shared_ptr<Component> >::const_iterator it2;
				it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//指定の型のコンポーネントが見つかった
					if (it2->second->IsUpdateActive()) {
						it2->second->OnUpdate();
					}
				}
				it++;
			}
			//TransformMatrixのUpdate
			if (TMptr->IsUpdateActive()) {
				TMptr->OnUpdate();
			}
		}
	}

	void GameObject::CollisionChk() {
		//Transformかどうかを検証
		auto Tptr = GetComponent<Transform>(false);
		if (Tptr) {
			auto CollisionPtr = GetComponent<Collision>(false);
			if (CollisionPtr && CollisionPtr->GetHitObject()) {
				//何かとヒットしてた
				OnCollision(CollisionPtr->GetHitObject());
			}
		}
	}

	void GameObject::ComponentUpdate2(){
		//Transformかどうかを検証
		auto Tptr = GetComponent<Transform>(false);
		if (Tptr) {
			auto GravityPtr = GetComponent<Gravity>(false);
			if (GravityPtr && GravityPtr->IsUpdate2Active()) {
				//GravityPtrがあればUpdate2()
				GravityPtr->Update2();
			}
			auto CollisionPtr = GetComponent<Collision>(false);
			if (CollisionPtr && CollisionPtr->IsUpdate2Active()) {
				//CollisionがあればUpdate2()
				CollisionPtr->Update2();
			}
		}
	}

	void GameObject::DrawShadowmap(){
		auto shadowptr = GetDynamicComponent<Shadowmap>(false);
		if (shadowptr){
			shadowptr->OnDraw();
		}
	}


	void GameObject::ComponentDraw(){
		//TransformMatrixがなければ例外
		auto Tptr = GetComponent<TransformMatrix>();
		//マップを検証してDraw
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()){
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (!dynamic_pointer_cast<Shadowmap>(it2->second)){
					//シャドウマップ以外なら実行
					//そのコンポーネントの子コンポーネントの描画
					if (it2->second->IsDrawActive()){
						it2->second->OnDraw();
					}
				}
			}
			it++;
		}
		auto RigidPtr = GetComponent<Rigidbody>(false);
		//RigidbodyのDraw
		//Rigidbodyの派生クラス対策
		if (RigidPtr && RigidPtr->IsDrawActive()){
			//RigidbodyがあればDraw()
			RigidPtr->OnDraw();
		}
		//TransformMatrixのDraw
		//TransformMatrixの派生クラス対策
		if (Tptr->IsDrawActive()){
			Tptr->OnDraw();
		}
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsDrawActive()){
			//CollisionがあればDraw()
			CollisionPtr->OnDraw();
		}
	}

	void GameObject::OnDraw(){
		//コンポーネント描画
		//派生クラスで多重定義する場合は
		//コンポーネント描画する場合は
		//GameObject::Draw()を呼び出す
		ComponentDraw();
	}



	//--------------------------------------------------------------------------------------
	//	struct GameObjectGroup::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct GameObjectGroup::Impl{
		vector< weak_ptr<GameObject> > m_Group;
		Impl(){}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup;
	//	用途: ゲームオブジェクトのweak_ptrをグループ化したもの
	//--------------------------------------------------------------------------------------
	GameObjectGroup::GameObjectGroup():
		Object(),
		pImpl(new Impl())
	{}
	GameObjectGroup::~GameObjectGroup(){}
	//アクセサ
	const vector< weak_ptr<GameObject> >& GameObjectGroup::GetGroupVector() const{
		return pImpl->m_Group;
	}
	shared_ptr<GameObject> GameObjectGroup::at(size_t index){
		if (index >= pImpl->m_Group.size()){
			wstring msg = Util::UintToWStr(index);
			msg += L" >= ";
			msg += Util::UintToWStr(pImpl->m_Group.size());
			throw BaseException(
				L"インデックスが範囲外です",
				msg,
				L"GameObjectGroup::at()"
				);
		}
		if (pImpl->m_Group.at(index).expired()){
			wstring msg = Util::UintToWStr(index);
			throw BaseException(
				L"そのインデックスのオブジェクトは無効です。",
				msg,
				L"GameObjectGroup::at()"
				);
		}
		return pImpl->m_Group.at(index).lock();
	}
	size_t GameObjectGroup::size() const{
		return pImpl->m_Group.size();
	}
	//操作
	void GameObjectGroup::IntoGroup(const shared_ptr<GameObject>& Obj){
		pImpl->m_Group.push_back(Obj);
	}

	void GameObjectGroup::AllClear(){
		pImpl->m_Group.clear();
	}


	//--------------------------------------------------------------------------------------
	//	struct Particle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Particle::Impl{
		DrawOption m_DrawOption;		//表示オプション
		vector<ParticleSprite> m_ParticleSpriteVec;	//保存しておくスプライトの配列
		Vector3 m_EmitterPos;			//エミッター位置
		float m_TotalTime;				//タイマー制御する場合に使用する変数
		float m_MaxTime;				//このパーティクル集合体の表示時間
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャ
		Impl(size_t Count, DrawOption Option):
			m_DrawOption(Option),
			m_ParticleSpriteVec(Count),
			m_EmitterPos(0,0,0),
			m_TotalTime(0),
			m_MaxTime(0)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Particle;
	//	用途: パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, DrawOption Option) :
		Object(),
		pImpl(new Impl(Count,Option))
	{}
	Particle::~Particle(){}

	Particle::DrawOption Particle::GetDrawOption()const{
		return pImpl->m_DrawOption;
	}
	void Particle::SetDrawOption(DrawOption Option){
		pImpl->m_DrawOption = Option;
	}

	const Vector3& Particle::GetEmitterPos() const{
		return pImpl->m_EmitterPos;
	}
	void Particle::SetEmitterPos(const Vector3& Pos){
		pImpl->m_EmitterPos = Pos;
	}

	float Particle::GetTotalTime() const{
		return pImpl->m_TotalTime;
	}
	void Particle::SetTotalTime(float f){
		pImpl->m_TotalTime = f;
	}
	void Particle::AddTotalTime(float f){
		pImpl->m_TotalTime += f;
	}

	float Particle::GetMaxTime() const{
		return pImpl->m_MaxTime;
	}
	void Particle::SetMaxTime(float f){
		pImpl->m_MaxTime = f;
	}



	bool Particle::IsActive() const{
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (Psp.m_Active){
				//1つでもアクティブがあればtrue
				return true;
			}
		}
		return false;
	}
	bool Particle::IsAllActive() const{
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (!Psp.m_Active){
				//1つでも非アクティブがあればfalse
				return false;
			}
		}
		return true;
	}
	void Particle::SetAllActive(){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.m_Active = true;
		}
	}
	void Particle::SetAllNoActive(){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.m_Active = false;
		}
	}

	void Particle::Reflesh(size_t Count, Particle::DrawOption Option){
		pImpl->m_DrawOption = Option;
		pImpl->m_EmitterPos = Vector3(0, 0, 0);
		pImpl->m_TotalTime = 0;
		pImpl->m_MaxTime = 0;
		pImpl->m_ParticleSpriteVec.clear();
		pImpl->m_ParticleSpriteVec.resize(Count);
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			Psp.Reflesh();
		}
	}

	vector<ParticleSprite>& Particle::GetParticleSpriteVec() const{
		return pImpl->m_ParticleSpriteVec;
	}

	shared_ptr<TextureResource> Particle::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"Particle::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void Particle::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"Particle::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Particle::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void Particle::Draw(const shared_ptr<ParticleManager>& Manager){
		for (auto Psp : pImpl->m_ParticleSpriteVec){
			if (Psp.m_Active && !pImpl->m_TextureResource.expired()){
				Manager->AddParticle(Psp, GetDrawOption(),
					GetEmitterPos(), pImpl->m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiParticle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiParticle::Impl{
		vector< shared_ptr<Particle> > m_ParticleVec;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	用途: マルチエフェクト 
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	MultiParticle::~MultiParticle(){}

	vector< shared_ptr<Particle> >& MultiParticle::GetParticleVec() const{
		return pImpl->m_ParticleVec;
	}


	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, Particle::DrawOption Option){
		for (size_t i = 0; i < pImpl->m_ParticleVec.size(); i++){
			//もし非アクティブのパーティクルがあれば初期化してリターン
			if (!pImpl->m_ParticleVec[i]->IsActive()){
				pImpl->m_ParticleVec[i]->Reflesh(Count, Option);
				return pImpl->m_ParticleVec[i];
			}
		}
		//新しいパーティクルを追加
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		pImpl->m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}

	void MultiParticle::OnUpdate(){
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()){
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()){
				if (rParticleSprite.m_Active){
					//移動速度に従って移動させる
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->GetTotalTime() >= ParticlePtr->GetMaxTime()){
						//制限時間になったら
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw(){
		if (pImpl->m_ParticleVec.size() > 0){
			for (auto Ptr : pImpl->m_ParticleVec){
				if (Ptr->IsActive()){
					Ptr->Draw(GetStage()->GetParticleManager());
				}
			}
		}
	}





	//Draw準備のための構造体
	struct ParticleSpriteForDraw{
		float m_ToCaneraLength;						//パーティクルのカメラまでの距離
		VertexPositionColorTexture m_Vertex[4];	//ローカル変換済みの頂点（描画に直接使用）
		shared_ptr<TextureResource> m_TextureRes;					//テクスチャ
		ParticleSpriteForDraw() :
			m_ToCaneraLength(0)
		{}
	};

	//中間的な構造体
	struct ParticleDrawSetter{
		shared_ptr<TextureResource> m_TextureRes;			//設定するテクスチャ
		UINT StartIndex;			//描画開始インデックス
		UINT DrawCount;				//描画数
	};


	//--------------------------------------------------------------------------------------
	//	struct ParticleManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ParticleManager::Impl{
		size_t m_ParticleCount;			//現在管理しているパーティクルの数
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
		bool m_ZBufferUse;				//Zバッファを使用するかどうか
		bool m_AlphaBlendSrcOne;	//透明処理のSRC_ONE設定
		bool m_SamplerWrap;				//サンプラーのラッピングするかどうか

		//Draw準備のための構造体の配列
		vector<ParticleSpriteForDraw> m_ParticleSpriteForDrawVec;
		Impl():
			m_ParticleCount{ 256 },
			m_NumVertices{ 0 },
			m_NumIndicis{ 0 },
			m_ZBufferUse{ true },
			m_AlphaBlendSrcOne(false),
			m_SamplerWrap{ false }
		{}
		~Impl(){}
		//頂点バッファの作成
		void CreateParticleBuffers();
	};
	void ParticleManager::Impl::CreateParticleBuffers(){
		try{
			//頂点を作成するための配列
			vector<VertexPositionColorTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			XMFLOAT4 col(1.0f, 1.0f, 1.0f, 0.0f);
			for (size_t i = 0; i < m_ParticleCount; i++){

				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(-0.5f, 0.5f, 0), col, XMFLOAT2(0.0f, 0.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(0.5f, 0.5f, 0), col, XMFLOAT2(1.0f, 0.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(-0.5f, -0.5f, 0), col, XMFLOAT2(0.0f, 1.0f)));
				vertices.push_back(VertexPositionColorTexture(XMFLOAT3(0.5f, -0.5f, 0), col, XMFLOAT2(1.0f, 1.0f)));

				indices.push_back((uint16_t)i * 4 + 0);
				indices.push_back((uint16_t)i * 4 + 1);
				indices.push_back((uint16_t)i * 4 + 2);
				indices.push_back((uint16_t)i * 4 + 1);
				indices.push_back((uint16_t)i * 4 + 3);
				indices.push_back((uint16_t)i * 4 + 2);

			}

			//配列をもとに頂点とインデックスを作成
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//頂点が変更できるバッファの作成
			{
				mutex Mutex;
				std::lock_guard<std::mutex> lock(Mutex);
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, &m_VertexBuffer);
			}
			//頂点数の設定
			m_NumVertices = static_cast<UINT>(vertices.size());
			//インデックスバッファの作成
			{
				mutex Mutex;
				std::lock_guard<std::mutex> lock(Mutex);
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, &m_IndexBuffer);
			}
			//インデックス数の設定
			m_NumIndicis = static_cast<UINT>(indices.size());

		}
		catch (...){
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	用途: パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	//構築と消滅
	ParticleManager::ParticleManager(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	ParticleManager::~ParticleManager(){}

	//初期化
	void ParticleManager::OnCreate(){
		try{
			//バッファの作成
			pImpl->CreateParticleBuffers();
			//透明処理のみ指定しておく
			SetAlphaActive(true);
		}
		catch (...){
			throw;
		}

	}

	bool ParticleManager::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	bool ParticleManager::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void ParticleManager::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool ParticleManager::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool ParticleManager::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void ParticleManager::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}


	bool ParticleManager::GetSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	bool ParticleManager::IsSamplerWrap() const{
		return pImpl->m_SamplerWrap;
	}
	void ParticleManager::SetSamplerWrap(bool b){
		pImpl->m_SamplerWrap = b;
	}

	void ParticleManager::AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
		const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes){
		//デバイスとコンテキストインターフェイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		auto PtrCamera = GetStage()->GetTargetCamera();
		//カメラの位置
		Vector3 CameraEye = PtrCamera->GetEye();
		Vector3 CameraAt = PtrCamera->GetAt();


		ParticleSpriteForDraw Item;
		Vector3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		Item.m_ToCaneraLength = Vector3EX::Length(CameraEye - WorldPos);
		Item.m_TextureRes = TextureRes;

		Vector3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vector3 Temp;
		Quaternion Qt;
		Matrix4X4 RotMatrix;
		Vector4 dammi(0, 0, 0, 0);
		Vector3 DefUp(0, 1.0f, 0);
		switch (Option){
			case Particle::DrawOption::Billboard:
			{
				Temp = CameraAt - CameraEye;
				Vector2 TempVec2(Temp.x, Temp.z);
				if (Vector2EX::Length(TempVec2) < 0.1f){
					DefUp = Vector3(0, 0, 1.0f);
				}
				Temp.Normalize();
				RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
				RotMatrix.Inverse(&dammi);
				Qt = RotMatrix.QtInMatrix();
				Qt.Normalize();
			}
			break;
			case Particle::DrawOption::Faceing:
			{
				Temp = WorldPos - CameraEye;
				Vector2 TempVec2(Temp.x, Temp.z);
				if (Vector2EX::Length(TempVec2) < 0.1f){
					DefUp = Vector3(0, 0, 1.0f);
				}
				RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
				RotMatrix.Inverse(&dammi);
				Qt = RotMatrix.QtInMatrix();
				Qt.Normalize();
			}
			break;
			case Particle::DrawOption::FaceingY:
				Temp = WorldPos - CameraEye;
				Temp.Normalize();
				Qt.RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
				Qt.Normalize();
			break;
			case Particle::DrawOption::Normal:
				Qt = rParticleSprite.m_LocalQt;
				Qt.Normalize();
			break;
		}

		Matrix4X4 matrix;
		matrix.DefTransformation(
			LocalScale,
			Qt,
			rParticleSprite.m_LocalPos
			);


		Vector3 Pos0(-0.5f, 0.5f, 0);
		Pos0.Transform(matrix);

		Vector3 Pos1(0.5f, 0.5f, 0);
		Pos1.Transform(matrix);

		Vector3 Pos2(-0.5f, -0.5f, 0);
		Pos2.Transform(matrix);

		Vector3 Pos3(0.5f, -0.5f, 0);
		Pos3.Transform(matrix);

		Item.m_Vertex[0].position = Pos0 + EmitterPos;
		Item.m_Vertex[0].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[0].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVLeftTop.x, rParticleSprite.m_UVLeftTop.y);

		Item.m_Vertex[1].position = Pos1 + EmitterPos;
		Item.m_Vertex[1].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[1].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVRightBottom.x, rParticleSprite.m_UVLeftTop.y);

		Item.m_Vertex[2].position = Pos2 + EmitterPos;
		Item.m_Vertex[2].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[2].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVLeftTop.x, rParticleSprite.m_UVRightBottom.y);

		Item.m_Vertex[3].position = Pos3 + EmitterPos;
		Item.m_Vertex[3].color = (XMFLOAT4)rParticleSprite.m_Color;
		Item.m_Vertex[3].textureCoordinate = XMFLOAT2(rParticleSprite.m_UVRightBottom.x, rParticleSprite.m_UVRightBottom.y);

		pImpl->m_ParticleSpriteForDrawVec.push_back(Item);
	}

	void ParticleManager::DrawSub(const shared_ptr<TextureResource>& TextureRes, UINT StartIndex, UINT DrawCount){

		auto PtrStage = GetStage();
		if (!PtrStage){
			return;
		}
		Matrix4X4 ShaderMatrix;

		//カメラの位置
		auto PtrCamera = GetStage()->GetTargetCamera();
		Matrix4X4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();
		Matrix4X4 m;
		ShaderMatrix.Identity();
		ShaderMatrix *= View;
		ShaderMatrix *= Proj;


		//テクスチャは必須
		if (TextureRes){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//ステータスのポインタ
			auto RenderStatePtr = PtrStage->GetRenderState();

			//コンスタントバッファの設定
			SpriteConstantBuffer sb;
			sb.World = ShaderMatrix;

			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//ストライドとオフセット
			UINT stride = sizeof(VertexPositionColorTexture);
			UINT offset = 0;
			//頂点バッファの設定
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(pImpl->m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//描画方法（3角形）
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			ID3D11SamplerState* samplerState;
			if (IsSamplerWrap()){
				samplerState = RenderStatePtr->GetLinearWrap();
			}
			else{
				samplerState = RenderStatePtr->GetLinearClamp();
			}
			//サンプラーを設定
			pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, TextureRes->GetShaderResourceView().GetAddressOf());
			//テクスチャありのピクセルシェーダの設定
			pID3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
			//頂点シェーダの設定
			pID3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
			//インプットレイアウトの設定
			pID3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
			if (IsZBufferUse()){
				//デプスステンシル使用
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//デプスステンシルは使用しない
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			}
			//ステータスの設定
			//アルファブレンド
			if (IsAlphaActive()){
				if (IsAlphaBlendSrcOne()){
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
				else{
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
			}
			else{
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xFFFFFFFF);
			}
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;

			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			if (IsAlphaActive()){
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//描画
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
			}
			else{
				//レンダリングステート
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//描画
				pID3D11DeviceContext->DrawIndexed(DrawCount, StartIndex, 0);
			}
			//後始末
			Dev->InitializeStates(RenderStatePtr);

		}

	}


	void ParticleManager::OnDraw(){

		if (pImpl->m_ParticleSpriteForDrawVec.size() <= 0){
			return;
		}
		size_t ParticleCount = pImpl->m_ParticleCount;
		while (ParticleCount < pImpl->m_ParticleSpriteForDrawVec.size()){
			ParticleCount *= 2;
			if (ParticleCount > 9000){
				throw BaseException(
					L"これ以上パーティクルは追加できません。",
					L"if(ParticleCount > 9000)",
					L"ParticleManager::Draw()"
					);
			}
		}
		if (ParticleCount > pImpl->m_ParticleCount){
			pImpl->m_ParticleCount = ParticleCount;
			pImpl->CreateParticleBuffers();
		}
		//カメラ位置でソート
		auto func = [](ParticleSpriteForDraw& Left, ParticleSpriteForDraw& Right)->bool{
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(pImpl->m_ParticleSpriteForDrawVec.begin(), pImpl->m_ParticleSpriteForDrawVec.end(),func);


		//デバイスとコンテキストインターフェイスの取得
		//デバイスとコンテキストインターフェイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//先頭のテクスチャを得る
		auto  NowTexPtr = pImpl->m_ParticleSpriteForDrawVec[0].m_TextureRes;
		size_t DrawCount = 0;
		size_t StartIndex = 0;

		//描画用の配列
		vector<ParticleDrawSetter> ParticleDrawSetterVec;
		ParticleDrawSetter Item;


		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = pImpl->m_VertexBuffer.Get();

		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"Sprite::UpdateVirtexBuffer()"
				);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < pImpl->m_ParticleSpriteForDrawVec.size(); i++){
			if (pImpl->m_ParticleSpriteForDrawVec[i].m_TextureRes != NowTexPtr){
				Item.m_TextureRes = NowTexPtr;
				Item.StartIndex = StartIndex;
				Item.DrawCount = DrawCount;
				ParticleDrawSetterVec.push_back(Item);
				NowTexPtr = pImpl->m_ParticleSpriteForDrawVec[i].m_TextureRes;
				DrawCount = 0;
				StartIndex = i;
			}
			vertices[i * 4 + 0] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[0];
			vertices[i * 4 + 1] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[1];
			vertices[i * 4 + 2] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[2];
			vertices[i * 4 + 3] = pImpl->m_ParticleSpriteForDrawVec[i].m_Vertex[3];
			DrawCount++;
		}
		Item.m_TextureRes = NowTexPtr;
		Item.StartIndex = StartIndex;
		Item.DrawCount = DrawCount;
		ParticleDrawSetterVec.push_back(Item);
		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

		//更新が終わったら、描画準備用配列をクリア
		pImpl->m_ParticleSpriteForDrawVec.clear();

		//描画
		for (size_t i = 0; i < ParticleDrawSetterVec.size(); i++){
			DrawSub(
				ParticleDrawSetterVec[i].m_TextureRes,
				ParticleDrawSetterVec[i].StartIndex * 6,
				ParticleDrawSetterVec[i].DrawCount * 6
				);
		}
		ParticleDrawSetterVec.clear();

	}

	
	//--------------------------------------------------------------------------------------
	//	struct InputTextManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct InputTextManager::Impl{
		//フォーカスを持ってる入力ストリング
		weak_ptr<StringSprite> m_FocusInputString;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class InputTextManager : public GameObject;
	//	用途: 入力テキストマネージャ
	//--------------------------------------------------------------------------------------
	//構築と消滅
	InputTextManager::InputTextManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	InputTextManager::~InputTextManager(){}

	//初期化
	void InputTextManager::OnCreate(){
		try{
		}
		catch (...){
			throw;
		}
	}

	//アクセサ
	shared_ptr<StringSprite> InputTextManager::GetFocusInputString() const{
		if (!pImpl->m_FocusInputString.expired()){
			auto ShPtr = pImpl->m_FocusInputString.lock();
			return ShPtr;
		}
		else{
			return nullptr;
		}
	}
	void InputTextManager::SetFocusInputString(const shared_ptr<StringSprite>& Ptr){
		pImpl->m_FocusInputString = Ptr;
	}


	void InputTextManager::OnKeyDown(WPARAM wParam, LPARAM lParam){
		if (GetFocusInputString()){
			GetFocusInputString()->OnKeyDown(wParam, lParam);
		}
	}

	void InputTextManager::OnChar(WPARAM wParam, LPARAM lParam){
		if (GetFocusInputString()){
			GetFocusInputString()->OnChar(wParam, lParam);
		}
	}
	

	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Stage::Impl{
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;	//シャドウマップのレンダリングターゲット
		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	//デフォルトのレンダリングターゲット
		shared_ptr<RenderState> m_RenderState;					//レンダリングステート
		vector< shared_ptr<GameObject> > m_GameObjectVec;	//オブジェクトの配列
		shared_ptr<ParticleManager> m_ParticleManager;		//パーティクルマネージャ
		shared_ptr<InputTextManager> m_InputTextManager;	//入力マネージャ
		//シェアオブジェクトポインタのマップ
		map<const wstring,weak_ptr<GameObject> > m_SharedMap;
		//シェアグループのポインタのマップ
		map<const wstring, shared_ptr<GameObjectGroup> >  m_SharedGroupMap;
		size_t m_DrawViewIndex;		//現在Drawされているビューのインデックス

		//背面をクリアするためのバッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		UINT m_NumVertices;				//頂点の数

		vector< shared_ptr<Stage> > m_ChildStageVec;	//子供ステージの配列
		weak_ptr<Stage> m_ParentStage;		//親ステージ

		//Spriteかそうでないかを分離する配列
		vector< shared_ptr<GameObject> > m_SpriteVec;
		vector< shared_ptr<GameObject> > m_Object3DVec;
		//3Dの透明と非透明を分離する配列
		vector< shared_ptr<GameObject> > m_Object3DNormalVec;
		vector< shared_ptr<GameObject> > m_Object3DAlphaVec;

		//途中にオブジェクトが追加された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;

		Impl() :m_DrawViewIndex(0){}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage;
	//	用途: ステージクラス
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	Stage();
	//	用途: コンストラクタ。シーンを生成
	//	戻り値: なし。失敗したら例外をthrow
	//--------------------------------------------------------------------------------------
	//プライベートサブ関数
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr){
		//このステージはクリエイト後である
		if (IsCreated()){
			pImpl->m_WaitAddObjectVec.push_back(Ptr);
		}
		else{
		//クリエイト前
			pImpl->m_GameObjectVec.push_back(Ptr);
		}
	}

	shared_ptr<GameObject> Stage::GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive)const{
		map<const wstring, weak_ptr<GameObject> >::const_iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()){
			if (it->second.expired()){
				//すでに無効
				if (ExceptionActive){
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"オブジェクトが無効です",
						keyerr,
						L"Stage::GetSharedGameObject()"
						);
				}
			}
			return it->second.lock();
		}
		else{
			//指定の名前が見つからなかった
			if (ExceptionActive){
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"オブジェクトが見つかりません",
					keyerr,
					L"Stage::GetSharedGameObject()"
					);
			}
		}
		return nullptr;
	}


	//構築と破棄
	Stage::Stage() :
		GameObject(nullptr),
		pImpl(new Impl())
	{}
	Stage::~Stage(){}

	//アクセサ
	shared_ptr<ShadowMapRenderTarget> Stage::GetShadowMapRenderTarget() const { return pImpl->m_ShadowMapRenderTarget; }
	shared_ptr<DefaultRenderTarget> Stage::GetDefaultRenderTarget() const { return pImpl->m_DefaultRenderTarget; }
	shared_ptr<RenderState> Stage::GetRenderState() const { return pImpl->m_RenderState; }
	shared_ptr<ParticleManager> Stage::GetParticleManager() const{ return pImpl->m_ParticleManager; }
	shared_ptr<InputTextManager> Stage::GetInputTextManager() const{ return pImpl->m_InputTextManager; }


	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec(){ return pImpl->m_GameObjectVec; }
	void Stage::SetGameObjectVec(vector< shared_ptr<GameObject> >& vec){
		pImpl->m_GameObjectVec.clear();
		pImpl->m_GameObjectVec = vec;
	}
	size_t Stage::GetDrawViewIndex() const { return  pImpl->m_DrawViewIndex; }
	void Stage::SetDrawViewIndex(size_t i){ pImpl->m_DrawViewIndex = i; }


	vector< shared_ptr<Stage> >& Stage::GetChileStageVec(){
		return pImpl->m_ChildStageVec;
	}
	void Stage::AddChileStageBase(const shared_ptr<Stage>& ChildStage){
		pImpl->m_ChildStageVec.push_back(ChildStage);
		ChildStage->SetParentStage(GetThis<Stage>());
	}

	shared_ptr<Stage> Stage::GetParentStage() const{
		if (!pImpl->m_ParentStage.expired()){
			return  pImpl->m_ParentStage.lock();
		}
		return nullptr;
	}
	void Stage::SetParentStage(const shared_ptr<Stage>& ParentStage){
		pImpl->m_ParentStage = ParentStage;
	}



	//サービスアクセサ
	const D3D11_VIEWPORT& Stage::GetViewPort(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetViewPort();
	}
	const D3D11_VIEWPORT& Stage::GetTargetViewPort() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetViewPort();
	}
	const D3D11_VIEWPORT* Stage::GetViewPortRealPtr(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetViewPortPtr();
	}
	const D3D11_VIEWPORT* Stage::GetTargetViewPortRealPtr() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetViewPortPtr();
	}
	const Color4& Stage::GetBackColor(size_t Index) const {
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetBackColor();
	}
	const Color4& Stage::GetTargetBackColor() const {
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetBackColor();
	}
	shared_ptr<Camera> Stage::GetCamera(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetCamera();
	}

	shared_ptr<Camera> Stage::GetTargetCamera() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetCamera();
	}

	shared_ptr<MultiLight> Stage::GetMultiLight(size_t Index) const{
		return GetDynamicComponent<MultiView>()->GetView(Index)->GetMultiLight();
	}

	shared_ptr<MultiLight> Stage::GetTargetMultiLight() const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetMultiLight();
	}

	shared_ptr<Light> Stage::GetLight(size_t ViewIndex, size_t LightIndex) const{
		return GetDynamicComponent<MultiView>()->GetView(ViewIndex)->GetMultiLight()->GetLight(LightIndex);
	}

	shared_ptr<Light> Stage::GetTargetLight(size_t LightIndex) const{
		return GetDynamicComponent<MultiView>()->GetView(pImpl->m_DrawViewIndex)->GetMultiLight()->GetLight(LightIndex);
	}

	//操作
	shared_ptr<GameObject> Stage::AddGameObject(const shared_ptr<GameObject>& Obj){
		try{
			//配置オブジェクトの検証
			for (auto ptr : pImpl->m_GameObjectVec){
				if (Obj == ptr){
					throw BaseException(
						L"すでにそのオブジェクトは登録されています",
						L"重複してます",
						L"Stage::AddGameObject()"
						);
				}
			}
			//親ステージの設定差し替え（自分自身）
			Obj->SetStage(GetThis<Stage>());
			//TranceformMatrix必須
			//なかったときの場合に対応
			Obj->AddComponent<TransformMatrix>();
			//オブジェクトの追加
			PushBackGameObject(Obj);
			return Obj;
		}
		catch (...){
			throw;
		}
	}

	//追加待ちになってるオブジェクトを追加する
	void Stage::SetWaitToObjectVec(){
		if (pImpl->m_WaitAddObjectVec.size() > 0){
			for (auto Ptr : pImpl->m_WaitAddObjectVec){
				pImpl->m_GameObjectVec.push_back(Ptr);
			}
		}
		pImpl->m_WaitAddObjectVec.clear();
	}


	shared_ptr<Object> Stage::GetSharedObject(const wstring& Key, bool ExceptionActive)const{
		shared_ptr<Object> Ptr = GetSharedGameObjectEx(Key, ExceptionActive);
		return Ptr;
	}


	void Stage::SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr){
		map<const wstring, weak_ptr<GameObject> >::iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()){
			//既に存在した
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクトがあります",
				keyerr,
				L"Stage::SetSharedGameObjectEx()"
				);
		}
		else{
			pImpl->m_SharedMap[Key] = Ptr;
		}
	}

	shared_ptr<GameObjectGroup> Stage::CreateSharedObjectGroup(const wstring& Key){
		try{
			map<const wstring, shared_ptr<GameObjectGroup> >::iterator it;
			//重複キーの検査
			it = pImpl->m_SharedGroupMap.find(Key);
			if (it != pImpl->m_SharedGroupMap.end()){
				//既に存在した
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"同名のシェアオブジェクト配列があります",
					keyerr,
					L"Stage::CreateSharedObjectGroup()"
					);
			}
			else{
				auto Ptr = ObjectFactory::Create<GameObjectGroup>();
				pImpl->m_SharedGroupMap[Key] = Ptr;
				return Ptr;
			}
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<GameObjectGroup> Stage::GetSharedObjectGroup(const wstring& Key, bool ExceptionActive)const{
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//ペアのsecondを返す
			return it->second;
		}
		else{
			//指定の名前が見つからなかった
			if (ExceptionActive){
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"指定のキーが見つかりません",
					keyerr,
					L"Stage::GetSharedObjectGroup() const"
					);
			}
		}
		return nullptr;
	}

	void Stage::SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr){
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクト配列があります",
				keyerr,
				L"Stage::SetSharedObjectGroup()"
				);
		}
		else{
			//指定の名前が見つからなかった
			//登録できる
			pImpl->m_SharedGroupMap[Key] = NewPtr;
		}
	}




	//デフォルトのレンダリングターゲット類を準備する
	void Stage::CreateDefaultRenderTargets(float ShadowMapDimension){

		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vector3(1.0f, 1.0f, 1.0f));
		Ptr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		Ptr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

		//シャドウマップのレンダリングターゲットを作成
		pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>(ShadowMapDimension);
		//デフォルトのレンダリングターゲットを作成
		pImpl->m_DefaultRenderTarget = make_shared<DefaultRenderTarget>(GetThis<Stage>());
		//レンダリングステートの作成
		pImpl->m_RenderState = make_shared<RenderState>();
	}

	void Stage::DrawBackColor(){
		
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto RenderStatePtr = GetRenderState();

		//コンスタントバッファの準備
		PDirectConstantBuffer sb;
		sb.Diffuse = GetTargetBackColor();
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPDirect::GetPtr()->GetInputLayout());

		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルは使用しない
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//
		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);

		//描画
		pD3D11DeviceContext->Draw(pImpl->m_NumVertices, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);

		
	}


	//仮想関数
	void Stage::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		bool IsFormMessage = false;
		//配置オブジェクトの検証
		for (auto ptr : pImpl->m_GameObjectVec){
			auto FormPtr = dynamic_pointer_cast<HttpForm>(ptr);
			//フォームを探す
			if (FormPtr){
				FormPtr->OnMessage(message,wParam,lParam);
				IsFormMessage = true;
			}
		}
		if (!IsFormMessage){
			switch (message){
			case WM_KEYDOWN:
				GetInputTextManager()->OnKeyDown(wParam, lParam);
				break;
			case WM_CHAR:
				GetInputTextManager()->OnChar(wParam, lParam);
				break;
			default:
				break;
			}
		}
	}

	void Stage::OnPreCreate(){
		GameObject::OnPreCreate();
		//ステージはTransform必須
		AddComponent<Transform>();
		//パーティクルマネージャの作成
		pImpl->m_ParticleManager = ObjectFactory::Create<ParticleManager>(GetThis<Stage>());
		//入力マネージャの作成
		pImpl->m_InputTextManager = ObjectFactory::Create<InputTextManager>(GetThis<Stage>());


		//頂点を作成するための配列
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(-1.0f, 1.0f, 1.0f)) },
			{ VertexPosition(Vector3(1.0f, 1.0f, 1.0f)) },
			{ VertexPosition(Vector3(-1.0f, -1.0f, 1.0f)) },
			{ VertexPosition(Vector3(1.0f, -1.0f, 1.0f)) },
		};
		VertexUtil::CreateVertexBuffer(pImpl->m_VertexBuffer, vertices);
		//頂点数の設定
		pImpl->m_NumVertices = static_cast<UINT>(vertices.size());
		//マルチビューコンポーネントの登録
		AddComponent<MultiView>();
	}


	//ステージ内の更新（シーンからよばれる）
	void Stage::UpdateStage(){
		//Transformコンポーネントの値をバックアップにコピー
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				auto ptr2 = ptr->GetComponent<TransformMatrix>();
				ptr2->SetToBefore();
			}
		}
		if (IsUpdateActive()){
			//自身のバックアップ
			auto ptr2 = GetComponent<TransformMatrix>();
			ptr2->SetToBefore();
		}
		//配置オブジェクトの更新1
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->OnUpdate();
			}
		}
		//自身の更新1
		if (IsUpdateActive()){
			OnUpdate();
		}
		//配置オブジェクトのコンポーネント更新1
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->ComponentUpdate();
			}
		}
		//自身のコンポーネントをアップデート
		if (IsUpdateActive()){
			ComponentUpdate();
		}
		//衝突判定チェック
		//配置オブジェクトの衝突チェック
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->CollisionChk();
			}
		}
		//配置オブジェクトのコンポーネント更新2
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->ComponentUpdate2();
			}
		}
		//自身のコンポーネントの更新2
		if (IsUpdateActive()){
			ComponentUpdate2();
		}
		//配置オブジェクトのコンポーネント更新3
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsUpdateActive()){
				ptr->OnLastUpdate();
			}
		}
		//自身の更新3
		if (IsUpdateActive()){
			OnLastUpdate();
		}
		//子供ステージの更新
		for (auto PtrChileStage : GetChileStageVec()){
			PtrChileStage->UpdateStage();
		}
	}



	//ステージ内の描画（シーンからよばれる）
	void Stage::DrawStage(){
		//Spriteかそうでないかを分離
		for (auto ptr : GetGameObjectVec()){
			if (ptr->IsDrawActive()){
				//Spriteかその派生クラスなら分離
				if (ptr->GetDynamicComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw()){
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else{
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//レイヤーの取得と設定
		set<int> DrawLayers;
		//3Dの透明と非透明を分離
		for (auto ptr : pImpl->m_Object3DVec){
			if (ptr->IsDrawActive()){
				DrawLayers.insert(ptr->GetDrawLayer());
				if (ptr->IsAlphaActive()){
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else{
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		

		auto MultiViewPtr = GetComponent<MultiView>();
		if (!GetParentStage()){
			//最上のステージの場合のみ全画面クリア
			GetDefaultRenderTarget()->ClearViews();
		}
		size_t DrawCount = MultiViewPtr->GetViewCount();

		for (size_t i = 0; i < DrawCount; i++){

			SetDrawViewIndex(i);
			//シャドウマップの開始
			GetShadowMapRenderTarget()->ClearViews();
			GetShadowMapRenderTarget()->StartRenderTarget();
			//配置オブジェクトのシャドウマップ描画(ノーマル)
			for (auto ptr : pImpl->m_Object3DNormalVec){
				ptr->DrawShadowmap();
			}
			//配置オブジェクトのシャドウマップ描画(透明)
			for (auto ptr : pImpl->m_Object3DAlphaVec){
				ptr->DrawShadowmap();
			}
			//ステージのDrawShadowmap()は呼ばない
			GetShadowMapRenderTarget()->EndRenderTarget();
			//レンダリングターゲットの開始
			GetDefaultRenderTarget()->StartRenderTarget();
			//背景色の描画
			DrawBackColor();
			auto PtrCamera = GetTargetCamera();
			//カメラの位置
			Vector3 CameraEye = PtrCamera->GetEye();
			//透明の3Dオブジェクトをカメラからの距離でソート
			//以下は、オブジェクトを引数に取りboolを返すラムダ式
			//--------------------------------------------------------
			auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool{
				auto PtrLeftTrans = Left->GetComponent<TransformMatrix>();
				auto PtrRightTrans = Right->GetComponent<TransformMatrix>();

				auto LeftPos = PtrLeftTrans->GetWorldMatrix().PosInMatrixSt();
				auto RightPos = PtrRightTrans->GetWorldMatrix().PosInMatrixSt();

				auto LeftLen = Vector3EX::Length(LeftPos - CameraEye);
				auto RightLen = Vector3EX::Length(RightPos - CameraEye);

				return (LeftLen > RightLen);
			};

			std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);


			//3Dノーマルオブジェクトの描画準備
			for (auto ptr : pImpl->m_Object3DNormalVec){
				ptr->OnPreDraw();
			}
			//3D透明オブジェクトの描画準備
			for (auto ptr : pImpl->m_Object3DAlphaVec){
				ptr->OnPreDraw();
			}
			//パーティクルの描画準備
			if (GetParticleManager()){
				GetParticleManager()->OnPreDraw();
			}
			//スプライトオブジェクトの描画準備
			for (auto ptr : pImpl->m_SpriteVec){
				ptr->OnPreDraw();
			}

			//スプライトをZ座標距離でソート
			//以下は、オブジェクトを引数に取りboolを返すラムダ式
			//--------------------------------------------------------
			auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool{
				auto PtrLeftTrans = Left->GetComponent<TransformMatrix>();
				auto PtrRightTrans = Right->GetComponent<TransformMatrix>();

				auto LeftPos = PtrLeftTrans->GetWorldMatrix().PosInMatrixSt();
				auto RightPos = PtrRightTrans->GetWorldMatrix().PosInMatrixSt();

				float LeftZ = LeftPos.z;
				float RightZ = RightPos.z;
				return (LeftZ > RightZ);
			};
			std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

			for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++){
				int Tgt = *it;
				//3Dノーマルオブジェクトの描画
				for (auto ptr : pImpl->m_Object3DNormalVec){
					if (ptr->GetDrawLayer() == Tgt){
						ptr->OnDraw();
					}
				}
				//3D透明オブジェクトの描画
				for (auto ptr : pImpl->m_Object3DAlphaVec){
					if (ptr->GetDrawLayer() == Tgt){
						ptr->OnDraw();
					}
				}
				//パーティクルの描画
				auto PartPtr = GetParticleManager();
				if (PartPtr && PartPtr->GetDrawLayer() == Tgt){
					PartPtr->OnDraw();
				}
				//スプライトオブジェクトの描画
				for (auto ptr : pImpl->m_SpriteVec){
					if (ptr->GetDrawLayer() == Tgt){
						ptr->OnDraw();
					}
				}
			}
			//ステージのDraw();
			OnDraw();
			//レンダリングターゲットの終了
			GetDefaultRenderTarget()->EndRenderTarget();
		}
		//ワーク用配列のクリア
		//ワーク配列は毎ターンごとに初期化されるが、
		//最大値は減らないので2回目のターン以降は高速に動作する
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
		//子供ステージの描画
		for (auto PtrChileStage : pImpl->m_ChildStageVec){
			PtrChileStage->DrawStage();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiAudioObject::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiAudioObject::Impl{
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl(){
		}
		SoundItem* GetItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//同じ名前が見つかった
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//同じ名前が見つかった
				//Mapから削除
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);
	};

	void MultiAudioObject::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Start()"
			);
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiAudioObject::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiAudioObject::Impl::Start()"
			);
	}

	void MultiAudioObject::Impl::Stop(const SoundItem* pItem){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Stop()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Stop()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Stop()"
			);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	用途: マルチオーディオ
	// ＊コンポーネントではないのでシーンに持たせることができる
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiAudioObject::MultiAudioObject() :
		pImpl(new Impl())
	{}
	MultiAudioObject::~MultiAudioObject(){}

	//アクセサ
	shared_ptr<AudioResource> MultiAudioObject::AddAudioResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::AddAudioResource()"
					);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem){
				//同じ名前が見つかった
				if (!pItem->m_AudioResource.expired()){
					return pItem->m_AudioResource.lock();
				}
				else{
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiAudioObject::AddAudioResource()"
						);
				}
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();
			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiAudioObject::AddAudioResource()"
				);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...){
			throw;
		}
	}
	void MultiAudioObject::RemoveAudioResource(const wstring& ResKey){
		if (ResKey == L""){
			throw BaseException(
				L"オーディオリソース名が空白です",
				L"if (ResKey == L\"\")",
				L"MultiAudioObject::RemoveAudioResource()"
				);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiAudioObject::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const{
		if (ResKey == L""){
			if (ExceptionActive){
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem){
			//同じ名前が見つかった
			if (!pItem->m_AudioResource.expired()){
				return pItem->m_AudioResource.lock();
			}
			else{
				if (ExceptionActive){
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiAudioObject::AddAudioResource()"
						);
				}
				else{
					return nullptr;
				}
			}
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"指定の名前のオーディオリソースがありません",
					ResKey,
					L"MultiAudioObject::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
	}

	void MultiAudioObject::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiAudioObject::Start(const wstring& ResKey, size_t LoopCount, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		if (pItem->m_AudioResource.expired()){
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImplの関数を呼ぶ
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiAudioObject::Stop(const wstring& ResKey){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Stop()"
				);
		}
		pImpl->Stop(pItem);
	}




	//--------------------------------------------------------------------------------------
	//	struct SceneBase::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SceneBase::Impl{
		shared_ptr<Stage> m_ActiveStage;	//アクティブなステージ
		shared_ptr<EventDispatcher> m_EventDispatcher;	//イベント配送クラス
		Impl() :
			m_ActiveStage()
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SceneBase: public Object;
	//	用途: シーンベースクラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SceneBase::SceneBase():
		pImpl(new Impl())
	{
		try{
			//デフォルトのリソースの作成
			App::GetApp()->RegisterResource(L"DEFAULT_SQUARE", MeshResource::CreateSquare(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_CUBE", MeshResource::CreateCube(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE", MeshResource::CreateSphere(1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CAPSULE", MeshResource::CreateCapsule(1.0f,1.0f,18));
			App::GetApp()->RegisterResource(L"DEFAULT_CYLINDER", MeshResource::CreateCylinder(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CONE", MeshResource::CreateCone(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TORUS", MeshResource::CreateTorus(1.0f, 0.3f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TETRAHEDRON", MeshResource::CreateTetrahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_OCTAHEDRON", MeshResource::CreateOctahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_DODECAHEDRON", MeshResource::CreateDodecahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_ICOSAHEDRON", MeshResource::CreateIcosahedron(1.0f));
		}
		catch (...){
			throw;
		}
	}
	SceneBase::~SceneBase(){}


	//アクセッサ
	shared_ptr<Stage> SceneBase::GetActiveStage() const{
		if (!pImpl->m_ActiveStage){
			//アクティブなステージが無効なら
			throw BaseException(
				L"アクティブなステージがありません",
				L"if(!m_ActiveStage.get())",
				L"SceneBase::GetActiveStage()"
				);
		}
		return pImpl->m_ActiveStage;
	}
	void SceneBase::SetActiveStage(const shared_ptr<Stage>& stage){
		pImpl->m_ActiveStage = stage;
	}

	shared_ptr<EventDispatcher> SceneBase::GetEventDispatcher() const{
		return pImpl->m_EventDispatcher;
	}
	void SceneBase::OnMessage(UINT message, WPARAM wParam, LPARAM lParam){
		if (pImpl->m_ActiveStage){
			pImpl->m_ActiveStage->OnMessage(message,wParam, lParam);
		}
	}

	void SceneBase::OnPreCreate(){
		//イベント送信オブジェクト作成
		pImpl->m_EventDispatcher = make_shared<EventDispatcher>(GetThis<SceneBase>());
	}
	void SceneBase::OnUpdate(){
		if (pImpl->m_ActiveStage){
			//追加待ちになっているオブジェクトの追加
			pImpl->m_ActiveStage->SetWaitToObjectVec();
			//イベントキューの送出
			pImpl->m_EventDispatcher->DispatchDelayedEvwnt();
			//ステージのアップデート
			pImpl->m_ActiveStage->UpdateStage();
		}
	}
	void SceneBase::OnDraw(){
		if (pImpl->m_ActiveStage){
			pImpl->m_ActiveStage->DrawStage();
		}
	}


}

//endof  basedx11
