/// <summary>
/// フェードクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class OnlineUpdateSpeed;

	class Fade : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//フェードの状態
		enum enState {
			StateIn,	//フェードイン
			StateOut,	//フェードアウト
			StateWait,	//待機
		};

		//フェードの色
		enum enFadeColor
		{
			enWhite,
			enBlack
		};

		//フェード画像
		SpriteRenderSub* m_fadeSprite = nullptr;

		OnlineUpdateSpeed* m_onlineUpdateSpeed = nullptr;

		int m_fadeColorState = enWhite;
		//画像のα値
		float m_alphaValue = 0.0f;
		int m_nowState = StateIn;  //現在のステート

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Fade();

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// </summary>
		void Update()override final;

		/// <summary>
		/// 毎フレーム実行される更新処理関数
		/// (通常の更新を止めてUIだけ更新させたい時に使う更新処理関数)
		/// </summary>
		void UiUpdate()override final;

	public:
		/// <summary>
		/// フェード画像を初期化
		/// </summary>
		/// <param name="alphaValue">α値</param>
		/// <param name="nowState">フェードの状態</param>
		/// <param name="fadeColorState">フェードの色のステート</param>
		void Init(const float alphaValue, const int nowState, const int fadeColorState);

		/// <summary>
		/// フェード画像のα値を取得
		/// </summary>
		/// <returns>フェード画像のα値</returns>
		float GetAlphaValue()const { return m_alphaValue; }

		/// <summary>
		/// フェードの状態を取得
		/// </summary>
		/// <returns>フェードの状態</returns>
		int GetNowState()const { return m_nowState; }
	};
}