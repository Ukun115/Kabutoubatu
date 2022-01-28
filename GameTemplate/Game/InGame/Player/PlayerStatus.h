/// <summary>
/// プレイヤーのステータスクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PlayerSound;	//プレイヤーのサウンド

	class PlayerStatus : public IGameObject
	{
	private:
		PlayerSound* m_playerSound = nullptr;

		//所持コイン
		int m_possessionCoin = 1000;
		//総取得コイン数
		int m_totalGetCoin = 0;
		//体力
		int m_hitPoint = 10;
		//最大体力
		int m_maxHitPoint = 10;
		//所持回復数
		int m_recoveryItemNum = 10;
		//コインの取得倍率
		float m_addMoneyMagnification = 1.0f;
		//所持武器番号
		int m_haveWeapon = 0;
		//移動力
		float m_movePower = 40.0f;
		//ジャンプ力
		float m_jumpPower = 7.5f;
		//回復力
		int m_recoveryPower = 1;
		//回復してもらった量の総数
		int m_recoveryReceiveNum = 0;
		//総死亡回数
		int m_deathNum = 0;
		//キル数
		int m_enemyKillNum = 0;
		//プレイヤー番号
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~PlayerStatus();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		/**
		* @brief セッター
		*/
		//プレイヤー番号をセットするメソッド
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//総回復してもらった量を保存するメソッド
		void AddRecoveryReceiveNum(const int addValue) { m_recoveryReceiveNum += addValue; };

		//敵を倒した数を増やすメソッド
		void AddEnemyKillNum() { m_enemyKillNum++; };

		//プレイヤーにダメージを与えるメソッド
		void ReduceHitPoint(const int reduceNum)
		{
			m_hitPoint -= reduceNum;
		};

		//プレイヤーを回復させるメソッド
		void AddHitPoint(const int addValue);

		//ヒットポイントの数値を指定して変更するメソッド
		void SetHitPoint(const int hitPoint) { m_hitPoint = hitPoint; };

		//プレイヤーのコインを増やすメソッド
		void AddCoin(const int coinValue);

		//デス数を１増やすメソッド
		void AddDeathNum() { m_deathNum++; };

		//回復アイテムの数を１減らす
		void ReduceRecoveryItemNum() { m_recoveryItemNum--; };

		/**
		* @brief 購入
		*/
		//プレイヤーの所持金を減らすメソッド
		void ReduceMoney(const int reduceNum) { m_possessionCoin -= reduceNum; };

		//プレイヤーの所持金を増やすメソッド
		void AddMoney(const int addValue) { m_possessionCoin += addValue; };

		//プレイヤーのコインを増やす倍率を増やすメソッド
		void AddMoneyMagnification() { m_addMoneyMagnification += 0.1f; };

		//プレイヤーの移動力をアップさせるメソッド
		void AddMovePower() { m_movePower += 5.0f; };

		//プレイヤーのジャンプ力をアップさせるメソッド
		void AddJumpPower() { m_jumpPower += 1.0f; };

		//回復アイテムの数を増やすメソッド
		void AddRecoveryItemNum() { m_recoveryItemNum++; };

		//回復力をアップさせるメソッド
		void AddRecoveryPower() { m_recoveryPower++; };

		//持っている武器を変更するメソッド
		void ChangeHavingWeapon(int weaponNum) { m_haveWeapon = weaponNum; };

		/**
		*
		* @brief 売却
		*/
		//プレイヤーのコインを増やす倍率を減らすメソッド
		void ReduceMoneyMagnification() { m_addMoneyMagnification -= 0.1f; };

		//プレイヤーの移動力を下げるメソッド
		void ReduceMovePower() { m_movePower -= 5.0f; };

		//プレイヤーのジャンプ力を下げるメソッド
		void ReduceJumpPower() { m_jumpPower -= 1.0f; };

		//回復力を下げるメソッド
		void ReduceRecoveryPower() { m_recoveryPower--; };

		/// <summary>
		/// プレイヤーの総死亡回数を取得
		/// </summary>
		/// <returns>プレイヤーの総死亡回数</returns>
		int GetDeathNum()const { return m_deathNum; };

		/// <summary>
		/// プレイヤーの総回復してもらった量を取得
		/// </summary>
		/// <returns>プレイヤーの総回復してもらった量</returns>
		int GetRecoveryReceiveNum()const { return m_recoveryReceiveNum; };

		/// <summary>
		/// プレイヤーのコイン所持数を取得
		/// </summary>
		/// <returns>プレイヤーのコイン所持数</returns>
		int GetCoinNum()const { return m_possessionCoin; };

		/// <summary>
		/// プレイヤーの総取得コイン数を取得
		/// </summary>
		/// <returns>プレイヤーの総取得コイン数</returns>
		int GetTotalCoinNum()const { return m_totalGetCoin; };

		/// <summary>
		/// 敵を倒した数を取得
		/// </summary>
		/// <returns>敵を倒した数</returns>
		int GetEnemyKillNum()const { return m_enemyKillNum; };

		/// <summary>
		/// 持っている回復アイテムの総数を取得
		/// </summary>
		/// <returns>持っている回復アイテムの総数</returns>
		int GetRecoveryItemNum()const { return m_recoveryItemNum; };

		/// <summary>
		/// プレイヤーのHPを取得
		/// </summary>
		/// <returns>プレイヤーのHP</returns>
		int GetHitPoint()const { return m_hitPoint; };

		/// <summary>
		/// プレイヤーの最大体力を取得
		/// </summary>
		/// <returns>プレイヤーの最大体力</returns>
		int GetMaxHitPoint()const { return m_maxHitPoint; };

		/// <summary>
		/// プレイヤーの現在持っている武器を取得
		/// </summary>
		/// <returns>プレイヤーの現在持っている武器</returns>
		int GetHaveWeapon()const { return m_haveWeapon; };

		/// <summary>
		/// プレイヤーの移動力を取得
		/// </summary>
		/// <returns>プレイヤーの移動力</returns>
		float GetMovePower()const { return m_movePower; };

		/// <summary>
		/// プレイヤーのジャンプ力を取得
		/// </summary>
		/// <returns>プレイヤーのジャンプ力</returns>
		float GetJumpPower()const { return m_jumpPower; };

		/// <summary>
		/// プレイヤーの回復してあげる力を取得
		/// </summary>
		/// <returns>プレイヤーの回復してあげる力</returns>
		int GetRecoveryPower()const { return m_recoveryPower; };
	};
}