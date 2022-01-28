/// <summary>
/// デプスシャドウクラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class DepthShadow : public IGameObject
	{
	private:
		RenderTarget m_shadowMap;	//シャドウマップのレンダリングターゲット

		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		/// <summary>
		/// デプスシャドウを初期化
		/// </summary>
		void Init();

		/// <summary>
		/// シャドウ描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void DrawShadow(RenderContext& rc);

		/// <summary>
		/// シャドウマップのテクスチャを取得
		/// </summary>
		/// <returns>シャドウマップのテクスチャ</returns>
		Texture& GetShadowMapTexture()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}
	};
}