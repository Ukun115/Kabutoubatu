/// <summary>
/// ゲームクリアシーンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PlayerStatus;		//プレイヤーのステータス
	class GameScene;		//ゲームシーン
	class GameUpdateStop;	//ゲーム更新ストップ

	class GameClearScene : public IGameObject
	{
	private:
		SoundSource* m_gameClearSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		PlayerStatus* m_playerStatus[2] = { nullptr };
		SpriteRenderSub* m_toubatuKannryou = nullptr;
		GameUpdateStop* m_gameUpdateStop = nullptr;
		SpriteRenderSub* m_gameClearSprite = nullptr;
		GameScene* m_gameScene = nullptr;
		FontRenderSub* m_enemyKillNumberFont[2] = { nullptr };
		FontRenderSub* m_coinGetNumberFont[2] = { nullptr };
		FontRenderSub* m_recoveryReceiveNumberFont[2] = { nullptr };
		FontRenderSub* m_deathNumberFont[2] = { nullptr };

		wchar_t m_text[256];
		Vector2 m_fontPos;
		int m_playerNum = 0;
		int m_spriteChangeTimer = 0;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GameClearScene();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}