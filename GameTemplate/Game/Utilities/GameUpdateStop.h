/// <summary>
/// �Q�[���̐������Ԃ̐i�s���~�߂�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class GameUpdateStop : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�ړ��^�[�Q�b�g
		enum enMoveTarget
		{
			enUI,			//UI
			enCameraMove	//�J�����ړ�
		};

		bool m_isStop = false;
		int m_moveTarget = 0;

	public:
		void SetMoveTarget(const int moveTarget) { m_moveTarget = moveTarget; };

		void SetIsStop(const bool isStop) { m_isStop = isStop; };

		/// <summary>
		/// �Q�[�����~�߂Ă��邩�ǂ������擾
		/// </summary>
		/// <returns>�Q�[�����~�߂Ă��邩�ǂ���</returns>
		bool GetIsStop()const { return m_isStop; };

		/// <summary>
		/// �ړ��^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�ړ��^�[�Q�b�g</returns>
		int GetMoveTarget()const { return m_moveTarget; };
	};
}