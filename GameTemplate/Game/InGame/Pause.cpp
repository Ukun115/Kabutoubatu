/// <summary>
/// �|�[�Y�@�\�N���X
/// </summary>
#include "stdafx.h"
#include "Pause.h"

namespace nsKabutoubatu
{
	namespace nsPause
	{
		const float PAUSE_SPRITE_TATE = 600.0f;
		const float PAUSE_SPRITE_YOKO = 300.0f;
		const float GRAYBACK_SCA = 1500.0f;
		const float HALF_VOLUME = 0.5f;
	}

	bool Pause::Start()
	{
		m_pauseSprite = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_6);
		//�|�[�Y�摜��������
		m_pauseSprite->Init("Pause", nsPause::PAUSE_SPRITE_TATE, nsPause::PAUSE_SPRITE_YOKO);
		m_pauseSprite->Deactivate();	//���߂͔�\��

		m_grayBack = NewGO<SpriteRenderSub>(nsStdafx::PRIORITY_1);
		//�|�[�Y���D�F�ɂ���摜��������
		m_grayBack->Init("GrayBack", nsPause::GRAYBACK_SCA, nsPause::GRAYBACK_SCA);
		m_grayBack->Deactivate();	//���߂͔�\��

		return true;
	}

	Pause::~Pause()
	{
		//�|�[�Y�摜���폜
		DeleteGO(m_pauseSprite);
		//�|�[�Y��ʂ̔w�i�摜���폜
		DeleteGO(m_grayBack);
	}

	void Pause::Update()
	{
		//�|�[�Y�@�\���g���Ȃ��Ƃ���return�ȍ~�͎��s���Ȃ�
		if (!m_canPause)
		{
			return;
		}

		//�|�[�Y�@�\
		for (int plaNum = enPlayer1; plaNum < enTotalPlayerNum; plaNum++)
		{
			//�Z���N�g�{�^���������ꂽ�Ƃ��̂�return�ȍ~����������
			if (!g_pad[plaNum]->IsTrigger(enButtonSelect))
			{
				return;
			}
			//TODO:�|�[�Y��ʂ��J�����Đ�

			//�|�[�Y�ɂ���
			m_pauseSprite->Activate();
			m_grayBack->Activate();

			//�|�[�Y���ɂ���
			m_isPauseFlg = true;
		}
	}

	//�|�[�Y�����̃A�b�v�f�[�g�����Ă΂�Ȃ��Ȃ�
	void Pause::PauseUpdate()
	{
		//�|�[�Y��~�@�\
		for (int plaNum = enPlayer1; plaNum < enTotalPlayerNum; plaNum++)
		{
			//�Z���N�g�{�^���������ꂽ�Ƃ��̂�return�ȍ~����������
			if (!g_pad[plaNum]->IsTrigger(enButtonSelect))
			{
				return;
			}

			//TODO:�|�[�Y���鉹�Đ�

			m_pauseSprite->Deactivate();
			m_grayBack->Deactivate();

			//�|�[�Y�����O��
			m_isPauseFlg = false;
		}
	}
}