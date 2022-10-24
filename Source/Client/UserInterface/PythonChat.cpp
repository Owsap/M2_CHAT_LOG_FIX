/// 1.
// Search @ void CPythonChat::UpdateViewMode
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);

// Replace with
#if defined(ENABLE_CHAT_LOG_FIX)
	TChatLineList* pLineList = &(dwID == CHAT_SET_LOG_WINDOW ? pChatSet->m_ShowingChatLogLineList : pChatSet->m_ShowingChatLineList);
#else
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);
#endif

/// 2.
// Search @ void CPythonChat::UpdateEditMode
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);

// Replace with
#if defined(ENABLE_CHAT_LOG_FIX)
	TChatLineList* pLineList = &(dwID == CHAT_SET_LOG_WINDOW ? pChatSet->m_ShowingChatLogLineList : pChatSet->m_ShowingChatLineList);
#else
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);
#endif

/// 3.
// Search @ void CPythonChat::UpdateLogMode
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);

// Replace with
#if defined(ENABLE_CHAT_LOG_FIX)
	TChatLineList* pLineList = &(dwID == CHAT_SET_LOG_WINDOW ? pChatSet->m_ShowingChatLogLineList : pChatSet->m_ShowingChatLineList);
#else
	TChatLineList* pLineList = &(pChatSet->m_ShowingChatLineList);
#endif

/// 4.
// Search @ int CPythonChat::GetLineCount
	for (DWORD i = 0; i < m_ChatLineDeque.size(); ++i)
	{
		if (!pChatSet->CheckMode(m_ChatLineDeque[i]->iType))
			continue;

		++iCount;
	}

// Replace with
#if defined(ENABLE_CHAT_LOG_FIX)
	TChatLineDeque ChatLineDeque = (dwID == CHAT_SET_LOG_WINDOW ? m_ChatLogLineDeque : m_ChatLineDeque);
	for (DWORD i = 0; i < ChatLineDeque.size(); ++i)
	{
		if (!pChatSet->CheckMode(ChatLineDeque[i]->iType))
			continue;

		++iCount;
	}
#else
	for (DWORD i = 0; i < m_ChatLineDeque.size(); ++i)
	{
		if (!pChatSet->CheckMode(m_ChatLineDeque[i]->iType))
			continue;

		++iCount;
	}
#endif

/// 5.
// Search @ CPythonChat::TChatLineList* CPythonChat::GetChatLineListPtr
	return &(rChatSet.m_ShowingChatLineList);

// Replace with
#if defined(ENABLE_CHAT_LOG_FIX)
	return &(dwID == CHAT_SET_LOG_WINDOW ? rChatSet.m_ShowingChatLogLineList : rChatSet.m_ShowingChatLineList);
#else
	return &(rChatSet.m_ShowingChatLineList);
#endif

/// 6.
// Add at the end of the function @v oid CPythonChat::ArrangeShowingChat
#if defined(ENABLE_CHAT_LOG_FIX)
	ArrangeShowingChatLog(dwID);
#endif

/// 7.
// Add at the end of the function @ void CPythonChat::AppendChat
#if defined(ENABLE_CHAT_LOG_FIX)
	AppendChatLog(iType, c_szChat);
#endif

/// 8.
// Search
DWORD CPythonChat::GetChatColor(int iType)

// Add above
#if defined(ENABLE_CHAT_LOG_FIX)
void CPythonChat::AppendChatLog(int iType, const char* c_szChat)
{
	// DEFAULT_FONT
	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
	{
		TraceError("CPythonChat::AppendChatLog - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	IAbstractApplication& rApp = IAbstractApplication::GetSingleton();
	SChatLine* pChatLine = SChatLine::New();

	pChatLine->iType = iType;
	pChatLine->Instance.SetValue(c_szChat);
#if defined(WJ_MULTI_TEXTLINE)
	pChatLine->Instance.DisableEnterToken();
#endif

	// DEFAULT_FONT
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	// END_OF_DEFAULT_FONT

	pChatLine->fAppendedTime = rApp.GetGlobalTime();
	pChatLine->SetColorAll(GetChatColor(iType));

	m_ChatLogLineDeque.emplace_back(pChatLine);
	if (m_ChatLogLineDeque.size() > CHAT_LINE_MAX_NUM)
	{
		SChatLine* pChatLine = m_ChatLogLineDeque.front();
		SChatLine::Delete(pChatLine);
		m_ChatLogLineDeque.pop_front();
	}

	for (TChatSetMap::iterator itor = m_ChatSetMap.begin(); itor != m_ChatSetMap.end(); ++itor)
	{
		TChatSet* pChatSet = &(itor->second);
		pChatSet->m_ShowingChatLogLineList.emplace_back(pChatLine);
		if (pChatSet->m_ShowingChatLogLineList.size() > CHAT_LINE_MAX_NUM)
			pChatSet->m_ShowingChatLogLineList.pop_front();
	}
}

void CPythonChat::ArrangeShowingChatLog(DWORD dwID)
{
	TChatSet* pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_ShowingChatLogLineList.clear();

	TChatLineDeque TempChatLineDeque;
	for (TChatLineDeque::iterator itor = m_ChatLogLineDeque.begin(); itor != m_ChatLogLineDeque.end(); ++itor)
	{
		TChatLine* pChatLine = *itor;
		if (pChatSet->CheckMode(pChatLine->iType))
			TempChatLineDeque.emplace_back(pChatLine);
	}

	int icurLineCount = TempChatLineDeque.size();
	int iVisibleLineCount = min(icurLineCount, (pChatSet->m_iHeight + pChatSet->m_iStep) / pChatSet->m_iStep);
	int iEndLine = iVisibleLineCount + int(float(icurLineCount - iVisibleLineCount - 1) * pChatSet->m_fEndPos);

	/////

	int iHeight = 12;
	for (int i = min(icurLineCount - 1, iEndLine); i >= 0; --i)
	{
		if (!pChatSet->CheckMode(TempChatLineDeque[i]->iType))
			continue;

		if (pChatSet->m_iHeight + pChatSet->m_iStep <= iHeight)
			break;

		pChatSet->m_ShowingChatLogLineList.emplace_front(TempChatLineDeque[i]);
		iHeight += pChatSet->m_iStep;
	}
}
#endif

/// 9.
// Search @ void CPythonChat::Close
		TChatLineList* pLineList = &(rChatSet.m_ShowingChatLineList);
		for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
		{
			TChatLine* pChatLine = (*itor);
			pChatLine->fAppendedTime = 0.0f;
		}

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
		pLineList = &(rChatSet.m_ShowingChatLogLineList);
		for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
		{
			TChatLine* pChatLine = (*itor);
			pChatLine->fAppendedTime = 0.0f;
		}
#endif

/// 10.
// Search @ void CPythonChat::Destroy
	m_ChatLineDeque.clear();

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
	m_ShowingChatLogLineList.clear();
	m_ChatLogLineDeque.clear();
#endif


/// 11.
// Search @ CPythonChat::~CPythonChat
	assert(m_WhisperMap.empty());

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
	assert(m_ChatLogLineDeque.empty());
	assert(m_ShowingChatLogLineList.empty());
#endif
