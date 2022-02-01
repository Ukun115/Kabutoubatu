/// <summary>
/// �������̐����f���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PlayerCamera;		//�v���C���[�J����

	class ConfusionStar : public IGameObject
	{
	private:
		SkinModelRender* m_model = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		SoundSource* m_sound = nullptr;

		Vector3 m_pos;		//�ʒu
		float m_rotationAngle = 0.0f;	//��]�p�x
		Quaternion m_rot;	//��]

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~ConfusionStar();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//��]���郁�\�b�h
		void Rotation();

	public:
		//�ʒu���Z�b�g���郁�\�b�h
		void SetPosition(Vector3& position) { m_pos = position; };
	};
}