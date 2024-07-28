#pragma once

#include "Datas.h"
#include <sqlext.h>
#include <sql.h>
#include <iostream>

struct Client;

namespace DB
{
	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
	SQLRETURN ConnectDatabase(SQLHENV& henv, SQLHDBC& hdbc, SQLHSTMT& hstmt, const wchar_t* dsnName);
	void DisconnectDatabase(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt);
	void DB_LoadCharacter(Client* clients, int user_id, const std::wstring& inputName, const int nameLen, const wchar_t* dsnName);
	void DB_CreateCharacter(const std::wstring& inputName, const wchar_t* dsnName);
	void DB_SaveCharacter(Client* clients, int user_id, const std::wstring& inputName, const wchar_t* dsnName);
}

