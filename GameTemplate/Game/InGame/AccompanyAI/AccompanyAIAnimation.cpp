/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̃A�j���[�V�����N���X
/// </summary>
#include "stdafx.h"
#include "AccompanyAIAnimation.h"

namespace nsKabutoubatu
{
	bool AccompanyAIAnimation::Start()
	{
		//�A�j���[�V���������[�h
		m_animationClips[enAnimationClip_idle].Load("AccompanyAI_idle");		//�A�C�h�����[�V���������[�h
		m_animationClips[enAnimationClip_idle].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimationClip_shieldIdle].Load("AccompanyAI_shieldIdle");		//����A�C�h�����[�V���������[�h
		m_animationClips[enAnimationClip_shieldIdle].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimationClip_walk].Load("AccompanyAI_walk");		//�������[�V���������[�h
		m_animationClips[enAnimationClip_walk].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimationClip_shieldWalk].Load("AccompanyAI_shieldWalk");		//����������[�V���������[�h
		m_animationClips[enAnimationClip_shieldWalk].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimationClip_run].Load("AccompanyAI_run");			//���胂�[�V���������[�h
		m_animationClips[enAnimationClip_run].SetLoopFlag(true);	//���[�v���[�V�����ɂ���B
		m_animationClips[enAnimationClip_attack1].Load("AccompanyAI_attack1");			//�U���P���[�V���������[�h
		m_animationClips[enAnimationClip_attack1].SetLoopFlag(false);	//�����V���b�g�Đ��ɂ���B
		m_animationClips[enAnimationClip_attack2].Load("AccompanyAI_attack2");			//�U���Q���[�V���������[�h
		m_animationClips[enAnimationClip_attack2].SetLoopFlag(false);	//�����V���b�g�Đ��ɂ���B
		m_animationClips[enAnimationClip_attack3].Load("AccompanyAI_attack3");			//�U���R���[�V���������[�h
		m_animationClips[enAnimationClip_attack3].SetLoopFlag(false);	//�����V���b�g�Đ��ɂ���B

		return true;
	}
}