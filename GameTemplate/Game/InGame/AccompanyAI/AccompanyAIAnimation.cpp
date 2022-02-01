/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̃A�j���[�V�����N���X
/// </summary>
#include "stdafx.h"
#include "AccompanyAIAnimation.h"

namespace nsKabutoubatu
{
	namespace nsAccompanyAIAnimation
	{
		//�A�j���[�V�����̃t�@�C���p�X
		const char* ANIMATION_FILE_PATH[8] =
		{
			"AccompanyAI_idle",			//�A�C�h��
			"AccompanyAI_shieldIdle",	//����A�C�h��
			"AccompanyAI_walk",			//����
			"AccompanyAI_shieldWalk",	//�������
			"AccompanyAI_run",			//����
			"AccompanyAI_attack1",		//�U���P
			"AccompanyAI_attack2",		//�U���Q
			"AccompanyAI_attack3",		//�U���R
		};
		//�A�j���[�V�������ꂼ��̃��[�v�A�j���[�V�������ǂ����̃t���O
		const bool ANIMATION_LOOP_FLAG[8] =
		{
			true,	//�A�C�h��
			true,	//����A�C�h��
			true,	//����
			true,	//�������
			true,	//����
			false,	//�U���P
			false,	//�U���Q
			false,	//�U���R
		};
	}

	bool AccompanyAIAnimation::Start()
	{
		//�A�j���[�V���������[�h
		for (int animationNum = enAnimationClip_idle; animationNum < enAnimationClips_num; animationNum++)
		{
			m_animationClips[animationNum].Load(nsAccompanyAIAnimation::ANIMATION_FILE_PATH[animationNum]);
			m_animationClips[animationNum].SetLoopFlag(nsAccompanyAIAnimation::ANIMATION_LOOP_FLAG);
		}

		return true;
	}
}