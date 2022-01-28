/// <summary>
/// �v���C���[�J�����̈ړ������ύX�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;		//�v���C���[
	class PlayerCamera;	//�v���C���[�J����

	class CameraMoveDirectionChange : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayer
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		//�J�����̈ړ�����
		enum enCameraMoveDirection
		{
			enX,			//���X�N���[��
			enY,			//�c�X�N���[��
			enZ,			//���X�N���[��
			enShop,			//�h���ƃA�C�e���V���b�v�̌Œ�J����
			enBossStage		//�{�X�X�e�[�W�̌Œ�J����
		};

		//�E�F�C�|�C���g
		enum enNowWayPoint
		{
			enWay1,	//�E�F�C1
			enWay2,	//�E�F�C2
			enWay3,	//�E�F�C3
			enWay4	//�E�F�C4
		};

		Player* m_player = nullptr;		//�v���C���[�������邽�߂̃C���X�^���X
		PlayerCamera* m_playerCamera = nullptr;		//�v���C���[�J�������������邽�߂̃C���X�^���X

		Vector3 m_playerPosition;		//�ʒu
		//���݂̃E�F�C�|�C���g
		int m_nowWayPoint = enWay1;
		int m_targetPlayer = enPlayer1;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~CameraMoveDirectionChange();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�E�F�C�P�̃J�����ݒ�
		void Way1Camera();

		//�E�F�C2�̃J�����ݒ�
		void Way2Camera();

		//�E�F�C3�̃J�����ݒ�
		void Way3Camera();

	public:
		void SetTargetPlayer(int targetPlayer) { m_targetPlayer = targetPlayer; };
	};
}