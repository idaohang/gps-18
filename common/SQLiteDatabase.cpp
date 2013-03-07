#include "SQLiteDatabase.hpp"

inline static std::string	getError(int error)
{
	switch (error)
	{
		case SQLITE_OK:
			return "";
		case SQLITE_ERROR:
			return "SQL error or missing database";
		case SQLITE_INTERNAL:
			return "Internal logic error in SQLite";
		case SQLITE_PERM:
			return "Access permission denied";
		case SQLITE_ABORT:
			return "Callback routine requested an abort";
		case SQLITE_BUSY:
			return "The database file is locked";
		case SQLITE_LOCKED:
			return "A table in the database is locked";
		case SQLITE_NOMEM:
			return "A malloc() failed";
		case SQLITE_READONLY:
			return "Attempt to write a readonly database";
		case SQLITE_INTERRUPT:
			return "Operation terminated by sqlite3_interrupt()";
		case SQLITE_IOERR:
			return "Some kind of disk I/O error occurred";
		case SQLITE_CORRUPT:
			return "The database disk image is malformed";
		case SQLITE_NOTFOUND:
			return "Unknown opcode in sqlite3_file_control()";
		case SQLITE_FULL:
			return "Insertion failed because database is full";
		case SQLITE_CANTOPEN:
			return "Unable to open the database file";
		case SQLITE_PROTOCOL:
			return "Database lock protocol error";
		case SQLITE_EMPTY:
			return "Database is empty";
		case SQLITE_SCHEMA:
			return "The database schema changed";
		case SQLITE_TOOBIG:
			return "String or BLOB exceeds size limit";
		case SQLITE_CONSTRAINT:
			return "Abort due to constraint violation";
		case SQLITE_MISMATCH:
			return "Data type mismatch";
		case SQLITE_MISUSE:
			return "Library used incorrectly";
		case SQLITE_NOLFS:
			return "Uses OS features not supported on host";
		case SQLITE_AUTH:
			return "Authorization denied";
		case SQLITE_FORMAT:
			return "Auxiliary database format error";
		case SQLITE_RANGE:
			return "2nd parameter to sqlite3_bind out of range";
		case SQLITE_NOTADB:
			return "File opened that is not a database file";
		case SQLITE_ROW:
			return "sqlite3_step() has another row ready";
		case SQLITE_DONE:
			return "sqlite3_step() has finished executing";
	};
	return "";
}

SQLiteDatabase::SQLiteDatabase()
	: _db(nullptr), _error(SQLITE_OK)
{
}

SQLiteDatabase::SQLiteDatabase(std::string const &path, OpenMode mode)
	: _db(nullptr), _error(SQLITE_OK)
{
	this->open(path, mode);
}

SQLiteDatabase::~SQLiteDatabase()
{
	if (_db)
		this->close();
}

bool			SQLiteDatabase::open(std::string const &path, SQLiteDatabase::OpenMode mode)
{
	if (_db == nullptr)
		this->close();
	int			realmode = SQLITE_OPEN_FULLMUTEX;

	if (mode & READWRITECREATE)
		realmode |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	else if (mode & READWRITE)
		realmode |= SQLITE_OPEN_READWRITE;
	else if (mode & READ)
		realmode |= SQLITE_OPEN_READONLY;

	int		ret = sqlite3_open_v2(path.c_str(), &_db, realmode, nullptr);
	if (ret == SQLITE_OK)
		return true;
	return false;
}

void			SQLiteDatabase::close()
{
	sqlite3_close_v2(_db);
	_db = nullptr;
}

bool			SQLiteDatabase::oneStepRequest(std::string const &str)
{
	sqlite3_stmt	*stmt = nullptr;
	_error = sqlite3_prepare_v2(_db, str.c_str(), str.size(), &stmt, nullptr);
	if (_error != SQLITE_OK)
	{
		if (stmt)
			sqlite3_finalize(stmt);
		return false;
	}
	_error = sqlite3_step(stmt);
	if (_error != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		return false;
	}
	_error = sqlite3_finalize(stmt);
	return _error == SQLITE_OK;
}

