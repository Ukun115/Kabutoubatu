/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̃A�j���[�V�����N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class AccompanyAIAnimation : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�A�j���[�V�����N���b�v
		enum enAnimation
		{
			enAnimationClip_idle,		//�A�C�h���A�j���[�V����
			enAnimationClip_shieldIdle,	//����A�C�h���A�j���[�V����
			enAnimationClip_walk,		//�����A�j���[�V����
			enAnimationClip_shieldWalk,	//��������A�j���[�V����
			enAnimationClip_run,		//����A�j���[�V����
			enAnimationClip_attack1,	//�U���A�j���[�V�����P
			enAnimationClip_attack2,	//�U���A�j���[�V�����Q
			enAnimationClip_attack3,	//�U���A�j���[�V�����R
			enAnimationClips_num,		//�A�j���[�V�����̑���
		};

		AnimationClip m_animationClips[enAnimationClips_num];

		int m_animState = enAnimationClip_idle;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

	public:
		/// <summary>
		/// �A�j���[�V�����N���b�v���擾
		/// </summary>
		/// <returns>�A�j���[�V�����N���b�v</returns>
		AnimationClip* GetAnimationClips() { return m_animationClips; };

		/// <summary>
		/// �A�j���[�V�����̑������擾
		/// </summary>
		/// <returns>�A�j���[�V�����̑���</returns>
		int GetAnimationNum()const { return enAnimationClips_num; };

		void ChangeAnimation(const int nextAnimationState) { m_animState = nextAnimationState; };

		/// <summary>
		/// �A�j���[�V�����̏�Ԃ��擾
		/// </summary>
		/// <returns>�A�j���[�V�����̏��</returns>
		int GetAnimationState()const { return m_animState; };
	};
}