/// <summary>
/// スライム(ザコ敵)クラス
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Slime : public EnemyBase	//エネミーのベースクラスを継承
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//状態
		enum enState
		{
			enKyoroKyoro,	//キョロキョロ
			enJump,			//ジャンプ
			enDeath,		//死亡
			enAnimationNum	//アニメーションの総数
		};

		//サウンド
		enum enSlimeSound
		{
			enAttackSound,		//攻撃サウンド
			enSlimeSoundNum		//サウンドの総数
		};

		SoundSource* m_slimeSound[enSlimeSoundNum] = { nullptr };

		AnimationClip m_animationClips[enAnimationNum];

		//ジャンプ力
		float m_jumpPower = 7.5f;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_jumpStartFlg = true;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Slime();

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

		//ジャンプメソッド
		void Jump();

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