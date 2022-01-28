/// <summary>
/// ���\�[�X�o���N�}�l�[�W���[�N���X
/// </summary>

class ResourceBankManager
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResourceBankManager() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ResourceBankManager() = default;

	static ResourceBankManager* m_instance;

	TResourceBank<TkmFile> m_tkmFileBank;			//tkm�t�@�C���o���N�B
	TResourceBank<Shader> m_shaderBank;				//�V�F�[�_�[�o���N

public:
	static void CreateInstance()
	{
		if (!m_instance)
		{
			m_instance = new ResourceBankManager;
		}
	}

	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static ResourceBankManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// tkm�t�@�C�����o���N����擾�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>tkm�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}

	/// <summary>
	/// tkm�t�@�C�����o���N�ɓo�^�B
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	/// <summary>
	/// �V�F�[�_�[�t�@�C���o���N����V�F�[�_�[���擾�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}

	/// <summary>
	/// �V�F�[�_�[�t�@�C�����o���N�ɓo�^�B
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
};

