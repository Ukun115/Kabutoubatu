/// <summary>
/// �����_�����O�G���W���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PostEffect;	//�|�X�g�G�t�F�N�g

	class RenderingEngine : public IGameObject
	{
	private:
		// ZPrepass�`��p�̃����_�����O�^�[�Q�b�g
		RenderTarget m_zprepassRenderTarget;
		// ���C�������_�����O�^�[�Q�b�g
		RenderTarget m_mainRenderTarget;
		//�|�X�g�G�t�F�N�g
		PostEffect* m_postEffect;

		/// <summary>
	   /// ZPrepass
	   /// </summary>
	   /// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void SettingZPrepass(RenderContext& rc);

		/// <summary>
		/// ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitZPrepassRenderTarget();

		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();

	public:
		/// <summary>
		/// �����_�����O�p�C�v���C����������
		/// </summary>
		void Init();

		/// <summary>
		/// �����_�����O�p�C�v���C�������s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����擾
		/// </summary>
		/// <returns>ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`��</returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
		}
	};
}