/// <summary>
/// タイトルシーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class GameScene;			//ゲームシーン
	class Online;				//オンライン通信
	class Pause;				//ポーズ機能
	class TitleSceneSprite;		//タイトルシーンの画像
	class TitleSceneSound;		//タイトルシーンのサウンド

	class TitleScene : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//選択状態
		enum enStartOrEndState
		{
			enSelectStart,	//はじめる
			enSelectEnd		//おわる
		};
		//選択モード
		enum enSelectGameMode
		{
			enSelectOnline,		//オンラインモード
			enSelectLocalMult,	//ローカルマルチモード
			enSelectSolo,		//ソロモード
			enSelectBack		//もどる
		};

		//オンラインルーム作成・参加状態
		enum enOnlineRoomCreateAndJoinState
		{
			enRoomCreate,				//ルーム作成
			enRoomJoin,					//ルーム参加
			enSelectBackGameMode,		//ゲームモード選択にもどる
		};


		enum enPlayerNo
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//プレイヤーの総数
		};

		//ゲームシーン
		GameScene* m_gameScene = nullptr;
		TitleSceneSprite* m_titleSceneSprite = nullptr;
		TitleSceneSound* m_titleSceneSound = nullptr;
		Pause* m_pause = nullptr;
		Online* m_online = nullptr;

		int m_cursorSelect = 0;
		int m_selectState = 0;
		char m_gamePadNo[256];
		int m_playerNo = 0;
		int m_otherPlayerNo = 0;
		/// <summary>
		/// フラグ
		/// </summary>
		bool m_isRoomCreate = false;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~TitleScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		//ゲームシーンに遷移するメソッド
		void ChangeGameScene(const int mode);

		//カーソルの移動処理
		void CursorMove(const int lastSelect);

	public:
		/// <summary>
		/// 選択状態を取得
		/// </summary>
		/// <returns>選択状態</returns>
		int GetState()const { return m_selectState; };

		/// <summary>
		/// カーソル選択状態を取得
		/// </summary>
		/// <returns>カーソル選択状態</returns>
		int GetCursorSelect()const { return m_cursorSelect; };

		/// <summary>
		/// オンライン通信の部屋を作成したかどうかを取得
		/// </summary>
		/// <returns>オンライン通信の部屋を作成したかどうか</returns>
		bool GetIsRoomCreate()const { return m_isRoomCreate; };
	};
}