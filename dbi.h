#ifndef _CAPWAP_SERVER_DBI_H_
#define _CAPWAP_SERVER_DBI_H_

#include <vector>
#include <string>
#include <sqlite3.h>

typedef std::vector< std::vector<std::string> > DBResult;

class DBI {
public:
	static int Open(const char * path) {
		return sqlite3_open(path, &_db_handle);
	}

	static int Query(const char * table, const char * field, DBResult & result, const char * cond = NULL) ;

	static int Insert(const char * table, const char * field, const char * value);

	static int Delete(const char * table, const char * cond = NULL);

	static int Update(const char * table, const char * key_value, const char * cond = NULL);

	static int Close() {
		return sqlite3_close(_db_handle);
	}

	static int exec(const char * sql, char ** errmsg);
private:
	static sqlite3 * _db_handle;
};


#endif
