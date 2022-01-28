/// <summary>
/// プレイヤーの回転クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;	//プレイヤー

	class PlayerRotation : public IGameObject
	{
	private:
		Player* m_player = nullptr;

		float m_rotAngle = 0.0f;
		Quaternion m_rot;
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//プレイヤーの回転角度を変えるメソッド
		void SetRotationAngle(const float rotationAngle) { m_rotAngle = rotationAngle; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// プレイヤーの回転を取得
		/// </summary>
		/// <returns>プレイヤーの回転</returns>
		Quaternion GetRotation()const { return m_rot; };
	};
}