/// <summary>
/// 矢クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Arrow : public IGameObject
	{
	private:
		SkinModelRender* m_arrowModel = nullptr;	//モデル

		Vector3 m_pos;			//位置
		Quaternion m_rot;		//回転
		Vector3 m_moveSpeed;	//発射される方向(移動速度)
		int m_playerNum = 0;
		int m_deleteTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Arrow();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//発射の開始位置をセットする関数
		void SetShotPosition(const Vector3& shotPos) { m_pos = shotPos; };

		void SetMoveDirection(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// 矢の位置を取得
		/// </summary>
		/// <returns>矢の位置</returns>
		Vector3 GetPosition()const { return m_pos; };
	};
}