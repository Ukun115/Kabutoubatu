/// <summary>
/// �񕜃A�C�e���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;		//�v���C���[
	class PlayerStatus;	//�v���C���[�̃X�e�[�^�X

	class RecoveryItem : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�񕜃A�C�e���̏��
		enum enRecoveryState
		{
			enRecoverySet,		//�񕜃A�C�e�������������
			enRecoveryDelete,	//�񕜃A�C�e�������܂������
			enRecoveryThrow,	//�����Ă�����
			enRecoveryStay		//�������Ȃ����
		};

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		SkinModelRender* m_model = nullptr;		//���f��
		Player* m_player[enPlayerNum] = { nullptr };		//�v���C���[�������邽�߂̃C���X�^���X
		PlayerStatus* m_playerStatus[enPlayerNum] = {nullptr};

		CharacterController m_charaCon;		//�L�����N�^�R���g���[���[�N���X���쐬

		int m_totalPlayerNum = enPlayerNum;
		Vector3 m_pos;				//�ʒu
		Vector3 m_moveSpeed;		//�ړ����x
		Vector3 m_decreasePower;	//�ړ����x������
		Quaternion m_rot;			//��]
		float m_rotAngle = 0.0f;	//��]�p�x
		Vector3 m_hitGroundNormal;	//����Ă���n�ʂ̖@��
		int m_fallTimer = 0;//�����폜���邽�߂̃^�C�}�[
		int m_playerNum = 0;	//�v���C���[�ԍ�
		int m_recoveryPower = 0;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_isHitGround = false;			//�n�ʂɏ���Ă��邩�̃t���O
		bool m_canPlayerMoveSpeedGet = true;	//�v���C���[�̈ړ����x���Q�b�g�ł��邩�̔���
		bool m_isSoloPlay = false;
		bool m_playerHit = false;


		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~RecoveryItem();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//��G�ɂ�����d�͂�ݒ肷�郁�\�b�h
		void Gravity()
		{
			m_moveSpeed.y -= nsStdafx::GRAVITY;
		}

		//�񕜃A�C�e�����f���̏�����
		void RecoveryItemInit();

		//�X�V�������\�b�h
		void DataUpdate();

	public:
		//�v���C���[�̔ԍ����Z�b�g���郁�\�b�h
		void SetPlayerNum(const int plaNum) { m_playerNum = plaNum; };

		//�񕜗͂��擾
		void SetRecoveryPower(const int recoveryPower) { m_recoveryPower = recoveryPower; };

		void SetIsSoloPlay(bool isSoloPlay) { m_isSoloPlay = isSoloPlay; };

		/// <summary>
		/// �v���C���[�Ƀq�b�g�������ǂ������擾
		/// </summary>
		/// <returns>�v���C���[�Ƀq�b�g�������ǂ���</returns>
		bool GetIsPlayerHit()const { return m_playerHit; };
	};
}