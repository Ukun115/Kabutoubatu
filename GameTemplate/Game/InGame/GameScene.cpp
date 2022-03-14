/// <summary>
/// �Q�[���V�[���N���X
/// </summary>
#include "stdafx.h"
#include "GameScene.h"
#include "Stage/Stage.h"
#include "Player/Player.h"
#include "Player/PlayerStatus.h"
#include "../Result/GameOverScene.h"
#include "GameScreenUI.h"
#include "Camera/PlayerCamera.h"
#include "../Graphics/SkyCube.h"
#include "Item/Weapon/Weapon.h"
#include "GameLimitTime.h"
#include "AccompanyAI/AccompanyAI.h"
#include "../Graphics/Fade.h"

namespace nsKabutoubatu
{
	namespace nsGameScene
	{
		const Vector2   NOWMONEY_POS[2] = { { -400.0f,-238.0f }, { 240.0f,-238.0f } };			//�������ԃt�H���g�̈ʒu
		const Vector2   RECOVERY_ITEM_NUM_POS[2] = { { -90.0f,-319.0f }, { 550.0f,-319.0f } };			//�������ԃt�H���g�̈ʒu
		const Vector4   FONT_COLOR = { 1.0f,1.0f,1.0f,1.0f };	//�������ԃt�H���g�̐F
		const float     FONT_ROT = 0.0f;			//�t�H���g�̌X��
		const Vector2   FONT_PIV = { 1.0f,1.0f };	//�t�H���g�̊�_
		const float     FONT_SCA = 0.5f;			//�t�H���g�̊g�嗦
	}

	bool GameScene::Start()
	{

		//�����_���֐��̎�����߂�(���l�͂Ȃ�ł�����)
		//�I�����C���̍ۂ̓����_�����l�𑊎葤�ƍ��킹�Ȃ��Ƃ����Ȃ��̂ŁA
		//�����_���̎�����킹��
		srand(100);

		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,1);

		//�X�J�C�L���[�u�𐶐�
		m_skyCube = NewGO<SkyCube>(nsStdafx::PRIORITY_0, nsStdafx::SKYCUBE_NAME);

		m_gameLimitTime = NewGO<GameLimitTime>(nsStdafx::PRIORITY_0,nsStdafx::GAMELIMITTIME);

		//�Q�[����ʂ�UI���쐬
		//��������ɕ\�����邽�߂ɗD��x��+1�����Ă���B
		m_gameScreenUI = NewGO<GameScreenUI>(nsStdafx::PRIORITY_1,nsStdafx::GAMESCREENUI_NAME);
		m_gameScreenUI->SetPlayerNumber(m_playerNum);

