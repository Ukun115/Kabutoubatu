/// <summary>
/// プレイヤーカメラクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;						//プレイヤー
	class CameraMoveDirectionChange;	//カメラ移動方向変更

	class PlayerCamera : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//カメラの移動方向
		enum enCameraMoveDirection
		{
			enX,		//横スクロール
			enY,		//縦スクロール
			enZ,		//奥スクロール
			enShop,		//宿屋とアイテムショップの固定カメラ
			enBossStage	//ボスステージの固定カメラ
		};

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		/// <summary>
		/// 構造体
		/// </summary>

		//カメラ
		struct stCamera
		{
			Matrix mCVP;	//カメラビュープロジェクション行列
		};

		Player* m_player = nullptr;		//プレイヤー検索するためのインスタンス
		CameraMoveDirectionChange* m_cameraMoveDirectionChange = nullptr;		//カメラの移動方向を変えるクラス

		stCamera m_cameraData;		//カメラ構造体のデータ

		Vector3 m_cameraPos;		//視点
		Vector3 m_cameraTarget;		//注視点
		int m_cameraState = enX;
		float m_cameraTargetXPosition = 0.0f;
		float m_cameraTargetYPosition = 0.0f;
		float m_cameraTargetZPosition = 0.0f;
		int m_targetPlayer = enPlayer1;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~PlayerCamera();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

	public:
		//カメラの移動方向セットメソッド
		void SetCameraState(const int& cameraState){m_cameraState = cameraState;};

		//※カメラ移動方向を切り替えた後に必ず下のメソッドを呼び、固定する位置を設定する
		//横スクロールの時のカメラ位置セットメソッド
		void SetXMoveCameraPos(const float& y, const float& z)
		{
			m_cameraTargetYPosition = y;
			m_cameraTargetZPosition = z;
		};

		//縦スクロールの時のカメラ位置セットメソッド
		void SetYMoveCameraPos(const float& x, const float& z)
		{
			m_cameraTargetXPosition = x;
			m_cameraTargetZPosition = z;
		};

		//奥スクロールの時のカメラ位置セットメソッド
		void SetZMoveCameraPos(const float& x, const float& y)
		{
			m_cameraTargetXPosition = x;
			m_cameraTargetYPosition = y;
		};

		void SetTargetPlayer(const int targetPlayer) { m_targetPlayer = targetPlayer; };

		/// <summary>
		/// 注視点を取得
		/// </summary>
		/// <returns>注視点</returns>
		Vector3 GetCameraTarget() { return m_cameraTarget; };

		/// <summary>
		/// カメラのデータを取得
		/// </summary>
		/// <returns>カメラのデータ</returns>
		stCamera GetCamera()const { return m_cameraData; }

		/// <summary>
		/// カメラのデータのアドレスを取得
		/// </summary>
		/// <returns>カメラのデータのアドレス</returns>
		stCamera* GetCameraAdoress() { return &m_cameraData; }
	};
}