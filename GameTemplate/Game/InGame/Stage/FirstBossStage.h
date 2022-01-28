/// <summary>
/// �ŏ��̃{�X�X�e�[�W�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class FirstBoss;		//�ŏ��̃{�X
	class GameScene;		//�Q�[���V�[��
	class GameClearScene;	//�Q�[���N���A�V�[��
	class Fade;				//�t�F�[�h

	class FirstBossStage : public IGameObject
	{
	private:
		SkinModelRender* m_stageModel = nullptr;		//���f��
		Fade* m_fade = nullptr;
		GameScene* m_gameScene = nullptr;
		GameClearScene* m_gameClearScene = nullptr;
		FirstBoss* m_firstBoss = nullptr;	//�{�X�G
		SoundSource* m_firstBossBGM = nullptr;

		PhysicsStaticObject m_physicsStaticObject;		//�ÓI�����I�u�W�F�N�g�N���X

		Vector3 m_pos;
		Quaternion m_rot;
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~FirstBossStage();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}