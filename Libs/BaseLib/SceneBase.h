
#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class GameObject :public Object, public ShapeInterface;
	//	用途: ゲーム配置オブジェクト親クラス
	//--------------------------------------------------------------------------------------
	class GameObject :public Object, public ShapeInterface{
		shared_ptr<Component> SearchComponent(type_index TypeIndex)const;
		shared_ptr<Rigidbody> GetRigidbody()const;
		shared_ptr<Gravity> GetGravity()const;
		shared_ptr<TransformMatrix> GetTransformMatrix()const;
		shared_ptr<Transform> GetTransform()const;
		shared_ptr<Collision> GetCollision()const;
		shared_ptr<CollisionSphere> GetCollisionSphere()const;
		shared_ptr<CollisionCapsule> GetCollisionCapsule()const;
		shared_ptr<CollisionObb> GetCollisionObb()const;

		void SetRigidbody(const shared_ptr<Rigidbody>& Ptr);
		void SetGravity(const shared_ptr<Gravity>& Ptr);
		void SetCollision(const shared_ptr<Collision>& Ptr);
		void SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr);
		void SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr);
		void SetCollisionObb(const shared_ptr<CollisionObb>& Ptr);
		void SetTransformMatrix(const shared_ptr<TransformMatrix>& Ptr);
		void SetTransform(const shared_ptr<Transform>& Ptr);
		void AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr);
		template<typename T>
		shared_ptr<T> SearchDynamicComponent()const{
			auto mapPtr = GetCompoMap();
			auto it = mapPtr.begin();
			while (it != mapPtr.end()){
				auto Ptr = dynamic_pointer_cast<T>(it->second);
				if (Ptr){
					return Ptr;
				}
				it++;
			}
			return nullptr;
		}
		map<type_index, shared_ptr<Component> >& GetCompoMap() const;
		void RemoveTgtComponent(type_index TypeIndex);
	public:
		//構築と破棄
		explicit GameObject(const shared_ptr<Stage>& StagePtr);
		virtual ~GameObject();
		//アクセサ
		bool IsUpdateActive() const;
		bool GetUpdateActive() const;
		void SetUpdateActive(bool b);

		bool IsDrawActive() const;
		bool GetDrawActive() const;
		void SetDrawActive(bool b);

		bool IsAlphaActive() const;
		bool GetAlphaActive() const;
		void SetAlphaActive(bool b);

		bool IsAlphaExActive() const;
		bool GetAlphaExActive() const;
		void SetAlphaExActive(bool b);


		//スプライトとしてDrawするかどうか
		bool IsSpriteDraw() const;
		bool GetSpriteDraw() const;
		void SetSpriteDraw(bool b);

		//描画レイヤーの取得と設定
		int GetDrawLayer() const;
		void  SetDrawLayer(int l);
		shared_ptr<Stage> GetStage() const;
		void SetStage(const shared_ptr<Stage>& stage);
		//操作
		//コンポーネントの取得
		//ExceptionActiveがtrueの場合は例外、無ければnullptrを返す
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const{
			auto Ptr = SearchComponent(type_index(typeid(T)));
			if (Ptr) {
				//指定の型のコンポーネントが見つかった
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseMBException(
						"コンポーネントがありましたが、型キャストできません",
						typeid(T).name(),
						"GameObject::GetComponent<T>()"
						);
				}
			}
			else{
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						typeid(T).name(),
						"GameObject::GetComponent<T>()"
						);
				}
			}
			return nullptr;
		}
		//Rigidbody特殊化
		template <>
		shared_ptr<Rigidbody> GetComponent<Rigidbody>(bool ExceptionActive)const{
			auto Ptr = GetRigidbody();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"Rigidbody",
						"GameObject::GetComponent<Rigidbody>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicRigidbody
		template <typename T>
		shared_ptr<T> GetDynamicRigidbody(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetRigidbody());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはRigidbodyからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicRigidbody<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//Gravity特殊化
		template <>
		shared_ptr<Gravity> GetComponent<Gravity>(bool ExceptionActive)const{
			auto Ptr = GetGravity();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"Gravity",
						"GameObject::GetComponent<Gravity>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicGravity
		template <typename T>
		shared_ptr<T> GetDynamicGravity(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetGravity());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはGravityからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicGravity<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//Collision特殊化
		//親クラスなのでGetのみ用意
		template <>
		shared_ptr<Collision> GetComponent<Collision>(bool ExceptionActive)const{
			auto Ptr = GetCollision();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"Collision",
						"GameObject::GetComponent<Collision>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollision
		template <typename T>
		shared_ptr<T> GetDynamicCollision(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollision());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはCollisionからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicCollision<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//CollisionSphere特殊化
		template <>
		shared_ptr<CollisionSphere> GetComponent<CollisionSphere>(bool ExceptionActive)const{
			auto Ptr = GetCollisionSphere();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"CollisionSphere",
						"GameObject::GetComponent<CollisionSphere>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionSphere
		template <typename T>
		shared_ptr<T> GetDynamicCollisionSphere(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionSphere());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはCollisionSphereからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionSphere<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}


		//CollisionCapsule特殊化
		template <>
		shared_ptr<CollisionCapsule> GetComponent<CollisionCapsule>(bool ExceptionActive)const{
			auto Ptr = GetCollisionCapsule();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"CollisionCapsule",
						"GameObject::GetComponent<CollisionCapsule>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionCapsule
		template <typename T>
		shared_ptr<T> GetDynamicCollisionCapsule(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionCapsule());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはCollisionCapsuleからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionCapsule<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//CollisionObb特殊化
		template <>
		shared_ptr<CollisionObb> GetComponent<CollisionObb>(bool ExceptionActive)const{
			auto Ptr = GetCollisionObb();
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"CollisionObb",
						"GameObject::GetComponent<CollisionObb>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicCollisionObb
		template <typename T>
		shared_ptr<T> GetDynamicCollisionObb(bool ExceptionActive = true)const{
			auto Ptr = dynamic_pointer_cast<T>(GetCollisionObb());
			if (!Ptr){
				if (ExceptionActive){
					throw BaseMBException(
						"指定の型へはCollisionObbからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicCollisionObb<T>()"
						);
				}
				else{
					return nullptr;
				}
			}
			return Ptr;
		}

		//TransformMatrix特殊化
		template <>
		shared_ptr<TransformMatrix> GetComponent<TransformMatrix>(bool ExceptionActive)const {
			auto Ptr = GetTransformMatrix();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"TransformMatrix",
						"GameObject::GetComponent<TransformMatrix>()"
						);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicTransform
		template <typename T>
		shared_ptr<T> GetDynamicTransformMatrix(bool ExceptionActive = true)const {
			auto Ptr = dynamic_pointer_cast<T>(GetTransformMatrix());
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseMBException(
						"指定の型へはTransformからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicTransformMatrix<T>()"
						);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//Transform特殊化
		template <>
		shared_ptr<Transform> GetComponent<Transform>(bool ExceptionActive)const {
			auto Ptr = GetTransform();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseMBException(
						"コンポーネントが見つかりません",
						"Transform",
						"GameObject::GetComponent<Transform>()"
						);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//DynamicTransform
		template <typename T>
		shared_ptr<T> GetDynamicTransform(bool ExceptionActive = true)const {
			auto Ptr = dynamic_pointer_cast<T>(GetTransform());
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseMBException(
						"指定の型へはTransformからキャストできません",
						typeid(T).name(),
						"GameObject::GetDynamicTransform<T>()"
						);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}


		//DynamicCastを利用したコンポーネントの取得
		//コンポーネントの派生クラス使用時に使う
		//ExceptionActiveがtrueの場合は例外、無ければnullptrを返す
		template <typename T>
		shared_ptr<T> GetDynamicComponent(bool ExceptionActive = true)const{
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr){
				return Ptr;
			}
			else{
				if (ExceptionActive){
					throw BaseMBException(
						"型キャストできるコンポーネントが見つかりません",
						typeid(T).name(),
						"GameObject::GetDynamicComponent<T>()"
						);
				}
			}
			return nullptr;
		}

		//コンポーネントの追加
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params){
			auto Ptr = SearchComponent(type_index(typeid(T)));
			//指定の型のコンポーネントが見つかった
			if (Ptr){
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr){
					return RetPtr;
				}
				else{
					throw BaseMBException(
						"すでにコンポーネントがありましたが、型キャストできません",
						typeid(T).name(),
						"GameObject::AddComponent<T>()"
						);
				}
			}
			else{
				//見つからない。新たに作成する
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
				AddMakedComponent(type_index(typeid(T)), newPtr);
				return newPtr;
			}
		}
		//Rigidbody特殊化
		template <>
		shared_ptr<Rigidbody> AddComponent<Rigidbody>(){
			auto Ptr = GetRigidbody();
			if (Ptr){
				return Ptr;
			}
			else{
				//無ければ新たに制作する
				auto RigidbodyPtr = ObjectFactory::Create<Rigidbody>(GetThis<GameObject>());
				SetRigidbody(RigidbodyPtr);
				return RigidbodyPtr;
			}
		}
		//DynamicRigidbody
		//Rigidbodyの派生クラスをRigidbodyにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicRigidbody(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//Rigidbodyにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Rigidbody>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはRigidbodyにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicRigidbodyt<T>()"
					);
			}
			SetRigidbody(newPtr);
			return newPtr;
		}
		//Gravity特殊化
		template <>
		shared_ptr<Gravity> AddComponent<Gravity>(){
			auto Ptr = GetGravity();
			if (Ptr){
				return Ptr;
			}
			else{
				//無ければ新たに制作する
				auto GravityPtr = ObjectFactory::Create<Gravity>(GetThis<GameObject>());
				SetGravity(GravityPtr);
				return GravityPtr;
			}
		}
		//DynamicGravity
		//Gravityの派生クラスをGravityにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicGravity(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//Gravityにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Gravity>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはGravityにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicGravityt<T>()"
					);
			}
			SetGravity(newPtr);
			return newPtr;
		}

		//Collision特殊化
		template <>
		shared_ptr<Collision> AddComponent<Collision>(){
			auto Ptr = GetCollision();
			if (Ptr){
				return Ptr;
			}
			else{
				//無くても作成できない
				throw BaseException(
					L"このコンポーネントは作成できません。",
					L"Collision",
					L"GameObject::AddComponent<Collision>()"
					);
			}
		}
		//DynamicCollision
		//Collisionの派生クラスをCollisionにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollision(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//Collisionにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Collision>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicCollision<T>()"
					);
			}
			SetCollision(newPtr);
			return newPtr;
		}

		//CollisionSphere特殊化
		template <>
		shared_ptr<CollisionSphere> AddComponent<CollisionSphere>(){
			auto Ptr = GetCollisionSphere();
			if (Ptr){
				return Ptr;
			}
			else{
				//無ければ新たに制作する
				auto CollisionSpherePtr = ObjectFactory::Create<CollisionSphere>(GetThis<GameObject>());
				SetCollisionSphere(CollisionSpherePtr);
				return CollisionSpherePtr;
			}
		}
		//DynamicCollisionSphere
		//CollisionSphereの派生クラスをCollisionにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionSphere(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//CollisionSphereにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<CollisionSphere>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionSphereにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionSphere<T>()"
					);
			}
			SetCollisionSphere(newPtr);
			return newPtr;
		}

		//CollisionCapsule特殊化
		template <>
		shared_ptr<CollisionCapsule> AddComponent<CollisionCapsule>(){
			auto Ptr = GetCollisionCapsule();
			if (Ptr){
				return Ptr;
			}
			else{
				//無ければ新たに制作する
				auto CollisionCapsulePtr = ObjectFactory::Create<CollisionCapsule>(GetThis<GameObject>());
				SetCollisionCapsule(CollisionCapsulePtr);
				return CollisionCapsulePtr;
			}
		}
		//DynamicCollisionCapsule
		//CollisionCapsuleの派生クラスをCollisionにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionCapsule(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//CollisionCapsuleにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<CollisionCapsule>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionCapsuleにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionCapsule<T>()"
					);
			}
			SetCollisionCapsule(newPtr);
			return newPtr;
		}



		//CollisionObb特殊化
		template <>
		shared_ptr<CollisionObb> AddComponent<CollisionObb>(){
			auto Ptr = GetCollisionObb();
			if (Ptr){
				return Ptr;
			}
			else{
				//無ければ新たに制作する
				auto CollisionObbPtr = ObjectFactory::Create<CollisionObb>(GetThis<GameObject>());
				SetCollisionObb(CollisionObbPtr);
				return CollisionObbPtr;
			}
		}
		//DynamicCollisionObb
		//CollisionObbの派生クラスをCollisionにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicCollisionObb(Ts&&... params){
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//CollisionObbにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<CollisionObb>(newPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionObbにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicCollisionObb<T>()"
					);
			}
			SetCollisionObb(newPtr);
			return newPtr;
		}


		//TransformMatrix特殊化
		template <>
		shared_ptr<TransformMatrix> AddComponent<TransformMatrix>() {
			auto Ptr = GetTransformMatrix();
			auto Ptr2 = GetTransform();
			if (Ptr) {
				if (!Ptr2){
					return Ptr;
				}
				else{
					throw BaseException(
						L"すでにTransformコンポーネントが実装されています",
						L"TransformMatrixとして設定できません",
						L"GameObject::AddComponent<TransformMatrix>()"
						);
				}
			}
			else {
				//無ければ新たに制作する
				auto TransformMatrixPtr = ObjectFactory::Create<TransformMatrix>(GetThis<GameObject>());
				SetTransformMatrix(TransformMatrixPtr);
				return TransformMatrixPtr;
			}
		}
		//DynamicTransformMatrix
		//TransformMatrixの派生クラスをTransformMatrixにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicTransformMatrix(Ts&&... params) {
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//TransformMatrixにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<TransformMatrix>(newPtr);
			if (!RetPtr) {
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはTransformMatrixにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicTransformMatrix<T>()"
					);
			}
			SetTransformMatrix(newPtr);
			return newPtr;
		}



		//Transform特殊化
		template <>
		shared_ptr<Transform> AddComponent<Transform>() {
			auto Ptr = GetTransform();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto TransformPtr = ObjectFactory::Create<Transform>(GetThis<GameObject>());
				SetTransform(TransformPtr);
				return TransformPtr;
			}
		}
		//DynamicTransform
		//Transformの派生クラスをTransformにセットする
		template<typename T, typename... Ts>
		shared_ptr<T> AddDynamicTransform(Ts&&... params) {
			//現在の検索は行わず、そのままセットする
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
			//Transformにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Transform>(newPtr);
			if (!RetPtr) {
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはTransformにキャストできません。",
					typeid(T).name(),
					"GameObject::AddDynamicTransform<T>()"
					);
			}
			SetTransform(newPtr);
			return newPtr;
		}


		//すでにあるコンポーネントのセット
		template <typename T>
		void SetComponent(const shared_ptr<T>& SetPtr){
			AddMakedComponent(type_index(typeid(T)), SetPtr);
		}
		//Rigidbody特殊化
		template <>
		void SetComponent<Rigidbody>(const shared_ptr<Rigidbody>& SetPtr){
			SetRigidbody(SetPtr);
		}
		//DynamicRigidbody
		//Rigidbodyの派生クラスをRigidbodyにセットする
		template <typename T>
		void SetDynamicRigidbody(const shared_ptr<T>& SetPtr){
			//Rigidbodyにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Rigidbody>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはRigidbodyにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicRigidbody<T>()"
					);
			}
			SetRigidbody(SetPtr);
		}
		//Gravity特殊化
		template <>
		void SetComponent<Gravity>(const shared_ptr<Gravity>& SetPtr){
			SetGravity(SetPtr);
		}
		//DynamicGravity
		//Gravityの派生クラスをGravityにセットする
		template <typename T>
		void SetDynamicGravity(const shared_ptr<T>& SetPtr){
			//Gravityにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Gravity>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはGravityにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicGravity<T>()"
					);
			}
			SetGravity(SetPtr);
		}


		//DynamicCollision
		//Collisionの派生クラスをCollisionにセットする
		template <typename T>
		void SetDynamicCollision(const shared_ptr<T>& SetPtr){
			//Collisionにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Collision>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicCollision<T>()"
					);
			}
			SetCollision(SetPtr);
		}

		//CollisionSphere特殊化
		template <>
		void SetComponent<CollisionSphere>(const shared_ptr<CollisionSphere>& SetPtr){
			SetCollisionSphere(SetPtr);
		}
		//DynamicCollisionSphere
		//CollisionSphereの派生クラスをCollisionにセットする
		template <typename T>
		void SetDynamicCollisionSphere(const shared_ptr<T>& SetPtr){
			//CollisionSphereにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<CollisionSphere>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionSphereにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicCollisionSphere<T>()"
					);
			}
			SetCollisionSphere(SetPtr);
		}
		//CollisionObb特殊化
		template <>
		void SetComponent<CollisionObb>(const shared_ptr<CollisionObb>& SetPtr){
			SetCollisionObb(SetPtr);
		}
		//DynamicCollisionObb
		//CollisionObbの派生クラスをCollisionにセットする
		template <typename T>
		void SetDynamicCollisionObb(const shared_ptr<T>& SetPtr){
			//CollisionObbにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<CollisionObb>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはCollisionObbにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicCollisionObb<T>()"
					);
			}
			SetCollisionObb(SetPtr);
		}


		//TransformMatrix特殊化
		template <>
		void SetComponent<TransformMatrix>(const shared_ptr<TransformMatrix>& SetPtr){
			SetTransformMatrix(SetPtr);
		}
		//DynamicTransformMatrix
		//TransformMatrixの派生クラスをTransformMatrixにセットする
		template <typename T>
		void SetDynamicTransformMatrix(const shared_ptr<T>& SetPtr){
			//TransformMatrixにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<TransformMatrix>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはTransformMatrixにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicTransform<T>()"
					);
			}
			SetTransformMatrix(SetPtr);
		}


		//Transform特殊化
		template <>
		void SetComponent<Transform>(const shared_ptr<Transform>& SetPtr){
			SetTransform(SetPtr);
		}
		//DynamicTransform
		//Transformの派生クラスをTransformにセットする
		template <typename T>
		void SetDynamicTransform(const shared_ptr<T>& SetPtr){
			//Transformにキャストしてみる
			auto RetPtr = dynamic_pointer_cast<Transform>(SetPtr);
			if (!RetPtr){
				//キャストできない
				throw BaseMBException(
					"そのコンポーネントはTransformにキャストできません。",
					typeid(T).name(),
					"GameObject::SetDynamicTransform<T>()"
					);
			}
			SetTransform(SetPtr);
		}
		//コンポーネントを削除
		template <typename T>
		void RemoveComponent(){
			RemoveTgtComponent(type_index(typeid(T)));
		}
		//Rigidbody特殊化
		//削除ではなく、null初期化
		template <>
		void RemoveComponent<Rigidbody>(){
			SetRigidbody(nullptr);
		}
		//DynamicRigidbody特殊化
		//削除ではなく、null初期化
		template <typename T>
		void RemoveDynamicRigidbody(bool ExceptionActive = true){
			//指定の型にキャストできなければ例外が出ることもある
			auto Ptr = GetDynamicRigidbody<T>(ExceptionActive);
			//例外が出なければnull初期化
			SetRigidbody(nullptr);
		}
		//Gravity特殊化
		//削除ではなく、null初期化
		template <>
		void RemoveComponent<Gravity>(){
			SetGravity(nullptr);
		}
		//DynamicGravity特殊化
		//削除ではなく、null初期化
		template <typename T>
		void RemoveDynamicGravity(bool ExceptionActive = true){
			//指定の型にキャストできなければ例外が出ることもある
			auto Ptr = GetDynamicGravity<T>(ExceptionActive);
			//例外が出なければnull初期化
			SetGravity(nullptr);
		}
		//DynamicCollision特殊化
		//削除ではなく、null初期化
		template <typename T>
		void RemoveDynamicCollision(bool ExceptionActive = true){
			//指定の型にキャストできなければ例外が出ることもある
			auto Ptr = GetDynamicCollision<T>(ExceptionActive);
			//例外が出なければnull初期化
			SetCollision(nullptr);
		}
		//CollisionSphere特殊化
		//削除ではなく、null初期化
		template <>
		void RemoveComponent<CollisionSphere>(){
			SetCollisionSphere(nullptr);
		}
		//DynamicCollisionSphere特殊化
		//削除ではなく、null初期化
		template <typename T>
		void RemoveDynamicCollisionSphere(bool ExceptionActive = true){
			//指定の型にキャストできなければ例外が出ることもある
			auto Ptr = GetDynamicCollisionSphere<T>(ExceptionActive);
			//例外が出なければnull初期化
			SetCollisionSphere(nullptr);
		}
		//CollisionObb特殊化
		//削除ではなく、null初期化
		template <>
		void RemoveComponent<CollisionObb>(){
			SetCollisionObb(nullptr);
		}
		//DynamicCollisionObb特殊化
		//削除ではなく、null初期化
		template <typename T>
		void RemoveDynamicCollisionObb(bool ExceptionActive = true){
			//指定の型にキャストできなければ例外が出ることもある
			auto Ptr = GetDynamicCollisionObb<T>(ExceptionActive);
			//例外が出なければnull初期化
			SetCollisionObb(nullptr);
		}
		//RemoveComponentはTransformとTransformMatrix特殊化は用意しない
		//もしTransformが削除されようとしても
		//RemoveComponentが参照するリストには
		//Transformは存在しないので安全である

		//コンポーネント操作
		void ComponentUpdate();
		void ComponentUpdate2();
		void DrawShadowmap();
		void ComponentDraw();

		//仮想関数
		virtual void OnPreCreate()override;
		virtual void OnUpdate()override{}
		virtual void OnPreDraw(){}
		virtual void OnDraw()override;
		//衝突判定チェック（ステージより呼ばれる）
		void CollisionChk();
		//衝突時のイベント
		virtual void OnCollision(const shared_ptr<GameObject>& other) {}

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup: public Object;
	//	用途: ゲームオブジェクトのweak_ptrをグループ化したもの
	//--------------------------------------------------------------------------------------
	class GameObjectGroup : public Object{
	public:
		GameObjectGroup();
		virtual ~GameObjectGroup();
		//アクセサ
		const vector< weak_ptr<GameObject> >& GetGroupVector() const;
		shared_ptr<GameObject> at(size_t index);
		size_t size() const;
		//操作
		void IntoGroup(const shared_ptr<GameObject>& Obj);
		void AllClear();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	struct ParticleSprite;
	//	用途: パーティクル内の、1つのスプライトをあらわす構造体
	//--------------------------------------------------------------------------------------
	struct ParticleSprite{
		bool m_Active;				//アクティブかどうか
		Color4 m_Color;				//色（透けさせる場合はaを0にする）
		Vector2 m_LocalScale;		//ローカルスケーリング（Zは1.0固定）		
		Quaternion m_LocalQt;		//ローカル回転。手動のときのみ使用
		Vector3 m_LocalPos;			//ローカル位置
		Vector2 m_UVLeftTop;		//UVの左上
		Vector2 m_UVRightBottom;	//UVの右下
		Vector3 m_Velocity;			//速度(必要に応じて使用)
		ParticleSprite() :
			m_Active(true),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_LocalScale(1.0f, 1.0f),
			m_LocalQt(),
			m_LocalPos(0, 0, 0),
			m_UVLeftTop(0, 0),
			m_UVRightBottom(1.0f, 1.0f),
			m_Velocity(0, 0, 0)
		{}
		void Reflesh(){
			m_Active = true;
			m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = Vector2(1.0f, 1.0f);
			m_LocalQt.Identity();
			m_LocalPos = Vector3(0, 0, 0);
			m_UVLeftTop = Vector2(0, 0);
			m_UVRightBottom = Vector2(1.0f, 1.0f);
			m_Velocity = Vector3(0, 0, 0);
		}
	};
	class ParticleManager;
	//--------------------------------------------------------------------------------------
	//	class Particle: public Object;
	//	用途: パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	class Particle : public Object{
	public:
		enum DrawOption{
			Billboard,	//ビルボード
			Faceing,	//フェイシング
			FaceingY,	//フェイシング(Y軸のみ)
			Normal		//通常(手動)
		};
		explicit Particle(size_t Count,DrawOption Option = DrawOption::Billboard);
		virtual ~Particle();
		//アクセサ
		DrawOption GetDrawOption()const;
		void SetDrawOption(DrawOption Option);

		bool IsActive() const;
		bool IsAllActive() const;
		void SetAllActive();
		void SetAllNoActive();

		void Reflesh(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);

		vector<ParticleSprite>& GetParticleSpriteVec() const;

		const Vector3& GetEmitterPos() const;
		void SetEmitterPos(const Vector3& Pos);

		float GetTotalTime() const;
		void SetTotalTime(float f);
		void AddTotalTime(float f);

		float GetMaxTime() const;
		void SetMaxTime(float f);


		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);

		//操作
		virtual void Draw(const shared_ptr<ParticleManager>& Manager);

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	用途: マルチエフェクト 
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject{
	public:
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		virtual ~MultiParticle();
		//アクセサ
		vector< shared_ptr<Particle> >& GetParticleVec() const;
		//操作
		shared_ptr<Particle> InsertParticle(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	用途: パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	class ParticleManager : public GameObject{
		void DrawSub(const shared_ptr<TextureResource>& TextureRes, UINT StartIndex, UINT DrawCount);
	public:
		//構築と消滅
		explicit ParticleManager(const shared_ptr<Stage>& StagePtr);
		virtual ~ParticleManager();
		//初期化
		virtual void OnCreate() override;

		//アクセサ
		bool GetZBufferUse() const;
		bool IsZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);


		bool GetSamplerWrap() const;
		bool IsSamplerWrap() const;
		void SetSamplerWrap(bool b);


		//操作
		void AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
			const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);

		virtual void OnUpdate() override{}
		virtual void OnDraw()override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	

	//--------------------------------------------------------------------------------------
	//	class InputTextManager : public GameObject;
	//	用途: 入力テキストマネージャ
	//--------------------------------------------------------------------------------------
	class InputTextManager : public GameObject{
	public:
		//構築と消滅
		explicit InputTextManager(const shared_ptr<Stage>& StagePtr);
		virtual ~InputTextManager();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		shared_ptr<StringSprite> GetFocusInputString() const;
		void SetFocusInputString(const shared_ptr<StringSprite>& Ptr);

		//操作
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void OnUpdate() override{}
		virtual void OnDraw()override{}
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	


	//--------------------------------------------------------------------------------------
	//	class Stage: public GameObject;
	//	用途: ステージクラス
	//--------------------------------------------------------------------------------------
	class Stage : public GameObject{
		//プライベートサブ関数
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		shared_ptr<GameObject> GetSharedGameObjectEx(const wstring& Key,bool ExceptionActive) const;
		void DrawBackColor();
		void SetParentStage(const shared_ptr<Stage>& ParentStage);
		void AddChileStageBase(const shared_ptr<Stage>& ChildStage);
	protected:
		//構築と破棄
		Stage();
		virtual ~Stage();
	public:
		//アクセサ
		shared_ptr<ShadowMapRenderTarget> GetShadowMapRenderTarget() const;
		shared_ptr<DefaultRenderTarget> GetDefaultRenderTarget() const;
		shared_ptr<RenderState> GetRenderState() const;
		shared_ptr<ParticleManager> GetParticleManager() const;
		shared_ptr<InputTextManager> GetInputTextManager() const;

		vector< shared_ptr<GameObject> >& GetGameObjectVec();
		void SetGameObjectVec(vector< shared_ptr<GameObject> >& vec);
		size_t GetDrawViewIndex() const;
		void SetDrawViewIndex(size_t i);

		vector< shared_ptr<Stage> >& GetChileStageVec();
		template<typename T>
		shared_ptr<Stage> AddChileStage(){
			auto Ptr = ObjectFactory::Create<T>();
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr){
				throw BaseMBException(
					"以下はStageに型キャストできません。",
					typeid(T).name(),
					"Stage::AddChileStage<T>()"
					);
			}
			AddChileStageBase(Ptr);
			return Ptr;
		}

		shared_ptr<Stage> GetParentStage() const;

		//サービスアクセサ
		const D3D11_VIEWPORT& GetViewPort(size_t Index) const;
		const D3D11_VIEWPORT& GetTargetViewPort() const;
		const D3D11_VIEWPORT* GetViewPortRealPtr(size_t Index) const;
		const D3D11_VIEWPORT* GetTargetViewPortRealPtr() const;
		const Color4& GetBackColor(size_t Index) const;
		const Color4& GetTargetBackColor() const;
		shared_ptr<Camera> GetCamera(size_t Index) const;
		shared_ptr<Camera> GetTargetCamera() const;
		shared_ptr<MultiLight> GetMultiLight(size_t Index)const;
		shared_ptr<MultiLight> GetTargetMultiLight() const;
		shared_ptr<Light> GetLight(size_t ViewIndex, size_t LightIndex) const;
		shared_ptr<Light> GetTargetLight(size_t LightIndex) const;

		//操作

		//GameObjectの新規追加
		template<typename T,typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params){
			try{
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(),params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...){
				throw;
			}
		}

		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObjectWithParam(Ts&&... params){
			try{
				auto Ptr = ObjectFactory::CreateGameObjectWithParam<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...){
				throw;
			}
		}



		//すでに存在するGemeObjectの追加
		shared_ptr<GameObject> AddGameObject(const shared_ptr<GameObject>& Obj);

		//追加待ちになってるオブジェクトを追加する
		void SetWaitToObjectVec();

		template<typename T>
		shared_ptr<T> GetSharedGameObject(const wstring& Key, bool ExceptionActive = true)const{
			shared_ptr<T> Ptr = dynamic_pointer_cast<T>(GetSharedGameObjectEx(Key, ExceptionActive));
			if (Ptr){
				return Ptr;
			}
			else{
				//キャストできない
				if (ExceptionActive){
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"オブジェクトをT型にキャストできません",
						keyerr,
						L"Stage::GetSharedGameObject()"
						);
				}
			}
			return nullptr;
		}
		shared_ptr<Object> GetSharedObject(const wstring& Key, bool ExceptionActive = true)const;
		void SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr);

		shared_ptr<GameObjectGroup> CreateSharedObjectGroup(const wstring& Key);
		shared_ptr<GameObjectGroup> GetSharedObjectGroup(const wstring& Key,bool ExceptionActive = true)const;
		template<typename T>
		shared_ptr<T> GetSharedObjectGroup(const wstring& Key, bool ExceptionActive = true)const{
			auto RetPtr = dynamic_pointer_cast<T>(GetSharedObjectGroup(Key, ExceptionActive));
			if (RetPtr){
				return RetPtr;
			}
			else{
				if (ExceptionActive){
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"指定のきーのグループはT型に変換できません",
						keyerr,
						L"Stage::GetSharedObjectGroup<T>()"
						);
				}
			}
			return nullptr;
		}


		void SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr);

		//デフォルトのレンダリングターゲット類を準備する
		void CreateDefaultRenderTargets(float ShadowMapDimension);

		//仮想関数
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void OnPreCreate()override;

		//ステージ内の更新（シーンからよばれる）
		virtual void UpdateStage();
		//ステージ内の描画（シーンからよばれる）
		virtual void DrawStage();


		//仮想関数
		//更新,描画はここでは実装しない
		//派生クラスGameStage等では実装できる
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	用途: マルチオーディオ
	// ＊コンポーネントではないのでシーンに持たせることができる
	//--------------------------------------------------------------------------------------
	class MultiAudioObject : public Object{
	public:
		//構築と破棄
		MultiAudioObject();
		virtual ~MultiAudioObject();
		//アクセサ
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey, bool ExceptionActive = true) const;
		//操作
		shared_ptr<AudioResource> AddAudioResource(const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class SceneBase: public Object, public ShapeInterface;
	//	用途: シーンベースクラス
	//--------------------------------------------------------------------------------------
	class SceneBase : public Object, public ShapeInterface
	{
	public:
		//構築と破棄
		SceneBase();
		virtual ~SceneBase();
		//アクセッサ
		shared_ptr<Stage> GetActiveStage() const;
		void SetActiveStage(const shared_ptr<Stage>& stage);
		shared_ptr<EventDispatcher> GetEventDispatcher() const;


		//操作
		//アクティブなステージを設定して初期化する
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params){
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr){
				throw BaseMBException(
					"以下はStageに型キャストできません。",
					typeid(T).name(),
					"SceneBase::ResetActiveStage<T>()"
					);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void OnPreCreate()override;
		//シーンを変化させる
		virtual void OnUpdate()override;
		//シーンを描画
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}

//endof  basedx11
