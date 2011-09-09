#ifndef PSTRING_H_
#define PSTRING_H_

#include "../pbytearray/pbytearray.h"


class PString
{
public:
	typedef PByteArray::value_type      value_type;
	typedef PByteArray::size_type       size_type;
	typedef PByteArray::iterator 		iterator;
	typedef PByteArray::const_iterator  const_iterator;
	typedef PByteArray::CopyPolicy      CopyPolicy;
	typedef PByteArray::ReferencePolicy ReferencePolicy;

public:
	/********** Constructors **********/
	PString() :
		m_string()
	{}
	PString(const PString &other) :
		m_string(other.m_string)
	{}
	PString(const value_type *data, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite) :
		m_string(data, policy)
	{}
	PString(const PByteArray &buffer) :
		m_string(buffer)
	{}
	template <typename T, size_type S>
	PString(const T (&string)[S]) :
		m_string(string)
	{}
	template <typename T, size_type S>
	PString(const T (&data)[S],
			const value_type *string1 = NULL,
			const value_type *string2 = NULL,
			const value_type *string3 = NULL,
			const value_type *string4 = NULL,
			const value_type *string5 = NULL,
			const value_type *string6 = NULL,
			const value_type *string7 = NULL,
			const value_type *string8 = NULL,
			const value_type *string9 = NULL,
			const value_type *string10 = NULL) :
		m_string(data, string1, string2, string3, string4, string5, string6, string7, string8, string9, string10)
	{}
	PString(const std::string &string, CopyPolicy::Type policy = CopyPolicy::CreateCopy) :
		m_string(string, policy)
	{}
	PString(value_type *string, size_type len, ReferencePolicy::Type policy = ReferencePolicy::Reference) :
		m_string(string, len, policy)
	{}
	PString(const value_type *string, size_type len, CopyPolicy::Type policy = CopyPolicy::CopyOnWrite) :
		m_string(string, len, policy)
	{}


	/********** Operators **********/
	value_type at(size_type index) const { return m_string.at(index); }
	value_type operator[](size_type index) const { return m_string[index]; }
	bool operator==(const PString &other) const { return m_string == other.m_string; }
	bool operator!=(const PString &other) const { return m_string != other.m_string; }
	bool operator<(const PString &other) const { return m_string < other.m_string; }
	void operator=(const PString &other) { m_string = other.m_string; }
	template <typename T, size_type S>
	void operator=(const T (&data)[S]) { m_string = data; }

	/********** Operations.Data **********/
	size_type size() const { return m_string.size(); }
	const value_type *c_str() const { return m_string.c_str(); }
	const value_type *data() const { return m_string.data(); }
	bool isEmpty() const { return m_string.isEmpty(); }

	/********** Operations.Compare **********/
	bool endsWith(const PString &other) const { return m_string.endsWith(other.m_string); }
	bool isEqual(const PString &other) const { return m_string.isEqual(other.m_string); }

	/********** Operations.Modification **********/
	PString &append(const value_type *string) { m_string.append(string); return *this; }
	PString &append(const PString &string) { m_string.append(string.m_string); return *this; }
	PString &append(const value_type *string, size_type size) { m_string.append(string, size); return *this; }

	/********** Operations.Conversion **********/
	int toInt(bool *ok = 0) const { return m_string.toInt(ok); }
	static PString fromInt(int value) { return PByteArray::fromInt(value); }

	/********** Iterators **********/
	iterator begin() { return m_string.begin(); }
	iterator end() { return m_string.end(); }
	const_iterator constBegin() const { return m_string.constBegin(); }
	const_iterator constEnd() const { return m_string.constEnd(); }

private:
	PByteArray m_string;
};

#endif /* PSTRING_H_ */
