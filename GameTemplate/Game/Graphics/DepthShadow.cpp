/// <summary>
/// �f�v�X�V���h�E�N���X
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

	//�V���h�E�̏��������\�b�h
	void DepthShadow::Init()
	{
		//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
		m_shadowMap.Create(
			nsDepthShadow::WIDTH,
			nsDepthShadow::HEIGHT,
			nsDepthShadow::MIP_LEBEL,
			nsDepthShadow::ARRAY_SIZE,
			//�V���h�E�}�b�v�̃J���[�o�b�t�@�̃t�H�[�}�b�g��ύX���Ă���B
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//�V���h�E�`�惁�\�b�h
	void DepthShadow::DrawShadow(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		//�����_�����O�^�[�Q�b�g���N���A�ɂ���
		rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());
		//�V���h�E����������
		GameObjectManager::GetInstance()->ExecuteShadowRender(rc);
		// �������݊����҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	}
}