		//�v���C���[�J����
		m_playerCamera = NewGO <PlayerCamera>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_CAMERA_NAME);
		m_playerCamera->SetTargetPlayer(m_playerNo);

		//�X�e�[�W�𐶐�
		m_stage = NewGO<Stage>();
		m_stage->SetPlayerNumber(m_playerNum);

		//�I�����C���v���C�̎��A
		if (m_online)
		{
			//�v���C���[�𐶐�
			m_player[m_playerNo] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[m_playerNo]);
			m_weapon[m_playerNo] = NewGO<Weapon>(nsStdafx::PRIORITY_0, nsStdafx::WEAPON_NAME[m_playerNo]);
			PlayerInit(m_player[m_playerNo], m_playerNo, m_playerGamePad, m_weapon[m_playerNo]);

			//�v���C���[�𐶐�
			m_player[m_otherPlayerNo] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[m_otherPlayerNo]);
			m_weapon[m_otherPlayerNo] = NewGO<Weapon>(nsStdafx::PRIORITY_0, nsStdafx::WEAPON_NAME[m_otherPlayerNo]);
			PlayerInit(m_player[m_otherPlayerNo], m_otherPlayerNo, m_otherPlayerGamePad, m_weapon[m_otherPlayerNo]);

			//�v���C���[�̃p�b�h�����X�e�[�W�ɑ���
			m_stage->SetPlayerGamePad(*m_playerGamePad, m_playerNo);
			m_stage->SetPlayerGamePad(*m_otherPlayerGamePad, m_otherPlayerNo);
			m_stage->SetOnlinePlayerPadNo(m_playerNo);
		}
		//�I�����C���v���C����Ȃ��Ƃ��A
		else
		{
			//�v���C���[�̐l��������
			for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
			{
				//�v���C���[�𐶐�
				m_player[playerNum] = NewGO<Player>(nsStdafx::PRIORITY_0, nsStdafx::PLAYER_NAME[playerNum]);
				m_weapon[playerNum] = NewGO<Weapon>( nsStdafx::PRIORITY_0,nsStdafx::WEAPON_NAME[playerNum]);
				PlayerInit(m_player[playerNum], playerNum, g_pad[playerNum], m_weapon[playerNum]);
				if (m_playerNum == 1)
				{
					//�\�����[�h�Ƃ������Ƃ��v���C���[�N���X�ɓn��
					m_player[playerNum]->SetIsSoloPlay(true);
					//����AI�𐶐�
					m_accompanyAI = NewGO<AccompanyAI>(nsStdafx::PRIORITY_0,nsStdafx::ACCOMPANYAI_NAME);
				}
			}
		}

		//�X�e�[�W��������
		m_stage->Init();

		return true;
	}

	GameScene::~GameScene()
	{
		//�v���C���[�̐l��������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			//�v���C���[���폜
			DeleteGO(m_player[playerNum]);
			//�����������폜
			DeleteGO(m_nowMoney[playerNum]);
			//
			DeleteGO(m_haveRecoveryItemNum[playerNum]);
			//�����폜
			DeleteGO(m_weapon[playerNum]);

			//����AI���폜
			if (m_accompanyAI != nullptr)
			{
				DeleteGO(m_accompanyAI);
			}
		}

		//�Q�[���̐������ԃN���X���폜
		DeleteGO(m_gameLimitTime);

		//�X�J�C�L���[�u���폜
		DeleteGO(m_skyCube);

		//�v���C���[�J�������폜
		DeleteGO(m_playerCamera);

		//�Q�[����ʂ�UI���폜
		DeleteGO(m_gameScreenUI);

		//�X�e�[�W���폜
		DeleteGO(m_stage);
	}

	void GameScene::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
			//�k���|�C���^�����Ă����B
			m_fade = nullptr;
		}

		if (m_gameSceneSound == nullptr)
		{
			//�Q�[��BGM�̏�����
			m_gameSceneSound = NewGO<SoundSource>();
			m_gameSceneSound->Init(L"Assets/sound/DayBGM.wav");
			m_gameSceneSound->SetVolume(0.2f);
			//�Q�[��BGM���Đ�
			m_gameSceneSound->Play(true);	//���[�v�Đ�
		}

		//�Q�[���I�[�o�[�ڍs����
		GameOver();

		//�������Ԃ������Ȃ�ƁA
		if (m_gameLimitTime->TimeOver())
		{
			if (!m_gameOverScene)
			{
				//�Q�[���I�[�o�[��ʂɈړ�
				m_gameOverScene = NewGO<GameOverScene>();
				m_gameOverScene->SetPlayerNumber(m_playerNum);
				m_gameSceneSound->SetVolume(0.0f);
			}
		}

		//�v���C���[�̐l��������
		for (int playerNum = enPlayer1; playerNum < m_playerNum; playerNum++)
		{
			if (m_playerStatus[playerNum] == nullptr)
			{
				m_playerStatus[playerNum] = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[playerNum]);

				continue;
			}

			//�v���C���[�̉񕜃A�C�e���̏��������Z�b�g���A�X�V�B
			RecoveryItemNumUpdate(playerNum);
			//�v���C���[�̌��݂̂������Z�b�g���A�X�V�B
			NowMoneyNumUpdate(playerNum);
		}
	}

	//�Q�[���I�[�o�[�ڍs�������\�b�h
	void GameScene::GameOver()
	{
		//���񂾂Ƃ��A�i�S�[�X�g��Ԃ̎��j
		if (m_player[enPlayer1]->GetNowState() == 2)
		{
			if (m_playerNum == 2)
			{
				if (m_player[enPlayer2]->GetNowState() != 2)
				{
					return;
				}
			}
			if (!m_gameOverScene)
			{
				//�Q�[���I�[�o�[��ʂɈړ�
				m_gameOverScene = NewGO<GameOverScene>();
				m_gameOverScene->SetPlayerNumber(m_playerNum);
				m_gameSceneSound->SetVolume(0.0f);
			}
		}
	}

	//�񕜃A�C�e�������̏��������\�b�h
	void GameScene::RecoveryItemFontInit(const int playerNum)
	{
		m_haveRecoveryItemNum[playerNum] = NewGO<FontRender>();
		//�񕜃A�C�e���������t�H���g�̏�����
		m_haveRecoveryItemNum[playerNum]->Init
		(
			L"",			//�e�L�X�g
			nsGameScene::RECOVERY_ITEM_NUM_POS[playerNum],	//�ʒu
			nsGameScene::FONT_COLOR,	//�F
			nsGameScene::FONT_ROT,		//�X��
			nsGameScene::FONT_SCA,		//�g�嗦
			nsGameScene::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_haveRecoveryItemNum[playerNum]->SetShadowParam(true, 1.5f, Vector4::Black);
	}

	//���������̏��������\�b�h
	void GameScene::NowMoneyFontInit(const int playerNum)
	{
		//���݂̂�����UI��������
		m_nowMoney[playerNum] = NewGO<FontRender>();
		//�����t�H���g�̏�����
		m_nowMoney[playerNum]->Init
		(
			L"",			//�e�L�X�g
			nsGameScene::NOWMONEY_POS[playerNum],	//�ʒu
			nsGameScene::FONT_COLOR,	//�F
			nsGameScene::FONT_ROT,		//�X��
			nsGameScene::FONT_SCA,		//�g�嗦
			nsGameScene::FONT_PIV		//��_
		);
		//�����̋��E���\��
		m_nowMoney[playerNum]->SetShadowParam(true, 1.5f, Vector4::Black);
	}

	//�񕜃A�C�e���̏����������̐��X�V
	void GameScene::RecoveryItemNumUpdate(const int playerNum)
	{
		swprintf_s(m_haveRecoveryItemNumText, L"%d", m_playerStatus[playerNum]->GetRecoveryItemNum());
		m_haveRecoveryItemNum[playerNum]->SetText(m_haveRecoveryItemNumText);
	}

	//�����̏����������̐��X�V
	void GameScene::NowMoneyNumUpdate(const int playerNum)
	{
		//�v���C���[�̏������Ă���񕜃A�C�e�������Z�b�g���A�X�V�B
		swprintf_s(m_nowMoneyText, L"%d", m_playerStatus[playerNum]->GetCoinNum());
		m_nowMoney[playerNum]->SetText(m_nowMoneyText);
	}

	void GameScene::PlayerInit(Player* player,int playerNo,GamePad*gamePad,Weapon*weapon)
	{
		//1P��2P���̔ԍ����Z�b�g
		player->SetPlayerNum(playerNo);
		//���삷��p�b�h�̔ԍ����Z�b�g
		player->SetGamePad(*gamePad);
		//������Z�b�g
		weapon->SetPlayerNum(playerNo);
		//�������������ݒ�
		weapon->SetWeaponNum(enWoodStick);
		//���݂̂���������������
		NowMoneyFontInit(playerNo);
		//�񕜃A�C�e��������������
		RecoveryItemFontInit(playerNo);
	}
}