/// 1.
// Search
	enum EBoardState

// Add above
#if defined(ENABLE_CHAT_LOG_FIX)
	enum EChatWindow
	{
		CHAT_SET_CHAT_WINDOW,
		CHAT_SET_LOG_WINDOW
	};
#endif

/// 2.
// Search @ struct SChatSet
		TChatLineList m_ShowingChatLineList;

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
		TChatLineList m_ShowingChatLogLineList;
#endif

/// 3.
// Search
	void ArrangeShowingChat(DWORD dwID);

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
	// Chat-log
	void AppendChatLog(int iType, const char* c_szChat);
	void ArrangeShowingChatLog(DWORD dwID);
#endif

/// 4.
// Search
	TWaitChatList m_WaitChatList;

// Add below
#if defined(ENABLE_CHAT_LOG_FIX)
	TChatLineDeque m_ChatLogLineDeque;
	TChatLineList m_ShowingChatLogLineList;
#endif
