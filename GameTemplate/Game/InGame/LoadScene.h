/// <summary>
/// ���[�h��ʃN���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class LoadScene : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�_�摜
		enum enPoint
		{
			enPointOne,		//�_�P��
			enPointTwo,		//�_�Q��
			enPointThree,	//�_�R��
			enPointNum		//�_�̑���
		};

		SpriteRender* m_backScreen = nullptr;
		SpriteRender* m_point[enPointNum] = { nullptr };

		int m_pointActiveTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~LoadScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;
	};
}