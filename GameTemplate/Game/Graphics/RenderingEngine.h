/// <summary>
/// レンダリングエンジンクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class PostEffect;	//ポストエフェクト

	class RenderingEngine : public IGameObject
	{
	private:
		// ZPrepass描画用のレンダリングターゲット
		RenderTarget m_zprepassRenderTarget;
		// メインレンダリングターゲット
		RenderTarget m_mainRenderTarget;
		//ポストエフェクト
		PostEffect* m_postEffect;

		/// <summary>
	   /// ZPrepass
	   /// </summary>
	   /// <param name="rc">レンダリングコンテキスト</param>
		void SettingZPrepass(RenderContext& rc);

		/// <summary>
		/// ZPrepass用のレンダリングターゲットを初期化
		/// </summary>
		void InitZPrepassRenderTarget();

		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();

	public:
		/// <summary>
		/// レンダリングパイプラインを初期化
		/// </summary>
		void Init();

		/// <summary>
		/// レンダリングパイプラインを実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// ZPrepassで作成された深度テクスチャを取得
		/// </summary>
		/// <returns>ZPrepassで作成された深度テクスチャ</returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
		}
	};
}