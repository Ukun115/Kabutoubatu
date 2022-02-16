/// <summary>
/// 宿屋シーンの選択クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class HotelSelectFont;	//宿屋シーンの選択文字
	class PlayerStatus;		//プレイヤーのステータス
	class PlayerCamera;		//プレイヤーのカメラ

	class HotelSelect : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ホテルのモード
		enum enHotelMode
		{
			enStay,		//宿泊
			enEnter		//出る
		};

		//ホテルの状態
		enum enHotelState
		{
			enSelectState,	//選択状態
			enStayState,	//宿泊状態
			enEnterState	//出る状態
		};

		PlayerStatus* m_playerStatus = nullptr;
		HotelSelectFont* m_hotelSelectFont = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		SoundSource* m_cantDecideSound = nullptr;
		SoundSource* m_buySound = nullptr;

		GamePad* m_playerGamePad;

		int m_nowHotelMode = enStay;	//泊まる、泊まらない。の２択
		int m_nowHotelState = enSelectState;
		int m_playerNum = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~HotelSelect();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//アイテムショップから出るメソッド
		void EnterFromHotel();

		//選択メソッド
		void CursorMove(const int selectMin, const int selectMax);

		//次の状態に行くメソッド
		void NextState();

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// 現在のショッピング状態を取得
		/// </summary>
		/// <returns>現在のショッピング状態</returns>
		int GetNowSelectState()const { return m_nowHotelState; };

		void SetPlayerGamePad(GamePad& gamePad) { m_playerGamePad = &gamePad; };
	};
}