/// <summary>
/// �K�E�V�A���u���[�N���X
/// </summary>
#include "stdafx.h"
#include "GaussianBlur.h"


namespace nsKabutoubatu
{
	namespace nsGaussianBlur
	{
		//����
		const int HALF = 2;
		//�~�b�v���x��
		const int MIP_LEVEL = 1;
		//�e�N�X�`���z��̃T�C�Y
		const int ARRAY_SIZE = 1;
		//�z��̍ŏ��̔ԍ�
		const int FIRST_ARRAY = 0;

		const char* const GAUSSIANBLUR_SHADER_FILE_PATH = "Assets/shader/GaussianBlur.fx";
	}

	//�K�E�V�A���u���[�̏��������\�b�h
	void GaussianBlur::Init(Texture* originalTexture)
	{
		m_originalTexture = originalTexture;

		//�����_�����O�^�[�Q�b�g���������B
		InitRenderTargets();
		//�X�v���C�g���������B
		InitSprites();
	}

	// �K�E�V�A���u���[��GPU��Ŏ��s���郁�\�b�h
	void GaussianBlur::ExecuteOnGPU(RenderContext& rc, float blurPower)
	{
		//�d�݃e�[�u�����X�V����B
		UpdateWeightsTable(blurPower);

		//���u���[�����s�B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
		rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		rc.ClearRenderTargetView(m_xBlurRenderTarget);
		//�h���[�B
		m_xBlurSprite.Draw(rc);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

		//�c�u���[�����s�B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���悤�ɂȂ�܂�wait������B
		rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		rc.ClearRenderTargetView(m_yBlurRenderTarget);
		//�h���[�B
		m_yBlurSprite.Draw(rc);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);
	}

	//�����_�[�^�[�Q�b�g�����������郁�\�b�h
	void GaussianBlur::InitRenderTargets()
	{
		//X�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
		m_xBlurRenderTarget.Create(
			m_originalTexture->GetWidth() / nsGaussianBlur::HALF ,
			m_originalTexture->GetHeight(),
			nsGaussianBlur::MIP_LEVEL,
			nsGaussianBlur::ARRAY_SIZE,
			m_originalTexture->GetFormat(),
			DXGI_FORMAT_D32_FLOAT
		);

		//Y�u���[�p�̃����_�����O�^�[�Q�b�g���쐬����B
		m_yBlurRenderTarget.Create(
			m_originalTexture->GetWidth() / nsGaussianBlur::HALF,
			m_originalTexture->GetHeight() / nsGaussianBlur::HALF,
			nsGaussianBlur::MIP_LEVEL,
			nsGaussianBlur::ARRAY_SIZE,
			m_originalTexture->GetFormat(),
			DXGI_FORMAT_D32_FLOAT
		);
	}

	//�c�u���[�A���u���[�̉摜�����������郁�\�b�h
	void GaussianBlur::InitSprites()
	{
		//���u���[�p�̃X�v���C�g������������B
		{
			SpriteInitData xBlurSpriteInitData;
			xBlurSpriteInitData.m_fxFilePath = nsGaussianBlur::GAUSSIANBLUR_SHADER_FILE_PATH;
			xBlurSpriteInitData.m_vsEntryPointFunc = "VSXBlur";
			xBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";
			//�X�v���C�g�̉𑜓x��m_xBlurRenderTarget�Ɠ����B
			xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
			xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
			//�e�N�X�`���͋P�x�e�N�X�`���B
			xBlurSpriteInitData.m_textures[nsGaussianBlur::FIRST_ARRAY] = m_originalTexture;
			//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
			xBlurSpriteInitData.m_colorBufferFormat[nsGaussianBlur::FIRST_ARRAY] = m_xBlurRenderTarget.GetColorBufferFormat();
			//���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
			xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
			xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

			//�������������Ƃɉ��u���[�p�̃X�v���C�g������������B
			m_xBlurSprite.Init(xBlurSpriteInitData);
		}
		//�c�u���[�p�̃X�v���C�g������������B
		{
			SpriteInitData yBlurSpriteInitData;
			yBlurSpriteInitData.m_fxFilePath = nsGaussianBlur::GAUSSIANBLUR_SHADER_FILE_PATH;
			yBlurSpriteInitData.m_vsEntryPointFunc = "VSYBlur";
			yBlurSpriteInitData.m_psEntryPoinFunc = "PSBlur";
			//�X�v���C�g�̉𑜓x��m_yBlurRenderTarget�Ɠ����B
			yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
			yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
			//�e�N�X�`���͉��u���[�����������́B
			yBlurSpriteInitData.m_textures[nsGaussianBlur::FIRST_ARRAY] = &m_xBlurRenderTarget.GetRenderTargetTexture();
			//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
			yBlurSpriteInitData.m_colorBufferFormat[nsGaussianBlur::FIRST_ARRAY] = m_yBlurRenderTarget.GetColorBufferFormat();
			//���[�U�[�g���̒萔�o�b�t�@�Ƀu���[�p�̃p�����[�^�[��ݒ肷��B
			yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
			yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

			//�������������Ƃɏc�u���[�p�̃X�v���C�g������������B
			m_yBlurSprite.Init(yBlurSpriteInitData);
		}
	}

	//�d�݃e�[�u�����X�V���郁�\�b�h
	void GaussianBlur::UpdateWeightsTable(float blurPower)
	{
		float total = 0;
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_weights[i] = expf(-0.5f * (float)(i * i) / blurPower);
			total += 2.0f * m_weights[i];

		}
		// �K�i��
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_weights[i] /= total;
		}
	}
}