/// <summary>
/// �Q�[���N���A�V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class GameScene;		//�Q�[���V�[��
	class GameUpdateStop;	//�Q�[���X�V�X�g�b�v

	class GameClearScene : public IGameObject
	{
	private:
		SoundSource* m_gameClearSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		PlayerStatus* m_playerStatus[2] = { nullptr };
		SpriteRenderSub* m_toubatuKannryou = nullptr;
		GameUpdateStop* m_gameUpdateStop = nullptr;
		SpriteRenderSub* m_gameClearSprite = nullptr;
		GameScene* m_gameScene = nullptr;
		FontRenderSub* m_enemyKillNumberFont[2] = { nullptr };
		FontRenderSub* m_coinGetNumberFont[2] = { nullptr };
		FontRenderSub* m_recoveryReceiveNumberFont[2] = { nullptr };
		FontRenderSub* m_deathNumberFont[2] = { nullptr };

		wchar_t m_text[256];
		Vector2 m_fontPos;
		int m_playerNum = 0;
		int m_spriteChangeTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~GameClearScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}