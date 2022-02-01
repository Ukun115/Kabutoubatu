/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;				//�v���C���[
	class AccompanyAISword;		//���sAI�̌�
	class AccompanyAIShield;	//���sAI�̏�
	class AccompanyAIAnimation;	//���sAI�̃A�j���[�V����

	class AccompanyAI : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayer
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		//�U��̏��
		enum enAttackAndDefenseMode
		{
			enAttackMode,	//�U�����[�h
			enDefenseMode,	//������[�h
		};

		//�A�j���[�V�����N���b�v
		enum enAnimation
		{
			enAnimationClip_idle,		//�A�C�h���A�j���[�V����
			enAnimationClip_shieldIdle,	//����A�C�h���A�j���[�V����
			enAnimationClip_walk,		//�����A�j���[�V����
			enAnimationClip_shieldWalk,	//��������A�j���[�V����
			enAnimationClip_run,		//����A�j���[�V����
			enAnimationClip_attack1,	//�U���A�j���[�V�����P
			enAnimationClip_attack2,	//�U���A�j���[�V�����Q
			enAnimationClip_attack3,	//�U���A�j���[�V�����R
			enAnimationClips_num,		//�A�j���[�V�����̑���
		};

		//�X�e�[�g
		enum enState
		{
			enStopping,			//�Î~���
			enIdle,				//�v���C���[�̋߂��ɗ����Ă�����
			enAccompany,		//���s������
			enShieldDashAttack,	//�V�[���h�œG�������ăv���C���[����q������
			enAttack,			//�U��������
			enReceiveAttack,	//�U�����󂯂����
			enDeath,			//����ł�����
		};

		//���Ƃ����f��
		SkinModelRender* m_otomoModel = nullptr;
		Player* m_player = nullptr;
		AccompanyAIAnimation* m_accompanyAIAnimation = nullptr;
		AccompanyAISword* m_sword = nullptr;
		AccompanyAIShield* m_shield = nullptr;

		CharacterController m_charaCon;		//�L�����N�^�R���g���[���[�N���X���쐬

		float m_playerToMeLength = 0.0f;
		Vector3 m_playerToMeDirection;
		int m_attackComboNo = 0;
		int m_attackAndDefenseMode = enDefenseMode;
		int m_state = enIdle;
		int m_randomTimer = 0;
		Vector3 m_playerPos;
		Vector3 m_circleSetPos;
		int m_loop = 0;
		Vector3 m_pos;	//�ʒu
		Vector3 m_moveSpeed;	//�ړ����x
		float m_rotAngle = 0;	//��]�p�x
		Quaternion m_rot;	//��]
		bool m_isHitGround = false;
		Vector3 m_hitGroundNormal;
		int m_hitPoint = 10;	//�̗�
		int m_randomAngle = 0;
		//�ړ����x���擾���邽�߂̕ϐ�
		Vector3 m_playerLookingDirection;
		int m_attackJumpTimer = 0;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_isPlayerGroundWalk = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~AccompanyAI();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�v���C���[�ɂ�����d�͂�ݒ肷�郁�\�b�h
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		//�����������\�b�h
		void Fall();

		//���Ƃ��̃f�[�^���X�V���郁�\�b�h
		//�ʒu�E��]�E�L�����R���E�A�j���[�V�������
		void DataUpdate();

		//�v���C���[�𒆐S�Ƃ���A���a(������)�̉~����̓_�������_���Ɏ擾���郁�\�b�h
		Vector3 PlayerAroundPos(const float r);

	public:
		void SetNowState(const int state) { m_state = state; };

		void SetShieldPushDashDirection(Vector3& moveSpeed);

		//���Ƃ��̈ʒu���ړ�������
		void SetPosition(const Vector3& position);

		/// <summary>
		/// ���Ƃ��̃X�P���g�����擾
		/// </summary>
		/// <returns>���Ƃ��̃X�P���g��</returns>
		Skeleton* GetSkeleton() { return m_otomoModel->GetSkeleton(); };

		/// <summary>
		/// �ʒu���擾
		/// </summary>
		/// <returns>�ʒu</returns>
		Vector3 GetPosition()const { return m_pos; };

		/// <summary>
		/// �U��̏�Ԃ��擾
		/// </summary>
		/// <returns>�U��̏��</returns>
		int GetMode()const { return m_attackAndDefenseMode; };

		/// <summary>
		/// ��Ԃ��擾
		/// </summary>
		/// <returns>���</returns>
		int GetNowState()const { return m_state; };
	};
}