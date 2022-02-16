/// <summary>
/// �I�����C���ʐM�N���X
/// </summary>
#include "stdafx.h"
#include "Online.h"
#include "SyncOnlineTwoPlayerMatchEngine.h"
#include "OnlineErrorScene.h"

namespace nsKabutoubatu
{
	void Online::Update()
	{
		//�I�����C������
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
				&m_playerNo,
				sizeof(int),
				[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
				[&]() { OnAllPlayerStartGame(); },
				[&]() { OnError(); }
			);

			OutputDebugStringA("�I�����C��������\n");
		}
	}

	//���ׂẴv���C���[�����[���ɃW���C�������Ƃ��ɌĂ΂�鏈���B
	void Online::OnAllPlayerJoined(void* pData, int size)
	{
		m_isJoined = true;

		OutputDebugStringA("�S�Ẵv���C���[�����[���ɎQ��\n");
	}

	//���ׂẴv���C���[���Q�[���X�^�[�g�������ɌĂ΂�鏈���B
	void Online::OnAllPlayerStartGame()
	{
		m_step = enStep_InGame;

		OutputDebugStringA("�S�Ẵv���C���[���Q�[�����X�^�[�g\n");
	}

	//�ʐM�G���[���N�����B
	void Online::OnError()
	{
		OutputDebugStringA("�ʐM�G���[���N���܂����B\n");

		m_step = enStep_Error;

		//�ʐM�G���[�V�[���ɑJ��
		m_onlineErrorScene = NewGO<OnlineErrorScene>();
		m_onlineErrorScene->SetPlayerGamePad(m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNo));
	}

	void Online::DeleteData()
	{
		//�I�����C���}�b�`���O���Ă��܂�Ȃ��悤�ɍ폜���Ă���
		if (m_playerNo != -1)
		{
			m_playerNo = -1;
			//�폜
			delete m_onlineTwoPlayerMatchEngine;
			//null�|�C���^�ɂ��Ă����B
			m_onlineTwoPlayerMatchEngine = nullptr;
			//�X�e�b�v���L�����N�^�[�Z���N�g�ɖ߂�
			m_step = enStep_CharacterSelect;

			OutputDebugStringA("�I�����C���폜\n");
		}
	}

	void Online::GameStart()
	{
		// ���[�h���I����ăQ�[���J�n�\�ɂȂ������Ƃ�ʒm����B
		m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
		// �ق��̃v���C���[���Q�[���J�n�\�ɂȂ�܂ő҂B
		m_step = enStep_WaitAllPlayerStartGame;

		OutputDebugStringA("�Q�[���J�n�\\n");
	}

	GamePad& Online::GetPlayerGamePad(int no)
	{
		return m_onlineTwoPlayerMatchEngine->GetGamePad(no);
	};
}