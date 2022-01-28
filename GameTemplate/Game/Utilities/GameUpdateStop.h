/// <summary>
/// ゲームの制限時間の進行を止めるクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class GameUpdateStop : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//移動ターゲット
		enum enMoveTarget
		{
			enUI,			//UI
			enCameraMove	//カメラ移動
		};

		bool m_isStop = false;
		int m_moveTarget = 0;

	public:
		void SetMoveTarget(const int moveTarget) { m_moveTarget = moveTarget; };

		void SetIsStop(const bool isStop) { m_isStop = isStop; };

		/// <summary>
		/// ゲームを止めているかどうかを取得
		/// </summary>
		/// <returns>ゲームを止めているかどうか</returns>
		bool GetIsStop()const { return m_isStop; };

		/// <summary>
		/// 移動ターゲットを取得
		/// </summary>
		/// <returns>移動ターゲット</returns>
		int GetMoveTarget()const { return m_moveTarget; };
	};
}