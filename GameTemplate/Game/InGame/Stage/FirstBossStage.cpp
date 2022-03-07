/// <summary>
/// �ŏ��̃{�X�X�e�[�W�N���X
/// </summary>
#include "stdafx.h"
#include "FirstBossStage.h"
#include "../Enemy/FirstBoss.h"
#include "../GameScene.h"
#include "../../Result/GameClearScene.h"
#include "../../Graphics/Fade.h"

namespace nsKabutoubatu
{
	bool FirstBossStage::Start()
	{
		m_fade = NewGO<Fade>();
		m_fade->Init(1.0f,0,0);

		//�Q�[���V�[���̃C���X�^���X������
		m_gameScene = FindGO< GameScene>(nsStdafx::GAMESCENE_NAME);
		//�Q�[��BGM���~�߂�
		m_gameScene->GameBGMStop();

		//�X�e�[�W���f���̏�����
		m_stageModel = NewGO<SkinModelRender>();
		m_stageModel->SetShadowCaster(false);	//�V���h�E�͗��Ƃ��Ȃ�
		m_stageModel->SetOutline(false);		//�֊s�������Ȃ�
		m_stageModel->Init("FirstBossStage");
		//�����蔻���K��
		m_physicsStaticObject.CreateFromModel(*m_stageModel->GetModel(), m_stageModel->GetModel()->GetWorldMatrix());
		m_pos = { 0.0f,-10000.0f,0.0f };
		m_stageModel->SetPosition(m_pos);
		//�����蔻����X�V
		m_physicsStaticObject.Update(m_pos, m_rot);

		//�{�X�G
		m_firstBoss = NewGO<FirstBoss>();
		m_firstBoss->SetPlayerNum(m_playerNum);

		return true;
	}

	FirstBossStage::~FirstBossStage()
	{
		//�X�e�[�W���f�����폜
		DeleteGO(m_stageModel);
		//�{�X�G���폜
		DeleteGO(m_firstBoss);
	}

	void FirstBossStage::Update()
	{
		if (m_fade != nullptr && m_fade->GetNowState() == 2)
		{
			DeleteGO(m_fade);
		}

		if (m_firstBossBGM == nullptr)
		{
			//�t�@�[�X�g�{�XBGM�̏�����
			m_firstBossBGM = NewGO<SoundSource>();
			m_firstBossBGM->Init(L"Assets/sound/FirstBossBGM.wav");
			m_firstBossBGM->SetVolume(0.2f);
			//�t�@�[�X�g�{�XBGM���Đ�
			m_firstBossBGM->Play(true);	//���[�v�Đ�
		}
		//�{�X��HP��0�ɂȂ�����A
		if (m_gameClearScene == nullptr && m_firstBoss->GetGameClearFlg())
		{
			//�Q�[���N���A�N���X�𐶐�
			m_gameClearScene = NewGO<GameClearScene>();
			m_gameClearScene->SetPlayerNum(m_playerNum);
			//BGM�̉��ʂ�����
			m_firstBossBGM->SetVolume(0.0f);
		}
	}
}