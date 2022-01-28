/// <summary>
/// 川瀬式ブルームクラス
/// </summary>
#include "stdafx.h"
#include "Bloom.h"
#include "GaussianBlur.h"

namespace nsKabutoubatu
{
	namespace nsBloom
	{
		const int SPRITE_WIDTH = 1280;
		const int SPRITE_HEIGHT = 720;
		const int BLUR_POWER = 10;
		const int FIRST_ARRAY = 0;

		const char* SPRITE2D_FX_FILEPATH = "Assets/shader/2D.fx";
		const char* BLOOM_FX_FILEPATH = "Assets/shader/bloom.fx";
	}

	//ブルーム初期化メソッド
	void Bloom::Init(RenderTarget& mrt)
	{
		// 解像度、ミップマップレベル
		m_luminnceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			//カラーバッファーのフォーマットを32bit浮動小数点にしている
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//輝度画像を初期化
		LuminanceSpriteInit(mrt);

		//ガウシアンブラーを初期化
		GaussianBlurInit();

		//ボケ画像を初期化
		BokeSpriteInit();

		// テクスチャはmainRenderTargetのカラーバッファー
		m_spriteInitData.m_textures[nsBloom::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();
		m_spriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_spriteInitData.m_height = nsBloom::SPRITE_HEIGHT;

		// モノクロ用のシェーダーを指定する
		m_spriteInitData.m_fxFilePath = nsBloom::SPRITE2D_FX_FILEPATH;

		m_copyToFrameBufferSprite.Init(m_spriteInitData);
	}

	//ブルーム描画メソッド
	void Bloom::DrawBloom(RenderContext& rc, RenderTarget& mrt)
	{
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mrt);

		// 輝度抽出
		// 輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_luminnceRenderTarget);

		// 輝度抽出を行う
		m_luminanceSprite.Draw(rc);

		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

		//ガウシアンブラーを4回実行する。
		for (int executionNum = enFirstGaussianBlurNum; executionNum < enMaxGaussianBlurNum; executionNum++)
		{
			m_gaussianBlur[executionNum]->ExecuteOnGPU(rc, nsBloom::BLUR_POWER);
		}

		//4枚のボケ画像を合成してメインレンダリングターゲットに加算合成。
		//レンダリングターゲットとして利用できるまで待つ。
		rc.WaitUntilToPossibleSetRenderTarget(mrt);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetAndViewport(mrt);
		//ボケ画像をメインレンダーターゲットに最終合成。
		m_bokeSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mrt);
	}

	//輝度画像の初期化メソッド
	void Bloom::LuminanceSpriteInit(RenderTarget& mrt)
	{
		// 輝度抽出用のシェーダーのファイルパスを指定する
		m_luminanceSpriteInitData.m_fxFilePath = nsBloom::BLOOM_FX_FILEPATH;

		// 頂点シェーダーのエントリーポイントを指定する
		m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

		// ピクセルシェーダーのエントリーポイントを指定する
		m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

		// スプライトの幅と高さはluminnceRenderTargetと同じ
		m_luminanceSpriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_luminanceSpriteInitData.m_height = nsBloom::SPRITE_HEIGHT;

		// テクスチャはメインレンダリングターゲットのカラーバッファー
		m_luminanceSpriteInitData.m_textures[nsBloom::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();

		// 描き込むレンダリングターゲットのフォーマットを指定する
		m_luminanceSpriteInitData.m_colorBufferFormat[nsBloom::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//作成した初期化情報をもとにスプライトを初期化する
		m_luminanceSprite.Init(m_luminanceSpriteInitData);
	}

	//ガウシアンブラーの初期化メソッド
	void Bloom::GaussianBlurInit()
	{
		//ガウシアンブラーを初期化。
		for (int executionNum = enFirstGaussianBlurNum; executionNum < enMaxGaussianBlurNum; executionNum++)
		{
			m_gaussianBlur[executionNum] = NewGO<GaussianBlur>();
		}
		//gaussianBlur[0]は輝度テクスチャにガウシアンブラーをかける。
		m_gaussianBlur[enFirstGaussianBlurNum]->Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
		//gaussianBlur[1]はgaussianBlur[0]のテクスチャにガウシアンブラーをかける。
		m_gaussianBlur[enSecondGaussianBlurNum]->Init(&m_gaussianBlur[enFirstGaussianBlurNum]->GetBokeTexture());
		//gaussianBlur[2]はgaussianBlur[1]のテクスチャにガウシアンブラーをかける。
		m_gaussianBlur[enThirdGaussianBlurNum]->Init(&m_gaussianBlur[enSecondGaussianBlurNum]->GetBokeTexture());
		//gaussianBlur[3]はgaussianBlur[2]のテクスチャにガウシアンブラーをかける。
		m_gaussianBlur[enFoursGaussianBlurNum]->Init(&m_gaussianBlur[enThirdGaussianBlurNum]->GetBokeTexture());
	}

	//ボケ画像の初期化メソッド
	void Bloom::BokeSpriteInit()
	{
		//ボケ画像を合成して書き込むためのスプライトを初期化。
		//初期化情報を設定する。
		//ボケテクスチャを4枚指定する。
		m_bokeSpriteInitData.m_textures[enFirstGaussianBlurNum] = &m_gaussianBlur[enFirstGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enSecondGaussianBlurNum] = &m_gaussianBlur[enSecondGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enThirdGaussianBlurNum] = &m_gaussianBlur[enThirdGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enFoursGaussianBlurNum] = &m_gaussianBlur[enFoursGaussianBlurNum]->GetBokeTexture();
		//解像度はmainRenderTargetの幅と高さ。
		m_bokeSpriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_bokeSpriteInitData.m_height = nsBloom::SPRITE_HEIGHT;
		//ボケ画像を合成する必要があるので、2D用のシェーダーではなく、専用シェーダーを指定。
		m_bokeSpriteInitData.m_fxFilePath = nsBloom::BLOOM_FX_FILEPATH;
		m_bokeSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
		m_bokeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
		m_bokeSpriteInitData.m_colorBufferFormat[nsBloom::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//初期化情報を元に加算合成用のスプライトを初期化する。
		m_bokeSprite.Init(m_bokeSpriteInitData);
	}
}