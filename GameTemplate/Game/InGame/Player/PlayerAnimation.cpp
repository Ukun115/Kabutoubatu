/// <summary>
/// �v���C���[�̃A�j���[�V�����N���X
/// </summary>
#include "stdafx.h"
#include "PlayerAnimation.h"
#include "Player.h"
#include "PlayerStatus.h"
#include "PlayerCharaCon.h"

namespace nsKabutoubatu
{
	namespace nsPlayerAnimation
	{
		//�A�j���[�V�����̃v���C���[�ԍ��ɑ����t�@�C���p�X
		const char* ANIMATION_FILE_PATH[enAnimationClips_num] =
		{
			"_idle",				//�A�C�h��
			"_run",					//����(�؂̖_)
			"_walk",				//����
			"_jump",				//�W�����v
			"_attack",				//�U��(�؂̖_)
			"_down",				//�_�E��
			"_rollingAvoidance",	//��]���
			"_haveSwordRun",		//����(��)
			"_swordAttack",			//�U��(��)
			"_largeSwordRun",		//����(�匕)
			"_largeSwordAttack",	//�U��(�匕)
			"_bowRun",				//����(�|)
			"_bowAttack"			//�U��(�|)
		};
		//�A�j���[�V�������ꂼ��̃��[�v�A�j���[�V�������ǂ����̃t���O
		const bool ANIMATION_LOOP_FLAG[enAnimationClips_num] =
		{
			true,	//�A�C�h��
			true,	//����(�؂̖_)
			true,	//����
			false,	//�W�����v
			false,	//�U��(�؂̖_)
			false,	//�_�E��
			false,	//��]���
			true,	//����(��)
			false,	//�U��(��)
			true,	//����(�匕)
			false,	//�U��(�匕)
			true,	//����(�|)
			false,	//�U��(�|)
		};
	}

	bool PlayerAnimation::Start()
	{
		//�C���X�^���X������
		m_player = FindGO<Player>(nsStdafx::PLAYER_NAME[m_playerNum]);
		m_playerStatus = FindGO<PlayerStatus>(nsStdafx::PLAYER_STATUS_NAME[m_playerNum]);
		m_playerCharaCon = FindGO<PlayerCharaCon>(nsStdafx::PLAYER_CHARACON_NAME[m_playerNum]);

		for (int animationNum = enAnimationClip_idle; animationNum < enAnimationClips_num; animationNum++)
		{
			switch (m_playerNum)
			{
				//�PP�̂Ƃ�
			case enPlayer1:
				//�A�j���[�V�����̃t�@�C���p�X���uPlayer1�v���瑱���悤�ɂ���B
				sprintf(m_animationFilePath, "Player1%s", nsPlayerAnimation::ANIMATION_FILE_PATH[animationNum]);
				break;
				//�QP�̂Ƃ�
			case enPlayer2:
				//�A�j���[�V�����̃t�@�C���p�X���uPlayer2�v���瑱���悤�ɂ���B
				sprintf(m_animationFilePath, "Player2%s", nsPlayerAnimation::ANIMATION_FILE_PATH[animationNum]);
				break;
			}

			//�A�j���[�V���������[�h
			m_animationClips[animationNum].Load(m_animationFilePath);
			//���[�v�A�j���[�V�������ǂ�����ݒ�
			m_animationClips[animationNum].SetLoopFlag(nsPlayerAnimation::ANIMATION_LOOP_FLAG[animationNum]);
		}

		return true;
	}
	//�ʏ�ړ���Ԏ��̃A�j���[�V�����������\�b�h
	void PlayerAnimation::NormalStateAnimation()
	{
		//����A�j���[�V�����̎��͒ʏ�ړ��A�j���[�V�����͂��Ȃ��B
		if (m_animState == enAnimationClip_rollingAvoidance)
		{
			return;
		}

		//�v���C���[���n�ʂɂ��Ă���Ƃ��A
		if (m_playerCharaCon->IsOnGround())
		{
			//�قڎ~�܂��Ă���Ƃ��A(��Βl��1.0f��菬����)
			if (fabsf(m_player->GetMoveSpeed().x) < 1.0f
				&& fabsf(m_player->GetMoveSpeed().z) < 1.0f)
			{
				//�A�j���[�V�������A�C�h���ɐ؂�ւ�
				m_animState = enAnimationClip_idle;
			}
			//���������Ă���Ƃ��A
			else if (fabsf(m_player->GetMoveSpeed().x) < 5.0f
				&& fabsf(m_player->GetMoveSpeed().z) < 5.0f)
			{
				//�A�j���[�V����������ɐ؂�ւ�
				m_animState = enAnimationClip_walk;
			}
			else
			{
				//�A�j���[�V�����𑖂�ɐ؂�ւ�
				switch (m_playerStatus->GetHaveWeapon())
				{
				//�؂̖_
				case enWoodStick:
					m_animState = enAnimationClip_run;

					break;

				//���ʂ̌�
				case enSword:
					m_animState = enAnimationClips_swordRun;

					break;
				//�匕
				case enLargeSword:
					m_animState = enAnimationClips_largeSwordRun;

					break;
				//�|
				case enBow:
					m_animState = enAnimationClips_bowRun;
				}
			}
		}
		else
		{
			//�A�j���[�V�������W�����v�ɐ؂�ւ�
			m_animState = enAnimationClip_jump;
		}
	}
}