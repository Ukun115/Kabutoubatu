/// <summary>
/// �����_�����O�G���W���N���X
/// </summary>
#include "stdafx.h"
#include "RenderingEngine.h"
#include "PostEffect.h"

namespace nsKabutoubatu
{
	void RenderingEngine::Init()
	{
		//���C�������_�����O�^�[�Q�b�g��������
		InitMainRenderTarget();
		//ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
		InitZPrepassRenderTarget();
		//�|�X�g�G�t�F�N�g��������
		m_postEffect = NewGO<nsKabutoubatu::PostEffect>();
		m_postEffect->Init(m_mainRenderTarget,m_zprepassRenderTarget);
	}

	//ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
	void RenderingEngine::InitZPrepassRenderTarget()
	{
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

	}

	//���C�������_�����O�^�[�Q�b�g��������
	void RenderingEngine::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::SettingZPrepass(RenderContext& rc)
	{
		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		//ZPrepass����������
		GameObjectManager::GetInstance()->ExecuteZPrepassRender(rc);

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
	}

	//�`�揇
	void RenderingEngine::Execute(RenderContext& rc)
	{
		// �V���h�E�}�b�v�ւ̕`��
		//�V���h�E��`��
		m_postEffect->DrawShadow(rc);

		//�V���G�b�g�`��
		m_postEffect->DrawSilhouette(rc);

		//�~�j�}�b�v�`��
		m_postEffect->DrawMiniMap(rc);

		//ZPrepass��`��
		SettingZPrepass(rc);

		//�����_�[�^�[�Q�b�g�Ƀ��C�������_�[�^�[�Q�b�g���Z�b�g
		m_postEffect->SettingRenderTarget(rc,m_mainRenderTarget);

		//���f����`��
		GameObjectManager::GetInstance()->ExecuteModelRender(rc);

		// �|�X�g�G�t�F�N�g��`��
		m_postEffect->DrawPostEffect(rc, m_mainRenderTarget);

		//�摜��`��
		GameObjectManager::GetInstance()->ExecuteSpriteRender(rc);

		//�t�H���g��`��
		GameObjectManager::GetInstance()->ExecuteFontDataRender(rc);

		//�摜��`��
		GameObjectManager::GetInstance()->ExecuteSpriteRenderSub(rc);

		//�t�H���g��`��
		GameObjectManager::GetInstance()->ExecuteFontDataRenderSub(rc);
	}
}