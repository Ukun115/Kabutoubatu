/// <summary>
/// 混乱時の星モデルクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PlayerCamera;		//プレイヤーカメラ

	class ConfusionStar : public IGameObject
	{
	private:
		SkinModelRender* m_model = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		SoundSource* m_sound = nullptr;

		Vector3 m_pos;		//位置
		float m_rotationAngle = 0.0f;	//回転角度
		Quaternion m_rot;	//回転

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~ConfusionStar();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//回転するメソッド
		void Rotation();

	public:
		//位置をセットするメソッド
		void SetPosition(Vector3& position) { m_pos = position; };
	};
}