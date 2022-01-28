/// <summary>
/// プレイヤーのアニメーションクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;			//プレイヤー
	class PlayerStatus;		//プレイヤーのステータス
	class PlayerCharaCon;	//プレイヤーのキャラクターコントローラー

	/// <summary>
	/// 列挙型
	/// </summary>

	//アニメーションクリップ
	enum enPlayerAnimation
	{
		enAnimationClip_idle,				//棒立ちアニメーション
		enAnimationClip_run,				//走りアニメーション
		enAnimationClip_walk,				//歩きアニメーション
		enAnimationClip_jump,				//ジャンプアニメーション
		enAnimationClip_attack,				//攻撃アニメーション
		enAnimationClip_down,				//死ぬときのアニメーション
		enAnimationClip_rollingAvoidance,	//転がり回避アニメーション
		enAnimationClips_swordRun,			//普通の剣を持っているときの走りアニメーション
		enAnimationClips_swordAttack,		//普通の剣を持っているときの攻撃アニメーション
		enAnimationClips_largeSwordRun,		//大剣を持っているときの走りアニメーション
		enAnimationClips_largeSwordAttack,	//大剣を持っているときの攻撃アニメーション
		enAnimationClips_bowRun,			//弓を持っているときの走りアニメーション
		enAnimationClips_bowAttack,			//弓を持っているときの攻撃アニメーション
		enAnimationClips_num,				//アニメーションの総数
	};

	class PlayerAnimation : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		//武器
		enum enWeapon
		{
			enWoodStick,	//木の棒
			enSword,		//普通の剣
			enLargeSword,	//大剣
			enBow,			//弓
			enWeaponNum		//武器の総数
		};

		Player* m_player = nullptr;
		PlayerStatus* m_playerStatus = nullptr;
		PlayerCharaCon* m_playerCharaCon = nullptr;

		AnimationClip m_animationClips[enAnimationClips_num];

		int m_animState = enAnimationClip_idle;
		int m_playerNum = 0;
		char m_animationFilePath[256];

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

	public:
		//通常移動状態時のアニメーション処理メソッド
		void NormalStateAnimation();

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//プレイヤーのアニメーションを変更するメソッド
		void SetAnimation(const int animationState) { m_animState = animationState; };

		void ChangeAnimation(const int nextAnimationState) { m_animState = nextAnimationState; };

		/// <summary>
		/// プレイヤーのアニメーション状態を取得
		/// </summary>
		/// <returns>プレイヤーのアニメーション状態</returns>
		int GetAnimationState()const { return m_animState; };

		/// <summary>
		/// プレイヤーのアニメーションクリップを取得
		/// </summary>
		/// <returns>プレイヤーのアニメーションクリップ</returns>
		AnimationClip* GetAnimationClips() { return m_animationClips; };

		/// <summary>
		/// プレイヤーのアニメーション総数を取得
		/// </summary>
		/// <returns>プレイヤーのアニメーション総数</returns>
		int GetAnimationNum()const { return enAnimationClips_num; };
	};
}