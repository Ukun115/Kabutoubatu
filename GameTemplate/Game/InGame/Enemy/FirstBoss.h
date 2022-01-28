/// <summary>
/// 最初のボスクラス
/// </summary>
#pragma once
#include "EnemyBase.h"

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class ConfusionStar;	//混乱時の星

	class FirstBoss : public EnemyBase	//エネミーのベースクラスを継承
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>


		//アニメーション
		enum enMoleAnimation
		{
			enIdleAnimation,				//アイドルアニメーション
			enWalkAnimation,				//歩きアニメーション
			enRunAnimation,					//ダッシュアニメーション
			enAttackHornsUpAnimation,		//角上げ攻撃アニメーション
			enAttackHornsForwardAnimation,	//角突き攻撃アニメーション
			enDamageAnimation,				//ダメージアニメーション
			enDeathAnimation,				//死亡アニメーション
			enAnimationNum					//アニメーションの総数
		};

		//HPバーUI
		enum enHPBarUI
		{
			enHPBar,		//HPバー
			enRemainingHP,	//残りHP
			enHPBarUINum	//HPバーUIの数
		};

		//状態
		enum enState
		{
			enNormalState,	//通常
			enDeathState	//死亡
		};

		//移動状態
		enum enMoveState
		{
			enRandomMoveState,			//ランダム移動
			enRunAttackState,			//ダッシュ攻撃
			enAttackHornsUpState,		//角上げ攻撃
			enAttackHornsForwardState	//角突き攻撃
		};

		SpriteRender* m_hpBarUI[enHPBarUINum] = { nullptr };
		FontRender* m_firstBossName = nullptr;
		FontRender* m_salespersonTalk = nullptr;
		ConfusionStar* m_confusionStar = nullptr;

		AnimationClip m_animationClips[enAnimationNum];

		int m_moveState = enRandomMoveState;
		wchar_t m_firstBossNameText[256];
		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//プレイヤーに伸びているベクトル
		float m_plaToEneDistanceLength[enPlayerNum];			//プレイヤーとの距離の長さ
		int m_moveStopTimer = 0;
		int m_moveStopTimer2 = 0;
		int m_randomMoveTimer = 0;
		//正面ベクトル
		Vector3 m_forward;
		int m_animState = enIdleAnimation;
		bool m_canConfusion = true;
		int m_confusionTimer = 0;
		bool m_gameClearFlg = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~FirstBoss();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void SubUpdate()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

		//更新処理メソッド
		void DataUpdate();

		//ランダム移動処理メソッド
		void RandomMove();

		//プレイヤーとの距離検索メソッド
		void DistanceSearch();

		//攻撃範囲内に入ったらフラグオンするメソッド
		bool IsAttackRangeIn();

	public:
		/// <summary>
		/// ゲームクリアしたかどうかを取得
		/// </summary>
		/// <returns>ゲームクリアしたかどうか</returns>
		bool GetGameClearFlg()const { return m_gameClearFlg; };
	};
}