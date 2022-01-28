/// <summary>
/// モグラ(ザコ敵)クラス
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Mole : public EnemyBase	//エネミーのベースクラスを継承
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//モグラの状態
		enum enMoleNowState
		{
			enPlayerSearchAndRandomMove,	//近くのプレイヤーを探している&ランダム移動状態
			enAttack,						//突進攻撃している状態
			enBlowAway,						//プレイヤーを吹っ飛ばしている状態
			enDeath,						//死亡状態
			enMoleNowStateNum				//プレイヤーの状態の総数数
		};

		//アニメーション
		enum enMoleAnimation
		{
			enWalkAnimation,	//歩き
			enTackleAnimation,	//タックル
			enWinAnimation,		//勝利
			enDeathAnimation,	//死亡アニメーション
			enAnimationNum		//アニメーションの総数
		};

		//サウンド
		enum enMoleSound
		{
			enAttackSound,	//攻撃サウンド
			enMoleSoundNum	//サウンドの総数
		};

		SkinModelRender* m_exclamationMark = nullptr;	//プレイヤーを発見したときのエクスクラメーションマークモデル
		SoundSource* m_moleSound[enMoleSoundNum] = { nullptr };

		AnimationClip m_animationClips[enAnimationNum];

		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//プレイヤーに伸びているベクトル
		Vector3 m_playerToEnemyDistanceDirecion2[enPlayerNum];	//プレイヤーに伸びているベクトル
		float m_plaToEneDistanceLength[enPlayerNum];			//プレイヤーとの距離の長さ
		int m_attackTimer = 0;		//攻撃のタイマー
		int m_attackDelayTimer = 0;	//攻撃前の攻撃を遅らせるタイマー
		int m_randomMoveTimer = 0;	//ランダム移動タイマー
		Vector3 m_exclamationMarkPos;	//エクスクラメーションマークの位置
		int m_blowAwayPlayer = 0;		//吹っ飛ばすプレイヤーの番号
		int m_blowAwayAfterWaitTimer = 0;	//吹っ飛ばし後の硬直タイマー
		//正面ベクトル
		Vector3 m_forward;
		int m_randomDelayTimer = 0;
		Vector3 m_finalOnGroundPos;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Mole();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void SubUpdate()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

		//ランダム移動処理メソッド
		void RandomMove();

		//距離検索メソッド
		void DistanceSearch();

		//攻撃(突進)処理メソッド
		void Attack();

		//更新処理メソッド
		void DataUpdate();

		//エクスクラメーションマークの表示処理メソッド
		void ExclamationMarkActive();

		//弱敵の吹っ飛ばす方向を渡す処理メソッド
		void PassBlowAwayDirection();

		//敵の情報を初期化するメソッド
		void EnemyInit();

		//攻撃範囲内に入ったらフラグオンするメソッド
		bool IsAttackRangeIn();

	public:
		//初期位置を指定するセットメソッド
		void SetInitPosition(const Vector3& position) { m_pos = position; };

		/// <summary>
		/// 落下したかどうかを取得
		/// </summary>
		/// <returns>落下したかどうか</returns>
		bool GetFall()const { return m_isFall; };
	};
}