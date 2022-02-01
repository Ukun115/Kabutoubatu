/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)のアニメーションクラス
/// </summary>
#include "stdafx.h"
#include "AccompanyAIAnimation.h"

namespace nsKabutoubatu
{
	namespace nsAccompanyAIAnimation
	{
		//アニメーションのファイルパス
		const char* ANIMATION_FILE_PATH[8] =
		{
			"AccompanyAI_idle",			//アイドル
			"AccompanyAI_shieldIdle",	//守備アイドル
			"AccompanyAI_walk",			//歩き
			"AccompanyAI_shieldWalk",	//守備歩き
			"AccompanyAI_run",			//走り
			"AccompanyAI_attack1",		//攻撃１
			"AccompanyAI_attack2",		//攻撃２
			"AccompanyAI_attack3",		//攻撃３
		};
		//アニメーションそれぞれのループアニメーションかどうかのフラグ
		const bool ANIMATION_LOOP_FLAG[8] =
		{
			true,	//アイドル
			true,	//守備アイドル
			true,	//歩き
			true,	//守備歩き
			true,	//走り
			false,	//攻撃１
			false,	//攻撃２
			false,	//攻撃３
		};
	}

	bool AccompanyAIAnimation::Start()
	{
		//アニメーションをロード
		for (int animationNum = enAnimationClip_idle; animationNum < enAnimationClips_num; animationNum++)
		{
			m_animationClips[animationNum].Load(nsAccompanyAIAnimation::ANIMATION_FILE_PATH[animationNum]);
			m_animationClips[animationNum].SetLoopFlag(nsAccompanyAIAnimation::ANIMATION_LOOP_FLAG);
		}

		return true;
	}
}