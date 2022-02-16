/// <summary>
/// シルエット描画クラス
/// </summary>
#include "stdafx.h"
#include "Silhouette.h"

namespace nsKabutoubatu
{
	namespace nsSilhouette
	{
		//シルエットマップの解像度
		const int SILHOUETTEMAP_WIDTH = 2048;
		const int SILHOUETTEMAP_HEIGHT = 2048;
		//ミップレベル
		const int MIP_LEVEL = 1;
		//テクスチャ配列のサイズ
		const int ARRAY_SIZE = 1;
	}

	//シルエットの初期化メソッド
	void Silhouette::Init()
	{
		//シルエットマップ描画用のレンダリングターゲットを作成する。
		float clearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
		m_silhouetteMap.Create(
			nsSilhouette::SILHOUETTEMAP_WIDTH,
			nsSilhouette::SILHOUETTEMAP_HEIGHT,
			nsSilhouette::MIP_LEVEL,
			nsSilhouette::ARRAY_SIZE,
			//シルエットマップのカラーバッファのフォーマットを変更している。
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//シルエット描画メソッド
	void Silhouette::DrawSilhouetto(RenderContext& rc)
	{
		// レンダリングターゲットをシルエットマップに変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_silhouetteMap);
		rc.SetRenderTargetAndViewport(m_silhouetteMap);
		//レンダリングターゲットをクリアにする
		rc.ClearRenderTargetView(m_silhouetteMap.GetRTVCpuDescriptorHandle(), m_silhouetteMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_silhouetteMap.GetDSVCpuDescriptorHandle(), m_silhouetteMap.GetDSVClearValue());
		//シルエットを書き込む
		GameObjectManager::GetInstance()->ExecuteSilhouetteRender(rc);
		// 書き込み完了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_silhouetteMap);
	}
}