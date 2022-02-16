/// <summary>
/// デプスシャドウクラス
/// </summary>
#include "stdafx.h"
#include "DepthShadow.h"

namespace nsKabutoubatu
{
	namespace nsDepthShadow
	{
		const int WIDTH = 8192;
		const int HEIGHT = 8192;
		const int MIP_LEBEL = 1;
		const int ARRAY_SIZE = 1;
	}

	//シャドウの初期化メソッド
	void DepthShadow::Init()
	{
		//シャドウマップ描画用のレンダリングターゲットを作成する。
		m_shadowMap.Create(
			nsDepthShadow::WIDTH,
			nsDepthShadow::HEIGHT,
			nsDepthShadow::MIP_LEBEL,
			nsDepthShadow::ARRAY_SIZE,
			//シャドウマップのカラーバッファのフォーマットを変更している。
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//シャドウ描画メソッド
	void DepthShadow::DrawShadow(RenderContext& rc)
	{
		// レンダリングターゲットをシャドウマップに変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		//レンダリングターゲットをクリアにする
		rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());
		//シャドウを書き込む
		GameObjectManager::GetInstance()->ExecuteShadowRender(rc);
		// 書き込み完了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	}
}