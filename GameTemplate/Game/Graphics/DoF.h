/// <summary>
/// ��ʊE�[�x�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class GaussianBlur;		//�K�E�V�A���u���[

	class DoF : public IGameObject
	{
	private:
		/// <summary>
		/// �\����
		/// </summary>

		//��ʊE�[�x�̃I���I�t
		struct stCanDoF
		{
			bool canDoF = false;	//��ʊE�[�x�̃I���I�t�t���O
		};

		GaussianBlur* m_gaussianBlur;		//�K�E�V�A���u���[

		stCanDoF m_canDoF;
		RenderTarget m_depthRenderTarget;	//�[�x�l�̃����_�[�^�[�Q�b�g
		SpriteInitData m_combineBokeImageSpriteInitData;	//�{�P�摜�̃f�[�^
		Sprite m_combineBokeImageSprite;		//�{�P�摜
		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[���邽�߂̉摜�̃f�[�^
		SpriteInitData m_spriteInitData;
		// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[���邽�߂̉摜
		Sprite m_copyToFrameBufferSprite;

	public:
		/// <summary>
		/// ��ʊE�[�x��������
		/// </summary>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		/// <param name="zprepassRenderTarget">Z�v���p�X�̃����_�[�^�[�Q�b�g</param>
		void Init(RenderTarget& mrt, RenderTarget& zprepassRenderTarget);

		/// <summary>
		/// ��ʊE�[�x�̃��[�_�[�^�[�Q�b�g��ݒ�
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void SettingDoFRenderTarget(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// ��ʊE�[�x��`��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void DrawDoF(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// �{�P�摜��������
		/// </summary>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void BokeSpriteInit(RenderTarget& mrt);

		/// <summary>
		/// ��ʊE�[�x�̃I���I�t��ݒ�
		/// </summary>
		/// <param name="canDoF">��ʊE�[�x���I�����ǂ���</param>
		void SetCanDoF(const bool canDoF) { m_canDoF.canDoF = canDoF; };

		/// <summary>
		/// ��ʊE�[�x�e�N�X�`���[���擾
		/// </summary>
		/// <returns>��ʊE�[�x�e�N�X�`���[</returns>
		Texture& GetDoFTexture()
		{
			return m_depthRenderTarget.GetRenderTargetTexture();
		}
	};
}