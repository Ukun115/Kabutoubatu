/// <summary>
/// 被写界深度クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class GaussianBlur;		//ガウシアンブラー

	class DoF : public IGameObject
	{
	private:
		/// <summary>
		/// 構造体
		/// </summary>

		//被写界深度のオンオフ
		struct stCanDoF
		{
			bool canDoF = false;	//被写界深度のオンオフフラグ
		};

		GaussianBlur* m_gaussianBlur;		//ガウシアンブラー

		stCanDoF m_canDoF;
		RenderTarget m_depthRenderTarget;	//深度値のレンダーターゲット
		SpriteInitData m_combineBokeImageSpriteInitData;	//ボケ画像のデータ
		Sprite m_combineBokeImageSprite;		//ボケ画像
		// メインレンダリングターゲットの絵をフレームバッファにコピーするための画像のデータ
		SpriteInitData m_spriteInitData;
		// メインレンダリングターゲットの絵をフレームバッファにコピーするための画像
		Sprite m_copyToFrameBufferSprite;

	public:
		/// <summary>
		/// 被写界深度を初期化
		/// </summary>
		/// <param name="mrt">メインレンダーターゲット</param>
		/// <param name="zprepassRenderTarget">Zプレパスのレンダーターゲット</param>
		void Init(RenderTarget& mrt, RenderTarget& zprepassRenderTarget);

		/// <summary>
		/// 被写界深度のレーダーターゲットを設定
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mrt">メインレンダーターゲット</param>
		void SettingDoFRenderTarget(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// 被写界深度を描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mrt">メインレンダーターゲット</param>
		void DrawDoF(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// ボケ画像を初期化
		/// </summary>
		/// <param name="mrt">メインレンダーターゲット</param>
		void BokeSpriteInit(RenderTarget& mrt);

		/// <summary>
		/// 被写界深度のオンオフを設定
		/// </summary>
		/// <param name="canDoF">被写界深度がオンかどうか</param>
		void SetCanDoF(const bool canDoF) { m_canDoF.canDoF = canDoF; };

		/// <summary>
		/// 被写界深度テクスチャーを取得
		/// </summary>
		/// <returns>被写界深度テクスチャー</returns>
		Texture& GetDoFTexture()
		{
			return m_depthRenderTarget.GetRenderTargetTexture();
		}
	};
}