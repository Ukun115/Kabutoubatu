/// <summary>
/// 波形データバンククラス
/// </summary>
#include "stdafx.h"
#include "../tkUtil.h"
#include "tkWaveFile.h"
#include "tkWaveFileBank.h"

	CWaveFileBank::CWaveFileBank()
	{
	}
	CWaveFileBank::~CWaveFileBank()
	{
		ReleaseAll();
	}
	void CWaveFileBank::Release(int groupID)
	{
		for (auto waveFile : m_waveFileMap[groupID]) {
			waveFile.second->Release();
		}
		m_waveFileMap[groupID].clear();
	}
	void CWaveFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
	{
		//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		m_waveFileMap->erase(waveFile->GetFilePathHash());
	}
	void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
	{
		//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
	}
	CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
	{
		//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		auto value = m_waveFileMap[groupID].find(CUtil::MakeHash(filePath));
		if (value != m_waveFileMap[groupID].end()) {
			return value->second;
		}
		return CWaveFilePtr();
	}
	//CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const WNameKey& nameKey)
	//{
	//	//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
	//	auto value = m_waveFileMap[groupID].find(nameKey.GetHashCode());
	//	if (value != m_waveFileMap[groupID].end()) {
	//		return value->second;
	//	}
	//	return CWaveFilePtr();
	//}
