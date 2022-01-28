/// <summary>
/// �Q�[���N���A�V�[���N���X
/// </summary>
#include "stdafx.h"
#include "GameClearScene.h"
#include "../Utilities/GameUpdateStop.h"
#include "../InGame/Player/PlayerStatus.h"
#include "../Title/TitleScene.h"
#include "../InGame/GameScene.h"

namespace nsKabutoubatu
{
	namespace nsGameClearScene
	{
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
		const float     FONT_ROT = 0.0f;
		const Vector2   FONT_PIV = { 1.0f,1.0f };
		const float     FONT_SCA = 0.7f;
	}

	bool GameClearScene::Start()
	{
		//�C���X�^���X������
		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
		m_gameUpdateStop = FindGO<GameUpdateStop>(nsStdafx::GAMEUPDATESTOP_NAME);

		//�Q�[���̍X�V���~�߂�
		m_gameUpdateStop->SetIsStop(true);
		//UI�ړ��ɐ؂�ւ���
		m_gameUpdateStop->SetMoveTarget(0);

		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			//�C���X�^���X������
			m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

			//�G��|������/////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = 50.0f;
			m_enemyKillNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_enemyKillNumberFont[playerNum]->Init
			(
				L"",		//�e�L�X�g
				m_fontPos,	//�ʒu
				nsGameClearScene::FONT_COLOR,	//�F
				nsGameClearScene::FONT_ROT,		//�X��
				nsGameClearScene::FONT_SCA,		//�g�嗦
				nsGameClearScene::FONT_PIV		//��_
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetEnemyKillNum());
			m_enemyKillNumberFont[playerNum]->SetText(m_text);
			//���߂͔�\��
			m_enemyKillNumberFont[playerNum]->Deactivate();
			m_enemyKillNumberFont[playerNum]->StartEnd();
			//�ʒu���炵
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetEnemyKillNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//�ʒu�Z�b�g
					m_enemyKillNumberFont[playerNum]->SetPosition(m_fontPos);
					//for�����I��点��
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//�R�C������///////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -2.0f;
			m_coinGetNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_coinGetNumberFont[playerNum]->Init
			(
				L"",		//�e�L�X�g
				m_fontPos,	//�ʒu
				nsGameClearScene::FONT_COLOR,	//�F
				nsGameClearScene::FONT_ROT,		//�X��
				nsGameClearScene::FONT_SCA,		//�g�嗦
				nsGameClearScene::FONT_PIV		//��_
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetTotalCoinNum());
			m_coinGetNumberFont[playerNum]->SetText(m_text);
			//���߂͔�\��
			m_coinGetNumberFont[playerNum]->Deactivate();
			m_coinGetNumberFont[playerNum]->StartEnd();

			//�ʒu���炵
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetTotalCoinNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//�ʒu�Z�b�g
					m_coinGetNumberFont[playerNum]->SetPosition(m_fontPos);
					//for�����I��点��
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//�񕜉�/////////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -54.0f;
			m_recoveryReceiveNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_recoveryReceiveNumberFont[playerNum]->Init
			(
				L"",		//�e�L�X�g
				m_fontPos,	//�ʒu
				nsGameClearScene::FONT_COLOR,	//�F
				nsGameClearScene::FONT_ROT,		//�X��
				nsGameClearScene::FONT_SCA,		//�g�嗦
				nsGameClearScene::FONT_PIV		//��_
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetRecoveryReceiveNum());
			m_recoveryReceiveNumberFont[playerNum]->SetText(m_text);
			//���߂͔�\��
			m_recoveryReceiveNumberFont[playerNum]->Deactivate();
			m_recoveryReceiveNumberFont[playerNum]->StartEnd();
			//�ʒu���炵
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetRecoveryReceiveNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//�ʒu�Z�b�g
					m_recoveryReceiveNumberFont[playerNum]->SetPosition(m_fontPos);
					//for�����I��点��
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////


			//���S��/////////////////////////////////////////////////////////////////////////
			switch (playerNum)
			{
			case 0:
				m_fontPos.x = -15.0f;
				break;
			case 1:
				m_fontPos.x = 160.0f;
				break;
			}
			m_fontPos.y = -106.0f;
			m_deathNumberFont[playerNum] = NewGO<FontRenderSub>();
			m_deathNumberFont[playerNum]->Init
			(
				L"",		//�e�L�X�g
				m_fontPos,	//�ʒu
				nsGameClearScene::FONT_COLOR,	//�F
				nsGameClearScene::FONT_ROT,		//�X��
				nsGameClearScene::FONT_SCA,		//�g�嗦
				nsGameClearScene::FONT_PIV		//��_
			);

			swprintf_s(m_text, L"%d", m_playerStatus[playerNum]->GetDeathNum());
			m_deathNumberFont[playerNum]->SetText(m_text);
			//���߂͔�\��
			m_deathNumberFont[playerNum]->Deactivate();
			m_deathNumberFont[playerNum]->StartEnd();
			//�ʒu���炵
			for (int i = 1; i < 10; i++)
			{
				if (m_playerStatus[playerNum]->GetDeathNum() >= pow(10, i))
				{
					m_fontPos.x -= 6.0f;
				}
				else
				{
					//�ʒu�Z�b�g
					m_deathNumberFont[playerNum]->SetPosition(m_fontPos);
					//for�����I��点��
					i = 10;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////
		}

		//�Q�[���N���A�T�E���h���Đ�
		m_gameClearSound = NewGO<SoundSource>();
		m_gameClearSound->Init(L"Assets/sound/GameClear.wav");
		m_gameClearSound->SetVolume(0.7f);
		m_gameClearSound->Play(false);
		//�u���������I�I�v�摜���쐬
		m_toubatuKannryou = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_0);
		m_toubatuKannryou->Init("toubatuKannryou", 1280, 820);

		//�Q�[���N���A�摜���쐬
		m_gameClearSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_1);
		m_gameClearSprite->Init("GameClear", 1400.0f, 800.0f);
		m_gameClearSprite->SetMulColor(1.0f,1.0f,1.0f,0.0f);

