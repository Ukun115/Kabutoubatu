/// <summary>
/// プレイヤーのキャラクターコントローラークラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class PlayerCharaCon : public IGameObject
	{
	private:
		CharacterController m_charaCon;		//キャラクタコントローラークラスを作成

	public:
		//キャラコンを初期化
		void InitRigidBody(const Vector3& playerPosition)
		{
			m_charaCon.Init(25.0f, 50.0f, playerPosition);
		}

		//キャラコンを破棄
		void RemoveRigidBoby()
		{
			m_charaCon.RemoveRigidBoby();
		}

		/// <summary>
		/// 地面についているかどうかを取得
		/// </summary>
		/// <returns>地面についているかどうか</returns>
		bool IsOnGround()const
		{
			return m_charaCon.IsOnGround();
		}

		/// <summary>
		/// 空中にあるかどうかを取得
		/// </summary>
		/// <returns>空中にあるかどうか</returns>
		bool IsJump()const
		{
			return m_charaCon.IsJump();
		}

		/// <summary>
		/// キャラクターコントローラーの更新データを取得
		/// </summary>
		/// <param name="moveSpeed">移動速度</param>
		/// <param name="isHitGround">地面の上かどうか</param>
		/// <param name="hitGroundNormal">当たっている地面に法線</param>
		/// <returns>キャラクターコントローラーの更新データ</returns>
		Vector3 RigidBodyExecute(Vector3& moveSpeed,bool isHitGround,Vector3& hitGroundNormal)
		{
			return m_charaCon.Execute(
				moveSpeed,
				1.0f,
				isHitGround,
				hitGroundNormal
			);
		}

		/// <summary>
		/// 空中にあるかどうかを取得
		/// </summary>
		/// <returns>空中にあるかどうか</returns>
		CharacterController* GetCharaCon() { return &m_charaCon; };
	};
}