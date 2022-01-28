/// <summary>
/// �v���C���[�̃A�j���[�V�����N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;			//�v���C���[
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class PlayerCharaCon;	//�v���C���[�̃L�����N�^�[�R���g���[���[

	/// <summary>
	/// �񋓌^
	/// </summary>

	//�A�j���[�V�����N���b�v
	enum enPlayerAnimation
	{
		enAnimationClip_idle,				//�_�����A�j���[�V����
		enAnimationClip_run,				//����A�j���[�V����
		enAnimationClip_walk,				//�����A�j���[�V����
		enAnimationClip_jump,				//�W�����v�A�j���[�V����
		enAnimationClip_attack,				//�U���A�j���[�V����
		enAnimationClip_down,				//���ʂƂ��̃A�j���[�V����
		enAnimationClip_rollingAvoidance,	//�]�������A�j���[�V����
		enAnimationClips_swordRun,			//���ʂ̌��������Ă���Ƃ��̑���A�j���[�V����
		enAnimationClips_swordAttack,		//���ʂ̌��������Ă���Ƃ��̍U���A�j���[�V����
		enAnimationClips_largeSwordRun,		//�匕�������Ă���Ƃ��̑���A�j���[�V����
		enAnimationClips_largeSwordAttack,	//�匕�������Ă���Ƃ��̍U���A�j���[�V����
		enAnimationClips_bowRun,			//�|�������Ă���Ƃ��̑���A�j���[�V����
		enAnimationClips_bowAttack,			//�|�������Ă���Ƃ��̍U���A�j���[�V����
		enAnimationClips_num,				//�A�j���[�V�����̑���
	};

	class PlayerAnimation : public IGameObject
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

		//����
		enum enWeapon
		{
			enWoodStick,	//�؂̖_
			enSword,		//���ʂ̌�
			enLargeSword,	//�匕
			enBow,			//�|
			enWeaponNum		//����̑���
		};

		Player* m_player = nullptr;
		PlayerStatus* m_playerStatus = nullptr;
		PlayerCharaCon* m_playerCharaCon = nullptr;

		AnimationClip m_animationClips[enAnimationClips_num];

		int m_animState = enAnimationClip_idle;
		int m_playerNum = 0;
		char m_animationFilePath[256];

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

	public:
		//�ʏ�ړ���Ԏ��̃A�j���[�V�����������\�b�h
		void NormalStateAnimation();

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//�v���C���[�̃A�j���[�V������ύX���郁�\�b�h
		void SetAnimation(const int animationState) { m_animState = animationState; };

		void ChangeAnimation(const int nextAnimationState) { m_animState = nextAnimationState; };

		/// <summary>
		/// �v���C���[�̃A�j���[�V������Ԃ��擾
		/// </summary>
		/// <returns>�v���C���[�̃A�j���[�V�������</returns>
		int GetAnimationState()const { return m_animState; };

		/// <summary>
		/// �v���C���[�̃A�j���[�V�����N���b�v���擾
		/// </summary>
		/// <returns>�v���C���[�̃A�j���[�V�����N���b�v</returns>
		AnimationClip* GetAnimationClips() { return m_animationClips; };

		/// <summary>
		/// �v���C���[�̃A�j���[�V�����������擾
		/// </summary>
		/// <returns>�v���C���[�̃A�j���[�V��������</returns>
		int GetAnimationNum()const { return enAnimationClips_num; };
	};
}