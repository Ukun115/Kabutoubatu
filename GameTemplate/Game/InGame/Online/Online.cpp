/// <summary>
/// �I�����C���ʐM�N���X
/// </summary>
#include "stdafx.h"
#include "Online.h"
#include "SyncOnlineTwoPlayerMatchEngine.h"
#include "OnlineErrorScene.h"
#include "OnlineUpdateSpeed.h"

namespace nsKabutoubatu
{
	namespace nsOnline
	{
		//�v���C���[�ԍ��̏������l
		int PLAYER_NO_INIT_VALUE = -1;
	}

	void Online::Update()
	{
		//���r�[�̍X�V����
		if (m_onlineTwoPlayerMatchEngine)
		{
			m_onlineTwoPlayerMatchEngine->Update();
		}
	}

	//�I�����C������������
	void Online::OnlineInit()
	{
		if (!m_onlineTwoPlayerMatchEngine)
		{
			// �L�������I�΂ꂽ�̃I�����C���}�b�`���X�^�[�g����B
			m_onlineTwoPlayerMatchEngine = new SyncOnlineTwoPlayerMatchEngine;
			m_onlineTwoPlayerMatchEngine->Init
			(
				L"c9354541-9e42-4465-b556-6c707bb22d85",	//appID
				L"1.0",										//appVer.
				&m_playerNo,								//�v���C���[�ԍ�
				sizeof(int),
				[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
				[&]() { OnAllPlayerStartGame(); },
				[&]() { OnError(); }
			);

			//�f�o�b�N���O
			OutputDebugStringA("�I�����C��������\n");

			m_onlineUpdateSpeed = FindGO<OnlineUpdateSpeed>(nsStdafx::ONLINEUPDATESPEED_NAME);
		}
	}

	//���ׂẴv���C���[�����[���ɃW���C�������Ƃ��ɌĂ΂�鏈���B
	void Online::OnAllPlayerJoined(void* pData, int size)
	{
		m_isJoined = true;

		//�f�o�b�N���O
		OutputDebugStringA("�S�Ẵv���C���[�����[���ɎQ��\n");
	}

	//���ׂẴv���C���[���Q�[���X�^�[�g�������ɌĂ΂�鏈���B
	void Online::OnAllPlayerStartGame()
	{
		//�X�e�b�v���C���Q�[���ɂ���
		m_step = enStep_InGame;

		//�f�o�b�N���O
		OutputDebugStringA("�S�Ẵv���C���[���Q�[�����X�^�[�g\n");

		m_onlineUpdateSpeed->SetIsOnline(true);
	}

	//�ʐM�G���[���N�����B
	void Online::OnError()
	{
		//�f�o�b�N���O
		OutputDebugStringA("�ʐM�G���[���N���܂����B\n");

		//�X�e�b�v���G���[�ɂ���
		m_step = enStep_Error;

		//�ʐM�G���[�V�[���ɑJ��
		m_onlineErrorScene = NewGO<OnlineErrorScene>();
		m_onlineErrorScene->SetPlayerGamePad(m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNo));

		m_onlineUpdateSpeed->SetIsOnline(false);
	}

	void Online::DeleteData()
	{
		//�I�����C���}�b�`���O���Ă��܂�Ȃ��悤�ɍ폜���Ă���
		if (m_playerNo != nsOnline::PLAYER_NO_INIT_VALUE)
		{
			m_playerNo = nsOnline::PLAYER_NO_INIT_VALUE;
			//�폜
			delete m_onlineTwoPlayerMatchEngine;
			//�k���|�C���^�����Ă����B
			m_onlineTwoPlayerMatchEngine = nullptr;
			//�X�e�b�v���L�����N�^�[�Z���N�g�ɖ߂�
			m_step = enStep_CharacterSelect;

			//�f�o�b�N���O
			OutputDebugStringA("�I�����C���폜\n");

			m_onlineUpdateSpeed->SetIsOnline(false);
		}
	}

	void Online::GameStart()
	{
		// ���[�h���I����ăQ�[���J�n�\�ɂȂ������Ƃ�ʒm����B
		m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
		// �ق��̃v���C���[���Q�[���J�n�\�ɂȂ�܂ő҂B
		m_step = enStep_WaitAllPlayerStartGame;

		//�f�o�b�N���O
		OutputDebugStringA("�Q�[���J�n�\\n");

		m_onlineUpdateSpeed->SetIsOnline(true);
	}

	//�v���C���[�̃Q�[���p�b�h���擾����֐�
	GamePad& Online::GetPlayerGamePad(const int no)
	{
		return m_onlineTwoPlayerMatchEngine->GetGamePad(no);
	};
}