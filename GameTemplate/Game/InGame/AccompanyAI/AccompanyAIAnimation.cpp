/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)のアニメーションクラス
/// </summary>
#include "stdafx.h"
#include "AccompanyAIAnimation.h"

namespace nsKabutoubatu
{
	bool AccompanyAIAnimation::Start()
	{
		//アニメーションをロード
		m_animationClips[enAnimationClip_idle].Load("AccompanyAI_idle");		//アイドルモーションをロード
		m_animationClips[enAnimationClip_idle].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimationClip_shieldIdle].Load("AccompanyAI_shieldIdle");		//守備アイドルモーションをロード
		m_animationClips[enAnimationClip_shieldIdle].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimationClip_walk].Load("AccompanyAI_walk");		//歩きモーションをロード
		m_animationClips[enAnimationClip_walk].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimationClip_shieldWalk].Load("AccompanyAI_shieldWalk");		//守備歩きモーションをロード
		m_animationClips[enAnimationClip_shieldWalk].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimationClip_run].Load("AccompanyAI_run");			//走りモーションをロード
		m_animationClips[enAnimationClip_run].SetLoopFlag(true);	//ループモーションにする。
		m_animationClips[enAnimationClip_attack1].Load("AccompanyAI_attack1");			//攻撃１モーションをロード
		m_animationClips[enAnimationClip_attack1].SetLoopFlag(false);	//ワンショット再生にする。
		m_animationClips[enAnimationClip_attack2].Load("AccompanyAI_attack2");			//攻撃２モーションをロード
		m_animationClips[enAnimationClip_attack2].SetLoopFlag(false);	//ワンショット再生にする。
		m_animationClips[enAnimationClip_attack3].Load("AccompanyAI_attack3");			//攻撃３モーションをロード
		m_animationClips[enAnimationClip_attack3].SetLoopFlag(false);	//ワンショット再生にする。

		return true;
	}
}