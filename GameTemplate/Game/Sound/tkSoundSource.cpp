/// <summary>
/// �����N���X
/// </summary>
//#include "tkEngine/tkEnginePreCompile.h"
#include "stdafx.h"

	SoundSource::SoundSource()
	{
		memset(m_emitterAzimuths, 0, sizeof(m_emitterAzimuths));
		memset(m_matrixCoefficients, 0, sizeof(m_matrixCoefficients));
	}
	SoundSource::~SoundSource()
	{
		Release();
	}
	void SoundSource::InitCommon()
	{
		m_dspSettings.SrcChannelCount = INPUTCHANNELS;
		m_dspSettings.DstChannelCount = SoundEngine::GetInstance()->GetNumChannel();
		m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;
		m_dspSettings.pDelayTimes = nullptr;
		m_dspSettings.DopplerFactor = 1.0f;
		m_dspSettings.LPFDirectCoefficient = 0.82142854f;
		m_dspSettings.LPFReverbCoefficient = 0.75f;
		m_dspSettings.ReverbLevel = 0.69114286f;
		m_dspSettings.EmitterToListenerAngle = 0.0f;
		m_dspSettings.EmitterToListenerDistance = 10.0f;
		m_dspSettings.EmitterVelocityComponent = 0.0f;
		m_dspSettings.ListenerVelocityComponent = 0.0f;
	}
	void SoundSource::Init(wchar_t* filePath, bool is3DSound)
	{
		if (SoundEngine::GetInstance()->IsAvailable() == false) {
			//�T�E���h�G���W�������p�s�B
			return;
		}
		m_isAvailable = false;

		m_waveFile = SoundEngine::GetInstance()->GetWaveFileBank().FindWaveFile(0, filePath);
		if (!m_waveFile) {
			m_waveFile.reset(new WaveFile);
			bool result = m_waveFile->Open(filePath);
			if (result == false) {
				//wave�t�@�C���̓ǂݍ��݂Ɏ��s�B
				SoundEngine::GetInstance()->GetWaveFileBank().UnregistWaveFile(0, m_waveFile);
				m_waveFile.reset();
				return;
			}
			m_waveFile->AllocReadBuffer(m_waveFile->GetSize());	//wave�t�@�C���̃T�C�Y���̓ǂݍ��݃o�b�t�@���m�ۂ���B
			SoundEngine::GetInstance()->GetWaveFileBank().RegistWaveFile(0, m_waveFile);
			unsigned int dummy;
			m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
			m_waveFile->ResetFile();

		}

		//�T�E���h�{�C�X�\�[�X���쐬�B
		m_sourceVoice = SoundEngine::GetInstance()->CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);
		if (is3DSound) {
			SoundEngine::GetInstance()->Add3DSoundSource(this);
		}
		InitCommon();

		m_is3DSound = is3DSound;
		m_isAvailable = true;
	}
	void SoundSource::Init(const wchar_t* filename, bool is3DSound)
	{
		if (SoundEngine::GetInstance()->IsAvailable() == false) {
			//�T�E���h�G���W�������p�s�B
			return;
		}
		m_isAvailable = false;
		m_waveFile = SoundEngine::GetInstance()->GetWaveFileBank().FindWaveFile(0, filename);
		if (!m_waveFile) {
			m_waveFile.reset(new WaveFile);
			m_waveFile->Open(filename);
			SoundEngine::GetInstance()->GetWaveFileBank().RegistWaveFile(0, m_waveFile);
			m_waveFile->AllocReadBuffer(m_waveFile->GetSize());	//wave�t�@�C���̃T�C�Y���̓ǂݍ��݃o�b�t�@���m�ۂ���B
			SoundEngine::GetInstance()->GetWaveFileBank().RegistWaveFile(0, m_waveFile);
			unsigned int dummy;
			m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
			m_waveFile->ResetFile();
		}

		//�T�E���h�{�C�X�\�[�X���쐬�B
		m_sourceVoice = SoundEngine::GetInstance()->CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);

		if (m_sourceVoice == nullptr) {
			MessageBoxA(nullptr, "wav�̃t�@�C���p�X���Ⴂ�܂�", "�t�@�C���p�X�𐳂������܂��傤�I", MB_OK);
			return;
		}
		if (is3DSound) {
			SoundEngine::GetInstance()->Add3DSoundSource(this);
		}
		InitCommon();

		m_is3DSound = is3DSound;
		m_isAvailable = true;
	}

	void SoundSource::InitStreaming(wchar_t* filePath, bool is3DSound, unsigned int ringBufferSize, unsigned int bufferSize)
	{
		if (SoundEngine::GetInstance()->IsAvailable() == false) {
			//�T�E���h�G���W�������p�s�B
			return;
		}
		m_isAvailable = false;
		//�X�g���[�~���O��CWaveFile�̎g���܂킵�͂ł��Ȃ��B
		m_waveFile.reset(new WaveFile);
		m_waveFile->Open(filePath);

		m_isStreaming = true;
		m_streamingBufferSize = bufferSize;
		m_waveFile->AllocReadBuffer(ringBufferSize);
		m_ringBufferSize = ringBufferSize;
		m_readStartPos = 0;
		m_currentBufferingSize = 0;
		//�T�E���h�{�C�X�\�[�X���쐬�B
		m_sourceVoice = SoundEngine::GetInstance()->CreateXAudio2SourceVoice(m_waveFile.get(), is3DSound);
		if (is3DSound) {
			SoundEngine::GetInstance()->Add3DSoundSource(this);
		}
		InitCommon();

		m_is3DSound = is3DSound;
		m_isAvailable = true;
	}
	void SoundSource::Release()
	{
		if (m_isStreaming) {
			if (m_waveFile) {
				m_waveFile->Release();
			}
		}
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->DestroyVoice();
			m_sourceVoice = nullptr;
		}
		Remove3DSound();
		DeleteGO(this);
	}
	void SoundSource::Play(char* buff, unsigned int bufferSize)
	{
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = (BYTE*)buff;
		buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		buffer.AudioBytes = bufferSize;
		if (m_sourceVoice != nullptr && bufferSize > 0) {
			//�Đ��J�n�B
			if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer))) {
				Release();
				//TK_LOG("Failed m_sourceVoice->SubmitSourceBuffer");
				return;
			}

		}
	}

	void SoundSource::StartStreamingBuffring()
	{
		char* readStartBuff = m_waveFile->GetReadBuffer();
		m_readStartPos += m_currentBufferingSize;
		if (m_readStartPos + m_streamingBufferSize >= m_ringBufferSize) {
			//�����O�o�b�t�@�̃T�C�Y�𒴂���B
			//�z����B
			m_readStartPos = 0;
		}
		m_waveFile->ReadAsync(&readStartBuff[m_readStartPos], m_streamingBufferSize, &m_currentBufferingSize);
		m_streamingState = enStreamingBuffering;
	}
	void SoundSource::Play(bool isLoop)
	{
		//�C���X�^���X�����p�\�ł͂Ȃ��Ƃ��A
		if (m_isAvailable == false) {
			return;
		}
		//�Đ����̎��A
		if (m_isPlaying) {
			//�Đ����̂��̂��ĊJ����B
			m_sourceVoice->Start(0);
		}
		//�Đ����ł͂Ȃ��Ƃ��A
		else {
			//�X�g���[�~���O�Đ�
			if (m_isStreaming) {
				//�o�b�t�@�����O�J�n
				m_waveFile->ResetFile();
				StartStreamingBuffring();
				m_sourceVoice->Start(0, 0);
			}
			//�X�g���[�~���O�Đ��ł͂Ȃ�
			else {
				m_sourceVoice->FlushSourceBuffers();
				m_sourceVoice->Start(0);
				Play(m_waveFile->GetReadBuffer(), m_waveFile->GetSize());
			}
			m_isPlaying = true;
		}
		m_isLoop = isLoop;
	}
	void SoundSource::UpdateStreaming()
	{
		if (!m_isPlaying) {
			return;
		}
		if (m_streamingState == enStreamingBuffering) {
			//�o�b�t�@�����O���B
			if (m_waveFile->IsReadEnd()) {
				//�o�b�t�@�����O���I������B
				m_streamingState = enStreamingQueueing;
			}
		}
		if (m_streamingState == enStreamingQueueing) {
			//�L���[�C���O���B
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);
			if (state.BuffersQueued <= 2) {	//�L���[�C���O����Ă���o�b�t�@���Q�ȉ��ɂȂ�����L���[�C���O�ł���B
				char* buff = m_waveFile->GetReadBuffer();
				Play(&buff[m_readStartPos], m_currentBufferingSize);
				if (m_currentBufferingSize == 0) {
					//�ǂݍ��񂾃T�C�Y���O�Ƃ������Ƃ͖��[�܂œǂݍ��݂��I������Ƃ������ƁB
					if (m_isLoop) {
						//���[�v����H
						//wave�t�@�C���̓ǂݍ��݈ʒu�����Z�b�g���ăo�b�t�@�����O�ĊJ�B
						m_waveFile->ResetFile();
						StartStreamingBuffring();
					}
					else {
						//���[�v���Ȃ��ꍇ�̓L���[����ɂȂ�����Đ��I���B
						if (state.BuffersQueued == 0) {
							//�Đ��I���B
							m_isPlaying = false;
							DeleteGO(this);
							Remove3DSound();
						}
					}
				}
				else {
					//�����o�b�t�@�����O����B
					StartStreamingBuffring();
				}
			}
		}
	}
	void SoundSource::Remove3DSound()
	{
		if (m_is3DSound) {
			SoundEngine::GetInstance()->Remove3DSoundSource(this);
			m_is3DSound = false;
		}
	}
	void SoundSource::UpdateOnMemory()
	{
		if (!m_isPlaying) {
			return;
		}
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);
		if (state.BuffersQueued <= 0) {
			m_isPlaying = false;
			if (m_isLoop) {
				//���[�v�B
				Play(m_isLoop);
			}
			else {
				DeleteGO(this);
				Remove3DSound();
			}
		}
	}
	void SoundSource::Update()
	{
		if (m_isAvailable == false) {
			return;
		}

		if (m_isStreaming) {
			//�X�g���[�~���O�Đ����̍X�V�B
			UpdateStreaming();
		}
		else {
			//�I���������Đ����̍X�V�����B
			UpdateOnMemory();
		}
		if (m_is3DSound == true) {
			//�����̈ړ����x���X�V�B
			m_velocity.Subtract(m_position, m_lastFramePosition);
			m_velocity.Div(GameTime::GameTimeFunc().GetFrameDeltaTime());
			m_lastFramePosition = m_position;
		}
	}