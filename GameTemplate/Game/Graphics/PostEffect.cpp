/// <summary>
/// �|�X�g�G�t�F�N�g�N���X
/// </summary>
#include "stdafx.h"
#include "PostEffect.h"
#include "DepthShadow.h"	//�f�v�X�V���h�E
#include "Bloom.h"			//�u���[��
#include "DoF.h"			//��ʊE�[�x
#include "Silhouette.h"		//�V���G�b�g
#include "MiniMap.h"		//�~�j�}�b�v

namespace nsKabutoubatu
{
	//�|�X�g�G�t�F�N�g�̏��������\�b�h
	void PostEffect::Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
	{
		//�|�X�g�G�t�F�N�g�̂��ꂼ��̃C���X�^���X�𐶐�
		m_depthShadow = NewGO<DepthShadow>(nsStdafx::PRIORITY_0,nsStdafx::DEPTHSHADOW_NAME);
		m_bloom = NewGO<Bloom>();
		m_dof = NewGO<DoF>(nsStdafx::PRIORITY_0, nsStdafx::DOF_NAME);
		m_silhouette = NewGO<Silhouette>(nsStdafx::PRIORITY_0, nsStdafx::SILHOUETTE_NAME);
		m_miniMap = NewGO<MiniMap>(nsStdafx::PRIORITY_9, nsStdafx::MINIMAP_NAME);

		//�f�v�X�V���h�E��������
		m_depthShadow->Init();
		//�u���[����������
		m_bloom->Init(mainRenderTarget);
		//��ʊE�[�x��������
		m_dof->Init(mainRenderTarget, zprepassRenderTarget);
		//�V���G�b�g��������
		m_silhouette->Init();
		//�~�j�}�b�v��������
		m_miniMap->Init();
	}

	//�V���h�E��`�悷�郁�\�b�h
	void PostEffect::DrawShadow(RenderContext& rc)
	{
		m_depthShadow->DrawShadow(rc);
	}

	//�V���G�b�g��`�悷�郁�\�b�h
	void PostEffect::DrawSilhouette(RenderContext& rc)
	{
		m_silhouette->DrawSilhouetto(rc);
	}

	//�~�j�}�b�v��`�悷�郁�\�b�h
	void PostEffect::DrawMiniMap(RenderContext& rc)
	{
		m_miniMap->DrawMiniMap(rc);
	}

	//�����_�[�^�[�Q�b�g�Ƀ��C�������_�[�^�[�Q�b�g���Z�b�g���郁�\�b�h
	void PostEffect::SettingRenderTarget(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		// �����_�����O�^�[�Q�b�g��m_mainRenderTarget�ɕύX����
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(mainRenderTarget);

		//��ʊE�[�x�̃����_�[�^�[�Q�b�g���Z�b�g
		//��ʊE�[�x���ڂ��Ȃ��悤�ɂ��邽�߁A���L���R�����g�A�E�g���Ă��܂��B
		//m_dof->SettingDoFRenderTarget(rc, mainRenderTarget);
	}


	//�|�X�g�G�t�F�N�g��`�搔���郁�\�b�h
	void PostEffect::DrawPostEffect(RenderContext& rc,RenderTarget& mainRenderTarget)
	{
		//�u���[����`��
		m_bloom->DrawBloom(rc, mainRenderTarget);
		//��ʊE�[�x��`��
		m_dof->DrawDoF(rc, mainRenderTarget);
	}
}