/// <summary>
/// �~�j�}�b�v�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class MiniMap : public IGameObject
	{
	private:
		RenderTarget m_miniMap;	//�~�j�}�b�v�̃����_�����O�^�[�Q�b�g
		Camera m_miniMapCamera;		//�~�j�}�b�v�J����
		SpriteInitData m_miniMapSpriteInitData;
		Sprite m_miniMapSprite;
		SpriteRender* m_miniMapBack = nullptr;

		Vector3 m_cameraPos;			//���_
		Vector3 m_cameraTargetPos;		//�����_

		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		//�`��֐�
		void SpriteDataRender(RenderContext& renderContext);

	public:
		//�~�j�}�b�v�̏��������\�b�h
		void Init();

		//�~�j�}�b�v�`�惁�\�b�h
		void DrawMiniMap(RenderContext& rc);

		/// <summary>
		/// �~�j�}�b�v�̃e�N�X�`�����擾
		/// </summary>
		/// <returns>�~�j�}�b�v�̃e�N�X�`��</returns>
		Texture& GetMiniMapTexture()
		{
			return m_miniMap.GetRenderTargetTexture();
		}

		/// <summary>
		/// �~�j�}�b�v�J�����̃A�h���X���擾
		/// </summary>
		/// <returns>�~�j�}�b�v�J�����̃A�h���X</returns>
		Camera* GetMiniMapCamera() { return &m_miniMapCamera; }
	};
}