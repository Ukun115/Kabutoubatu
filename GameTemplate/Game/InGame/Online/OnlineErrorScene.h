/// <summary>
/// オンライン通信時の通信エラークラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class GameScene;		//ゲームシーン
	class Pause;			//ポーズ機能
	class SpriteRenderSub;	//画像描画処理サブ
	class Online;			//オンライン

	class OnlineErrorScene : public IGameObject
	{
	private:
		GameScene* m_gameScene = nullptr;
		Pause* m_pause = nullptr;
		SpriteRenderSub* m_grayBack = nullptr;
		SpriteRenderSub* m_errorMessageBox = nullptr;

		Online* m_online = nullptr;

		int m_padNo = 0;
		float m_errorMessageBoxXscale = 0.0f;

		GamePad* m_playerGamePad = nullptr;

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~OnlineErrorScene();

		//ポーズ用のアップデート関数を利用する
		void PauseUpdate()override final;

		void BackTitle();

	public:
		void SetPadNumber(const int padNo) { m_padNo = padNo; };

		void SetPlayerGamePad(GamePad& gamePad) { m_playerGamePad = &gamePad; };
	};
}