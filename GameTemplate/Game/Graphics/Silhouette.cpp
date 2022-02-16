/// <summary>
/// �V���G�b�g�`��N���X
/// </summary>
#include "stdafx.h"
#include "Silhouette.h"

namespace nsKabutoubatu
{
	namespace nsSilhouette
	{
		//�V���G�b�g�}�b�v�̉𑜓x
		const int SILHOUETTEMAP_WIDTH = 2048;
		const int SILHOUETTEMAP_HEIGHT = 2048;
		//�~�b�v���x��
		const int MIP_LEVEL = 1;
		//�e�N�X�`���z��̃T�C�Y
		const int ARRAY_SIZE = 1;
	}

	//�V���G�b�g�̏��������\�b�h
	void Silhouette::Init()
	{
		//�V���G�b�g�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
		float clearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
		m_silhouetteMap.Create(
			nsSilhouette::SILHOUETTEMAP_WIDTH,
			nsSilhouette::SILHOUETTEMAP_HEIGHT,
			nsSilhouette::MIP_LEVEL,
			nsSilhouette::ARRAY_SIZE,
			//�V���G�b�g�}�b�v�̃J���[�o�b�t�@�̃t�H�[�}�b�g��ύX���Ă���B
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	//�V���G�b�g�`�惁�\�b�h
	void Silhouette::DrawSilhouetto(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g���V���G�b�g�}�b�v�ɕύX����
		rc.WaitUntilToPossibleSetRenderTarget(m_silhouetteMap);
		rc.SetRenderTargetAndViewport(m_silhouetteMap);
		//�����_�����O�^�[�Q�b�g���N���A�ɂ���
		rc.ClearRenderTargetView(m_silhouetteMap.GetRTVCpuDescriptorHandle(), m_silhouetteMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_silhouetteMap.GetDSVCpuDescriptorHandle(), m_silhouetteMap.GetDSVClearValue());
		//�V���G�b�g����������
		GameObjectManager::GetInstance()->ExecuteSilhouetteRender(rc);
		// �������݊����҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_silhouetteMap);
	}
}