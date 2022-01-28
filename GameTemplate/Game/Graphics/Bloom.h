/// <summary>
/// 川瀬式ブルームクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// 前方宣言
	/// </summary>
	class GaussianBlur;	//ガウシアンブラー

	class Bloom : public IGameObject
	{
	private:
		/// <summary>
		/// 列挙型
		/// </summary>

		//ガウシアンブラーをかける回数
		enum enGaussianBlurNum
		{
			enFirstGaussianBlurNum,		//１回目
			enSecondGaussianBlurNum,	//２回目
			enThirdGaussianBlurNum,		//３回目
			enFoursGaussianBlurNum,		//４回目
			enMaxGaussianBlurNum		//ガウシアンブラーをかける回数
		};

		GaussianBlur* m_gaussianBlur[enMaxGaussianBlurNum];	//ガウシアンブラー

		RenderTarget m_luminnceRenderTarget;	// 輝度抽出用のレンダリングターゲット
		SpriteInitData m_luminanceSpriteInitData;//輝度画像のデータ
		Sprite m_luminanceSprite;				//輝度画像
		SpriteInitData m_bokeSpriteInitData;	//ボケ画像のデータ
		Sprite m_bokeSprite;		//ボケ画像
		// mainRenderTargetのテクスチャをフレームバッファーに貼り付けるための画像のデータ
		SpriteInitData m_spriteInitData;
		// mainRenderTargetのテクスチャをフレームバッファーに貼り付けるための画像
		Sprite m_copyToFrameBufferSprite;

	public:
		/// <summary>
		/// ブルームを初期化
		/// </summary>
		/// <param name="mrt">メインレンダーターゲット</param>
		void Init(RenderTarget& mrt);

		//輝度画像の初期化メソッド
		/// <summary>
		/// 輝度画像を初期化
		/// </summary>
		/// <param name="mrt">メインレンダーターゲット</param>
		void LuminanceSpriteInit(RenderTarget& mrt);

		/// <summary>
		/// ガウシアンブラーを初期化
		/// </summary>
		void GaussianBlurInit();

		/// <summary>
		///	ボケ画像を初期化
		/// </summary>
		void BokeSpriteInit();

		/// <summary>
		/// ブルーム描画
		/// </summary>
		/// <param name="rt">レンダーターゲット</param>
		/// <param name="mrt">メインレンダーターゲット</param>
		void DrawBloom(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// 輝度抽出用のレンダーターゲットを取得
		/// </summary>
		/// <returns>輝度抽出用のレンダーターゲット</returns>
		RenderTarget& GetLuminnceRenderTarget()
		{
			return m_luminnceRenderTarget;
		}
	};
}