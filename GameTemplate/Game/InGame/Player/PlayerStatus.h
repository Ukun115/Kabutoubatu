/// <summary>
/// �v���C���[�̃X�e�[�^�X�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PlayerSound;	//�v���C���[�̃T�E���h

	class PlayerStatus : public IGameObject
	{
	private:
		PlayerSound* m_playerSound = nullptr;

		//�����R�C��
		int m_possessionCoin = 1000;
		//���擾�R�C����
		int m_totalGetCoin = 0;
		//�̗�
		int m_hitPoint = 10;
		//�ő�̗�
		int m_maxHitPoint = 10;
		//�����񕜐�
		int m_recoveryItemNum = 10;
		//�R�C���̎擾�{��
		float m_addMoneyMagnification = 1.0f;
		//��������ԍ�
		int m_haveWeapon = 0;
		//�ړ���
		float m_movePower = 40.0f;
		//�W�����v��
		float m_jumpPower = 7.5f;
		//�񕜗�
		int m_recoveryPower = 1;
		//�񕜂��Ă�������ʂ̑���
		int m_recoveryReceiveNum = 0;
		//�����S��
		int m_deathNum = 0;
		//�L����
		int m_enemyKillNum = 0;
		//�v���C���[�ԍ�
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~PlayerStatus();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		/**
		* @brief �Z�b�^�[
		*/
		//�v���C���[�ԍ����Z�b�g���郁�\�b�h
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		//���񕜂��Ă�������ʂ�ۑ����郁�\�b�h
		void AddRecoveryReceiveNum(const int addValue) { m_recoveryReceiveNum += addValue; };

		//�G��|�������𑝂₷���\�b�h
		void AddEnemyKillNum() { m_enemyKillNum++; };

		//�v���C���[�Ƀ_���[�W��^���郁�\�b�h
		void ReduceHitPoint(const int reduceNum)
		{
			m_hitPoint -= reduceNum;
		};

		//�v���C���[���񕜂����郁�\�b�h
		void AddHitPoint(const int addValue);

		//�q�b�g�|�C���g�̐��l���w�肵�ĕύX���郁�\�b�h
		void SetHitPoint(const int hitPoint) { m_hitPoint = hitPoint; };

		//�v���C���[�̃R�C���𑝂₷���\�b�h
		void AddCoin(const int coinValue);

		//�f�X�����P���₷���\�b�h
		void AddDeathNum() { m_deathNum++; };

		//�񕜃A�C�e���̐����P���炷
		void ReduceRecoveryItemNum() { m_recoveryItemNum--; };

		/**
		* @brief �w��
		*/
		//�v���C���[�̏����������炷���\�b�h
		void ReduceMoney(const int reduceNum) { m_possessionCoin -= reduceNum; };

		//�v���C���[�̏������𑝂₷���\�b�h
		void AddMoney(const int addValue) { m_possessionCoin += addValue; };

		//�v���C���[�̃R�C���𑝂₷�{���𑝂₷���\�b�h
		void AddMoneyMagnification() { m_addMoneyMagnification += 0.1f; };

		//�v���C���[�̈ړ��͂��A�b�v�����郁�\�b�h
		void AddMovePower() { m_movePower += 5.0f; };

		//�v���C���[�̃W�����v�͂��A�b�v�����郁�\�b�h
		void AddJumpPower() { m_jumpPower += 1.0f; };

		//�񕜃A�C�e���̐��𑝂₷���\�b�h
		void AddRecoveryItemNum() { m_recoveryItemNum++; };

		//�񕜗͂��A�b�v�����郁�\�b�h
		void AddRecoveryPower() { m_recoveryPower++; };

		//�����Ă��镐���ύX���郁�\�b�h
		void ChangeHavingWeapon(int weaponNum) { m_haveWeapon = weaponNum; };

		/**
		*
		* @brief ���p
		*/
		//�v���C���[�̃R�C���𑝂₷�{�������炷���\�b�h
		void ReduceMoneyMagnification() { m_addMoneyMagnification -= 0.1f; };

		//�v���C���[�̈ړ��͂������郁�\�b�h
		void ReduceMovePower() { m_movePower -= 5.0f; };

		//�v���C���[�̃W�����v�͂������郁�\�b�h
		void ReduceJumpPower() { m_jumpPower -= 1.0f; };

		//�񕜗͂������郁�\�b�h
		void ReduceRecoveryPower() { m_recoveryPower--; };

		/// <summary>
		/// �v���C���[�̑����S�񐔂��擾
		/// </summary>
		/// <returns>�v���C���[�̑����S��</returns>
		int GetDeathNum()const { return m_deathNum; };

		/// <summary>
		/// �v���C���[�̑��񕜂��Ă�������ʂ��擾
		/// </summary>
		/// <returns>�v���C���[�̑��񕜂��Ă��������</returns>
		int GetRecoveryReceiveNum()const { return m_recoveryReceiveNum; };

		/// <summary>
		/// �v���C���[�̃R�C�����������擾
		/// </summary>
		/// <returns>�v���C���[�̃R�C��������</returns>
		int GetCoinNum()const { return m_possessionCoin; };

		/// <summary>
		/// �v���C���[�̑��擾�R�C�������擾
		/// </summary>
		/// <returns>�v���C���[�̑��擾�R�C����</returns>
		int GetTotalCoinNum()const { return m_totalGetCoin; };

		/// <summary>
		/// �G��|���������擾
		/// </summary>
		/// <returns>�G��|������</returns>
		int GetEnemyKillNum()const { return m_enemyKillNum; };

		/// <summary>
		/// �����Ă���񕜃A�C�e���̑������擾
		/// </summary>
		/// <returns>�����Ă���񕜃A�C�e���̑���</returns>
		int GetRecoveryItemNum()const { return m_recoveryItemNum; };

		/// <summary>
		/// �v���C���[��HP���擾
		/// </summary>
		/// <returns>�v���C���[��HP</returns>
		int GetHitPoint()const { return m_hitPoint; };

		/// <summary>
		/// �v���C���[�̍ő�̗͂��擾
		/// </summary>
		/// <returns>�v���C���[�̍ő�̗�</returns>
		int GetMaxHitPoint()const { return m_maxHitPoint; };

		/// <summary>
		/// �v���C���[�̌��ݎ����Ă��镐����擾
		/// </summary>
		/// <returns>�v���C���[�̌��ݎ����Ă��镐��</returns>
		int GetHaveWeapon()const { return m_haveWeapon; };

		/// <summary>
		/// �v���C���[�̈ړ��͂��擾
		/// </summary>
		/// <returns>�v���C���[�̈ړ���</returns>
		float GetMovePower()const { return m_movePower; };

		/// <summary>
		/// �v���C���[�̃W�����v�͂��擾
		/// </summary>
		/// <returns>�v���C���[�̃W�����v��</returns>
		float GetJumpPower()const { return m_jumpPower; };

		/// <summary>
		/// �v���C���[�̉񕜂��Ă�����͂��擾
		/// </summary>
		/// <returns>�v���C���[�̉񕜂��Ă������</returns>
		int GetRecoveryPower()const { return m_recoveryPower; };
	};
}