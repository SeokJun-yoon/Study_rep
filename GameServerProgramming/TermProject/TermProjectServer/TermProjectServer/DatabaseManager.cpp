#include "DatabaseManager.h"
#include "DBConstants.h"

using namespace std;


// 진단 레코드 처리 함수
void DB::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	SQLWCHAR wszMessage[1000];
	SQLWCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) 
	{
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRecW(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(SQLWCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) 
	{
		// 데이터가 잘린 경우는 숨김
		if (wcsncmp(wszState, L"01004", 5)) 
		{
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

SQLRETURN DB::ConnectDatabase(SQLHENV& henv, SQLHDBC& hdbc, SQLHSTMT& hstmt, const wchar_t* dsnName)
{
	SQLRETURN retcode;
	// 환경 핸들 할당
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		// ODBC 버전 설정
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);  // 수정: (SQLPOINTER*)
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			// 연결 핸들 할당
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{
				// 로그인 타임아웃 설정
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				// 데이터 소스에 연결
				retcode = SQLConnect(hdbc, (SQLWCHAR*)dsnName, SQL_NTS, NULL, 0, NULL, 0);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
				{
					// 스테이트먼트 핸들 할당
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
					if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) 
					{
						DB::HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
					}
				}
				else 
				{
					DB::HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
				}
			}
			else 
			{
				DB::HandleDiagnosticRecord(henv, SQL_HANDLE_ENV, retcode);
			}
		}
		else 
		{
			DB::HandleDiagnosticRecord(henv, SQL_HANDLE_ENV, retcode);
		}
	}
	else 
	{
		DB::HandleDiagnosticRecord(SQL_NULL_HANDLE, SQL_HANDLE_ENV, retcode);
	}
	return retcode;
}

// 데이터베이스 연결 해제 함수
void DB::DisconnectDatabase(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
{
	if (hstmt) 
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}
	if (hdbc) 
	{
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	}
	if (henv) 
	{
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
}

// 캐릭터 정보를 로드하는 함수
void DB::DB_LoadCharacter(Client* clients, int user_id, const wstring& inputName, const int nameLen, const wchar_t* dsnName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;

	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt, dsnName);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		wcout << L"ODBC Connect Ok!" << endl;

		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.LoadCharacter '%s'", inputName.c_str());

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			wcout << L"SELECT OK!" << endl;

			std::vector<SQLWCHAR> Name(nameLen);  // 동적 할당
			SQLINTEGER x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp;
			SQLLEN cbName = 0, cbx = 0, cby = 0, cbLevel = 0, cbAttackDamage = 0, cbAttackRange = 0,
				cbCurrExp = 0, cbMaxExp = 0, cbCurrHp = 0, cbMaxHp = 0;

			// 컬럼 바인딩
			SQLBindCol(hstmt, 1, SQL_C_WCHAR, Name.data(), nameLen * sizeof(SQLWCHAR), &cbName);
			SQLBindCol(hstmt, 2, SQL_C_LONG, &x, 0, &cbx);
			SQLBindCol(hstmt, 3, SQL_C_LONG, &y, 0, &cby);
			SQLBindCol(hstmt, 4, SQL_C_LONG, &Level, 0, &cbLevel);
			SQLBindCol(hstmt, 5, SQL_C_LONG, &AttackDamage, 0, &cbAttackDamage);
			SQLBindCol(hstmt, 6, SQL_C_LONG, &AttackRange, 0, &cbAttackRange);
			SQLBindCol(hstmt, 7, SQL_C_LONG, &CurrExp, 0, &cbCurrExp);
			SQLBindCol(hstmt, 8, SQL_C_LONG, &MaxExp, 0, &cbMaxExp);
			SQLBindCol(hstmt, 9, SQL_C_LONG, &CurrHp, 0, &cbCurrHp);
			SQLBindCol(hstmt, 10, SQL_C_LONG, &MaxHp, 0, &cbMaxHp);

			// 데이터 가져오기
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_NO_DATA)
			{
				wcout << L"No data found." << endl;
			}
			else if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				clients[user_id].x = x;
				clients[user_id].y = y;
				clients[user_id].m_level = Level;
				clients[user_id].m_att = AttackDamage;
				clients[user_id].m_attackrange = AttackRange;
				clients[user_id].m_exp = CurrExp;
				clients[user_id].m_maxexp = MaxExp;
				clients[user_id].m_hp = CurrHp;
				clients[user_id].m_maxhp = MaxHp;

				// 데이터 확인 출력 (선택 사항)
				wprintf(L"Name: %s, Level: %d, Exp: %d, HP: %d/%d\n",
					Name.data(), Level, CurrExp, CurrHp, MaxHp);
			}
			else
			{
				wcout << L"Error fetching data." << endl;
				HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
			}
		}
		else
		{
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else
	{
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}

// 캐릭터를 정보를 생성하는 함수
void DB::DB_CreateCharacter(const std::wstring& inputName, const wchar_t* dsnName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;

	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt, dsnName);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		wcout << L"ODBC Connect Ok!" << endl;

		int x = ((rand() % WORLD_WIDTH) % 6) + 11;
		int y = ((rand() % WORLD_HEIGHT) % 6) + 16;

		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.CreateCharacter '%s', %d, %d", inputName.c_str(), x, y);

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			wcout << L"INSERT OK!" << endl;
		}
		else
		{
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else
	{
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}

// 캐릭터 정보를 저장하는 함수
void DB::DB_SaveCharacter(Client * clients, int user_id, const std::wstring & inputName, const wchar_t* dsnName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;
	SQLINTEGER x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp;
	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt, dsnName);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		wcout << L"ODBC Connect Ok!" << endl;

		x = clients[user_id].x;
		y = clients[user_id].y;
		Level = clients[user_id].m_level;
		AttackDamage = clients[user_id].m_att;
		AttackRange = clients[user_id].m_attackrange;
		MaxExp = clients[user_id].m_maxexp;
		CurrExp = clients[user_id].m_exp;
		MaxHp = clients[user_id].m_maxhp;
		CurrHp = clients[user_id].m_hp;

		// 데이터 확인 출력 (선택 사항)
		wprintf(L"Name: %s, Level: %d, Exp: %d / %d , HP: %d / %d \n",
			inputName.c_str(), Level, CurrExp, MaxExp, CurrHp, MaxHp);


		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.SaveCharacter '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d",
			inputName.c_str(), x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp);

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			wcout << L"SAVE OK!" << endl;
		}
		else
		{
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else {
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}
