/// <summary>
/// �쐣���u���[���N���X
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

	//�u���[�����������\�b�h
	void Bloom::Init(RenderTarget& mrt)
	{
		// �𑜓x�A�~�b�v�}�b�v���x��
		m_luminnceRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			//�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�P�x�摜��������
		LuminanceSpriteInit(mrt);

		//�K�E�V�A���u���[��������
		GaussianBlurInit();

		//�{�P�摜��������
		BokeSpriteInit();

		// �e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�[
		m_spriteInitData.m_textures[nsBloom::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();
		m_spriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_spriteInitData.m_height = nsBloom::SPRITE_HEIGHT;

		// ���m�N���p�̃V�F�[�_�[���w�肷��
		m_spriteInitData.m_fxFilePath = nsBloom::SPRITE2D_FX_FILEPATH;

		m_copyToFrameBufferSprite.Init(m_spriteInitData);
	}

	//�u���[���`�惁�\�b�h
	void Bloom::DrawBloom(RenderContext& rc, RenderTarget& mrt)
	{
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mrt);

		// �P�x���o
		// �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_luminnceRenderTarget);

		// �P�x���o���s��
		m_luminanceSprite.Draw(rc);

		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

		//�K�E�V�A���u���[��4����s����B
		for (int executionNum = enFirstGaussianBlurNum; executionNum < enMaxGaussianBlurNum; executionNum++)
		{
			m_gaussianBlur[executionNum]->ExecuteOnGPU(rc, nsBloom::BLUR_POWER);
		}

		//4���̃{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z�����B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		rc.WaitUntilToPossibleSetRenderTarget(mrt);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargetAndViewport(mrt);
		//�{�P�摜�����C�������_�[�^�[�Q�b�g�ɍŏI�����B
		m_bokeSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mrt);
	}

	//�P�x�摜�̏��������\�b�h
	void Bloom::LuminanceSpriteInit(RenderTarget& mrt)
	{
		// �P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
		m_luminanceSpriteInitData.m_fxFilePath = nsBloom::BLOOM_FX_FILEPATH;

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		m_luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

		// �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		m_luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";

		// �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
		m_luminanceSpriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_luminanceSpriteInitData.m_height = nsBloom::SPRITE_HEIGHT;

		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
		m_luminanceSpriteInitData.m_textures[nsBloom::FIRST_ARRAY] = &mrt.GetRenderTargetTexture();

		// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		m_luminanceSpriteInitData.m_colorBufferFormat[nsBloom::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//�쐬�����������������ƂɃX�v���C�g������������
		m_luminanceSprite.Init(m_luminanceSpriteInitData);
	}

	//�K�E�V�A���u���[�̏��������\�b�h
	void Bloom::GaussianBlurInit()
	{
		//�K�E�V�A���u���[���������B
		for (int executionNum = enFirstGaussianBlurNum; executionNum < enMaxGaussianBlurNum; executionNum++)
		{
			m_gaussianBlur[executionNum] = NewGO<GaussianBlur>();
		}
		//gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������B
		m_gaussianBlur[enFirstGaussianBlurNum]->Init(&m_luminnceRenderTarget.GetRenderTargetTexture());
		//gaussianBlur[1]��gaussianBlur[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������B
		m_gaussianBlur[enSecondGaussianBlurNum]->Init(&m_gaussianBlur[enFirstGaussianBlurNum]->GetBokeTexture());
		//gaussianBlur[2]��gaussianBlur[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������B
		m_gaussianBlur[enThirdGaussianBlurNum]->Init(&m_gaussianBlur[enSecondGaussianBlurNum]->GetBokeTexture());
		//gaussianBlur[3]��gaussianBlur[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������B
		m_gaussianBlur[enFoursGaussianBlurNum]->Init(&m_gaussianBlur[enThirdGaussianBlurNum]->GetBokeTexture());
	}

	//�{�P�摜�̏��������\�b�h
	void Bloom::BokeSpriteInit()
	{
		//�{�P�摜���������ď������ނ��߂̃X�v���C�g���������B
		//����������ݒ肷��B
		//�{�P�e�N�X�`����4���w�肷��B
		m_bokeSpriteInitData.m_textures[enFirstGaussianBlurNum] = &m_gaussianBlur[enFirstGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enSecondGaussianBlurNum] = &m_gaussianBlur[enSecondGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enThirdGaussianBlurNum] = &m_gaussianBlur[enThirdGaussianBlurNum]->GetBokeTexture();
		m_bokeSpriteInitData.m_textures[enFoursGaussianBlurNum] = &m_gaussianBlur[enFoursGaussianBlurNum]->GetBokeTexture();
		//�𑜓x��mainRenderTarget�̕��ƍ����B
		m_bokeSpriteInitData.m_width = nsBloom::SPRITE_WIDTH;
		m_bokeSpriteInitData.m_height = nsBloom::SPRITE_HEIGHT;
		//�{�P�摜����������K�v������̂ŁA2D�p�̃V�F�[�_�[�ł͂Ȃ��A��p�V�F�[�_�[���w��B
		m_bokeSpriteInitData.m_fxFilePath = nsBloom::BLOOM_FX_FILEPATH;
		m_bokeSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���B
		m_bokeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//�J���[�o�b�t�@�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�B
		m_bokeSpriteInitData.m_colorBufferFormat[nsBloom::FIRST_ARRAY] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//�������������ɉ��Z�����p�̃X�v���C�g������������B
		m_bokeSprite.Init(m_bokeSpriteInitData);
	}
}