std::shared_ptr<SQLiteRequest>	SQLiteDatabase::request(std::string const &str)
{
	return std::shared_ptr<SQLiteRequest>(new SQLiteRequest(*this, str));
}

int64_t							SQLiteDatabase::getLastInsertRowID()
{
	return sqlite3_last_insert_rowid(_db);
}

// Request

SQLiteRequest::SQLiteRequest(SQLiteDatabase &base)
	: _db(base._db), _request(nullptr), _finished(true),
	  _error(SQLITE_OK), _index(1)
{
}

SQLiteRequest::SQLiteRequest(SQLiteDatabase &base, std::string const &str)
	: _db(base._db), _request(nullptr), _finished(false),
	  _error(SQLITE_OK), _index(1)
{
	this->request(str);
}

SQLiteRequest::~SQLiteRequest()
{
	this->finalize();
}

SQLiteRequest	&SQLiteRequest::operator++()
{
	this->next();
	return *this;
}

void		SQLiteRequest::request(std::string const &str)
{
	this->finalize();
	_finished = false;
	_error = sqlite3_prepare_v2(_db, str.c_str(), str.size(), &_request, nullptr);
}

bool		SQLiteRequest::next()
{
	_error = sqlite3_step(_request);
	if (_error == SQLITE_ROW)
		return true;
	if (_error == SQLITE_DONE)
		_finished = true;
	return false;
}

int			SQLiteRequest::getCount() const
{
	return (_request) ? sqlite3_column_count(_request) : 0;
}

SQLiteRequest::ColumnType	SQLiteRequest::getType(int iCol) const
{
	int	ret = sqlite3_column_type(_request, iCol);
	if (ret == SQLITE_INTEGER)
		return INTEGER;
	if (ret == SQLITE_FLOAT)
		return FLOAT;
	if (ret == SQLITE_TEXT)
		return TEXT;
	if (ret == SQLITE_BLOB)
		return BLOB;
	return NULLVALUE;
}

bool		SQLiteRequest::isFinished() const
{
	return _finished;
}

std::string	SQLiteRequest::getLastError() const
{
	return getError(_error);
}

void		SQLiteRequest::finalize()
{
	if (_request)
	{
		sqlite3_finalize(_request);
		_index = 1;
		_request = nullptr;
	}
}

// bind template functions
int					SQLiteRequest::bindNull()
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_null(_request, _index);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindInt(int value)
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_int(_request, _index, value);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindInt64(int64_t value)
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_int64(_request, _index, value);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindDouble(double value)
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_double(_request, _index, value);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindText(char const *str, int size, void (*function)(void*))
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_text(_request, _index, str, size, function);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindText16(void const *ptr, int size, void (*function)(void*))
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_text16(_request, _index, ptr, size, function);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindBlob(void const *ptr, int size, void (*function)(void*))
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_blob(_request, _index, ptr, size, function);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}

int					SQLiteRequest::bindZeroBlob(int size)
{
	if (_request == nullptr)
		return 0;
	_error = sqlite3_bind_zeroblob(_request, _index, size);
	if (_error != SQLITE_OK)
		return 0;
	return _index++;
}


template <>
int					SQLiteRequest::bind(int value)
{
	return this->bindInt(value);
}

template <>
int					SQLiteRequest::bind(float value)
{
	return this->bindDouble(static_cast<double>(value));
}

template <>
int					SQLiteRequest::bind(double value)
{
	return this->bindDouble(value);
}

