/// <summary>
/// �|�[�Y�@�\�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Pause : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayer
		{
			enPlayer1,			//�PP
			enPlayer2,			//�QP
			enTotalPlayerNum	//�v���C���[�̑���
		};

		SpriteRenderSub* m_pauseSprite = nullptr;	//�|�[�Y�摜
		SpriteRenderSub* m_grayBack = nullptr;		//�|�[�Y���̔w�i�摜

		bool m_isPauseFlg = false;		//�|�[�Y�����ǂ����t���O
		bool m_canPause = false;			//�|�[�Y�@�\�����������g���邩�ǂ����t���O

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Pause();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update() override final;

		//�|�[�Y�����̃A�b�v�f�[�g�����Ă΂�Ȃ��Ȃ�
		void PauseUpdate()override final;

	public:
		void SetPause(bool isPauseFlg) { m_isPauseFlg = isPauseFlg; };

		void SetCanPause(bool canPause) { m_canPause = canPause; };

		/// <summary>
		/// �|�[�Y�����ǂ������擾
		/// </summary>
		/// <returns>�|�[�Y�����ǂ���</returns>
		bool GetIsPause()const { return m_isPauseFlg; };
	};
}