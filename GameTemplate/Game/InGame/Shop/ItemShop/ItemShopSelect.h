/// <summary>
/// �A�C�e���V���b�v�V�[���̑I���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class ItemShopSelectFont;	//�A�C�e���V���b�v�V�[���̑I�𕶎�
	class Player;				//�v���C���[
	class PlayerStatus;			//�v���C���[�̃X�e�[�^�X
	class PlayerCamera;			//�v���C���[�̃J����
	class Weapon;				//����
	class GameScreenUI;			//�Q�[�����UI

	class ItemShopSelect : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�V���b�v�̃��[�h
		enum enShoppingMode
		{
			enBuy,		//�w��
			enSold,		//���p
			enEnter		//�o��
		};

		//�V���b�v�̏��
		enum enShoppingState
		{
			enShoppingModeSelectState,		//�V���b�v���[�h�I�����
			enBuyState,						//�w�����
			enSoldState,					//���p���
			enEnterState					//�o����
		};

		//�w���I�����
		enum enBuySelectState
		{
			enFirstItem,		//���i�P��
			enSecondItem,		//���i�Q��
			enThirdItem,		//���i�R��
			enBackState			//�߂�
		};

		//�X�e�[�^�X�㏸�A�C�e��
		enum enStatusUpItem
		{
			enStatusUpItem1,	//�A�C�e���P
			enStatusUpItem2,	//�A�C�e���Q
			enStatusUpItem3,	//�A�C�e���R
			enStatusUpItem4,	//�A�C�e���S
			enStatusUpItem5,	//�A�C�e���T
			enBack				//�߂�
		};

		Player* m_player = nullptr;
		PlayerStatus* m_playerStatus = nullptr;
		SpriteRender* m_pushXSprite = nullptr;
		SpriteRender* m_ItemSetumeiSprite = nullptr;
		ItemShopSelectFont* m_itemShopSelectFont = nullptr;
		GameScreenUI* m_gameScreenUi = nullptr;
		SoundSource* m_selectSound = nullptr;
		SoundSource* m_decideSound = nullptr;
		SoundSource* m_cantDecideSound = nullptr;
		SoundSource* m_buySound = nullptr;
		Weapon* m_weapon = nullptr;

		int m_nowShoppingMode = enBuy;	//�����B����B�o��B�̂R��
		int m_nowShoppingState = enShoppingModeSelectState;
		int m_playerNum = 0;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~ItemShopSelect();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�A�C�e���V���b�v����o�郁�\�b�h
		void EnterFromShop();

		//�I�����\�b�h
		void CursorMove(const int selectMin, const int selectMax);

		//���̏�Ԃɍs�����\�b�h
		void NextState();

		//�w������Ƃ��̑I��
		void BuySelect();

		//���p����Ƃ��̑I��
		void SoldSelect();

	public:
		void SetPlayerNum(const int playerNum) { m_playerNum = playerNum; };

		/// <summary>
		/// ���݂̃V���b�s���O��Ԃ��擾
		/// </summary>
		/// <returns>���݂̃V���b�s���O���</returns>
		int GetNowShoppingState()const { return m_nowShoppingState; };
	};
}