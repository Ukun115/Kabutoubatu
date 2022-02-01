/// <summary>
/// �I�����C���ʐM�N���X
/// </summary>
#pragma once

namespace nsKabutoubatu
{
	/// <summary>
	/// �O���錾
	/// </summary>
	class SyncOnlineTwoPlayerMatchEngine;	//�I�����C���ʐM�G���W��
	class OnlineErrorScene;					//�I�����C�����̃G���[�V�[��

	class Online : public IGameObject
	{
	private:
		/// <summary>
		/// �񋓌^
		/// </summary>

		//�X�e�b�v
		enum enStep {
			enStep_CharacterSelect,			// �L�����N�^�[�Z���N�g�B
			enStep_WaitAllPlayerJoined,		// ���ׂẴv���C���[�������܂ő҂B
			enStep_WaitAllPlayerStartGame,	// ���ׂẴv���C���[���Q�[���J�n����̂�҂B
			enStep_InGame,					// �C���Q�[���B
			enStep_Error,					// �G���[�B
		};

		//�ʐM�G���W��
		SyncOnlineTwoPlayerMatchEngine* m_onlineTwoPlayerMatchEngine = nullptr;
		//�I�����C���ʐM�G���[�V�[��
		OnlineErrorScene* m_onlineErrorScene = nullptr;

		enStep m_step = enStep_CharacterSelect;

		//�v���C���[�ԍ�
		int m_playerNo = -1;
		/// <summary>
		/// �t���O
		/// </summary>
		//���ׂẴv���C���[�����[���ɃW���C���������ǂ���
		bool m_isJoined = false;

		/// <summary>
		/// ���t���[�����s�����X�V�����֐�
		/// </summary>
		void Update()override final;

		/// <summary>
		/// ���ׂẴv���C���[�����[���ɃW���C�������Ƃ��ɌĂ΂�鏈���B
		/// </summary>
		/// <param name="pData"></param>
		/// <param name="size"></param>
		void OnAllPlayerJoined(void* pData, int size);

		/// <summary>
		/// ���ׂẴv���C���[���Q�[���X�^�[�g�������ɌĂ΂�鏈���B
		/// </summary>
		void OnAllPlayerStartGame();

		/// <summary>
		/// �ʐM�G���[���N�����B
		/// </summary>
		void OnError();

	public:
		//�I�����C������
		void OnlineInit();

		void DeleteData();

		void GameStart();

		void SetPlayerNo(int playerNo) { m_playerNo = playerNo ; };

		/// <summary>
		/// �v���C���[�ԍ����擾
		/// </summary>
		/// <returns>�v���C���[�ԍ�</returns>
		int GetPlayerNo() { return m_playerNo; };

		/// <summary>
		/// �ʐM����̃v���C���[�ԍ����擾
		/// </summary>
		/// <returns>�ʐM����̃v���C���[�ԍ�</returns>
		int GetOtherPlayerNo() { return !GetPlayerNo(); };

		/// <summary>
		/// ���ׂẴv���C���[�����[���ɃW���C���������ǂ������擾
		/// </summary>
		/// <returns>���ׂẴv���C���[�����[���ɃW���C���������ǂ���</returns>
		bool GetIsJoined() { return m_isJoined; };

		/// <summary>
		/// �v���C���[�̃Q�[���p�b�h���擾
		/// </summary>
		/// <returns>�v���C���[�̃Q�[���p�b�h</returns>
		GamePad& GetPlayerGamePad(int no);
	};
}