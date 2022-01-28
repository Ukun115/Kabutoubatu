/// <summary>
/// �A�C�e���V���b�v�V�[���̑I�𕶎��N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	class ItemShopSelectFont : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�V���b�v�̃��[�h
		enum enShoppingMode
		{
			enBuy,				//����
			enSold,				//����
			enEnter,			//�o��
			enShoppingModeNum	//�V���b�v�̃��[�h����
		};

		//�V���b�v�̏��
		enum enShoppingState
		{
			enShoppingModeSelectState,	//��������o���I��ł�����
			enBuyState,					//�������
			enSoldState,				//������
			enEnterState				//�o����
		};

		//�w���I�����
		enum enBuySelectState
		{
			enFirstItem,			//1�ڂ̃A�C�e��
			enSecondItem,			//�Q�ڂ̃A�C�e��
			enThirdItem,			//�R�ڂ̃A�C�e��
			enBackState,			//�߂�
			enBuySelectStateNum		//�w���I����Ԃ̑���
		};

		//�X�e�[�^�X�㏸�A�C�e��
		enum enStatusUpItemSelect
		{
			enStatusUpItem1,	//�X�e�[�^�X�㏸�A�C�e���P��
			enStatusUpItem2,	//�X�e�[�^�X�㏸�A�C�e���Q��
			enStatusUpItem3,	//�X�e�[�^�X�㏸�A�C�e���R��
			enStatusUpItem4,	//�X�e�[�^�X�㏸�A�C�e���S��
			enStatusUpItem5,	//�X�e�[�^�X�㏸�A�C�e���T��
			enBack,				//�߂�
			enStatusUpItemNum	//�X�e�[�^�X�㏸�A�C�e���I�𑍐�
		};

		//��������o�镶��
		FontRender* m_KUDFont[enShoppingModeNum] = { nullptr };
		//�A�C�e���P�`�R�Ɩ߂镶��
		FontRender* m_Item1kara3BackFont[enBuySelectStateNum] = { nullptr };
		FontRender* m_statusUpItem1kara5BackFont[enStatusUpItemNum] = { nullptr };
		//�A�C�e���̒l�i
		FontRender* m_itemValueFont[3] = { nullptr };
		//�I���J�[�\������
		FontRender* m_cursorFont = nullptr;
		//�ҋ@������
		FontRender* m_waitFont = nullptr;
		//�A�C�e��������
		FontRender* m_itemSetumeiSentence = nullptr;
		//��������A�C�e����
		FontRender* m_setumeiItemName= nullptr;

		//�t�H���g�̕���
		wchar_t m_text[256];
		//�v���C���[�ԍ�
		int m_playerNum = 0;
		//���݂̃V���b�s���O���
		int m_nowShoppingState = enShoppingModeSelectState;
		//���݂̃V���b�s���O���[�h
		int m_nowShoppingMode = 0;
		bool m_isFontActive = true;
		Vector2 FONT_POS[6] = { {-550.0f,200.0f},{-550.0f,150.0f},{-550.0f,100.0f},{-550.0f,50.0f},{-500.0f,0.0f},{-500.0f,-50.0f} };
		Vector2 CURSOR_POS[6] = { {-600.0f,200.0f},{-600.0f,150.0f},{-600.0f,100.0f},{-600.0f,50.0f},{-600.0f,0.0f},{-600.0f,-50.0f} };

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~ItemShopSelectFont();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

	public:
		//���݂̃V���b�s���O��ԃZ�b�g���\�b�h
		void SetNowShoppingState(const int nowShoppingState) { m_nowShoppingState = nowShoppingState; };

		//���݂̃V���b�s���O���[�h�Z�b�g���\�b�h
		void SetNowShoppingMode(const int nowShoppingMode) { m_nowShoppingMode = nowShoppingMode; };

		//�v���C���[�ԍ��Z�b�g���\�b�h
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };
	};
}