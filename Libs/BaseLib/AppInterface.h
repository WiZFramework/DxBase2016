#pragma once
#include "stdafx.h"

namespace basedx11{

	class ObjectFactory;

	//--------------------------------------------------------------------------------------
	//	class ObjectInterface;
	/*!
	Create��PreCreate�������AThis�X�}�[�g�|�C���^���Ƃ��C���^�[�t�F�C�X
	*/
	//--------------------------------------------------------------------------------------
	class ObjectInterface : public std::enable_shared_from_this<ObjectInterface> {
		friend class ObjectFactory;
		//�N���G�C�g�ς݂��ǂ���
		//Create�֐����Ăяo�����true�ɂȂ�
		bool m_Created{ false };
		void SetCreated(bool b) {
			m_Created = b;
		}
	protected:
		//�\�z
		ObjectInterface(){}
		//�j��
		virtual ~ObjectInterface() {}
	public:
		//this�|�C���^�擾
		template<typename T>
		std::shared_ptr<T> GetThis() {
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				throw BaseException(
					L"this��T�^�ɃL���X�g�ł��܂���",
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"ObjectInterface::GetThis()"
					);
			}
			return nullptr;
		}
		//���������s���i���z�֐��j
		//��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�\�z����
		virtual void OnPreCreate() = 0;
		virtual void OnCreate() = 0;
		//�N���G�C�g�ς݂��ǂ���
		bool IsCreated() {
			return m_Created;
		}
	private:
		//�R�s�[�֎~
		ObjectInterface(const ObjectInterface&) = delete;
		ObjectInterface& operator=(const ObjectInterface&) = delete;
		//���[�u�֎~
		ObjectInterface(const ObjectInterface&&) = delete;
		ObjectInterface& operator=(const ObjectInterface&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class ObjectFactory;
	/*!
	Object�C���^�[�t�F�C�X�̔h���N���X���\�z����
	*/
	//--------------------------------------------------------------------------------------
	class Stage;
	class ObjectFactory {
	public:
		//�I�u�W�F�N�g�쐬�istatic�֐��j
		template<typename T, typename... Ts>
		static shared_ptr<T> Create(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}

		//�I�u�W�F�N�g�쐬�istatic�֐��j
		//�R���X�g���N�^�ł͂Ȃ��ACreateWithParam()�֐��Ńp�����[�^��n���ꍇ
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateWithParam(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T());
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}

		template<typename T, typename... Ts>
		static shared_ptr<T> CreateGameObjectWithParam(const shared_ptr<Stage>& StagePtr, Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(StagePtr));
			//���z�֐��Ăяo��
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	class ShapeInterface;
	//	�p�r: Update��Draw�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class ShapeInterface {
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	ShapeInterface();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface() {}
		//--------------------------------------------------------------------------------------
		//	virtual ~ShapeInterface();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface() {}
		//����
		//--------------------------------------------------------------------------------------
		//	virtual void OnUpdate() = 0;
		/*!
		@breaf OnUpdate����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void OnLastUpdate();
		/*!
		@breaf OnLastUpdate����.<br />
		���ŏI�A�b�v�f�[�g
		���������z�֐��ɂ��Ȃ�
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate() {}
		//--------------------------------------------------------------------------------------
		//	virtual void OnDraw() = 0;
		/*!
		@breaf OnDraw����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() = 0;
	private:
		//�R�s�[�֎~
		ShapeInterface(const ShapeInterface&) = delete;
		ShapeInterface& operator=(const ShapeInterface&) = delete;
		//���[�u�֎~
		ShapeInterface(const ShapeInterface&&) = delete;
		ShapeInterface& operator=(const ShapeInterface&&) = delete;
	};

}
// end namespace basedx11


