/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
#include "stdafx.h"
#include "TitleScene.h"
#include "../InGame/GameScene.h"
#include "../InGame/Online/Online.h"
#include "../InGame/Pause.h"
#include "TitleSceneSprite.h"
#include "TitleSceneSound.h"

namespace nsKabutoubatu
{
	namespace nsTitleScene
	{
		//�^�C�}�[��0�ɏ���������
		const int TIMER_COUNT_INIT = 0;
		//�J�[�\���̏����ʒu
		const int CURSOR_INIT_POS = 0;
		//�V�[���J�ڂł���t���[���Ԋu
		const int CAN_SCENECHANGE_FRAME_INTERVAL = 5;
		//�I�����C��������Ȃ��Ƃ��̒ʏ�fps�l
		const int NORMAL_FPS_VALUE = 60;
	}

	bool TitleScene::Start()
	{
		//�I�����C���ʐM�G���W���𐶐�
		m_online = NewGO<nsKabutoubatu::Online>(nsStdafx::PRIORITY_0, nsStdafx::ONLINE_NAME);

		//�^�C�g���V�[���̉摜�N���X�𐶐�
		m_titleSceneSprite = NewGO<TitleSceneSprite>();

		//�^�C�g���V�[���̃T�E���h�N���X�𐶐�
		m_titleSceneSound = NewGO<TitleSceneSound>();

		return true;
	}

	TitleScene::~TitleScene()
	{
		//�^�C�g���V�[���̉摜�N���X���폜
		DeleteGO(m_titleSceneSprite);
	}

	void TitleScene::Update()
	{
		switch (m_selectState)
		{
			//�J�nor�I���I��
		case enStartOrEnd:
			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
					//�͂��߂�
				case enSelectStart:
					//���[�h�I���Ɉڍs
					m_selectState = enGameModeSelect;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					break;
					//�����
				case enSelectEnd:
					//exe����ăQ�[���I��
					exit(EXIT_SUCCESS);
					//����//
					//exit(EXIT_FAILURE);�ُ͈�I��		EXIT_FAILURE = 1
					//exit(EXIT_SUCCESS);�͐���I��		EXIT_SUCCESS = 0
					break;
				}
			}

			//�J�[�\���̈ړ�����
			CursorMove(enSelectEnd);

			break;

			//�Q�[�����[�h�I��
		case enGameModeSelect:
			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
					//�I�����C�����[�h
				case enSelectOnline:
					//�I�����C���̃��[���쐬or���[���Q���ֈڍs
					m_selectState = enOnlineRoomCreateOrJoin;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;

					break;

					//���[�J���}���`���[�h
				case enSelectLocalMult:
					//�Q�[���V�[���ɑJ�ڂ��鏈��
					ChangeGameScene(enSelectLocalMult);

					break;

					//�\�����[�h
				case enSelectSolo:
					//�Q�[���V�[���ɑJ�ڂ��鏈��
					ChangeGameScene(enSelectSolo);

					break;

					//���ǂ�
				case enSelectBack:
					//�͂��߂邨���I���ɂ��ǂ�
					m_selectState = enStartOrEnd;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;

