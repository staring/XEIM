#pragma once

class CMysql
{
public:
	CMysql(void);
	~CMysql(void);

	BOOL  Connect(string sIP, UINT nPort);
	Recordset* ExeSql(string sSql);
	void ExcNoSql(string sSql);
	void Close();
};
