/// <summary>
/// �I�����C�����̍X�V���x�ύX�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class OnlineUpdateSpeed : public IGameObject
	{
	private:
		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~OnlineUpdateSpeed();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update() override final;

		bool m_isOnline = false;
		float m_speed = 1.0f;

	public:
		bool GetIsOnline()const { return m_isOnline; };
		float GetSpeed()const { return m_speed; };
		void SetIsOnline(const bool isOnline) { m_isOnline = isOnline; };
	};
}