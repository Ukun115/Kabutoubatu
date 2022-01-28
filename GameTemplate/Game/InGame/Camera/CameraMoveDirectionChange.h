/// <summary>
/// プレイヤーカメラの移動方向変更クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;		//プレイヤー
	class PlayerCamera;	//プレイヤーカメラ

	class CameraMoveDirectionChange : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayer
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		//カメラの移動方向
		enum enCameraMoveDirection
		{
			enX,			//横スクロール
			enY,			//縦スクロール
			enZ,			//奥スクロール
			enShop,			//宿屋とアイテムショップの固定カメラ
			enBossStage		//ボスステージの固定カメラ
		};

		//ウェイポイント
		enum enNowWayPoint
		{
			enWay1,	//ウェイ1
			enWay2,	//ウェイ2
			enWay3,	//ウェイ3
			enWay4	//ウェイ4
		};

		Player* m_player = nullptr;		//プレイヤー検索するためのインスタンス
		PlayerCamera* m_playerCamera = nullptr;		//プレイヤーカメラを検索するためのインスタンス

		Vector3 m_playerPosition;		//位置
		//現在のウェイポイント
		int m_nowWayPoint = enWay1;
		int m_targetPlayer = enPlayer1;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~CameraMoveDirectionChange();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//ウェイ１のカメラ設定
		void Way1Camera();

		//ウェイ2のカメラ設定
		void Way2Camera();

		//ウェイ3のカメラ設定
		void Way3Camera();

	public:
		void SetTargetPlayer(int targetPlayer) { m_targetPlayer = targetPlayer; };
	};
}