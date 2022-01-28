/// <summary>
/// ��ʊE�[�x�N���X
/// </summary>
#include "stdafx.h"
#include "DoF.h"
#include "GaussianBlur.h"

namespace nsKabutoubatu
{
    namespace nsDoF
    {
        //�~�b�v���x��
        const int MIP_LEBEL = 1;
        //�e�N�X�`���z��̃T�C�Y
        const int ARRAY_SIZE = 1;
        //�z��̍ŏ�
        const int FIRST_ARRAY = 0;
        //�摜�̏c��
        const int SPRITE_WIDTH = 1280;
        const int SPRITE_HEIGHT = 720;
        //���f����`�悷�郌���_�[�^�[�Q�b�g�̖���
        const int RENDER_TARGET_NUMBER = 2;
        //�u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B
        const int BLUR_POWER = 5;

        const char* SPRITE2D_FX_FILEPATH = "Assets/shader/2D.fx";
    }

    //DoF�̏��������\�b�h
    void DoF::Init(RenderTarget& mrt, RenderTarget& zprepassRenderTarget)
    {
        //�V�[���̃J������Ԃł�Z�l���������ރ����_�����O�^�[�Q�b�g���쐬�B
        m_depthRenderTarget.Create(
            g_graphicsEngine->GetFrameBufferWidth(),
            g_graphicsEngine->GetFrameBufferHeight(),
            nsDoF::ARRAY_SIZE,
            nsDoF::MIP_LEBEL,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_UNKNOWN
        );

        //�{�P�摜��������
        BokeSpriteInit(mrt);

        // ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
        // �X�v���C�g�̏������I�u�W�F�N�g���쐬����
        // �e�N�X�`����yBlurRenderTarget�̃J���[�o�b�t�@�[
        m_spriteInitData.m_textures[nsDoF::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();
        // �����_�����O�悪�t���[���o�b�t�@�[�Ȃ̂ŁA�𑜓x�̓t���[���o�b�t�@�[�Ɠ���
        m_spriteInitData.m_width = nsDoF::SPRITE_WIDTH;
        m_spriteInitData.m_height = nsDoF::SPRITE_HEIGHT;
        // �{�P�摜�����̂܂ܓ\��t���邾���Ȃ̂ŁA�ʏ��2D�`��̃V�F�[�_�[���w�肷��
        m_spriteInitData.m_fxFilePath = nsDoF::SPRITE2D_FX_FILEPATH;
        // �������I�u�W�F�N�g���g���āA�X�v���C�g������������
        m_copyToFrameBufferSprite.Init(m_spriteInitData);
    }

    //DoF�̃����_�[�^�[�Q�b�g���Z�b�g���郁�\�b�h
    void DoF::SettingDoFRenderTarget(RenderContext& rc, RenderTarget& mrt)
    {
        //2���̃����_�����O�^�[�Q�b�g��ݒ肵�āA���f����`�悷��
        RenderTarget* rts[] = {
            &mrt,
            & m_depthRenderTarget
        };
        rc.WaitUntilToPossibleSetRenderTargets(nsDoF::RENDER_TARGET_NUMBER, rts);
        rc.SetRenderTargetsAndViewport(nsDoF::RENDER_TARGET_NUMBER, rts);
        rc.ClearRenderTargetViews(nsDoF::RENDER_TARGET_NUMBER, rts);
    }

    //DoF�̕`�惁�\�b�h
    void DoF::DrawDoF(RenderContext& rc,RenderTarget& mrt)
    {
        //���C�������_�����O�^�[�Q�b�g�̃{�P�摜���쐬
        m_gaussianBlur->ExecuteOnGPU(rc, nsDoF::BLUR_POWER);
        //�{�P�摜�Ɛ[�x�e�N�X�`���𗘗p���āA�{�P�摜��`������ł���
        rc.WaitUntilToPossibleSetRenderTarget(mrt);
        rc.SetRenderTargetAndViewport(mrt);
        m_combineBokeImageSprite.Draw(rc);
        rc.WaitUntilFinishDrawingToRenderTarget(mrt);

        //�G�t�F�N�g��`��
        EffectEngine::GetInstance()->Draw();

        // ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );

        // �r���[�|�[�g���w�肷��
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

    //�{�P�摜�̏��������\�b�h
    void DoF::BokeSpriteInit(RenderTarget& mrt)
    {
        //�V�[���e�N�X�`�����ڂ������߂̃K�E�V�A���u���[�I�u�W�F�N�g��������
        m_gaussianBlur = NewGO<GaussianBlur>(nsStdafx::PRIORITY_0, nullptr);
        m_gaussianBlur->Init(&mrt.GetRenderTargetTexture());
        //�{�P�摜�����p�̃X�v���C�g������������
        //�g�p����e�N�X�`���͂Q���B
        m_combineBokeImageSpriteInitData.m_textures[0] = &m_gaussianBlur->GetBokeTexture();
        m_combineBokeImageSpriteInitData.m_textures[1] = &m_depthRenderTarget.GetRenderTargetTexture();
        m_combineBokeImageSpriteInitData.m_width = nsDoF::SPRITE_WIDTH;
        m_combineBokeImageSpriteInitData.m_height = nsDoF::SPRITE_HEIGHT;
        //�����p�̃V�F�[�_�[���w�肷��B
        m_combineBokeImageSpriteInitData.m_fxFilePath = "Assets/shader/DoF.fx";
        m_combineBokeImageSpriteInitData.m_colorBufferFormat[nsDoF::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;
        // �����𗘗p���ă{�P�摜���A���t�@�u�����f�B���O����̂ŁA�������������[�h�ɂ���B
        m_combineBokeImageSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
        //�萔�o�b�t�@��DoF�������Ă��������̃t���O��n��
        m_combineBokeImageSpriteInitData.m_expandConstantBuffer = &m_canDoF;
        m_combineBokeImageSpriteInitData.m_expandConstantBufferSize = sizeof(m_canDoF);
        // �������I�u�W�F�N�g�𗘗p���ăX�v���C�g������������B
        m_combineBokeImageSprite.Init(m_combineBokeImageSpriteInitData);
    }
}