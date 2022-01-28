/// <summary>
/// �|�X�g�G�t�F�N�g�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class DepthShadow;	//�f�v�X�V���h�E
	class Bloom;		//�u���[��
	class DoF;			//��ʊE�[�x
	class Silhouette;	//�V���G�b�g�`��
	class MiniMap;		//�~�j�}�b�v

	class PostEffect : public IGameObject
	{
	private:
		DepthShadow* m_depthShadow;		//�f�v�X�V���h�E
		Bloom* m_bloom;					//�쐣���u���[��
		DoF* m_dof;						//��ʊE�[�x
		Silhouette* m_silhouette;		//�V���G�b�g
		MiniMap* m_miniMap;				//�~�j�}�b�v

	public:
		//�|�X�g�G�t�F�N�g�̏��������\�b�h
		void Init(RenderTarget& mainRenderTarget , RenderTarget& zprepassRenderTarget);

		//�V���h�E��`��
		void DrawShadow(RenderContext& rc);

		//�V���G�b�g��`��
		void DrawSilhouette(RenderContext& rc);

		//�~�j�}�b�v��`�悷�郁�\�b�h
		void DrawMiniMap(RenderContext& rc);

		//�����_�[�^�[�Q�b�g�Ƀ��C�������_�[�^�[�Q�b�g���Z�b�g���郁�\�b�h
		void SettingRenderTarget(RenderContext& rc,RenderTarget& mainRenderTarget);

		//�|�X�g�G�t�F�N�g��`�搔���郁�\�b�h
		void DrawPostEffect(RenderContext& rc , RenderTarget& mainRenderTarget);
	};
}