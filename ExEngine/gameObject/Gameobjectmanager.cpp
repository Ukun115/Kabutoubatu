/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{
	//死亡フラグがついているゲームオブジェクトを破棄する。
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
	//物理エンジンのアップデートを呼び出す。
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}

//ポーズ中のみ呼ばれる更新処理を実行
void GameObjectManager::ExecutePauseUpdate()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->PauseUpdateWrapper();
		}
	}
}

//UI演出中のみ呼ばれる更新処理を実行
void GameObjectManager::ExecuteUiUpdate()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UiUpdateWrapper();
		}
	}
}

//モデルのレンダー
void GameObjectManager::ExecuteModelRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ModelRenderWrapper(rc);
		}
	}
}

//フォントのレンダー
void GameObjectManager::ExecuteFontDataRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontDataRenderWrapper(rc);
		}
	}
}

//フォント(サブ)のレンダー
void GameObjectManager::ExecuteFontDataRenderSub(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontDataRenderWrapperSub(rc);
		}
	}
}

//シャドウのレンダー
void GameObjectManager::ExecuteShadowRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ShadowRenderWrapper(rc);
		}
	}
}

//ZPrepassのレンダー
void GameObjectManager::ExecuteZPrepassRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ZPrepassRenderWrapper(rc);
		}
	}
}

//シルエットのレンダー
void GameObjectManager::ExecuteSilhouetteRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SilhouetteRenderWrapper(rc);
		}
	}
}

//スプライトのレンダー
void GameObjectManager::ExecuteSpriteRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SpriteRenderWrapper(rc);
		}
	}
}


//スプライト(サブ)のレンダー
void GameObjectManager::ExecuteSpriteRenderSub(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SpriteRenderWrapperSub(rc);
		}
	}
}

//ミニマップのレンダー
void GameObjectManager::ExecuteMiniMapRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->MiniMapRenderWrapper(rc);
		}
	}
}