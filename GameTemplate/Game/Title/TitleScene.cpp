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
		//�V�[���J�ڂł���t���[���Ԋu
		const int CAN_SCENECHANGE_FRAME_INTERVAL = 5;
	}

	bool TitleScene::Start()
	{
		//�C���X�^���X������
		m_online = FindGO<Online>(nsStdafx::ONLINE_NAME);

		//�^�C�g���V�[���̉摜�N���X
		m_titleSceneSprite = NewGO<TitleSceneSprite>();

		//�^�C�g���V�[���̃T�E���h�N���X
		m_titleSceneSound = NewGO<TitleSceneSound>();

		return true;
	}

	TitleScene::~TitleScene()
	{
		//�^�C�g���V�[���̉摜�N���X���폜
		DeleteGO(m_titleSceneSprite);
		//�^�C�g���V�[���̃T�E���h�N���X���폜
		DeleteGO(m_titleSceneSound);
	}

	void TitleScene::Update()
	{
		switch (m_selectState)
		{
		//�J�nor�I���I��
		case 0:
			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
				//�͂��߂�
				case enSelectStart:
					//���[�h�I���Ɉڍs
					m_selectState = 1;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;
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
		case 1:
			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
				//�I�����C�����[�h
				case enSelectOnline:
					//�I�����C���̃��[���쐬or���[���Q���ֈڍs
					m_selectState = 2;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;

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
					m_selectState = 0;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;

					break;
				}
			}

			//�J�[�\���̈ړ�����
			CursorMove(enSelectBack);

			break;

			//�I�����C���̃��[���쐬or���[���Q��
		case 2:
			//�I�����C���̃f�[�^���폜
			m_online->DeleteData();

			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();

				//�J�[�\�����w���Ă�����̂̏������s��
				switch (m_cursorSelect)
				{
					//���[���쐬
				case enRoomCreate:
					m_online->SetPlayerNo(0);	//1P�Ƃ��ēo�^
					//�I�����C���}�b�`�ҋ@���Ɉڍs
					m_selectState = 3;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;
					m_isRoomCreate = true;
					break;

					//���[���Q��
				case enRoomJoin:
					m_online->SetPlayerNo(1);	//2P�Ƃ��ēo�^
					//�I�����C���}�b�`�ҋ@���Ɉڍs
					m_selectState = 3;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;
					m_isRoomCreate = false;
					break;

					//�߂�
				case enSelectBackGameMode:
					//�Q�[�����[�h�I���ɂ��ǂ�
					m_selectState = 1;
					//�J�[�\���������ʒu�ɖ߂�
					m_cursorSelect = 0;
					break;
				}
			}

			//�J�[�\���̈ړ�����
			CursorMove(enSelectBackGameMode);

			break;

		//�I�����C���}�b�`�ҋ@��
		case 3:
			//�I�����C������
			m_online->OnlineInit();

			//A�{�^���������ꂽ�Ƃ��A
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//���艹�Đ�
				m_titleSceneSound->PlayDecideSound();
				//�I�����C���̃��[���쐬or���[���Q���ɖ߂�
				m_selectState = 2;
				//�J�[�\���������ʒu�ɖ߂�
				m_cursorSelect = 0;
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
			// ���������삷��Q�[���p�b�h�i���o�[��n��
			m_playerNo = m_online->GetPlayerNo();
			m_otherPlayerNo = m_online->GetOtherPlayerNo();
			m_gameScene->SetPlayerNo(m_playerNo);
			m_gameScene->SetPlayerGamePad(m_online->GetPlayerGamePad(m_playerNo));
			m_gameScene->SetOtherPlayerNo(m_otherPlayerNo);

			//���g�̃Q�[���p�b�h�ԍ����o�͂Ƀf�o�b�N�\��
			sprintf(m_gamePadNo, "�����̃Q�[���p�b�h�ԍ���%d�ł��B\n", m_playerNo);
			OutputDebugStringA(m_gamePadNo);
			//���g�̃Q�[���p�b�h�ԍ����o�͂Ƀf�o�b�N�\��
			sprintf(m_gamePadNo, "����̃Q�[���p�b�h�ԍ���%d�ł��B\n", m_otherPlayerNo);
			OutputDebugStringA(m_gamePadNo);

			m_gameScene->SetOtherPlayerGamePad(m_online->GetPlayerGamePad(m_otherPlayerNo));
			m_gameScene->SetOnlinePlay(true);

			m_online->GameStart();

			break;

		//���[�J���}���`�v���C���[�h
		case enSelectLocalMult:
			//�����Ȃ��B

			break;

			//�\�����[�h
		case enSelectSolo:
			//�v���C���[�̐l������l�ɕύX����B
			m_gameScene->SetPlayerNumber(1);

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
		if (lastSelect == 0)
		{
			return;
		}

		//��{�^���������ꂽ��A
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//�O�̑I���ɃJ�[�\���ړ�
			m_cursorSelect--;
			//�����I���ꏊ����ԏ�̑O�������Ƃ��A
			if (m_cursorSelect < 0)
			{
				//�I���ꏊ����ԉ��ɂ���
				m_cursorSelect = lastSelect;
			}

			//�I�����Đ�
			m_titleSceneSound->PlaySelectSound();
		}
		//���{�^���������ꂽ��A
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			//���̑I���ɃJ�[�\���ړ�
			m_cursorSelect++;
			//�����I���ꏊ����ԉ��̎��������Ƃ��A
			if (m_cursorSelect > lastSelect)
			{
				//�I���ꏊ����ԏ�ɂ���
				m_cursorSelect = 0;
			}

			//�I�����Đ�
			m_titleSceneSound->PlaySelectSound();
		}
	}
}