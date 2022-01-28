/// <summary>
/// �v���C���[�̃L�����N�^�[�R���g���[���[�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class PlayerCharaCon : public IGameObject
	{
	private:
		CharacterController m_charaCon;		//�L�����N�^�R���g���[���[�N���X���쐬

	public:
		//�L�����R����������
		void InitRigidBody(const Vector3& playerPosition)
		{
			m_charaCon.Init(25.0f, 50.0f, playerPosition);
		}

		//�L�����R����j��
		void RemoveRigidBoby()
		{
			m_charaCon.RemoveRigidBoby();
		}

		/// <summary>
		/// �n�ʂɂ��Ă��邩�ǂ������擾
		/// </summary>
		/// <returns>�n�ʂɂ��Ă��邩�ǂ���</returns>
		bool IsOnGround()const
		{
			return m_charaCon.IsOnGround();
		}

		/// <summary>
		/// �󒆂ɂ��邩�ǂ������擾
		/// </summary>
		/// <returns>�󒆂ɂ��邩�ǂ���</returns>
		bool IsJump()const
		{
			return m_charaCon.IsJump();
		}

		/// <summary>
		/// �L�����N�^�[�R���g���[���[�̍X�V�f�[�^���擾
		/// </summary>
		/// <param name="moveSpeed">�ړ����x</param>
		/// <param name="isHitGround">�n�ʂ̏ォ�ǂ���</param>
		/// <param name="hitGroundNormal">�������Ă���n�ʂɖ@��</param>
		/// <returns>�L�����N�^�[�R���g���[���[�̍X�V�f�[�^</returns>
		Vector3 RigidBodyExecute(Vector3& moveSpeed,bool isHitGround,Vector3& hitGroundNormal)
		{
			return m_charaCon.Execute(
				moveSpeed,
				1.0f,
				isHitGround,
				hitGroundNormal
			);
		}

		/// <summary>
		/// �󒆂ɂ��邩�ǂ������擾
		/// </summary>
		/// <returns>�󒆂ɂ��邩�ǂ���</returns>
		CharacterController* GetCharaCon() { return &m_charaCon; };
	};
}