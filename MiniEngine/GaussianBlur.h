/// <summary>
/// ガウシアンブラークラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class GaussianBlur : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//重みの数
		enum { NUM_WEIGHTS = 8 };

		Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。

		RenderTarget m_xBlurRenderTarget;		//横ボケ画像を描画するレンダリングターゲット。
		RenderTarget m_yBlurRenderTarget;		//縦ボケ画像を描画するレンダリングターゲット。
		Sprite m_xBlurSprite;					//横ボケ画像を描画するためのスプライト。
		Sprite m_yBlurSprite;					//縦ボケ画像を描画するためのスプライト。

		float m_weights[NUM_WEIGHTS];			//重みテーブル。

		/// <summary>
		/// レンダリングターゲットを初期化。
		/// </summary>
		void InitRenderTargets();

		/// <summary>
		/// スプライトを初期化。
		/// </summary>
		void InitSprites();

		/// <summary>
		/// 重みテーブルを更新する。
		/// </summary>
		void UpdateWeightsTable(float blurPower);

	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="originalTexture">ガウシアンブラーをかけるオリジナルテクスチャ。</param>
		void Init(Texture* originalTexture);

		/// <summary>
		/// ガウシアンブラーをGPU上で実行。
		/// </summary>
		/// <remarks>
		/// 本関数の呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出す必要があります。
		/// </remarks>
		/// <param name="rc">レンダリングターゲット</param>
		/// <param name="blurPower">ブラーの強さ。値が大きいほどボケが強くなる。</param>
		void ExecuteOnGPU(RenderContext& rc, float blurPower);

		/// <summary>
		/// ボケテクスチャを取得。
		/// </summary>
		/// <returns>ボケテクスチャ</returns>
		Texture& GetBokeTexture()
		{
			return m_yBlurRenderTarget.GetRenderTargetTexture();
		}
	};
}