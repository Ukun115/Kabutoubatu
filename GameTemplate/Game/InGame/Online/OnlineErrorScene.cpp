/// <summary>
/// �I�����C���ʐM���̒ʐM�G���[�N���X
/// </summary>
#include "stdafx.h"
#include "OnlineErrorScene.h"
#include "../GameScene.h"
#include "../../Title/TitleScene.h"
#include "../Pause.h"
#include "../../Graphics/SpriteRenderSub.h"

namespace nsKabutoubatu
{
	bool OnlineErrorScene::Start()
	{
		//�Q�[���V�[���̃C���X�^���X������
		m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
		m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);

		//�|�[�Y�@�\�𗘗p����
		m_pause->SetPause(true);
		//�|�[�Y�@�\�͎g���Ȃ��悤�ɂ���
		m_pause->SetCanPause(false);

		//�w�i���Â����邽�߂̉摜
		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_11);
		m_grayBack->Init("OnlineErrorGrayBack", 1280, 720);
		//�G���[���b�Z�[�W�摜
		m_errorMessageBox = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_12);
		m_errorMessageBox->Init("ErrorMessageBox", 1280, 720);

		//�p�b�h�i���o�[���擾
		m_padNo = m_gameScene->GetPlayerNo();

		return true;
	}

	OnlineErrorScene::~OnlineErrorScene()
	{
		DeleteGO(m_grayBack);
		DeleteGO(m_errorMessageBox);
	}

	void OnlineErrorScene::PauseUpdate()
	{
		//TODO:���@�\���Ă��Ȃ��̂ŋ@�\������
		if (m_errorMessageBoxXscale < 1.0f)
		{
			m_errorMessageBoxXscale += 0.01f;
			m_errorMessageBox->SetScale({ m_errorMessageBoxXscale,1.0f,1.0f });
		}

		BackTitle(m_padNo);
	}

	//A�{�^���Ń^�C�g����ʂɖ߂郁�\�b�h
	void OnlineErrorScene::BackTitle(const int padNo)
	{
		if (g_pad[m_padNo]->IsTrigger(enButtonA))
		{
			//TODO:�^�C�g����ʂɖ߂�悤�ɂ���B

			////�Q�[���V�[�����폜���A�^�C�g���V�[���𐶐�
			//m_gameScene->SetOnlineError(true);
			//NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);

			////�N���X��j��
			//DeleteGO(this);

			//exe����ăQ�[���I��
			exit(EXIT_SUCCESS);
		}
	}
}