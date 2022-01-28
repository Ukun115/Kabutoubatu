/// <summary>
/// �h���V�[���̑I�𕶎��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class HotelSelectFont : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�z�e���̃��[�h
		enum enHotelMode
		{
			enStay,				//�h��
			enEnter,			//�o��
			enHotelModeNum		//�z�e���̃��[�h�̑���
		};

		//�z�e���̏��
		enum enHotelState
		{
			enSelectState,	//�I�����
			enStayState,	//�h�����
			enEnterState	//�o����
		};

		//���܂锑�܂�Ȃ�����
		FontRender* m_stayOrEnter[enHotelModeNum] = { nullptr };
		//�I���J�[�\������
		FontRender* m_cursorFont = nullptr;
		//�ҋ@������
		FontRender* m_waitFont = nullptr;

		//�t�H���g�̕���
		wchar_t m_text[256];
		//�v���C���[�ԍ�
		int m_playerNum = 0;
		//���݂̏��
		int m_nowHotelState = enSelectState;
		//���݂̃��[�h
		int m_nowHotelMode = 0;
		Vector2 FONT_POS[2] = { {-550.0f,200.0f},{-550.0f,150.0f} };
		Vector2 CURSOR_POS[2] = { {-600.0f,200.0f},{-600.0f,150.0f} };

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~HotelSelectFont();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//���݂̃V���b�s���O��ԃZ�b�g���\�b�h
		void SetNowHotelState(const int nowHotelState) { m_nowHotelState = nowHotelState; };

		//���݂̃V���b�s���O���[�h�Z�b�g���\�b�h
		void SetNowHotelMode(const int nowHotelMode) { m_nowHotelMode = nowHotelMode; };

		//�v���C���[�ԍ��Z�b�g���\�b�h
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}