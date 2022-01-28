/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̌��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class AccompanyAI;	//���sAI

	class AccompanyAISword : public IGameObject
	{
	private:

		/// <summary>
		/// �񋓌^
		/// </summary>

		//�U��̏��
		enum enAttackAndDefenseMode
		{
			enAttackMode,	//�U�����[�h
			enDefenseMode,	//������[�h
		};

		AccompanyAI* m_accompaanyAi = nullptr;
		Bone* m_rightHandBone = nullptr;
		SkinModelRender* m_model = nullptr;		//���f��

		Vector3		 m_pos;		//�ʒu
		Quaternion	 m_rot;		//��]
		Vector3		 m_sca;		//�g�嗦
		int m_rightHandBoneId = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~AccompanyAISword()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;
	};
}