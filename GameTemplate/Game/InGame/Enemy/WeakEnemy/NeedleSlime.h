/// <summary>
/// トゲスライム(ザコ敵)クラス
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Needle;	//トゲ

	class NeedleSlime : public EnemyBase	//エネミーのベースクラスを継承
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//トゲスライムの状態
		enum enState
		{
			enIdle,			//アイドル
			enWalk,			//歩く
			enAttack,		//攻撃
			enDeath,		//死亡
			enAnimationNum	//アニメーションの総数
		};

		Needle* m_needle = nullptr;					//トゲクラス

		AnimationClip m_animationClips[enAnimationNum];

		Vector3 m_needleInitPos;
		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//プレイヤーに伸びているベクトル
		int m_randomMoveTimer = 0;	//ランダム移動タイマー
		float m_plaToEneDistanceLength[enPlayerNum];			//プレイヤーとの距離の長さ
		int m_idleTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~NeedleSlime();

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

		//攻撃範囲内に入ったらフラグオンするメソッド
		bool IsAttackRangeIn();

		//距離検索メソッド
		void DistanceSearch();

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