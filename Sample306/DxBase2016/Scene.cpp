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
			ResetActiveStage<GameStage>();
		}
		catch (...){
			throw;
		}
	}

}
//end basedx11
