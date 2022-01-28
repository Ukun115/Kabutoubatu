/// <summary>
/// 被写界深度クラス
/// </summary>
#include "stdafx.h"
#include "DoF.h"
#include "GaussianBlur.h"

namespace nsKabutoubatu
{
    namespace nsDoF
    {
        //ミップレベル
        const int MIP_LEBEL = 1;
        //テクスチャ配列のサイズ
        const int ARRAY_SIZE = 1;
        //配列の最初
        const int FIRST_ARRAY = 0;
        //画像の縦横
        const int SPRITE_WIDTH = 1280;
        const int SPRITE_HEIGHT = 720;
        //モデルを描画するレンダーターゲットの枚数
        const int RENDER_TARGET_NUMBER = 2;
        //ブラーの強さ。値が大きいほどボケが強くなる。
        const int BLUR_POWER = 5;

        const char* SPRITE2D_FX_FILEPATH = "Assets/shader/2D.fx";
    }

    //DoFの初期化メソッド
    void DoF::Init(RenderTarget& mrt, RenderTarget& zprepassRenderTarget)
    {
        //シーンのカメラ空間でのZ値を書きこむレンダリングターゲットを作成。
        m_depthRenderTarget.Create(
            g_graphicsEngine->GetFrameBufferWidth(),
            g_graphicsEngine->GetFrameBufferHeight(),
            nsDoF::ARRAY_SIZE,
            nsDoF::MIP_LEBEL,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_UNKNOWN
        );

        //ボケ画像を初期化
        BokeSpriteInit(mrt);

        // メインレンダリングターゲットの絵をフレームバッファにコピーするためのスプライトを初期化
        // スプライトの初期化オブジェクトを作成する
        // テクスチャはyBlurRenderTargetのカラーバッファー
        m_spriteInitData.m_textures[nsDoF::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();
        // レンダリング先がフレームバッファーなので、解像度はフレームバッファーと同じ
        m_spriteInitData.m_width = nsDoF::SPRITE_WIDTH;
        m_spriteInitData.m_height = nsDoF::SPRITE_HEIGHT;
        // ボケ画像をそのまま貼り付けるだけなので、通常の2D描画のシェーダーを指定する
        m_spriteInitData.m_fxFilePath = nsDoF::SPRITE2D_FX_FILEPATH;
        // 初期化オブジェクトを使って、スプライトを初期化する
        m_copyToFrameBufferSprite.Init(m_spriteInitData);
    }

    //DoFのレンダーターゲットをセットするメソッド
    void DoF::SettingDoFRenderTarget(RenderContext& rc, RenderTarget& mrt)
    {
        //2枚のレンダリングターゲットを設定して、モデルを描画する
        RenderTarget* rts[] = {
            &mrt,
            & m_depthRenderTarget
        };
        rc.WaitUntilToPossibleSetRenderTargets(nsDoF::RENDER_TARGET_NUMBER, rts);
        rc.SetRenderTargetsAndViewport(nsDoF::RENDER_TARGET_NUMBER, rts);
        rc.ClearRenderTargetViews(nsDoF::RENDER_TARGET_NUMBER, rts);
    }

    //DoFの描画メソッド
    void DoF::DrawDoF(RenderContext& rc,RenderTarget& mrt)
    {
        //メインレンダリングターゲットのボケ画像を作成
        m_gaussianBlur->ExecuteOnGPU(rc, nsDoF::BLUR_POWER);
        //ボケ画像と深度テクスチャを利用して、ボケ画像を描きこんでいく
        rc.WaitUntilToPossibleSetRenderTarget(mrt);
        rc.SetRenderTargetAndViewport(mrt);
        m_combineBokeImageSprite.Draw(rc);
        rc.WaitUntilFinishDrawingToRenderTarget(mrt);

        //エフェクトを描画
        EffectEngine::GetInstance()->Draw();

        // メインレンダリングターゲットの絵をフレームバッファーにコピー
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );

        // ビューポートを指定する
        D3D12_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = nsDoF::SPRITE_WIDTH;
        viewport.Height = nsDoF::SPRITE_HEIGHT;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        rc.SetViewportAndScissor(viewport);
        m_copyToFrameBufferSprite.Draw(rc);
    }

    //ボケ画像の初期化メソッド
    void DoF::BokeSpriteInit(RenderTarget& mrt)
    {
        //シーンテクスチャをぼかすためのガウシアンブラーオブジェクトを初期化
        m_gaussianBlur = NewGO<GaussianBlur>(nsStdafx::PRIORITY_0, nullptr);
        m_gaussianBlur->Init(&mrt.GetRenderTargetTexture());
        //ボケ画像合成用のスプライトを初期化する
        //使用するテクスチャは２枚。
        m_combineBokeImageSpriteInitData.m_textures[0] = &m_gaussianBlur->GetBokeTexture();
        m_combineBokeImageSpriteInitData.m_textures[1] = &m_depthRenderTarget.GetRenderTargetTexture();
        m_combineBokeImageSpriteInitData.m_width = nsDoF::SPRITE_WIDTH;
        m_combineBokeImageSpriteInitData.m_height = nsDoF::SPRITE_HEIGHT;
        //合成用のシェーダーを指定する。
        m_combineBokeImageSpriteInitData.m_fxFilePath = "Assets/shader/DoF.fx";
        m_combineBokeImageSpriteInitData.m_colorBufferFormat[nsDoF::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;
        // 距離を利用してボケ画像をアルファブレンディングするので、半透明合成モードにする。
        m_combineBokeImageSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
        //定数バッファにDoFをかけてもいいかのフラグを渡す
        m_combineBokeImageSpriteInitData.m_expandConstantBuffer = &m_canDoF;
        m_combineBokeImageSpriteInitData.m_expandConstantBufferSize = sizeof(m_canDoF);
        // 初期化オブジェクトを利用してスプライトを初期化する。
        m_combineBokeImageSprite.Init(m_combineBokeImageSpriteInitData);
    }
}