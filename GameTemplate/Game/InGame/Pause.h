/// <summary>
/// ポーズ機能クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Pause : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//プレイヤー番号
		enum enPlayer
		{
			enPlayer1,			//１P
			enPlayer2,			//２P
			enTotalPlayerNum	//プレイヤーの総数
		};

		SpriteRenderSub* m_pauseSprite = nullptr;	//ポーズ画像
		SpriteRenderSub* m_grayBack = nullptr;		//ポーズ中の背景画像

		bool m_isPauseFlg = false;		//ポーズ中かどうかフラグ
		bool m_canPause = false;			//ポーズ機能をそもそも使えるかどうかフラグ

		/// <summary>
		/// 呼ばれて最初に実行される関数
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Pause();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update() override final;

		//ポーズ中このアップデートしか呼ばれなくなる
		void PauseUpdate()override final;

	public:
		void SetPause(bool isPauseFlg) { m_isPauseFlg = isPauseFlg; };

		void SetCanPause(bool canPause) { m_canPause = canPause; };

		/// <summary>
		/// ポーズ中かどうかを取得
		/// </summary>
		/// <returns>ポーズ中かどうか</returns>
		bool GetIsPause()const { return m_isPauseFlg; };
	};
}