/// <summary>
/// �g�Q�X���C��(�U�R�G)�N���X
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Needle;	//�g�Q

	class NeedleSlime : public EnemyBase	//�G�l�~�[�̃x�[�X�N���X���p��
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�g�Q�X���C���̏��
		enum enState
		{
			enIdle,			//�A�C�h��
			enWalk,			//����
			enAttack,		//�U��
			enDeath,		//���S
			enAnimationNum	//�A�j���[�V�����̑���
		};

		Needle* m_needle = nullptr;					//�g�Q�N���X

		AnimationClip m_animationClips[enAnimationNum];

		Vector3 m_needleInitPos;
		Vector3 m_playerToEnemyDistanceDirecion[enPlayerNum];	//�v���C���[�ɐL�тĂ���x�N�g��
		int m_randomMoveTimer = 0;	//�����_���ړ��^�C�}�[
		float m_plaToEneDistanceLength[enPlayerNum];			//�v���C���[�Ƃ̋����̒���
		int m_idleTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~NeedleSlime();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void SubUpdate()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// (�ʏ�̍X�V���~�߂�UI�����X�V�����������Ɏg���X�V�����֐�)
		/// </summary>
		void UiUpdate()override final;

		//�X�V�������\�b�h
		void DataUpdate();

		//�����_���ړ��������\�b�h
		void RandomMove();

		//�U���͈͓��ɓ�������t���O�I�����郁�\�b�h
		bool IsAttackRangeIn();

		//�����������\�b�h
		void DistanceSearch();

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