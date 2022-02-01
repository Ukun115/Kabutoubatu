/// <summary>
/// オンライン通信クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class SyncOnlineTwoPlayerMatchEngine;	//オンライン通信エンジン
	class OnlineErrorScene;					//オンライン時のエラーシーン

	class Online : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ステップ
		enum enStep {
			enStep_CharacterSelect,			// キャラクターセレクト。
			enStep_WaitAllPlayerJoined,		// すべてのプレイヤーが揃うまで待つ。
			enStep_WaitAllPlayerStartGame,	// すべてのプレイヤーがゲーム開始するのを待つ。
			enStep_InGame,					// インゲーム。
			enStep_Error,					// エラー。
		};

		//通信エンジン
		SyncOnlineTwoPlayerMatchEngine* m_onlineTwoPlayerMatchEngine = nullptr;
		//オンライン通信エラーシーン
		OnlineErrorScene* m_onlineErrorScene = nullptr;

		enStep m_step = enStep_CharacterSelect;

		//プレイヤー番号
		int m_playerNo = -1;
		/// <summary>
		/// フラグ
		/// </summary>
		//すべてのプレイヤーがルームにジョインしたかどうか
		bool m_isJoined = false;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// すべてのプレイヤーがルームにジョインしたときに呼ばれる処理。
		/// </summary>
		/// <param name="pData"></param>
		/// <param name="size"></param>
		void OnAllPlayerJoined(void* pData, int size);

		/// <summary>
		/// すべてのプレイヤーがゲームスタートした時に呼ばれる処理。
		/// </summary>
		void OnAllPlayerStartGame();

		/// <summary>
		/// 通信エラーが起きた。
		/// </summary>
		void OnError();

	public:
		//オンライン処理
		void OnlineInit();

		void DeleteData();

		void GameStart();

		void SetPlayerNo(int playerNo) { m_playerNo = playerNo ; };

		/// <summary>
		/// プレイヤー番号を取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetPlayerNo() { return m_playerNo; };

		/// <summary>
		/// 通信相手のプレイヤー番号を取得
		/// </summary>
		/// <returns>通信相手のプレイヤー番号</returns>
		int GetOtherPlayerNo() { return !GetPlayerNo(); };

		/// <summary>
		/// すべてのプレイヤーがルームにジョインしたかどうかを取得
		/// </summary>
		/// <returns>すべてのプレイヤーがルームにジョインしたかどうか</returns>
		bool GetIsJoined() { return m_isJoined; };

		/// <summary>
		/// プレイヤーのゲームパッドを取得
		/// </summary>
		/// <returns>プレイヤーのゲームパッド</returns>
		GamePad& GetPlayerGamePad(int no);
	};
}