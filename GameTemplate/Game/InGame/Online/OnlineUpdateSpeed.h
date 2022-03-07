/// <summary>
/// オンライン時の更新速度変更クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class OnlineUpdateSpeed : public IGameObject
	{
	private:
		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~OnlineUpdateSpeed();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update() override final;

		bool m_isOnline = false;
		float m_speed = 1.0f;

	public:
		bool GetIsOnline()const { return m_isOnline; };
		float GetSpeed()const { return m_speed; };
		void SetIsOnline(const bool isOnline) { m_isOnline = isOnline; };
	};
}