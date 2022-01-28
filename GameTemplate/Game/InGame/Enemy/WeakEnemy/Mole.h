/// <summary>
/// ���O��(�U�R�G)�N���X
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Mole : public EnemyBase	//�G�l�~�[�̃x�[�X�N���X���p��
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//���O���̏��
		enum enMoleNowState
		{
			enPlayerSearchAndRandomMove,	//�߂��̃v���C���[��T���Ă���&�����_���ړ����
			enAttack,						//�ːi�U�����Ă�����
			enBlowAway,						//�v���C���[�𐁂���΂��Ă�����
			enDeath,						//���S���
			enMoleNowStateNum				//�v���C���[�̏�Ԃ̑�����
		};

		//�A�j���[�V����
		enum enMoleAnimation
		{
			enWalkAnimation,	//����
			enTackleAnimation,	//�^�b�N��
			enWinAnimation,		//����
			enDeathAnimation,	//���S�A�j���[�V����
			enAnimationNum		//�A�j���[�V�����̑���
		};

		//�T�E���h
		enum enMoleSound
		{
			enAttackSound,	//�U���T�E���h
			enMoleSoundNum	//�T�E���h�̑���
		};

		SkinModelRender* m_exclamationMark = nullptr;	//�v���C���[�𔭌������Ƃ��̃G�N�X�N�����[�V�����}�[�N���f��
		SoundSource* m_moleSound[enMoleSoundNum] = { nullptr };

		AnimationClip m_animationClips[enAnimationNum];

		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//�v���C���[�ɐL�тĂ���x�N�g��
		Vector3 m_playerToEnemyDistanceDirecion2[enPlayerNum];	//�v���C���[�ɐL�тĂ���x�N�g��
		float m_plaToEneDistanceLength[enPlayerNum];			//�v���C���[�Ƃ̋����̒���
		int m_attackTimer = 0;		//�U���̃^�C�}�[
		int m_attackDelayTimer = 0;	//�U���O�̍U����x�点��^�C�}�[
		int m_randomMoveTimer = 0;	//�����_���ړ��^�C�}�[
		Vector3 m_exclamationMarkPos;	//�G�N�X�N�����[�V�����}�[�N�̈ʒu
		int m_blowAwayPlayer = 0;		//������΂��v���C���[�̔ԍ�
		int m_blowAwayAfterWaitTimer = 0;	//������΂���̍d���^�C�}�[
		//���ʃx�N�g��
		Vector3 m_forward;
		int m_randomDelayTimer = 0;
		Vector3 m_finalOnGroundPos;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Mole();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void SubUpdate()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�����_���ړ��������\�b�h
		void RandomMove();

		//�����������\�b�h
		void DistanceSearch();

		//�U��(�ːi)�������\�b�h
		void Attack();

		//�X�V�������\�b�h
		void DataUpdate();

		//�G�N�X�N�����[�V�����}�[�N�̕\���������\�b�h
		void ExclamationMarkActive();

		//��G�̐�����΂�������n���������\�b�h
		void PassBlowAwayDirection();

		//�G�̏������������郁�\�b�h
		void EnemyInit();

		//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
		bool IsAttackRangeIn();

	public:
		//�����ʒu���w�肷��Z�b�g���\�b�h
		void SetInitPosition(const Vector3& position) { m_pos = position; };

		/// <summary>
		/// �����������ǂ������擾
		/// </summary>
		/// <returns>�����������ǂ���</returns>
		bool GetFall()const { return m_isFall; };
	};
}