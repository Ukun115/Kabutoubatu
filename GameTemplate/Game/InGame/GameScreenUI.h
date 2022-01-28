/// <summary>
/// �Q�[�����UI�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class PlayerStatus;	//�v���C���[�̃X�e�[�^�X

	class GameScreenUI : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//UI�̖��O�B���ɍs���قǕ\�����C���[�̗D��x�������Ȃ�
		enum enUiName
		{
			enFlame,	//�t���[��
			enBack,		//�t���[�����̔w�i
			enIcon,		//�A�C�R��
			enUiNum		//�Œ�UI�̑���
		};

		//�v���C���[�ԍ�
		enum enPlayerNum
		{
			enPlayer1,		//1P
			enPlayer2,		//2P
			enPlayerNum		//�v���C���[�̑���
		};

		//����
		enum enWeapon
		{
			enWoodStick,	//�؂̖_
			enSword,		//��
			enLargeSword,	//�匕
			enBow,			//�|
			enWeaponNum		//����̑���
		};

		//�A�C�e��
		enum enItem
		{
			enItem1,	//�A�C�e���P
			enItem2,	//�A�C�e���Q
			enItem3,	//�A�C�e���R
			enItem4,	//�A�C�e���S
			enItem5,	//�A�C�e���T
			enItemNum	//�A�C�e���̑���
		};

		//�A�C�e��UI
		SpriteRender* m_itemUI[enPlayerNum][enItemNum] = { nullptr };
		//�Œ�UI
		SpriteRender* m_fixedUI[enUiNum] = { nullptr };
		//�q�b�g�|�C���gUI
		SpriteRender* m_HitPointUI[enPlayerNum][10] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum];
		SpriteRender* m_haveWeaponUI[enPlayerNum][enWeaponNum] = { nullptr };
		SpriteRender* m_playerIconUI[enPlayerNum] = { nullptr };
		FontRender* m_haveItemNum[enPlayerNum][enItemNum] = { nullptr };

		int m_itemNum[enPlayerNum][enItemNum] = {0};
		int m_playerNum = enPlayerNum;
		wchar_t m_itemText[256];
		int m_blinkingTimer[2] = { 0 };
		Vector2 m_itemFontPos;
		//�q�b�g�|�C���g�摜�̃t�@�C����
		char m_hitPointFileName[256];
		char m_weaponFileName[256];
		char m_itemFileName[256];

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start() override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~GameScreenUI();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update() override final;

		//�q�b�g�|�C���gUI�̏��������\�b�h
		void HitPointUiInit(const int playerNum);

		void HaveWeaponUiInit(const int playerNum);

		void ItemUiInit(const int playerNum);

		//�v���C���[�̃q�b�g�|�C���g��\����Ԃɂ��邩��\����Ԃɂ��邩���X�V���鏈�����\�b�h
		void HitPointUiUpdate();

		//�q�b�g�|�C���g�����Ȃ��Ƃ��n�[�g���_�ł��鏈��
		void BlinkingHeart();

		//��������̉摜�\���X�V����
		void HaveWeapon();

		//�A�C�e�������������̏��������\�b�h
		void ItemFontInit(const int playerNum);

		//�\�̓A�b�v�A�C�e���̏����������̐��X�V
		void ItemNumUpdate();

	public:
		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		void AddItemNum(const int playerNum, const int itemNum) { m_itemNum[playerNum][itemNum]++; };

		void ReduceItemNum(const int playerNum, const int itemNum) { m_itemNum[playerNum][itemNum]--; };

		/// <summary>
		/// �X�e�[�^�X�㏸�A�C�e�����擾
		/// </summary>
		/// <returns>�X�e�[�^�X�㏸�A�C�e��</returns>
		int GetStatusUpItemNum(const int playerNum,const int statusUpItemNum)const { return m_itemNum[playerNum][statusUpItemNum]; };

		void ReduceStatusUpItemNum(const int playerNum, const int statusUpItemNum) { m_itemNum[playerNum][statusUpItemNum]--; };
	};
}