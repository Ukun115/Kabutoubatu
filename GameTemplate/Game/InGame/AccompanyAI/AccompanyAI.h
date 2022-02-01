/// <summary>
/// ソロモード時の同行AI(以降「おとも」と呼ぶ)クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;				//プレイヤー
	class AccompanyAISword;		//同行AIの剣
	class AccompanyAIShield;	//同行AIの盾
	class AccompanyAIAnimation;	//同行AIのアニメーション

	class AccompanyAI : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayer
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		//攻守の状態
		enum enAttackAndDefenseMode
		{
			enAttackMode,	//攻撃モード
			enDefenseMode,	//守備モード
		};

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

		//ステート
		enum enState
		{
			enStopping,			//静止状態
			enIdle,				//プレイヤーの近くに立っている状態
			enAccompany,		//同行する状態
			enShieldDashAttack,	//シールドで敵を押してプレイヤーを護衛する状態
			enAttack,			//攻撃する状態
			enReceiveAttack,	//攻撃を受けた状態
			enDeath,			//死んでいる状態
		};

		//おともモデル
		SkinModelRender* m_otomoModel = nullptr;
		Player* m_player = nullptr;
		AccompanyAIAnimation* m_accompanyAIAnimation = nullptr;
		AccompanyAISword* m_sword = nullptr;
		AccompanyAIShield* m_shield = nullptr;

		CharacterController m_charaCon;		//キャラクタコントローラークラスを作成

		float m_playerToMeLength = 0.0f;
		Vector3 m_playerToMeDirection;
		int m_attackComboNo = 0;
		int m_attackAndDefenseMode = enDefenseMode;
		int m_state = enIdle;
		int m_randomTimer = 0;
		Vector3 m_playerPos;
		Vector3 m_circleSetPos;
		int m_loop = 0;
		Vector3 m_pos;	//位置
		Vector3 m_moveSpeed;	//移動速度
		float m_rotAngle = 0;	//回転角度
		Quaternion m_rot;	//回転
		bool m_isHitGround = false;
		Vector3 m_hitGroundNormal;
		int m_hitPoint = 10;	//体力
		int m_randomAngle = 0;
		//移動速度を取得するための変数
		Vector3 m_playerLookingDirection;
		int m_attackJumpTimer = 0;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_isPlayerGroundWalk = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~AccompanyAI();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

		//プレイヤーにかかる重力を設定するメソッド
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		//落下処理メソッド
		void Fall();

		//おとものデータを更新するメソッド
		//位置・回転・キャラコン・アニメーション状態
		void DataUpdate();

		//プレイヤーを中心とする、半径(第一引数)の円周上の点をランダムに取得するメソッド
		Vector3 PlayerAroundPos(const float r);

	public:
		void SetNowState(const int state) { m_state = state; };

		void SetShieldPushDashDirection(Vector3& moveSpeed);

		//おともの位置を移動させる
		void SetPosition(const Vector3& position);

		/// <summary>
		/// おとものスケルトンを取得
		/// </summary>
		/// <returns>おとものスケルトン</returns>
		Skeleton* GetSkeleton() { return m_otomoModel->GetSkeleton(); };

		/// <summary>
		/// 位置を取得
		/// </summary>
		/// <returns>位置</returns>
		Vector3 GetPosition()const { return m_pos; };

		/// <summary>
		/// 攻守の状態を取得
		/// </summary>
		/// <returns>攻守の状態</returns>
		int GetMode()const { return m_attackAndDefenseMode; };

		/// <summary>
		/// 状態を取得
		/// </summary>
		/// <returns>状態</returns>
		int GetNowState()const { return m_state; };
	};
}