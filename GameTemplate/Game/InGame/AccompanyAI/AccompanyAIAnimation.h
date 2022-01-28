/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)のアニメーションクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class AccompanyAIAnimation : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//アニメーションクリップ
		enum enAnimation
		{
			enAnimationClip_idle,		//アイドルアニメーション
			enAnimationClip_shieldIdle,	//守備アイドルアニメーション
			enAnimationClip_walk,		//歩きアニメーション
			enAnimationClip_shieldWalk,	//守備歩きアニメーション
			enAnimationClip_run,		//走りアニメーション
			enAnimationClip_attack1,	//攻撃アニメーション１
			enAnimationClip_attack2,	//攻撃アニメーション２
			enAnimationClip_attack3,	//攻撃アニメーション３
			enAnimationClips_num,		//アニメーションの総数
		};

		AnimationClip m_animationClips[enAnimationClips_num];

		int m_animState = enAnimationClip_idle;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

	public:
		/// <summary>
		/// アニメーションクリップを取得
		/// </summary>
		/// <returns>アニメーションクリップ</returns>
		AnimationClip* GetAnimationClips() { return m_animationClips; };

		/// <summary>
		/// アニメーションの総数を取得
		/// </summary>
		/// <returns>アニメーションの総数</returns>
		int GetAnimationNum()const { return enAnimationClips_num; };

		void ChangeAnimation(const int nextAnimationState) { m_animState = nextAnimationState; };

		/// <summary>
		/// アニメーションの状態を取得
		/// </summary>
		/// <returns>アニメーションの状態</returns>
		int GetAnimationState()const { return m_animState; };
	};
}