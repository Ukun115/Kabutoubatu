/// <summary>
/// �Q�[���I�[�o�[�V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class GameUpdateStop;	//�Q�[���X�V�X�g�b�v
	class GameScene;		//�Q�[���V�[��

	class GameOverScene : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		GameUpdateStop* m_gameUpdateStop = nullptr;
		GameScene* m_gameScene = nullptr;
		SoundSource* m_gameOverGingle = nullptr;
		SoundSource* m_decideSound = nullptr;
		SpriteRender* m_gameOverSprite = nullptr;
		SpriteRender* m_aPushSprite = nullptr;

		int m_playerNum = enPlayerNum;
		float m_alphaValue = 0.0f;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~GameOverScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}