template <>
int					SQLiteRequest::bind(short value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(char value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(bool value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(int64_t value)
{
	return this->bindInt64(value);
}

template <>
int					SQLiteRequest::bind(unsigned int value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(unsigned short value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(unsigned char value)
{
	return this->bindInt(static_cast<int>(value));
}

template <>
int					SQLiteRequest::bind(uint64_t value)
{
	return this->bindInt64(static_cast<int64_t>(value));
}

template <>
int					SQLiteRequest::bind(std::string value)
{
	return this->bindText(value.c_str(), value.size(), PTRCOPY);
}

template <>
int					SQLiteRequest::bind(std::string const &value)
{
	return this->bindText(value.c_str(), value.size(), nullptr);
}

template <>
int					SQLiteRequest::bind(std::wstring value)
{
	return this->bindText16(value.c_str(), value.size(), PTRCOPY);
}

template <>
int					SQLiteRequest::bind(std::wstring const &value)
{
	return this->bindText16(value.c_str(), value.size(), PTRCOPY);
}


// get functions
void const			*SQLiteRequest::getBlob(int iCol, int &size) const
{
	if (_request == nullptr)
		return nullptr;
	void const	*value = sqlite3_column_blob(_request, iCol);
	if (value == nullptr)
		return nullptr;
	size = sqlite3_column_bytes(_request, iCol);
	return value;
}

unsigned char const	*SQLiteRequest::getText(int iCol, int &size) const
{
	if (_request == nullptr)
		return nullptr;
	unsigned char const	*value = sqlite3_column_text(_request, iCol);
	if (value == nullptr)
		return nullptr;
	size = sqlite3_column_bytes(_request, iCol);
	return value;
}

void const			*SQLiteRequest::getText16(int iCol, int &size) const
{
	if (_request == nullptr)
		return nullptr;
	void const	*value = sqlite3_column_text16(_request, iCol);
	if (value == nullptr)
		return nullptr;
	size = sqlite3_column_bytes(_request, iCol);
	return value;
}

int					SQLiteRequest::getInt(int iCol) const
{
	if (_request == nullptr)
		return 0;
	return sqlite3_column_int(_request, iCol);
}

double				SQLiteRequest::getDouble(int iCol) const
{
	if (_request == nullptr)
		return 0;
	return sqlite3_column_double(_request, iCol);
}

int64_t				SQLiteRequest::getInt64(int iCol) const
{
	if (_request == nullptr)
		return 0;
	return sqlite3_column_int64(_request, iCol);
}


template <>
int					SQLiteRequest::get(int iCol) const
{
	return this->getInt(iCol);
}

template <>
float				SQLiteRequest::get(int iCol) const
{
	return static_cast<float>(this->getDouble(iCol));
}

template <>
double				SQLiteRequest::get(int iCol) const
{
	return this->getDouble(iCol);
}

template <>
short				SQLiteRequest::get(int iCol) const
{
	return static_cast<short>(this->getInt(iCol));
}

template <>
char				SQLiteRequest::get(int iCol) const
{
	return static_cast<char>(this->getInt(iCol));
}

template <>
bool				SQLiteRequest::get(int iCol) const
{
	return this->getInt(iCol) != 0;
}

template <>
int64_t				SQLiteRequest::get(int iCol) const
{
	return this->getInt64(iCol);
}

template <>
unsigned int		SQLiteRequest::get(int iCol) const
{
	return static_cast<unsigned int>(this->getInt(iCol));
}

template <>
unsigned short		SQLiteRequest::get(int iCol) const
{
	return static_cast<unsigned short>(this->getInt(iCol));
}

template <>
unsigned char		SQLiteRequest::get(int iCol) const
{
	return static_cast<unsigned char>(this->getInt(iCol));
}

template <>
uint64_t			SQLiteRequest::get(int iCol) const
{
	return static_cast<uint64_t>(this->getInt64(iCol));
}

template <>
std::string			SQLiteRequest::get(int iCol) const
{
	int					size;
	unsigned char const	*text = this->getText(iCol, size);
	if (text == nullptr)
		return std::string();
	return std::string(reinterpret_cast<char const *>(text), size);
}

template <>
std::wstring		SQLiteRequest::get(int iCol) const
{
	int			size;
	void const	*text = this->getText16(iCol, size);
	if (text == nullptr)
		return std::wstring();
	return std::wstring(reinterpret_cast<wchar_t const *>(text), size);
}