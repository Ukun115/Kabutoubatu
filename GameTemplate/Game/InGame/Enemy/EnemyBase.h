/// <summary>
/// 敵の基底クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;		//プレイヤー
	class PlayerStatus;	//プレイヤーステータス
	class AccompanyAI;	//同行AI
	class DropCoin;		//コイン
	class PlayerCamera;	//プレイヤーカメラ
	class Weapon;		//武器

	class EnemyBase : public IGameObject
	{
	private:
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Weapon* m_weapon[2] = { nullptr };

		int m_playerNum = enPlayerNum;
		//敵個別のプレイヤーの攻撃が当たる距離
		float m_attackReceiveLength = 0.0f;
		//敵個人のプレイヤーとの距離
		float m_DistanceLength = 0.0f;
		//敵個人のプレイヤーと体が接触する距離
		float m_hitBodyLength = 0.0f;
		int m_attackJudgeTimer[2] = { 0 };
		Vector3 m_dir[2];
		bool m_canHitBody = true;
		//攻撃を受けた！フラグ
		bool m_receiveDamageFlg = false;
		Vector3 m_playerToEnemyVector;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//敵の攻撃を受けるメソッド
		void AttackReceive();

		//プレイヤーと接触するとダメージを与える処理
		void HitBody();

	protected:
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
			enWoodStick,		//木の棒
			enSword,			//剣
			enLargeSword,		//大剣
			enArow				//弓
		};

		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		SkinModelRender* m_model = nullptr;			//敵モデル
		DropCoin* m_dropCoin = nullptr;				//コインクラス

		CharacterController m_charaCon;				//キャラクタコントローラークラス

		Vector3 m_pos;	//位置
		Vector3 m_moveSpeed;	//移動速度
		bool m_isHitGround;	//地面に乗っているかフラグ
		Vector3 m_hitGroundNormal;	//乗っている地面の法線
		float m_rotAngle = 0.0f;	//回転角度
		Quaternion m_rot;	//回転
		bool m_isFall = false;
		int m_nowState = 0;		//現在の状態
		//HP
		int m_hitPoint = 0;
		Vector3 m_cameraTargetPos;
		int m_damageTimer = 0;
		int m_lastAttackPlayer = 0;
		int m_nearPlayer = 0;		//一番近いプレイヤーの番号

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~EnemyBase();

	public:

		//弱敵にかかる重力を設定するメソッド
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		/// <summary>
		/// 敵個別の呼ばれて最初に実行される関数
		/// </summary>
		virtual void SubStart() {};

		/// <summary>
		/// 敵個別の毎フレーム実行される更新処理関数
		/// </summary>
		virtual void SubUpdate(){};

		//敵個別のプレイヤーの攻撃が当たる距離をセットするメソッド
		void SetAttackReceiveLength(const float attackReceiveLength) { m_attackReceiveLength = attackReceiveLength; };

		//敵個人のプレイヤーと体が接触する距離をセットするメソッド
		void SetHitBodyLength(const float hitBodyLength) { m_hitBodyLength = hitBodyLength; };

		//敵個人の位置をセットするメソッド
		void SetPosition(const Vector3& position) { m_pos = position; };

		//接触判定のオンオフをするメソッド
		void SetCanHitBody(const bool canHitBody) { m_canHitBody = canHitBody; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		void SetState(const int state) { m_nowState = state; };

		/// <summary>
		/// 攻撃を受けたかどうかを取得
		/// </summary>
		/// <returns>攻撃を受けたかどうか</returns>
		bool GetReceiveAttackFlg()const { return m_receiveDamageFlg; };

		/// <summary>
		/// 最後に攻撃をしてきたプレイヤー番号を取得
		/// </summary>
		/// <returns>最後に攻撃をしてきたプレイヤー番号</returns>
		int GetLastKillPlayer()const { return m_lastAttackPlayer; };

		/// <summary>
		/// プレイヤーの総数を取得
		/// </summary>
		/// <returns>プレイヤーの総数</returns>
		int GetPlayerNum()const { return m_playerNum; };
	};
}