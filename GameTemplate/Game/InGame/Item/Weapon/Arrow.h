/// <summary>
/// ��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class Arrow : public IGameObject
	{
	private:
		SkinModelRender* m_arrowModel = nullptr;	//���f��

		Vector3 m_pos;			//�ʒu
		Quaternion m_rot;		//��]
		Vector3 m_moveSpeed;	//���˂�������(�ړ����x)
		int m_playerNum = 0;
		int m_deleteTimer = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Arrow();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//���˂̊J�n�ʒu���Z�b�g����֐�
		void SetShotPosition(const Vector3& shotPos) { m_pos = shotPos; };

		void SetMoveDirection(const Vector3& moveSpeed) { m_moveSpeed = moveSpeed; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// ��̈ʒu���擾
		/// </summary>
		/// <returns>��̈ʒu</returns>
		Vector3 GetPosition()const { return m_pos; };
	};
}