		return true;
	}

	GameClearScene::~GameClearScene()
	{
		//�Q�[���N���A�T�E���h���폜
		DeleteGO(m_gameClearSound);
		//�u���������I�I�v�摜���폜
		DeleteGO(m_toubatuKannryou);
		//�Q�[���N���A�摜���폜
		DeleteGO(m_gameClearSprite);
		//����T�E���h���폜
		DeleteGO(m_decideSound);
		//�t�H���g
		for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
		{
			DeleteGO(m_enemyKillNumberFont[playerNum]);
			DeleteGO(m_coinGetNumberFont[playerNum]);
			DeleteGO(m_recoveryReceiveNumberFont[playerNum]);
			DeleteGO(m_deathNumberFont[playerNum]);
		}
	}

	void GameClearScene::UiUpdate()
	{
		if (m_spriteChangeTimer < 300)
		{
			m_spriteChangeTimer++;

			//�Q�[���N���A�����I��������A
			if (m_spriteChangeTimer == 250)
			{
				//�u���������I�I�v�摜���B��
				m_toubatuKannryou->Deactivate();
				//�Q�[���N���A�摜��\��
				m_gameClearSprite->SetMulColor(1.0f, 1.0f, 1.0f, 1.0f);

				//�X�R�A��\��
				for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
				{
					m_enemyKillNumberFont[playerNum]->Activate();
					m_coinGetNumberFont[playerNum]->Activate();
					m_recoveryReceiveNumberFont[playerNum]->Activate();
					m_deathNumberFont[playerNum]->Activate();
				}
			}
		}
		else
		{
			//A�{�^���������ꂽ��^�C�g����ʂɖ߂�
			for (int playerNum = 0; playerNum < m_playerNum; playerNum++)
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