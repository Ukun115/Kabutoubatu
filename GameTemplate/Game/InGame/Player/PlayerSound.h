/// <summary>
/// プレイヤーのサウンドクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;	//プレイヤー

	class PlayerSound : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤーのサウンド
		enum enPlayerSound
		{
			enJumpSound,				//ジャンプ音
			enAvoidanceSound,			//回避音
			enSwordWingSound,			//攻撃音
			enRecoverySound,			//回復音
			enRecoveryItemThrowSound,	//回復アイテムを投げる音
			enAddCoinSound,				//コイン取得音
			enPlayerSoundNum			//サウンドの総数
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

		SoundSource* m_playerSound[enPlayerSoundNum] = { nullptr };

		bool m_attackSoundFlg = false;
		int m_haveWeaponNo = 0;
		int m_attackSoundTimer = 0;
		int m_weaponSoundTiming = 0;

		void Update()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~PlayerSound();

		//攻撃音再生メソッド
		void PlayWeaponAttackSound(const int haveWeaponNo);

	public:
		//ジャンプ音再生メソッド
		void PlayJumpSound();

		//回復音再生メソッド
		void PlayRecoverySound();

		//回復アイテムを投げる音再生メソッド
		void PlayRecoveryItemThrowSound();

		//コインゲット音再生メソッド
		void PlayAddCoinSound();

		//回避音再生メソッド
		void PlayAvoidanceSound();

		void SetAttackSoundFlg(const int haveWeaponNo)
		{
			m_attackSoundFlg = true;
			m_attackSoundTimer = 0;
			m_haveWeaponNo = haveWeaponNo;
		};
	};
}