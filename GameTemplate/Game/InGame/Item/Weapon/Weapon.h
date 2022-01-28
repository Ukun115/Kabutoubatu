/// <summary>
/// ����N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;		//�v���C���[
	class PlayerStatus;	//�v���C���[�̃X�e�[�^�X

	class Weapon : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//����
		enum enWeapon
		{
			enWoodStick,	//�؂̖_
			enSword,		//���ʂ̌�
			enLargeSword,	//�匕
			enBow,			//�|
			enWeaponNum		//����̑���
		};

		Player* m_player = nullptr;		//�v���C���[�������邽�߂̃C���X�^���X
		PlayerStatus* m_playerStatus = nullptr;
		SkinModelRender* m_model = nullptr;		//���f��
		Bone* m_rightHandBone = nullptr;
		Bone* m_leftHandBone = nullptr;

		Vector3 m_pos;	//�ʒu
		Quaternion m_rot;		//��]
		Vector3 m_sca;	//�g�嗦
		int m_playerNum = 0;	//�v���C���[�ԍ�
		const char* m_weaponModelName;
		int m_attackDamage = 0;
		int m_rightHandBoneId = 0;
		int m_leftHandBoneId = 0;
		int m_weaponNum = 0;
		int m_attackJudgeTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Weapon();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

	public:
		//�v���C���[�ԍ����Z�b�g���郁�\�b�h
		void SetPlayerNum(const int plaNum) { m_playerNum = plaNum; };

		//������Z���N�g
		void SetWeaponNum(const int weaponNum) { m_weaponNum = weaponNum; };

		void Init(const int weaponNum);

		/// <summary>
		/// ���킲�Ƃ̃q�b�g�^�C�~���O���擾
		/// </summary>
		/// <returns>���킲�Ƃ̃q�b�g�^�C�~���O</returns>
		int GetAttackJudgeTimer()const { return m_attackJudgeTimer; };
	};
}