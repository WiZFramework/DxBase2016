#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Scene : public SceneBase;
	//	用途: シーンクラス
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try{
			//最初のアクティブステージの設定
			ResetActiveStage<MenuStage>();
		}
		catch (...){
			throw;
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event)
	{
		if (event->m_MsgStr == L"ToGame"){
			ResetActiveStage<GameStage>();
		}

		if (event->m_MsgStr == L"ToMenu"){
			ResetActiveStage<MenuStage>();
		}
	}


}
//end basedx11
