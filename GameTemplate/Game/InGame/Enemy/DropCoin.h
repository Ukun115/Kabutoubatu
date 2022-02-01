/// <summary>
/// コインクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Player;		//プレイヤー
	class PlayerStatus;	//プレイヤーステータス
	class PlayerCamera;	//プレイヤーカメラ

	class DropCoin : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		Player* m_player[enPlayerNum] = { nullptr };		//プレイヤー検索するためのインスタンス
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		SkinModelRender* m_coin = nullptr;	//モデル
		PlayerCamera* m_playerCamera = nullptr;
		SoundSource* m_coinSound = nullptr;

		int m_playerNum = enPlayerNum;
		Vector3 m_pos;		//位置
		float m_playerToCoinDistancelength = 0.0f;	//プレイヤーとモデルとの距離の長さ
		//コインの値
		int m_coinValue = 100;
		float m_rotAngle = 0.0f;	//回転角度
		Quaternion m_rot;		//回転
		float m_boundPower = 0.0f;		//バウンド力
		float m_boundStartPosY = 0.0f;	//バウンド開始位置
		int m_boundPowerDown = 5;	//バウンド力を下げる力
		//回転初速度
		float m_firstRotSpeed = 30.0f;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_getOkFlg = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~DropCoin();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//コインモデルの初期化メソッド
		void CoinModelInit();

		//コインがバウンドする処理メソッド
		void Bound();

		//コインが回転する処理メソッド
		void Turn();

		//コイン取得処理メソッド
		void CoinGet();

	public:
		//コインの位置セットメソッド
		void SetPosition(const Vector3& pos) { m_pos = pos; };

		//コインの値セットメソッド
		void SetValue(const int& coinValue) { m_coinValue = coinValue; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}