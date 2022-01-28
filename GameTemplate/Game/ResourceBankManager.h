/// <summary>
/// リソースバンクマネージャークラス
/// </summary>

class ResourceBankManager
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResourceBankManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceBankManager() = default;

	static ResourceBankManager* m_instance;

	TResourceBank<TkmFile> m_tkmFileBank;			//tkmファイルバンク。
	TResourceBank<Shader> m_shaderBank;				//シェーダーバンク

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
	/// tkmファイルをバンクから取得。
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>tkmファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}

	/// <summary>
	/// tkmファイルをバンクに登録。
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	/// <summary>
	/// シェーダーファイルバンクからシェーダーを取得。
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}

	/// <summary>
	/// シェーダーファイルをバンクに登録。
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

