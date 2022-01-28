/// <summary>
/// �����N���X
/// </summary>
#pragma once
#include "../stdafx.h"
#include "tkWaveFile.h"
#include "tkSoundEngine.h"
#include "xaudio2.h"
#include "x3daudio.h"

	/*!
	 * @brief	�����N���X�B
	 *@details
	 * ���̃N���X�ɂ͔g�`�f�[�^���I���������ɏ悹�čĐ�������@�ƁA�X�g���[�~���O�Đ�������@������܂��B</br>
	 * �I���������ɏ悹��ꍇ��CSoundSource::Init���g���ď��������s���Ă��������B</br>
	 * �X�g���[�~���O�Đ����s���ꍇ��CSoundSource::InitStreaming���g�p���Ă��������B</br>
	 * �I���������ł̍Đ��͔g�`�f�[�^��S�ă��C����������ɏ悹�čĐ����s�����@�ł��B</br>
	 * �X�g���[�~���O�Đ��̓��f�B�A(DVD�AHDD�Ȃǂ̊O���⏕�L�����u)���班���Âǂݍ��݂��s���āA</br>
	 * �������s���Ă������@�ł��B�X�g���[�~���O�Đ��͕K�v�ȕ������ǂݍ��݂��s��(�o�b�t�@�����O)���s���A</br>
	 * �Đ�����������Ƃ��̃o�b�t�@��j�����܂��B���̂��߁A���Ȃ��������ő傫�Ȕg�`�f�[�^���Đ����邱�Ƃ��\�ł��B</br>
	 * �I���������Đ��͓ǂݍ��݂̃I�[�o�[�w�b�h���ŏ������ɂȂ邪�A�X�g���[�~���O�Đ��͏�ɓǂݍ��݂̃I�[�o�[�w�b�h��</br>
	 * �������邽�߃p�t�H�}�[���X���ቺ���܂��B</br>
	 * ��ʓI�ɃI���������Đ��͌��ʉ��Ȃǂ̂悤�ȃT�C�Y�̏�����SE�̍Đ��ɓK���Ă��܂��B</br>
	 * �X�g���[�~���O�Đ��͑傫�ȃT�C�Y��BGM�̍Đ��ɓK���Ă��܂��B</br>
	 *
	 * ���̃N���X�̃C���X�^���X���Q�[���I�u�W�F�N�g�}�l�[�W���[�ɓo�^�����ꍇ�A���[�v�Đ��ł͂Ȃ��ꍇ�͍Đ������������</br>
	 * �����I�ɓo�^��������܂��B���[�v�Đ��̏ꍇ�̓��[�U�[�������I�ɓo�^����������K�v������܂��B
	 *@code
		//������
		void Init()
		{
			prefab::SoundSource* ss = NewGO<prefab::SoundSource>(0);
			ss->Init("test.wave");				//������ŏ���������ƃI���������Đ��B
			//soundSource.InitStreaming("test.wave");	//������ŏ���������ƃX�g���[�~���O�Đ��B
			ss->Play(true);						//����̓��[�v�Đ��B
		}

	 *@endcode
	 */
	class SoundSource : public IGameObject {
	public:
		/// <summary>
		///	�R���X�g���N�^
		/// </summary>
		SoundSource();

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~SoundSource();

		/*!
		 * @brief	�������B
		 *@details
		 * �I���������Đ������̏������B
		 *@param[in]	filePath	�t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)
		 *@param[in]	is3DSound	3D�T�E���h�H
		 */
		void Init(wchar_t* filePath, bool is3DSound = false);

		/*!
		 * @brief	�������B
		 *@details
		 * �I���������Đ������̏������B��������g������������Ƃ��������B
		 *@param[in]	nameKey		���O�L�[�B
		 *@param[in]	is3DSound	3D�T�E���h�H
		 */
		void Init(const wchar_t* filename, bool is3DSound = false);

		/*!
		* @brief	�������B
		*@details
		* �X�g���[�~���O�Đ������̏������B
		*@details
		* �����O�o�b�t�@�ɃX�g���[�~���O�ǂݍ��݂��s���āA�Đ����s���Ă����܂��B</br>
		* ��x�ɓǂݍ��܂��f�[�^�̍ő�T�C�Y��bufferingSize�ł��B</br>
		* �ǂݍ��܂ꂽ�f�[�^�̓����O�o�b�t�@�ɃR�s�[����Ă����܂��B</br>
		*@param[in]	filePath		�t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)
		*@param[in] is3DSound		3D�T�E���h�H
		*@param[in] ringBufferSize	�����O�o�b�t�@�̃T�C�Y�B(bufferSize�̔{���ɂȂ��Ă���Ɩ��ʂȂ����p�ł��܂��B)
		*@param[in]	bufferSize		�X�g���[�~���O�̍ő�o�b�t�@�����O�T�C�Y�B
		*/
		void InitStreaming(wchar_t* filePath, bool is3DSound = false, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);

		/*!
		* @brief	�J���B
		*@details
		* �f�X�g���N�^���玩���I�ɌĂ΂�܂��B�����I�ɊJ�����s�������ꍇ�Ɏg�p���Ă��������B
		*/
		void Release();

		/*!
		* @brief	�Đ��B
		*@param[in]	isLoop		���[�v�Đ��t���O�B
		* ���̃t���O��false�̏ꍇ�̓����V���b�g�Đ��ƂȂ�܂��B
		* �����V���b�g�Đ��̏ꍇ�́A�Đ�����������Ǝ����I�ɃC���X�^���X���j������܂��B
		* �����V���b�g�Đ���CSoundSource�̃C���X�^���X�������o�ϐ��Ȃǂŕێ����Ă���ƁA�j�����ꂽ
		* �C���X�^���X�ɃA�N�Z�X���Ă��܂��\��������̂ŁA�ێ����Ȃ����Ƃ������߂��܂��B
		* �Đ��̊��������o�������ꍇ�Ȃǂ̓C�x���g���X�i�[���g�p���Ă��������B
		*/
		void Play(bool isLoop);

		/*!
		* @brief	��~�B
		*/
		void Stop()
		{
			m_sourceVoice->Stop();
			m_isPlaying = false;
		}

		/*!
		* @brief	�ꎞ��~�B
		*/
		void Pause()
		{
			m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
		}

		/*!
		* @brief	�Đ����H�B
		*/
		bool IsPlaying() const
		{
			return m_isPlaying;
		}

		/*!
		* @brief	�{�����[����ݒ�B
		*@param[in]	vol		�{�����[���B
		*/
		void SetVolume(float vol)
		{
			m_sourceVoice->SetVolume(vol);
		}

		/*!
		* @brief	�{�����[�����擾�B
		*/
		float GetVolume() const
		{
			float vol;
			m_sourceVoice->GetVolume(&vol);
			return vol;
		}
		/*!
		* @brief	�����̍��W��ݒ�B
		* @details
		*  3D�T�E���h�̎��ɕK�v�ɂȂ�܂��B
		*  2D�T�E���h�ł͖�������܂��B
		*@param[in] pos		�����̍��W�B
		*/
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
			if (m_isSetPositionFirst) {
				m_lastFramePosition = m_position;
				m_isSetPositionFirst = false;
			}
		}
		/*!
		* @brief	�����̍��W���擾�B
		*/
		Vector3 GetPosition() const
		{
			return m_position;
		}
		/*!
		* @brief	�����̈ړ����x���擾�B
		*/
		Vector3 GetVelocity() const
		{
			return m_velocity;
		}
		IXAudio2SourceVoice* GetSourceVoice() {
			return m_sourceVoice;
		}

		/*!
		* @brief	���[�v�t���O���擾�B
		*/
		bool GetLoopFlag() const
		{
			return m_isLoop;
		}
		/*!
		* @brief	�{�C�X�̎��g��������
		*@details
		* �ڍׂ�IXAudio2SourceVoice��SetFrequencyRatio���Q�Ƃ��Ă��������B
		*@param[in]	ratio		���g����B
		*/
		void SetFrequencyRatio(float ratio)
		{
			if (m_sourceVoice != nullptr) {
				m_sourceVoice->SetFrequencyRatio(ratio);
			}
		}
		IXAudio2SourceVoice* GetXAudio2SourceVoice()
		{
			return m_sourceVoice;
		}
		//���̓`�����l�������擾�B
		int GetNumInputChannel()const
		{
			return m_waveFile->GetFormat()->nChannels;
		}
		FLOAT32* GetEmitterAzimuths()
		{
			return m_emitterAzimuths;
		}
		FLOAT32* GetMatrixCoefficients()
		{
			return m_matrixCoefficients;
		}
		X3DAUDIO_DSP_SETTINGS* GetDspSettings()
		{
			return &m_dspSettings;
		}
	private:
		void InitCommon();
		//�X�g���[�~���O�Đ����̍X�V�����B
		void UpdateStreaming();
		//�I���������Đ����̍X�V�����B
		void UpdateOnMemory();
		void Play(char* buff, unsigned int bufferSize);
		/*!
		* @brief	�X�g���[�~���O�o�b�t�@�����O�̊J�n�B
		*/
		void StartStreamingBuffring();
		void Remove3DSound();
		/*!
		* @brief	�X�V�B
		*/
		void Update() override;
	private:
		enum EnStreamingStatus {
			enStreamingBuffering,	//�o�b�t�@�����O���B
			enStreamingQueueing,	//�L���[�C���O���B
		};
		std::shared_ptr<WaveFile>	m_waveFile;					//!<�g�`�f�[�^�B
		IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//!<�\�[�X�{�C�X�B
		bool						m_isLoop = false;			//!<���[�v�t���O�B
		bool						m_isPlaying = false;		//!<�Đ����t���O�B
		bool					m_isStreaming = false;		//!<�X�g���[�~���O�Đ��H
		unsigned int			m_streamingBufferSize = 0;	//!<�X�g���[�~���O�p�̃o�b�t�@�����O�T�C�Y�B
		unsigned int			m_currentBufferingSize = 0;	//!<���݂̃o�b�t�@�����O�̃T�C�Y�B
		unsigned int			m_readStartPos = 0;			//!<�ǂݍ��݊J�n�ʒu�B
		unsigned int			m_ringBufferSize = 0;		//!<�����O�o�b�t�@�̃T�C�Y�B
		EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<�X�g���[�~���O�X�e�[�^�X�B
		bool					m_is3DSound = false;		//!<3D�T�E���h�H
		Vector3				m_position = Vector3::Zero;	//!<�����̍��W�B3D�T�E���h�̎��ɕK�v�B
		Vector3				m_lastFramePosition = Vector3::Zero;//!<������1�t���[���O�̍��W�B3D�T�E���h�̎��ɕK�v�B
		Vector3				m_velocity = Vector3::Zero;	//!<���x�B3D�T�E���h�̎��ɕK�v�E
		FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
		FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
		X3DAUDIO_DSP_SETTINGS m_dspSettings;
		bool m_isSetPositionFirst = true;	//!<��ԍŏ���setPosition?
		bool m_isAvailable = false;			//!<�C���X�^���X�����p�\�H
	};
