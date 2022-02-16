/// <summary>
/// �h���V�[���̑I���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class HotelSelectFont;	//�h���V�[���̑I�𕶎�
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class PlayerCamera;		//�v���C���[�̃J����

	class HotelSelect : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�z�e���̃��[�h
		enum enHotelMode
		{
			enStay,		//�h��
			enEnter		//�o��
		};

		//�z�e���̏��
		enum enHotelState
		{
			enSelectState,	//�I�����
			enStayState,	//�h�����
			enEnterState	//�o����
		};

		PlayerStatus* m_playerStatus = nullptr;
		HotelSelectFont* m_hotelSelectFont = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		SoundSource* m_cantDecideSound = nullptr;
		SoundSource* m_buySound = nullptr;

		GamePad* m_playerGamePad;

		int m_nowHotelMode = enStay;	//���܂�A���܂�Ȃ��B�̂Q��
		int m_nowHotelState = enSelectState;
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~HotelSelect();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�A�C�e���V���b�v����o�郁�\�b�h
		void EnterFromHotel();

		//�I�����\�b�h
		void CursorMove(const int selectMin, const int selectMax);

		//���̏�Ԃɍs�����\�b�h
		void NextState();

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// ���݂̃V���b�s���O��Ԃ��擾
		/// </summary>
		/// <returns>���݂̃V���b�s���O���</returns>
		int GetNowSelectState()const { return m_nowHotelState; };

		void SetPlayerGamePad(GamePad& gamePad) { m_playerGamePad = &gamePad; };
	};
}