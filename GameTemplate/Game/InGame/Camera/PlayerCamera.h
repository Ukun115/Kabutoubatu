/// <summary>
/// �v���C���[�J�����N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;						//�v���C���[
	class CameraMoveDirectionChange;	//�J�����ړ������ύX

	class PlayerCamera : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�J�����̈ړ�����
		enum enCameraMoveDirection
		{
			enX,		//���X�N���[��
			enY,		//�c�X�N���[��
			enZ,		//���X�N���[��
			enShop,		//�h���ƃA�C�e���V���b�v�̌Œ�J����
			enBossStage	//�{�X�X�e�[�W�̌Œ�J����
		};

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		/// <summary>
		/// �\����
		/// </summary>

		//�J����
		struct stCamera
		{
			Matrix mCVP;	//�J�����r���[�v���W�F�N�V�����s��
		};

		Player* m_player = nullptr;		//�v���C���[�������邽�߂̃C���X�^���X
		CameraMoveDirectionChange* m_cameraMoveDirectionChange = nullptr;		//�J�����̈ړ�������ς���N���X

		stCamera m_cameraData;		//�J�����\���̂̃f�[�^

		Vector3 m_cameraPos;		//���_
		Vector3 m_cameraTarget;		//�����_
		int m_cameraState = enX;
		float m_cameraTargetXPosition = 0.0f;
		float m_cameraTargetYPosition = 0.0f;
		float m_cameraTargetZPosition = 0.0f;
		int m_targetPlayer = enPlayer1;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~PlayerCamera();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//�J�����̈ړ������Z�b�g���\�b�h
		void SetCameraState(const int& cameraState){m_cameraState = cameraState;};

		//���J�����ړ�������؂�ւ�����ɕK�����̃��\�b�h���ĂсA�Œ肷��ʒu��ݒ肷��
		//���X�N���[���̎��̃J�����ʒu�Z�b�g���\�b�h
		void SetXMoveCameraPos(const float& y, const float& z)
		{
			m_cameraTargetYPosition = y;
			m_cameraTargetZPosition = z;
		};

		//�c�X�N���[���̎��̃J�����ʒu�Z�b�g���\�b�h
		void SetYMoveCameraPos(const float& x, const float& z)
		{
			m_cameraTargetXPosition = x;
			m_cameraTargetZPosition = z;
		};

		//���X�N���[���̎��̃J�����ʒu�Z�b�g���\�b�h
		void SetZMoveCameraPos(const float& x, const float& y)
		{
			m_cameraTargetXPosition = x;
			m_cameraTargetYPosition = y;
		};

		void SetTargetPlayer(const int targetPlayer) { m_targetPlayer = targetPlayer; };

		/// <summary>
		/// �����_���擾
		/// </summary>
		/// <returns>�����_</returns>
		Vector3 GetCameraTarget() { return m_cameraTarget; };

		/// <summary>
		/// �J�����̃f�[�^���擾
		/// </summary>
		/// <returns>�J�����̃f�[�^</returns>
		stCamera GetCamera()const { return m_cameraData; }

		/// <summary>
		/// �J�����̃f�[�^�̃A�h���X���擾
		/// </summary>
		/// <returns>�J�����̃f�[�^�̃A�h���X</returns>
		stCamera* GetCameraAdoress() { return &m_cameraData; }
	};
}