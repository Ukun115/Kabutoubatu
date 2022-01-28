/// <summary>
/// �V���G�b�g�`��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Silhouette : public IGameObject
	{
	private:
		RenderTarget m_silhouetteMap;	//�V���G�b�g�}�b�v�̃����_�����O�^�[�Q�b�g

	public:
		//�V���G�b�g�̏��������\�b�h
		void Init();

		//�V���G�b�g�`�惁�\�b�h
		void DrawSilhouetto(RenderContext& rc);

		/// <summary>
		/// �V���G�b�g�}�b�v�̃e�N�X�`�����擾
		/// </summary>
		/// <returns>�V���G�b�g�}�b�v�̃e�N�X�`��</returns>
		Texture& GetSilhouetteMapTexture()
		{
			return m_silhouetteMap.GetRenderTargetTexture();
		}
	};
}