					break;
				}
			}

			//�J�[�\���̈ړ�����
			CursorMove(enSelectBack);

			break;

			//�I�����C���̃��[���쐬or���[���Q��
		case enOnlineRoomCreateOrJoin:
			//�I�����C���̃f�[�^���폜
			m_online->DeleteData();

			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
					//���[���쐬
				case enRoomCreate:
					m_online->SetPlayerNo(enPlayer1);	//1P�Ƃ��ēo�^
					//�I�����C���}�b�`�ҋ@���Ɉڍs
					m_selectState = enOnlineMatchWaiting;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					m_isRoomCreate = true;
					break;

					//���[���Q��
				case enRoomJoin:
					m_online->SetPlayerNo(enPlayer2);	//2P�Ƃ��ēo�^
					//�I�����C���}�b�`�ҋ@���Ɉڍs
					m_selectState = enOnlineMatchWaiting;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					m_isRoomCreate = false;
					break;

					//�߂�
				case enSelectBackGameMode:
					//�Q�[�����[�h�I���ɂ��ǂ�
					m_selectState = enGameModeSelect;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
					break;
				}
			}

			//�J�[�\���̈ړ�����
			CursorMove(enSelectBackGameMode);

			break;

			//�I�����C���}�b�`�ҋ@��
		case enOnlineMatchWaiting:
			//�I�����C������
			m_online->OnlineInit();

			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[enPlayer1]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();
				//�I�����C���̃��[���쐬or���[���Q���ɖ߂�
				m_selectState = enOnlineRoomCreateOrJoin;
				//�J�[�\���������ʒu�ɖ߂�
				m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
			}

			//�J�[�\���̈ړ�����
			CursorMove(0);


			//�I�����C���}�b�`���O����������A
			if (m_online->GetIsJoined())
			{
				//�J��
				ChangeGameScene(enSelectOnline);
			}
			break;
		}
	}

	//�Q�[���V�[���ɑJ�ڂ��郁�\�b�h
	void TitleScene::ChangeGameScene(const int mode)
	{
		if (m_gameScene != nullptr)
		{
			return;
		}
		//�Q�[���V�[���𐶐�
		m_gameScene = NewGO<GameScene>(nsStdafx::PRIORITY_0,nsStdafx::GAMESCENE_NAME);
		switch (mode)
		{
		//�I�����C�����[�h
		case enSelectOnline:
			// ���������삷��Q�[���p�b�h�i���o�[��ݒ肷��
			m_playerNo = m_online->GetPlayerNo();
			//�ʐM����̃Q�[���p�b�h�i���o�[��ݒ肷��
			m_otherPlayerNo = m_online->GetOtherPlayerNo();

			//�Q�[���p�b�h�̔ԍ���ݒ�
			m_online->GetPlayerGamePad(m_playerNo).Init(m_playerNo);
			m_online->GetPlayerGamePad(m_otherPlayerNo).Init(m_otherPlayerNo);

			//���g�̃Q�[���p�b�h�i���o�[���Q�[���V�[���N���X�ɓn��
			m_gameScene->SetPlayerNo(m_playerNo);
			//���g�̃Q�[���p�b�h���Q�[���V�[���N���X�ɓn��
			m_gameScene->SetPlayerGamePad(m_online->GetPlayerGamePad(m_playerNo));
			//�ʐM����̃Q�[���p�b�h�i���o�[���Q�[���V�[���N���X�ɓn��
			m_gameScene->SetOtherPlayerNo(m_otherPlayerNo);
			//�ʐM����̃Q�[���p�b�h���Q�[���V�[���N���X�ɓn��
			m_gameScene->SetOtherPlayerGamePad(m_online->GetPlayerGamePad(m_otherPlayerNo));

			//���g�̃Q�[���p�b�h�ԍ����o�͂Ƀf�o�b�N�\��
			sprintf(m_gamePadNo, "�����̃Q�[���p�b�h�ԍ���%d�ł��B\n", m_playerNo);
			OutputDebugStringA(m_gamePadNo);
			//�ʐM����̃Q�[���p�b�h�ԍ����o�͂Ƀf�o�b�N�\��
			sprintf(m_gamePadNo, "����̃Q�[���p�b�h�ԍ���%d�ł��B\n", m_otherPlayerNo);
			OutputDebugStringA(m_gamePadNo);

			m_gameScene->SetOnlinePlay(true);

			m_online->GameStart();

			break;

		//���[�J���}���`�v���C���[�h
		case enSelectLocalMult:
			//�|�[�Y�@�\���g����悤�ɂ���
			m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);
			m_pause->SetCanPause(true);
			break;

			//�\�����[�h
		case enSelectSolo:
			//�v���C���[�̐l������l�ɕύX����B
			m_gameScene->SetPlayerNumber(enPlayer2);

			//�|�[�Y�@�\���g����悤�ɂ���
			m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);
			m_pause->SetCanPause(true);

			break;
		}

		//�^�C�g���V�[����j��
		DeleteGO(this);
	}

	//�J�[�\���̈ړ�����
	void TitleScene::CursorMove(const int lastSelect)
	{
		//�I���ł���̂��P�����Ȃ��ꍇ�̓J�[�\���ړ��������s��Ȃ��B
		if (lastSelect == nsTitleScene::CURSOR_INIT_POS)
		{
			return;
		}

		//��{�^���������ꂽ��A
		if (g_pad[enPlayer1]->IsTrigger(enButtonUp))
		{
			//�O�̑I���ɃJ�[�\���ړ�
			m_cursorSelect--;
			//�����I���ꏊ����ԏ�̑O�������Ƃ��A
			if (m_cursorSelect < nsTitleScene::CURSOR_INIT_POS)
			{
				//�I���ꏊ����ԉ��ɂ���
				m_cursorSelect = lastSelect;
			}

			//�I�����Đ�
			m_titleSceneSound->PlaySelectSound();
		}
		//���{�^���������ꂽ��A
		if (g_pad[enPlayer1]->IsTrigger(enButtonDown))
		{
			//���̑I���ɃJ�[�\���ړ�
			m_cursorSelect++;
			//�����I���ꏊ����ԉ��̎��������Ƃ��A
			if (m_cursorSelect > lastSelect)
			{
				//�I���ꏊ����ԏ�ɂ���
				m_cursorSelect = nsTitleScene::CURSOR_INIT_POS;
			}

			//�I�����Đ�
			m_titleSceneSound->PlaySelectSound();
		}
	}
};