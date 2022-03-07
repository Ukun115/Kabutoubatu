/// <summary>
/// �Q�[���I�[�o�[�V�[���N���X
/// </summary>
#include "stdafx.h"
#include "GameOverScene.h"
#include "../InGame/GameScene.h"
#include "../Title/TitleScene.h"
#include "../Utilities/GameUpdateStop.h"

namespace nsKabutoubatu
{
	namespace nsGameOverScene
	{

	}

	bool GameOverScene::Start()
	{
		m_gameUpdateStop = FindGO<GameUpdateStop>(nsStdafx::GAMEUPDATESTOP_NAME);
		m_gameUpdateStop->SetIsStop(true);
		m_gameUpdateStop->SetMoveTarget(0);

		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);

		//�Q�[���I�[�o�[�W���O��
		m_gameOverGingle = NewGO<SoundSource>();
		m_gameOverGingle->Init(L"Assets/sound/GameOver.wav");
		m_gameOverGingle->SetVolume(0.5f);
		//�Q�[���I�[�o�[�W���O�����Đ�
		m_gameOverGingle->Play(false);	//�����V���b�g�Đ�

		//�w�i�摜���쐬
		m_gameOverSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_gameOverSprite->Init("GameOver", 1400.0f, 800.0f);
		m_gameOverSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);

		m_aPushSprite = NewGO<SpriteRender>(nsStdafx::PRIORITY_0);
		m_aPushSprite->Init("GameOverAPush", 1400.0f, 800.0f);
		m_aPushSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);

		return true;
	}

	GameOverScene::~GameOverScene()
	{
		//�w�i�摜���폜
		DeleteGO(m_gameOverSprite);
		//�uA�{�^���Ń^�C�g����ʂɖ߂�v�摜���폜
		DeleteGO(m_aPushSprite);
	}

	void GameOverScene::UiUpdate()
	{
		if (m_alphaValue <= 1.0f)
		{
			m_alphaValue += 0.008f;
			m_gameOverSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);
			m_aPushSprite->SetMulColor(1.0f, 1.0f, 1.0f, m_alphaValue);
		}
		else
		{
			//A�{�^���������ꂽ��^�C�g����ʂɖ߂�
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				if (m_decideSound == nullptr)
				{
					if (g_pad[playerNum]->IsTrigger(enButtonA))
					{
						//����T�E���h
						m_decideSound = NewGO<SoundSource>();
						m_decideSound->Init(L"Assets/sound/Decide.wav");
						m_decideSound->SetVolume(0.5f);
						//����T�E���h���Đ�
						m_decideSound->Play(false);	//�����V���b�g�Đ�

						m_gameUpdateStop->SetIsStop(false);
						//�Q�[���V�[�����폜����
						m_gameScene->DeleteGameSceneClass();
						//�^�C�g���V�[���𐶐�
						NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);
						//���̃N���X��j������
						DeleteGO(this);
					}
				}
			}
		}
	}
}