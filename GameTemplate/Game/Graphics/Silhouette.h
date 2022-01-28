/// <summary>
/// シルエット描画クラス
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Silhouette : public IGameObject
	{
	private:
		RenderTarget m_silhouetteMap;	//シルエットマップのレンダリングターゲット

	public:
		//シルエットの初期化メソッド
		void Init();

		//シルエット描画メソッド
		void DrawSilhouetto(RenderContext& rc);

		/// <summary>
		/// シルエットマップのテクスチャを取得
		/// </summary>
		/// <returns>シルエットマップのテクスチャ</returns>
		Texture& GetSilhouetteMapTexture()
		{
			return m_silhouetteMap.GetRenderTargetTexture();
		}
	};
}