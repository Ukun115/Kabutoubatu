/// <summary>
/// プレイヤークラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PlayerCamera;		//プレイヤーのカメラ
	class PlayerRotation;	//プレイヤーの回転
	class PlayerAnimation;	//プレイヤーのアニメーション
	class PlayerSound;		//プレイヤーのサウンド
	class PlayerCharaCon;	//プレイヤーのキャラクターコントローラー
	class PlayerStatus;		//プレイヤーのステータス
	class RecoveryItem;		//回復アイテム
	class Arrow;			//矢
	class MiniMap;			//ミニマップ


	class Player : public IGameObject
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

		//プレイヤーの状態
		enum enPlayerNowState
		{
			enNormal,			//通常
			enSufferAnAttack,	//攻撃を受けた状態
			enGhost,			//ゴースト状態
			enDontMove			//何もしない状態
		};

		//回復アイテムの状態
		enum enRecoveryState
		{
			enRecoverySet,		//セット
			enRecoveryDelete,	//削除
			enRecoveryThrow,	//投げる
			enRecoveryStay		//何もしないステート
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

		//重力
		enum enGravity
		{
			enNormalGravity,	//通常の重力
			enHalfGravity		//半重力
		};

		SkinModelRender* m_model = nullptr;
		Player* m_otherPlayer = nullptr;
		RecoveryItem* m_recoveryItem = nullptr;
		Arrow* m_arrow = nullptr;
		//プレイヤーの回転クラス
		PlayerRotation* m_playerRotation = nullptr;
		//プレイヤーのアニメーションクラス
		PlayerAnimation* m_playerAnimation = nullptr;
		//プレイヤーのサウンドクラス
		PlayerSound* m_playerSound = nullptr;
		//プレイヤーのキャラコンクラス
		PlayerCharaCon* m_playerCharaCon = nullptr;
		//プレイヤーのステータスクラス
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		GamePad* m_gamePad;
		MiniMap* m_miniMap = nullptr;

		Effect m_fireBallEffect;	//ゴースト状態の時の火の玉エフェクト

		int m_gravityState = enNormalGravity;
		//1Pか2Pかを表す番号
		int m_playerNum = 0;
		Vector3 m_pos;
		Vector3 m_moveSpeed;
		Vector3 m_friction;		//摩擦
		int m_nowState = 0;
		Vector3 m_hitGroundNormal;
		int m_recoveryState = enRecoveryStay;
		//正面ベクトル
		Vector3 m_forward;
		//最後に着地していた場所のポジション
		Vector3 m_finalOnGroundPos;
		int m_damageTimer = 0;
		int m_fireBallEffectTimer = 0;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_isHitGround;
		bool m_haveRecoveryItem = false;
		//攻撃中かどうかのフラグ
		bool m_isAttackStateFlg = false;
		bool m_nowDoorTatch = false;
		bool m_isSoloPlay = false;
		//回避中フラグ
		bool m_isAvoidance = false;
		bool m_damageFlg = false;
		bool m_fireBallEffectActive = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Player();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

		//プレイヤーにかかる重力を設定するメソッド
		void Gravity(const int gravityPower);

		//摩擦メソッド
		void Friction(const float frictionPower);

		//通常移動メソッド
		void NormalMove();

		//ゴースト状態の移動メソッド
		void GhostMove();

		//ジャンプメソッド
		void Jump();

		//回避
		void Avoidance();

		//プレイヤーのデータを更新するメソッド
		//位置・回転・キャラコン・アニメーション状態
		void DataUpdate();

		//回復処理メソッド
		void Recovery();

		//落下処理メソッド
		void Fall();

		//ミニマップを開くメソッド
		void MiniMapOpen();

		//矢を飛ばす関数
		void ShotArrow();

	public:
		//回復アイテムをしまうメソッド
		void RecoveryItemDelete();

		//プレイヤー番号セットメソッド
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//プレイヤーの状態セットメソッド
		void SetNowState(const int nowState) { m_nowState = nowState; };

		void SetRecoveryState(const int recoveryState) { m_recoveryState = recoveryState; };

		//プレイヤーの移動速度セットメソッド
		void SetMoveSpeed(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		//プレイヤーの位置を移動させる
		void SetPositionA(const Vector3& position);

		//ドアを開閉しているかの判定を変更するメソッド
		void SetNowDoorTatch(const bool nowDoorTatch){ m_nowDoorTatch = nowDoorTatch;};

		//プレイヤーのコインを増やすメソッド
		void AddCoin(const int coinValue);

		void SetGamePad(GamePad& gamePad) { m_gamePad = &gamePad; };

		void SetIsSoloPlay(bool isSoloPlay) { m_isSoloPlay = isSoloPlay; };

		void InitDamageTimer()
		{
			m_damageTimer = 0;
			m_damageFlg = true;
		};

		void DamageEffect();

		/// <summary>
		/// プレイヤーの位置を取得
		/// </summary>
		/// <returns>プレイヤーの位置</returns>
		Vector3 GetPosition()const { return m_pos; };

		/// <summary>
		/// プレイヤーの状態を取得
		/// </summary>
		/// <returns>プレイヤーの状態</returns>
		int GetNowState()const { return m_nowState; };

		/// <summary>
		/// プレイヤーの回復アイテムの状態を取得
		/// </summary>
		/// <returns>プレイヤーの回復アイテムの状態</returns>
		int GetRecoveryItemState()const { return m_recoveryState; };

		/// <summary>
		/// プレイヤーの移動速度を取得
		/// </summary>
		/// <returns>プレイヤーの移動速度</returns>
		Vector3 GetMoveSpeed()const { return m_moveSpeed; };

		/// <summary>
		/// プレイヤーの正面ベクトルを取得
		/// </summary>
		/// <returns>プレイヤーの正面ベクトル</returns>
		Vector3 GetForwardVector()const { return m_forward; };

		/// <summary>
		/// プレイヤースケルトンを取得
		/// </summary>
		/// <returns>プレイヤースケルトン</returns>
		Skeleton* GetSkeleton()const { return m_model->GetSkeleton(); };

		/// <summary>
		/// 攻撃中かどうかを取得
		/// </summary>
		/// <returns>攻撃中かどうか</returns>
		bool IsAttack()const { return m_isAttackStateFlg; };

		/// <summary>
		/// 火の玉中かどうかを取得
		/// </summary>
		/// <returns>火の玉中かどうか</returns>
		bool GetFireBallActive()const {return m_fireBallEffectActive;};
	};
}