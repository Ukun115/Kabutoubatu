/// <summary>
/// �R�C���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Player;		//�v���C���[
	class PlayerStatus;	//�v���C���[�X�e�[�^�X
	class PlayerCamera;	//�v���C���[�J����

	class DropCoin : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		Player* m_player[enPlayerNum] = { nullptr };		//�v���C���[�������邽�߂̃C���X�^���X
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		SkinModelRender* m_coin = nullptr;	//���f��
		PlayerCamera* m_playerCamera = nullptr;
		SoundSource* m_coinSound = nullptr;

		int m_playerNum = enPlayerNum;
		Vector3 m_pos;		//�ʒu
		float m_playerToCoinDistancelength = 0.0f;	//�v���C���[�ƃ��f���Ƃ̋����̒���
		//�R�C���̒l
		int m_coinValue = 100;
		float m_rotAngle = 0.0f;	//��]�p�x
		Quaternion m_rot;		//��]
		float m_boundPower = 0.0f;		//�o�E���h��
		float m_boundStartPosY = 0.0f;	//�o�E���h�J�n�ʒu
		int m_boundPowerDown = 5;	//�o�E���h�͂��������
		//��]�����x
		float m_firstRotSpeed = 30.0f;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_getOkFlg = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~DropCoin();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�R�C�����f���̏��������\�b�h
		void CoinModelInit();

		//�R�C�����o�E���h���鏈�����\�b�h
		void Bound();

		//�R�C������]���鏈�����\�b�h
		void Turn();

		//�R�C���擾�������\�b�h
		void CoinGet();

	public:
		//�R�C���̈ʒu�Z�b�g���\�b�h
		void SetPosition(const Vector3& pos) { m_pos = pos; };

		//�R�C���̒l�Z�b�g���\�b�h
		void SetValue(const int& coinValue) { m_coinValue = coinValue; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };
	};
}