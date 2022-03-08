/// <summary>
/// �I�����C���ʐM���̒ʐM�G���[�N���X
/// </summary>
#include "stdafx.h"
#include "OnlineErrorScene.h"
#include "../GameScene.h"
#include "../../Title/TitleScene.h"
#include "../Pause.h"
#include "../../Graphics/SpriteRenderSub.h"
#include "Online.h"

namespace nsKabutoubatu
{
	namespace nsOnlineErrorScene
	{
		//�I�����C��������Ȃ��Ƃ��̒ʏ�fps�l
		const int NORMAL_FPS_VALUE = 60;
		const int IMAGE_WIDTH = 1280;
		const int IMAGE_HEIGHT = 720;
		const int PAD_NO = 0;
	}

	bool OnlineErrorScene::Start()
	{
		m_pause = FindGO<Pause>(nsStdafx::PAUSE_NAME);

		//�|�[�Y�@�\�𗘗p����
		m_pause->SetPause(true);
		//�|�[�Y�@�\�͎g���Ȃ��悤�ɂ���
		m_pause->SetCanPause(false);

		//�w�i���Â����邽�߂̉摜
		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_11);
		m_grayBack->Init("OnlineErrorGrayBack", nsOnlineErrorScene::IMAGE_WIDTH, nsOnlineErrorScene::IMAGE_HEIGHT);
		//�G���[���b�Z�[�W�摜
		m_errorMessageBox = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_12);
		m_errorMessageBox->Init("ErrorMessageBox", nsOnlineErrorScene::IMAGE_WIDTH, nsOnlineErrorScene::IMAGE_HEIGHT);

		return true;
	}

	OnlineErrorScene::~OnlineErrorScene()
	{
		//�w�i���Â�����摜���폜
		DeleteGO(m_grayBack);
		//�G���[���b�Z�[�W�摜���폜
		DeleteGO(m_errorMessageBox);
	}

	void OnlineErrorScene::PauseUpdate()
	{
		////TODO:���@�\���Ă��Ȃ�
		//if (m_errorMessageBoxXscale < 1.0f)
		//{
		//	m_errorMessageBoxXscale += 0.01f;
		//	m_errorMessageBox->SetScale({ m_errorMessageBoxXscale,1.0f,1.0f });
		//}

		//A�{�^���Ń^�C�g����ʂɖ߂�
		BackTitle();
	}

	//A�{�^���Ń^�C�g����ʂɖ߂郁�\�b�h
	void OnlineErrorScene::BackTitle()
	{
		//A�{�^������������A
		if (g_pad[nsOnlineErrorScene::PAD_NO]->IsTrigger(enButtonA))
		{
			//�I�����C���̃C���X�^���X���폜����
			m_online = FindGO<Online>(nsStdafx::ONLINE_NAME);
			DeleteGO(m_online);
			//�k���|�C���^�����Ă����B
			m_online = nullptr;

			//�Q�[���V�[���̃C���X�^���X������
			m_gameScene = FindGO<GameScene>(nsStdafx::GAMESCENE_NAME);
			//�Q�[���V�[�����폜���A�^�C�g���V�[���𐶐�
			m_gameScene->DeleteGameSceneClass();
			NewGO<TitleScene>(nsStdafx::PRIORITY_0, nsStdafx::TITLESCENE_NAME);
			//�|�[�Y�@�\�𗘗p���Ȃ��悤�ɂ���
			m_pause->SetPause(false);
			//�|�[�Y�@�\���g����悤�ɂ���
			m_pause->SetCanPause(true);
			//�N���X��j��
			DeleteGO(this);

			//�I�����C���͒ʏ�60fps����I�����C���p��30fps�܂ŉ����Ă����̂ŁA
			//�����Ă�������߂��Ă����B
			g_engine->SetFrameRateMode(TkEngine::enFrameRateMode_Fix, nsOnlineErrorScene::NORMAL_FPS_VALUE);
		}
	}
}