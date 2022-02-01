/// <summary>
/// �v���C���[�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PlayerCamera;		//�v���C���[�̃J����
	class PlayerRotation;	//�v���C���[�̉�]
	class PlayerAnimation;	//�v���C���[�̃A�j���[�V����
	class PlayerSound;		//�v���C���[�̃T�E���h
	class PlayerCharaCon;	//�v���C���[�̃L�����N�^�[�R���g���[���[
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class RecoveryItem;		//�񕜃A�C�e��
	class Arrow;			//��
	class MiniMap;			//�~�j�}�b�v


	class Player : public IGameObject
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

		//�v���C���[�̏��
		enum enPlayerNowState
		{
			enNormal,			//�ʏ�
			enSufferAnAttack,	//�U�����󂯂����
			enGhost,			//�S�[�X�g���
			enDontMove			//�������Ȃ����
		};

		//�񕜃A�C�e���̏��
		enum enRecoveryState
		{
			enRecoverySet,		//�Z�b�g
			enRecoveryDelete,	//�폜
			enRecoveryThrow,	//������
			enRecoveryStay		//�������Ȃ��X�e�[�g
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

		//�d��
		enum enGravity
		{
			enNormalGravity,	//�ʏ�̏d��
			enHalfGravity		//���d��
		};

		SkinModelRender* m_model = nullptr;
		Player* m_otherPlayer = nullptr;
		RecoveryItem* m_recoveryItem = nullptr;
		Arrow* m_arrow = nullptr;
		//�v���C���[�̉�]�N���X
		PlayerRotation* m_playerRotation = nullptr;
		//�v���C���[�̃A�j���[�V�����N���X
		PlayerAnimation* m_playerAnimation = nullptr;
		//�v���C���[�̃T�E���h�N���X
		PlayerSound* m_playerSound = nullptr;
		//�v���C���[�̃L�����R���N���X
		PlayerCharaCon* m_playerCharaCon = nullptr;
		//�v���C���[�̃X�e�[�^�X�N���X
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		GamePad* m_gamePad;
		MiniMap* m_miniMap = nullptr;

		Effect m_fireBallEffect;	//�S�[�X�g��Ԃ̎��̉΂̋ʃG�t�F�N�g

		int m_gravityState = enNormalGravity;
		//1P��2P����\���ԍ�
		int m_playerNum = 0;
		Vector3 m_pos;
		Vector3 m_moveSpeed;
		Vector3 m_friction;		//���C
		int m_nowState = 0;
		Vector3 m_hitGroundNormal;
		int m_recoveryState = enRecoveryStay;
		//���ʃx�N�g��
		Vector3 m_forward;
		//�Ō�ɒ��n���Ă����ꏊ�̃|�W�V����
		Vector3 m_finalOnGroundPos;
		int m_damageTimer = 0;
		int m_fireBallEffectTimer = 0;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_isHitGround;
		bool m_haveRecoveryItem = false;
		//�U�������ǂ����̃t���O
		bool m_isAttackStateFlg = false;
		bool m_nowDoorTatch = false;
		bool m_isSoloPlay = false;
		//��𒆃t���O
		bool m_isAvoidance = false;
		bool m_damageFlg = false;
		bool m_fireBallEffectActive = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Player();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�v���C���[�ɂ�����d�͂�ݒ肷�郁�\�b�h
		void Gravity(const int gravityPower);

		//���C���\�b�h
		void Friction(const float frictionPower);

		//�ʏ�ړ����\�b�h
		void NormalMove();

		//�S�[�X�g��Ԃ̈ړ����\�b�h
		void GhostMove();

		//�W�����v���\�b�h
		void Jump();

		//���
		void Avoidance();

		//�v���C���[�̃f�[�^���X�V���郁�\�b�h
		//�ʒu�E��]�E�L�����R���E�A�j���[�V�������
		void DataUpdate();

		//�񕜏������\�b�h
		void Recovery();

		//�����������\�b�h
		void Fall();

		//�~�j�}�b�v���J�����\�b�h
		void MiniMapOpen();

		//����΂��֐�
		void ShotArrow();

	public:
		//�񕜃A�C�e�������܂����\�b�h
		void RecoveryItemDelete();

		//�v���C���[�ԍ��Z�b�g���\�b�h
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//�v���C���[�̏�ԃZ�b�g���\�b�h
		void SetNowState(const int nowState) { m_nowState = nowState; };

		void SetRecoveryState(const int recoveryState) { m_recoveryState = recoveryState; };

		//�v���C���[�̈ړ����x�Z�b�g���\�b�h
		void SetMoveSpeed(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		//�v���C���[�̈ʒu���ړ�������
		void SetPositionA(const Vector3& position);

		//�h�A���J���Ă��邩�̔����ύX���郁�\�b�h
		void SetNowDoorTatch(const bool nowDoorTatch){ m_nowDoorTatch = nowDoorTatch;};

		//�v���C���[�̃R�C���𑝂₷���\�b�h
		void AddCoin(const int coinValue);

		void SetGamePad(GamePad& gamePad) { m_gamePad = &gamePad; };

		void SetIsSoloPlay(bool isSoloPlay) { m_isSoloPlay = isSoloPlay; };

		void InitDamageTimer()
		{
			m_damageTimer = 0;
			m_damageFlg = true;
		};

		void DamageEffect();

		/// <summary>
		/// �v���C���[�̈ʒu���擾
		/// </summary>
		/// <returns>�v���C���[�̈ʒu</returns>
		Vector3 GetPosition()const { return m_pos; };

		/// <summary>
		/// �v���C���[�̏�Ԃ��擾
		/// </summary>
		/// <returns>�v���C���[�̏��</returns>
		int GetNowState()const { return m_nowState; };

		/// <summary>
		/// �v���C���[�̉񕜃A�C�e���̏�Ԃ��擾
		/// </summary>
		/// <returns>�v���C���[�̉񕜃A�C�e���̏��</returns>
		int GetRecoveryItemState()const { return m_recoveryState; };

		/// <summary>
		/// �v���C���[�̈ړ����x���擾
		/// </summary>
		/// <returns>�v���C���[�̈ړ����x</returns>
		Vector3 GetMoveSpeed()const { return m_moveSpeed; };

		/// <summary>
		/// �v���C���[�̐��ʃx�N�g�����擾
		/// </summary>
		/// <returns>�v���C���[�̐��ʃx�N�g��</returns>
		Vector3 GetForwardVector()const { return m_forward; };

		/// <summary>
		/// �v���C���[�X�P���g�����擾
		/// </summary>
		/// <returns>�v���C���[�X�P���g��</returns>
		Skeleton* GetSkeleton()const { return m_model->GetSkeleton(); };

		/// <summary>
		/// �U�������ǂ������擾
		/// </summary>
		/// <returns>�U�������ǂ���</returns>
		bool IsAttack()const { return m_isAttackStateFlg; };

		/// <summary>
		/// �΂̋ʒ����ǂ������擾
		/// </summary>
		/// <returns>�΂̋ʒ����ǂ���</returns>
		bool GetFireBallActive()const {return m_fireBallEffectActive;};
	};
}