/// <summary>
/// �Q�[���V�[���N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class Stage;			//�X�e�[�W
	class SkyCube;			//�X�J�C�L���[�u
	class Player;			//�v���C���[
	class PlayerStatus;		//�v���C���[�̃X�e�[�^�X
	class PlayerCamera;		//�v���C���[�̃J����
	class Weapon;			//����
	class AccompanyAI;		//���sAI
	class GameScreenUI;		//�Q�[�����UI
	class GameLimitTime;	//�Q�[���̐�������
	class GameOverScene;	//�Q�[���I�[�o�[�V�[��
	class Fade;				//�t�F�[�h

	class GameScene : public IGameObject
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

		//����
		enum enWeapon
		{
			enWoodStick,	//�؂̖_
			enSword,		//��
			enLargeSword,	//�匕
			enArow			//�|
		};

		SoundSource* m_gameSceneSound = nullptr;
		//�X�J�C�L���[�u
		SkyCube* m_skyCube = nullptr;
		Fade* m_fade = nullptr;
		GameLimitTime* m_gameLimitTime = nullptr;
		AccompanyAI* m_accompanyAI = nullptr;
		PlayerCamera* m_playerCamera = nullptr;
		Stage* m_stage = nullptr;
		Player* m_player[enPlayerNum] = { nullptr };
		PlayerStatus* m_playerStatus[enPlayerNum] = { nullptr };
		GameOverScene* m_gameOverScene = nullptr;
		GameScreenUI* m_gameScreenUI = nullptr;
		FontRender* m_nowMoney[enPlayerNum] = { nullptr };
		GamePad* m_playerGamePad;
		GamePad* m_otherPlayerGamePad;
		FontRender* m_haveRecoveryItemNum[enPlayerNum] = { nullptr };
		Weapon* m_weapon[enPlayerNum] = { nullptr };

		int m_playerNum = enPlayerNum;
		wchar_t m_nowMoneyText[256];
		wchar_t m_haveRecoveryItemNumText[256];
		//�I�����C�����̃v���C���[�ԍ�
		int m_onlinePlayerNo = 0;
		//�v���C���[�ԍ�
		int m_playerNo = enPlayer1;
		//���̃v���C���[�ԍ�
		int m_otherPlayerNo = enPlayer1;
		/// <summary>
		/// �t���O
		/// </summary>
		bool m_online = false;

		/// <summary>
		/// �Ă΂�čŏ��Ɏ��s�����֐�
		/// </summary>
		bool Start()override final;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~GameScene();

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		//�Q�[���I�[�o�[�ڍs�������\�b�h
		void GameOver();

		//�񕜃A�C�e�������̏��������\�b�h
		void RecoveryItemFontInit(const int playerNum);

		//���������̏��������\�b�h
		void NowMoneyFontInit(const int playerNum);

		//�񕜃A�C�e���̏����������̐��X�V
		void RecoveryItemNumUpdate(const int playerNum);

		//�����̏����������̐��X�V7
		void NowMoneyNumUpdate(const int playerNum);

		void PlayerInit(Player* player, int playerNo, GamePad* gamePad, Weapon* weapon);

	public:
		//�v���C���[�ԍ����Z�b�g���郁�\�b�h
		void SetPlayerNo(const int playerNo) { m_playerNo = playerNo; };

		void SetOtherPlayerNo(const int otherPlayerNo) { m_otherPlayerNo = otherPlayerNo; };

		void SetPlayerGamePad(GamePad& gamePad) { m_playerGamePad = &gamePad; };

		void SetOtherPlayerGamePad(GamePad& gamePad) { m_otherPlayerGamePad = &gamePad; };

		void SetOnlinePlay(bool online) { m_online = online; };

		void SetPlayerNumber(const int playerNum) { m_playerNum = playerNum; };

		//�Q�[�������~�߂郁�\�b�h
		void GameBGMStop() { m_gameSceneSound->Stop(); };

		void GameBGMPlay() { m_gameSceneSound->Play(true); };

		void DeleteGameSceneClass() { DeleteGO(this); };

		/// <summary>
		/// �v���C���[�ԍ����擾
		/// </summary>
		/// <returns>�v���C���[�ԍ�</returns>
		int GetPlayerNo() { return m_playerNo; };
	};
}