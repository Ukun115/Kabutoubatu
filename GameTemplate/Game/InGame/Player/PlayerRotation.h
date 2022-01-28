/// <summary>
/// �v���C���[�̉�]�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;	//�v���C���[

	class PlayerRotation : public IGameObject
	{
	private:
		Player* m_player = nullptr;

		float m_rotAngle = 0.0f;
		Quaternion m_rot;
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//�v���C���[�̉�]�p�x��ς��郁�\�b�h
		void SetRotationAngle(const float rotationAngle) { m_rotAngle = rotationAngle; };

		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// �v���C���[�̉�]���擾
		/// </summary>
		/// <returns>�v���C���[�̉�]</returns>
		Quaternion GetRotation()const { return m_rot; };
	};
}