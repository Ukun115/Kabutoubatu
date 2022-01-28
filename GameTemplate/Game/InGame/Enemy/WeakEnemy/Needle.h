/// <summary>
/// とげスライム(ザコ敵)の遠距離攻撃のとげクラス
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Needle : public EnemyBase	//エネミーのベースクラスを継承
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//トゲのサウンド
		enum enNeedleSound
		{
			enAttackSound,		//攻撃サウンド
			enNeedleSoundNum	//サウンドの総数
		};

		SoundSource* m_needleSlimeSound[enNeedleSoundNum] = { nullptr };

		int m_timer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Needle();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void SubUpdate()override final;

	public:
		void SetMoveDirection(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		void SetInitPosition(const Vector3& pos) { m_pos = pos; };
	};
}