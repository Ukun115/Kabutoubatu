/// <summary>
/// �f�v�X�V���h�E�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class DepthShadow : public IGameObject
	{
	private:
		RenderTarget m_shadowMap;	//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g

		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		/// <summary>
		/// �f�v�X�V���h�E��������
		/// </summary>
		void Init();

		/// <summary>
		/// �V���h�E�`��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void DrawShadow(RenderContext& rc);

		/// <summary>
		/// �V���h�E�}�b�v�̃e�N�X�`�����擾
		/// </summary>
		/// <returns>�V���h�E�}�b�v�̃e�N�X�`��</returns>
		Texture& GetShadowMapTexture()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}
	};
}