/// <summary>
/// �쐣���u���[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class GaussianBlur;	//�K�E�V�A���u���[

	class Bloom : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�K�E�V�A���u���[���������
		enum enGaussianBlurNum
		{
			enFirstGaussianBlurNum,		//�P���
			enSecondGaussianBlurNum,	//�Q���
			enThirdGaussianBlurNum,		//�R���
			enFoursGaussianBlurNum,		//�S���
			enMaxGaussianBlurNum		//�K�E�V�A���u���[���������
		};

		GaussianBlur* m_gaussianBlur[enMaxGaussianBlurNum];	//�K�E�V�A���u���[

		RenderTarget m_luminnceRenderTarget;	// �P�x���o�p�̃����_�����O�^�[�Q�b�g
		SpriteInitData m_luminanceSpriteInitData;//�P�x�摜�̃f�[�^
		Sprite m_luminanceSprite;				//�P�x�摜
		SpriteInitData m_bokeSpriteInitData;	//�{�P�摜�̃f�[�^
		Sprite m_bokeSprite;		//�{�P�摜
		// mainRenderTarget�̃e�N�X�`�����t���[���o�b�t�@�[�ɓ\��t���邽�߂̉摜�̃f�[�^
		SpriteInitData m_spriteInitData;
		// mainRenderTarget�̃e�N�X�`�����t���[���o�b�t�@�[�ɓ\��t���邽�߂̉摜
		Sprite m_copyToFrameBufferSprite;

	public:
		/// <summary>
		/// �u���[����������
		/// </summary>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void Init(RenderTarget& mrt);

		//�P�x�摜�̏��������\�b�h
		/// <summary>
		/// �P�x�摜��������
		/// </summary>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void LuminanceSpriteInit(RenderTarget& mrt);

		/// <summary>
		/// �K�E�V�A���u���[��������
		/// </summary>
		void GaussianBlurInit();

		/// <summary>
		///	�{�P�摜��������
		/// </summary>
		void BokeSpriteInit();

		/// <summary>
		/// �u���[���`��
		/// </summary>
		/// <param name="rt">�����_�[�^�[�Q�b�g</param>
		/// <param name="mrt">���C�������_�[�^�[�Q�b�g</param>
		void DrawBloom(RenderContext& rc, RenderTarget& mrt);

		/// <summary>
		/// �P�x���o�p�̃����_�[�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�P�x���o�p�̃����_�[�^�[�Q�b�g</returns>
		RenderTarget& GetLuminnceRenderTarget()
		{
			return m_luminnceRenderTarget;
		}
	};
}