/// <summary>
/// �v���C���[�̃T�E���h�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;	//�v���C���[

	class PlayerSound : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�̃T�E���h
		enum enPlayerSound
		{
			enJumpSound,				//�W�����v��
			enAvoidanceSound,			//�����
			enSwordWingSound,			//�U����
			enRecoverySound,			//�񕜉�
			enRecoveryItemThrowSound,	//�񕜃A�C�e���𓊂��鉹
			enAddCoinSound,				//�R�C���擾��
			enPlayerSoundNum			//�T�E���h�̑���
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

		SoundSource* m_playerSound[enPlayerSoundNum] = { nullptr };

		bool m_attackSoundFlg = false;
		int m_haveWeaponNo = 0;
		int m_attackSoundTimer = 0;
		int m_weaponSoundTiming = 0;

		void Update()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~PlayerSound();

		//�U�����Đ����\�b�h
		void PlayWeaponAttackSound(const int haveWeaponNo);

	public:
		//�W�����v���Đ����\�b�h
		void PlayJumpSound();

		//�񕜉��Đ����\�b�h
		void PlayRecoverySound();

		//�񕜃A�C�e���𓊂��鉹�Đ����\�b�h
		void PlayRecoveryItemThrowSound();

		//�R�C���Q�b�g���Đ����\�b�h
		void PlayAddCoinSound();

		//������Đ����\�b�h
		void PlayAvoidanceSound();

		void SetAttackSoundFlg(const int haveWeaponNo)
		{
			m_attackSoundFlg = true;
			m_attackSoundTimer = 0;
			m_haveWeaponNo = haveWeaponNo;
		};
	};
}