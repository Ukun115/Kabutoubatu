/// <summary>
/// �X���C��(�U�R�G)�N���X
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Slime : public EnemyBase	//�G�l�~�[�̃x�[�X�N���X���p��
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//���
		enum enState
		{
			enKyoroKyoro,	//�L�����L����
			enJump,			//�W�����v
			enDeath,		//���S
			enAnimationNum	//�A�j���[�V�����̑���
		};

		//�T�E���h
		enum enSlimeSound
		{
			enAttackSound,		//�U���T�E���h
			enSlimeSoundNum		//�T�E���h�̑���
		};

		SoundSource* m_slimeSound[enSlimeSoundNum] = { nullptr };

		AnimationClip m_animationClips[enAnimationNum];

		//�W�����v��
		float m_jumpPower = 7.5f;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_jumpStartFlg = true;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Slime();

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

		//�W�����v���\�b�h
		void Jump();

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