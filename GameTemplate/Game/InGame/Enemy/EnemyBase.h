/// <summary>
/// �G�̊��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;		//�v���C���[
	class PlayerStatus;	//�v���C���[�X�e�[�^�X
	class AccompanyAI;	//���sAI
	class DropCoin;		//�R�C��
	class PlayerCamera;	//�v���C���[�J����
	class Weapon;		//����

	class EnemyBase : public IGameObject
	{
	private:
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Weapon* m_weapon[2] = { nullptr };

		int m_playerNum = enPlayerNum;
		//�G�ʂ̃v���C���[�̍U���������鋗��
		float m_attackReceiveLength = 0.0f;
		//�G�l�̃v���C���[�Ƃ̋���
		float m_DistanceLength = 0.0f;
		//�G�l�̃v���C���[�Ƒ̂��ڐG���鋗��
		float m_hitBodyLength = 0.0f;
		int m_attackJudgeTimer[2] = { 0 };
		Vector3 m_dir[2];
		bool m_canHitBody = true;
		//�U�����󂯂��I�t���O
		bool m_receiveDamageFlg = false;
		Vector3 m_playerToEnemyVector;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�G�̍U�����󂯂郁�\�b�h
		void AttackReceive();

		//�v���C���[�ƐڐG����ƃ_���[�W��^���鏈��
		void HitBody();

	protected:
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
			enWoodStick,		//�؂̖_
			enSword,			//��
			enLargeSword,		//�匕
			enArow				//�|
		};

		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		SkinModelRender* m_model = nullptr;			//�G���f��
		DropCoin* m_dropCoin = nullptr;				//�R�C���N���X

		CharacterController m_charaCon;				//�L�����N�^�R���g���[���[�N���X

		Vector3 m_pos;	//�ʒu
		Vector3 m_moveSpeed;	//�ړ����x
		bool m_isHitGround;	//�n�ʂɏ���Ă��邩�t���O
		Vector3 m_hitGroundNormal;	//����Ă���n�ʂ̖@��
		float m_rotAngle = 0.0f;	//��]�p�x
		Quaternion m_rot;	//��]
		bool m_isFall = false;
		int m_nowState = 0;		//���݂̏��
		//HP
		int m_hitPoint = 0;
		Vector3 m_cameraTargetPos;
		int m_damageTimer = 0;
		int m_lastAttackPlayer = 0;
		int m_nearPlayer = 0;		//��ԋ߂��v���C���[�̔ԍ�

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~EnemyBase();

	public:

		//��G�ɂ�����d�͂�ݒ肷�郁�\�b�h
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		/// <summary>
		/// �G�ʂ̌Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		virtual void SubStart() {};

		/// <summary>
		/// �G�ʂ̖��t���[�����s�����X�V�����֐�
		/// </summary>
		virtual void SubUpdate(){};

		//�G�ʂ̃v���C���[�̍U���������鋗�����Z�b�g���郁�\�b�h
		void SetAttackReceiveLength(const float attackReceiveLength) { m_attackReceiveLength = attackReceiveLength; };

		//�G�l�̃v���C���[�Ƒ̂��ڐG���鋗�����Z�b�g���郁�\�b�h
		void SetHitBodyLength(const float hitBodyLength) { m_hitBodyLength = hitBodyLength; };

		//�G�l�̈ʒu���Z�b�g���郁�\�b�h
		void SetPosition(const Vector3& position) { m_pos = position; };

		//�ڐG����̃I���I�t�����郁�\�b�h
		void SetCanHitBody(const bool canHitBody) { m_canHitBody = canHitBody; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		void SetState(const int state) { m_nowState = state; };

		/// <summary>
		/// �U�����󂯂����ǂ������擾
		/// </summary>
		/// <returns>�U�����󂯂����ǂ���</returns>
		bool GetReceiveAttackFlg()const { return m_receiveDamageFlg; };

		/// <summary>
		/// �Ō�ɍU�������Ă����v���C���[�ԍ����擾
		/// </summary>
		/// <returns>�Ō�ɍU�������Ă����v���C���[�ԍ�</returns>
		int GetLastKillPlayer()const { return m_lastAttackPlayer; };

		/// <summary>
		/// �v���C���[�̑������擾
		/// </summary>
		/// <returns>�v���C���[�̑���</returns>
		int GetPlayerNum()const { return m_playerNum; };
	};
}