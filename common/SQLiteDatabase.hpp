#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include "sqlite3.h"

class SQLiteRequest;

class SQLiteDatabase
{
public:
	enum OpenMode : char
	{
		READ = 1,
		READWRITE = 2,
		READWRITECREATE = 4
	};

	SQLiteDatabase();
	SQLiteDatabase(std::string const &path, OpenMode mode = READWRITECREATE);
	virtual ~SQLiteDatabase();

	bool			open(std::string const &path, OpenMode mode = READWRITECREATE);
	void			close();

	bool							oneStepRequest(std::string const &str);
	std::shared_ptr<SQLiteRequest>	request(std::string const &str);

	int64_t			getLastInsertRowID();

	std::string		getLastError() const;

private:
	SQLiteDatabase(SQLiteDatabase const &);
	SQLiteDatabase		&operator=(SQLiteDatabase const &);

	sqlite3		*_db;
	int			_error;
	friend		SQLiteRequest;
};

class SQLiteRequest
{
public:
	enum ColumnType
	{
		INTEGER,
		FLOAT,
		TEXT,
		BLOB,
		NULLVALUE
	};

	#define PTRCOPY		SQLITE_TRANSIENT

	SQLiteRequest(SQLiteDatabase &base);
	SQLiteRequest(SQLiteDatabase &base, std::string const &str);
	
	virtual ~SQLiteRequest();
	SQLiteRequest		&operator++();

	void				request(std::string const &str);
	bool				next();

	// bind functions
	template <typename T>
	int					bind(T value);
	int					bindNull();
	int					bindInt(int value);
	int					bindInt64(int64_t value);
	int					bindDouble(double value);
	int					bindText(char const *str, int size, void (*function)(void*) = nullptr);
	int					bindText16(void const *ptr, int size, void (*function)(void*) = nullptr);
	int					bindBlob(void const *ptr, int size, void (*function)(void*) = nullptr);
	int					bindZeroBlob(int size);

	template <typename A, typename B>
	void				bind(A const &a, B const &b)
	{
		this->bind(a);
		this->bind(b);
	}

	template <typename A, typename B, typename C>
	void				bind(A const &a, B const &b, C const &c)
	{
		this->bind(a, b);
		this->bind(c);
	}

	template <typename A, typename B, typename C, typename D>
	void				bind(A const &a, B const &b, C const &c, D const &d)
	{
		this->bind(a, b, c);
		this->bind(d);
	}

	template <typename A, typename B, typename C, typename D, typename E>
	void				bind(A const &a, B const &b, C const &c, D const &d, E const &e)
	{
		this->bind(a, b, c, d);
		this->bind(e);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F>
	void				bind(A const &a, B const &b, C const &c, D const &d, E const &e, F const &f)
	{
		this->bind(a, b, c, d, e);
		this->bind(f);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	void				bind(A const &a, B const &b, C const &c, D const &d, E const &e, F const &f, G const &g)
	{
		this->bind(a, b, c, d, e, f);
		this->bind(g);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
	void				bind(A const &a, B const &b, C const &c, D const &d, E const &e, F const &f, G const &g, H const h)
	{
		this->bind(a, b, c, d, e, f, g);
		this->bind(h);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
	void				bind(A const &a, B const &b, C const &c, D const &d, E const &e, F const &f, G const &g, H const h, I const i)
	{
		this->bind(a, b, c, d, e, f, g, i);
		this->bind(i);
	}

	// column functions
	template <typename T>
	T					get(int iCol) const;
	void const			*getBlob(int iCol, int &size) const;
	unsigned char const	*getText(int iCol, int &size) const;
	void const			*getText16(int iCol, int &size) const;
	int					getInt(int iCol) const;
	double				getDouble(int iCol) const;
	int64_t				getInt64(int iCol) const;
	int					getCount() const;
	ColumnType			getType(int iCol) const;


	template <typename A, typename B>
	void				get(A &a, B &b) const
	{
		a = this->get<A>(0);
		b = this->get<B>(1);
	}
	template <typename A, typename B, typename C>
	void				get(A &a, B &b, C &c) const
	{
		this->get(a, b);
		c = this->get<C>(2);
	}
	template <typename A, typename B, typename C, typename D>
	void				get(A &a, B &b, C &c, D &d) const
	{
		this->get(a, b, c);
		d = this->get<D>(3);
	}
	template <typename A, typename B, typename C, typename D, typename E>
	void				get(A &a, B &b, C &c, D &d, E &e) const
	{
		this->get(a, b, c, d);
		e = this->get<E>(4);
	}
	template <typename A, typename B, typename C, typename D, typename E, typename F>
	void				get(A &a, B &b, C &c, D &d, E &e, F &f) const
	{
		this->get(a, b, c, d, e);
		f = this->get<F>(5);
	}
	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	void				get(A &a, B &b, C &c, D &d, E &e, F &f, G &g) const
	{
		this->get(a, b, c, d, e, f);
		g = this->get<G>(6);
	}
	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
	void				get(A &a, B &b, C &c, D &d, E &e, F &f, G &g, H &h) const
	{
		this->get(a, b, c, d, e, f, g);
		h = this->get<H>(7);
	}
	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
	void				get(A &a, B &b, C &c, D &d, E &e, F &f, G &g, H &h, I &i) const
	{
		this->get(a, b, c, d, e, f, g, h);
		i = this->get<I>(8);
	}

	bool				isFinished() const;
	std::string			getLastError() const;

private:
	SQLiteRequest();
	SQLiteRequest(SQLiteRequest const &other);
	SQLiteRequest		&operator=(SQLiteRequest const &other);

	void				finalize();

	sqlite3				*_db;
	sqlite3_stmt		*_request;
	bool				_finished;
	int					_error;
	int					_index;
};

// bind template
template <>
int				SQLiteRequest::bind(int value);
template <>
int				SQLiteRequest::bind(float value);
template <>
int				SQLiteRequest::bind(double value);
template <>
int				SQLiteRequest::bind(short value);
template <>
int				SQLiteRequest::bind(char value);
template <>
int				SQLiteRequest::bind(bool value);
template <>
int				SQLiteRequest::bind(int64_t value);
template <>
int				SQLiteRequest::bind(unsigned int value);
template <>
int				SQLiteRequest::bind(unsigned short value);
template <>
int				SQLiteRequest::bind(unsigned char value);
template <>
int				SQLiteRequest::bind(uint64_t value);
template <>
int				SQLiteRequest::bind(std::string value);
template <>
int				SQLiteRequest::bind(std::string const &value);
template <>
int				SQLiteRequest::bind(std::wstring value);
template <>
int				SQLiteRequest::bind(std::wstring const &value);

// get template
template <>
int				SQLiteRequest::get(int iCol) const;
template <>
float			SQLiteRequest::get(int iCol) const;
template <>
double			SQLiteRequest::get(int iCol) const;
template <>
short			SQLiteRequest::get(int iCol) const;
template <>
char			SQLiteRequest::get(int iCol) const;
template <>
bool			SQLiteRequest::get(int iCol) const;
template <>
int64_t			SQLiteRequest::get(int iCol) const;
template <>
unsigned int	SQLiteRequest::get(int iCol) const;
template <>
unsigned short	SQLiteRequest::get(int iCol) const;
template <>
unsigned char	SQLiteRequest::get(int iCol) const;
template <>
uint64_t		SQLiteRequest::get(int iCol) const;
template <>
std::string		SQLiteRequest::get(int iCol) const;
template <>
std::wstring	SQLiteRequest::get(int iCol) const;