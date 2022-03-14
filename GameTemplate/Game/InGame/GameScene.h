/// <summary>
/// ゲームシーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class Stage;			//ステージ
	class SkyCube;			//スカイキューブ
	class Player;			//プレイヤー
	class PlayerStatus;		//プレイヤーのステータス
	class PlayerCamera;		//プレイヤーのカメラ
	class Weapon;			//武器
	class AccompanyAI;		//同行AI
	class GameScreenUI;		//ゲーム画面UI
	class GameLimitTime;	//ゲームの制限時間
	class GameOverScene;	//ゲームオーバーシーン
	class Fade;				//フェード

	class GameScene : public IGameObject
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

		//武器
		enum enWeapon
		{
			enWoodStick,	//木の棒
			enSword,		//剣
			enLargeSword,	//大剣
			enArow			//弓
		};

		SoundSource* m_gameSceneSound = nullptr;
		//スカイキューブ
		SkyCube* m_skyCube = nullptr;
		Fade* m_fade = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Stage* m_stage = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		GameOverScene* m_gameOverScene = nullptr;
		GameScreenUI* m_gameScreenUI = nullptr;
		FontRender* m_nowMoney[enPlayerNum] = { nullptr };
		GamePad* m_playerGamePad;
		GamePad* m_otherPlayerGamePad;
		FontRender* m_haveRecoveryItemNum[enPlayerNum] = { nullptr };
		Weapon* m_weapon[enPlayerNum] = { nullptr };

		int m_playerNum = enPlayerNum;
		wchar_t m_nowMoneyText[256];
		wchar_t m_haveRecoveryItemNumText[256];
		//オンライン時のプレイヤー番号
		int m_onlinePlayerNo = 0;
		//プレイヤー番号
		int m_playerNo = enPlayer1;
		//他のプレイヤー番号
		int m_otherPlayerNo = enPlayer1;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_online = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//ゲームオーバー移行処理メソッド
		void GameOver();

		//回復アイテム文字の初期化メソッド
		void RecoveryItemFontInit(const int playerNum);

		//お金文字の初期化メソッド
		void NowMoneyFontInit(const int playerNum);

		//回復アイテムの所持数文字の数更新
		void RecoveryItemNumUpdate(const int playerNum);

		//お金の所持数文字の数更新7
		void NowMoneyNumUpdate(const int playerNum);

		void PlayerInit(Player* player, int playerNo, GamePad* gamePad, Weapon* weapon);

	public:
		//プレイヤー番号をセットするメソッド
		void SetPlayerNo(const int playerNo) { m_playerNo = playerNo; };

		void SetOtherPlayerNo(const int otherPlayerNo) { m_otherPlayerNo = otherPlayerNo; };

		void SetPlayerGamePad(GamePad& gamePad) { m_playerGamePad = &gamePad; };

		void SetOtherPlayerGamePad(GamePad& gamePad) { m_otherPlayerGamePad = &gamePad; };

		void SetOnlinePlay(bool online) { m_online = online; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		//ゲーム音を止めるメソッド
		void GameBGMStop() { m_gameSceneSound->Stop(); };

		void GameBGMPlay() { m_gameSceneSound->Play(true); };

		void DeleteGameSceneClass() { DeleteGO(this); };

		/// <summary>
		/// プレイヤー番号を取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetPlayerNo() { return m_playerNo; };
	};
}