/// <summary>
/// 回復アイテムクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;		//プレイヤー
	class PlayerStatus;	//プレイヤーのステータス

	class RecoveryItem : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//回復アイテムの状態
		enum enRecoveryState
		{
			enRecoverySet,		//回復アイテムを持った状態
			enRecoveryDelete,	//回復アイテムをしまった状態
			enRecoveryThrow,	//投げている状態
			enRecoveryStay		//何もしない状態
		};

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		SkinModelRender* m_model = nullptr;		//モデル
		Player* m_player[enPlayerNum] = { nullptr };		//プレイヤー検索するためのインスタンス
		PlayerStatus* m_playerStatus[enPlayerNum] = {nullptr};

		CharacterController m_charaCon;		//キャラクタコントローラークラスを作成

		int m_totalPlayerNum = enPlayerNum;
		Vector3 m_pos;				//位置
		Vector3 m_moveSpeed;		//移動速度
		Vector3 m_decreasePower;	//移動速度減衰力
		Quaternion m_rot;			//回転
		float m_rotAngle = 0.0f;	//回転角度
		Vector3 m_hitGroundNormal;	//乗っている地面の法線
		int m_fallTimer = 0;//自動削除するためのタイマー
		int m_playerNum = 0;	//プレイヤー番号
		int m_recoveryPower = 0;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_isHitGround = false;			//地面に乗っているかのフラグ
		bool m_canPlayerMoveSpeedGet = true;	//プレイヤーの移動速度をゲットできるかの判定
		bool m_isSoloPlay = false;
		bool m_playerHit = false;


		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~RecoveryItem();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//弱敵にかかる重力を設定するメソッド
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		//回復アイテムモデルの初期化
		void RecoveryItemInit();

		//更新処理メソッド
		void DataUpdate();

	public:
		//プレイヤーの番号をセットするメソッド
		void SetPlayerNum(const int plaNum) { m_playerNum = plaNum; };

		//回復力を取得
		void SetRecoveryPower(const int recoveryPower) { m_recoveryPower = recoveryPower; };

		void SetIsSoloPlay(bool isSoloPlay) { m_isSoloPlay = isSoloPlay; };

		/// <summary>
		/// プレイヤーにヒットしたかどうかを取得
		/// </summary>
		/// <returns>プレイヤーにヒットしたかどうか</returns>
		bool GetIsPlayerHit()const { return m_playerHit; };
	};
}