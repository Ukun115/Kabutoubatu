#pragma once

#include "HID/GamePad.h"
#include "../GameTemplate/Game/GameTime.h"
#include "../MiniEngine/time/FPSLimitter.h"

class TkmFile;
class Shader;
class GraphicsEngine;

class TkEngine {
public:

	/// <summary>
		/// �t���[�����[�g���[�h
		/// </summary>
	enum EnFrameRateMode {
		enFrameRateMode_Fix,		// �Œ�t���[�����[�g���[�h�B
		enFrameRateMode_Variable,	// �σt���[�����[�g���[�h�B
	};
	/// <summary>
	/// �t���[�����[�g�Ɋւ�����
	/// </summary>
	struct FrameRateInfo {
		EnFrameRateMode frameRateMode;	// �t���[�����[�g���[�h
		int maxFPS;						// �ő�FPS
	};

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~TkEngine();
	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈���B
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

	const int GetRate()const {
		return m_rate;
	}

	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}

	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}

	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
	/// <summary>
		/// ���݂̃t���[�����[�g�Ɋւ�������擾
		/// </summary>
		/// <returns></returns>
	const FrameRateInfo& GetFrameRateInfo() const
	{
		return m_frameRateInfo;
	}
	/// <summary>
	/// �t���[�����[�g���[�h��ݒ肷��B
	/// </summary>
	/// <param name="frameRateMode">EnFrameRateMode���Q��</param>
	/// <param name="maxFPS">�ő�FPS</param>
	void SetFrameRateMode(EnFrameRateMode frameRateMode, int maxFPS)
	{
		if (frameRateMode == enFrameRateMode_Fix) {
			// �Œ�t���[�����[�g
			// 1�t���[���̌o�ߎ��Ԃ̒l���Œ�ɂ���B
			m_gameTime.EnableFixedFrameDeltaTime(1.0f / maxFPS);
		}
		else {
			// �σt���[�����[�g
			m_gameTime.DisableFixedFrameDeltaTime();
		}
		m_fpsLimitter.SetMaxFPS(maxFPS);
		// �t���[�����[�g�Ɋւ�������L���B
		m_frameRateInfo.frameRateMode = frameRateMode;
		m_frameRateInfo.maxFPS = maxFPS;
	}

private:
	TResourceBank<TkmFile> m_tkmFileBank;
	TResourceBank<Shader> m_shaderBank;

	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	GameTime m_gameTime;							//�Q�[���^�C���B
	int m_rate = 0;
	FrameRateInfo m_frameRateInfo = { enFrameRateMode_Variable , 60 };
	nsKabutoubatu::FPSLimitter m_fpsLimitter;						// FPS�ɐ����������鏈���B
};

extern TkEngine* g_engine;	//TK�G���W���B
extern GameTime* g_gameTime;

//�f�o�b�O�@�\���ȈՓI�ɃA�N�Z�X���邽�߂̖��O���
namespace dbg {

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
	/*!
	* @brief	�R���W�����̃f�o�b�O�\����L���ɂ���B
	*@param[in]	colliObj	�R���W�����I�u�W�F�N�g�B
	*/
	static inline void SetDrawPhysicsCollisionEnable()
	{
		PhysicsWorld().SetDebugDrawMode(1);
	}
	/*!
	* @brief	�R���W�����̃f�o�b�O�\����L���ɂ���B
	*@param[in]	colliObj	�R���W�����I�u�W�F�N�g�B
	*/
	static inline void SetDrawPhysicsCollisionDisable()
	{
		PhysicsWorld().SetDebugDrawMode(0);
	}
	/*!
	* @brief	�x�N�g���̃f�o�b�O�o�́B
	*/
	static inline void DrawVector(Vector3 vector, Vector3 origin, const char* name = "no name")
	{
		prefab::CVectorRender::SRenderData renderData = { name, vector, origin };
		Engine().GetVectorRender()->AddVector(renderData);
	}
	/*!
	* @brief	�x�N�g���̃f�o�b�O�`���L���ɂ���B
	*/
	static inline void SetDrawVectorEnable()
	{
		Engine().GetVectorRender()->SetRenderFlag(true);
	}
	/*!
	* @brief	�x�N�g���̃f�o�b�O�`��𖳌��ɂ���B
	*/
	static inline void SetDrawVectorDisable()
	{
		Engine().GetVectorRender()->SetRenderFlag(false);
	}
	static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
	{
		Engine().GetVectorRender()->SetRenderVectorNameList(list);
	}
#else

	static inline void SetDrawPhysicsCollisionEnable()
	{
	}

	static inline void SetDrawPhysicsCollisionDisable()
	{
	}

	static inline void DrawVector(Vector3 vector, Vector3 origin, const char* name = "no name")
	{
	}

	static inline void SetDrawVectorEnable()
	{
	}

	static inline void SetDrawVectorDisable()
	{
	}
	static inline void SetDrawVectorNameList(const std::vector<std::string>& list)
	{
	}
#endif

}
