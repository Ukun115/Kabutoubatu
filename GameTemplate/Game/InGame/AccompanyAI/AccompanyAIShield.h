/// <summary>
/// �\�����[�h���̓��sAI(�ȍ~�u���Ƃ��v�ƌĂ�)�̏��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class AccompanyAI;	//���sAI

	class AccompanyAIShield	: public IGameObject
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
		SkinModelRender* m_model = nullptr;		//���f��
		Bone* m_leftHandBone = nullptr;

		Vector3		 m_pos;		//�ʒu
		Quaternion	 m_rot;		//��]
		Vector3		 m_sca;		//�g�嗦
		int m_leftHandBoneId = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~AccompanyAIShield()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;
	};
}