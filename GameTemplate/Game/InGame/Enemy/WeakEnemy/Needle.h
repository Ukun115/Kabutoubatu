/// <summary>
/// �Ƃ��X���C��(�U�R�G)�̉������U���̂Ƃ��N���X
/// </summary>
#pragma once
#include "../EnemyBase.h"

namespace nsKabutoubatu
{
	class Needle : public EnemyBase	//�G�l�~�[�̃x�[�X�N���X���p��
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�g�Q�̃T�E���h
		enum enNeedleSound
		{
			enAttackSound,		//�U���T�E���h
			enNeedleSoundNum	//�T�E���h�̑���
		};

		SoundSource* m_needleSlimeSound[enNeedleSoundNum] = { nullptr };

		int m_timer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		void SubStart()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Needle();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void SubUpdate()override final;

	public:
		void SetMoveDirection(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		void SetInitPosition(const Vector3& pos) { m_pos = pos; };
	};
}