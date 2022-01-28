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

	class OnlineErrorScene : public IGameObject
	{
	private:
		GameScene* m_gameScene = nullptr;
		Pause* m_pause = nullptr;
		SpriteRenderSub* m_grayBack = nullptr;
		SpriteRenderSub* m_errorMessageBox = nullptr;

		int m_padNo = 0;
		float m_errorMessageBoxXscale = 0.0f;

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

		void BackTitle(const int padNo);

	public:
		void SetPadNumber(const int padNo) { m_padNo = padNo; };
	};
}