/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
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
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
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
	//�����G���W���̃A�b�v�f�[�g���Ăяo���B
	PhysicsWorld::GetInstance()->Update(1.0f/60.0f);
}

//�|�[�Y���̂݌Ă΂��X�V���������s
void GameObjectManager::ExecutePauseUpdate()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->PauseUpdateWrapper();
		}
	}
}

//UI���o���̂݌Ă΂��X�V���������s
void GameObjectManager::ExecuteUiUpdate()
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UiUpdateWrapper();
		}
	}
}

//���f���̃����_�[
void GameObjectManager::ExecuteModelRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ModelRenderWrapper(rc);
		}
	}
}

//�t�H���g�̃����_�[
void GameObjectManager::ExecuteFontDataRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontDataRenderWrapper(rc);
		}
	}
}

//�t�H���g(�T�u)�̃����_�[
void GameObjectManager::ExecuteFontDataRenderSub(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontDataRenderWrapperSub(rc);
		}
	}
}

//�V���h�E�̃����_�[
void GameObjectManager::ExecuteShadowRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ShadowRenderWrapper(rc);
		}
	}
}

//ZPrepass�̃����_�[
void GameObjectManager::ExecuteZPrepassRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ZPrepassRenderWrapper(rc);
		}
	}
}

//�V���G�b�g�̃����_�[
void GameObjectManager::ExecuteSilhouetteRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SilhouetteRenderWrapper(rc);
		}
	}
}

//�X�v���C�g�̃����_�[
void GameObjectManager::ExecuteSpriteRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SpriteRenderWrapper(rc);
		}
	}
}


//�X�v���C�g(�T�u)�̃����_�[
void GameObjectManager::ExecuteSpriteRenderSub(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->SpriteRenderWrapperSub(rc);
		}
	}
}

//�~�j�}�b�v�̃����_�[
void GameObjectManager::ExecuteMiniMapRender(RenderContext& rc)
{
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->MiniMapRenderWrapper(rc);
		}
	}
}