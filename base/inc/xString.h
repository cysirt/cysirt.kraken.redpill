#pragma once

#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>

#include "xAssert.h"
#include "xDefinition.h"
#include "xMemory.h"



class xString
{
private:
	struct Data
	{
		int32_t countRef;

		uint32_t sizeData;
		
		uint32_t sizeMem;
		
		xChar* _GetData()
		{
			return (xChar*) (this + 1);
		}
	};

public:
	xString()
	{
		Init();
	}

	xString(const xString& s)
	{
		X_ASSERT(s.GetData()->countRef != 0);

		if (s.GetData()->countRef >= 0)
		{
			X_ASSERT(s.GetData() != s_dataTemp);

			m_string = s.m_string;

			GetData()->countRef++;
		}
		else
		{
			Init();

			*this = s.m_string;
		}
	}

	xString(const xChar s, const uint32_t repeat = 1)
	{
		Init();

		if (repeat >= 1)
		{
			if (Alloc(repeat))
			{
				xMemorySet(m_string, s, repeat);
			}
		}
	}

	xString(const xChar* s)
	{
		Init();

		const uint32_t length = GetSafeLength(s);

		if (length)
		{
			if (Alloc(length))
			{
				xMemoryCopy(m_string, s, length * sizeof(xChar));
			}
		}
	}

	xString(const xChar* s, const uint32_t length)
	{
		Init();

		if (length)
		{
			if (Alloc(length))
			{
				xMemoryCopy(m_string, s, length * sizeof(xChar));
			}
		}
	}

	xString(const uint8_t* buffer)
	{
		Init();

		*this = (const xChar*) buffer;
	}

	~xString()
	{
		if (GetData() != s_dataTemp)
		{
			if ((--(GetData()->countRef)) <= 0)
			{
				xMemoryFree(GetData());
			}
		}
	}

public:
	uint32_t GetLength() const
	{
		return GetData()->sizeData;
	}

	bool IsEmpty() const
	{
		return !GetData()->sizeData;
	}

	void Empty()
	{
		if (!GetData()->sizeData)
		{
			return;
		}

		if (GetData()->countRef >= 0)
		{
			Release();
		}
		else
		{
			*this = L"";
		}

		X_ASSERT(GetData()->sizeData == 0);

		X_ASSERT(GetData()->countRef < 0 || GetData()->sizeMem == 0);
	}

	xChar GetAt(const uint32_t index) const
	{
		X_ASSERT(index < GetData()->sizeData);

		if (index >= GetData()->sizeData)
		{
			return xChar('\0');
		}

		return m_string[index];
	}

	xChar operator[](const uint32_t index) const
	{
		return GetAt(index);
	}

	void SetAt(const uint32_t index, const xChar s)
	{
		X_ASSERT(index < GetData()->sizeData);

		if (index >= GetData()->sizeData)
		{
			return;
		}

		Copy();

		m_string[index] = s;
	}

	operator const xChar*() const
	{
		return m_string;
	}

	xString& operator=(const xString& s)
	{
		if (m_string != s.m_string)
		{
			if ((GetData()->countRef < 0 && GetData() != s_dataTemp) || s.GetData()->countRef < 0)
			{
				AssignCopy(s.GetData()->sizeData, s.m_string);
			}
			else
			{
				Release();

				X_ASSERT(s.GetData() != s_dataTemp);

				m_string = s.m_string;

				(GetData()->countRef)++;
			}
		}

		return *this;
	}

	xString& operator=(const xChar s)
	{
		AssignCopy(1, &s);

		return *this;
	}

	xString& operator=(const xChar* s)
	{
		X_ASSERT(!s || IsValid(s));

		AssignCopy(GetSafeLength(s), s);

		return *this;
	}

	xString& operator=(const uint8_t* buffer)
	{
		*this = (const xChar*) buffer;

		return *this;
	}

	xString& operator+=(const xString& s)
	{
		ConcatInPlace(s.GetData()->sizeData, s.m_string);

		return *this;
	}

	xString& operator+=(const xChar s)
	{
		ConcatInPlace(1, &s);

		return *this;
	}

	xString& operator+=(const xChar* s)
	{
		X_ASSERT(!s || IsValid(s));

		ConcatInPlace(GetSafeLength(s), s);

		return *this;
	}

	friend xString __X_STDCALL operator+(const xString& s1, const xString& s2)
	{
		xString s;

		s.ConcatCopy(s1.GetData()->sizeData, s1.m_string, s2.GetData()->sizeData, s2.m_string);

		return s;
	}

	friend xString __X_STDCALL operator+(const xString& s1, const xChar s2)
	{
		xString s;

		s.ConcatCopy(s1.GetData()->sizeData, s1.m_string, 1, &s2);

		return s;
	}

	friend xString __X_STDCALL operator+(const xChar s1, const xString& s2)
	{
		xString s;

		s.ConcatCopy(1, &s1, s2.GetData()->sizeData, s2.m_string);

		return s;
	}

	friend xString __X_STDCALL operator+(const xString& s1, const xChar* s2)
	{
		X_ASSERT(!s2 || IsValid(s2));

		xString s;

		s.ConcatCopy(s1.GetData()->sizeData, s1.m_string, GetSafeLength(s2), s2);

		return s;
	}

	friend xString __X_STDCALL operator+(const xChar* s1, const xString& s2)
	{
		X_ASSERT(!s1 || IsValid(s1));

		xString s;

		s.ConcatCopy(GetSafeLength(s1), s1, s2.GetData()->sizeData, s2.m_string);

		return s;
	}

	int32_t Compare(const xChar* s) const
	{
		return xCharCompare(m_string, s);
	}

	int32_t CompareNoCase(const xChar* s) const
	{
		return xCharCompareNoCase(m_string, s);
	}

	xString Mid(const uint32_t first) const
	{
		return Mid(first, GetData()->sizeData - first);
	}

	xString Mid(const uint32_t first, const uint32_t count) const
	{
		uint32_t countCopy = count;

		if (first + count > GetData()->sizeData)
		{
			countCopy = GetData()->sizeData - first;
		}

		if (first > GetData()->sizeData)
		{
			countCopy = 0;
		}

		xString s;

		AllocCopy(s, count, first, 0);

		return s;
	}

	xString Left(const uint32_t count) const
	{
		uint32_t countCopy = count;

		if (count > GetData()->sizeData)
		{
			countCopy = GetData()->sizeData;
		}

		xString s;

		AllocCopy(s, count, 0, 0);

		return s;
	}

	xString Right(const uint32_t count) const
	{
		uint32_t countCopy = count;

		if (count > GetData()->sizeData)
		{
			countCopy = GetData()->sizeData;
		}

		xString s;

		AllocCopy(s, count, GetData()->sizeData - count, 0);

		return s;
	}

	void TrimLeft()
	{
		Copy();

		const xChar* s = m_string;

		while (iswspace(*s))
		{
			// TODO: Mulitbyte handling
			s++;
		}

		const uint32_t lengthData = GetData()->sizeData - (uint32_t) (uintptr_t) (s - m_string);

		xMemoryMove(m_string, s, (lengthData + 1) * sizeof(xChar));

		GetData()->sizeData = lengthData;
	}

	void TrimRight()
	{
		Copy();

		xChar* s = m_string;

		xChar* last = NULL;

		while (*s != '\0')
		{
			if (iswspace(*s))
			{
				if (!last)
				{
					last = s;
				}
			}
			else
			{
				last = NULL;
			}

			// TODO: Multibyte handling
			s++;
		}

		if (last)
		{
			*last = X_CHAR('\0');

			GetData()->sizeData = (int32_t) (uintptr_t) (last - m_string);
		}
	}

	void TrimLeft(const xChar s)
	{
		Copy();

		const xChar* temp = m_string;

		while (s == *temp)
		{
			// TODO: Multibyte handling
			temp++;
		}

		if (temp != m_string)
		{
			const uint32_t lengthData = GetData()->sizeData - (uint32_t) (uintptr_t) (temp - m_string);

			xMemoryMove(m_string, temp, (lengthData + 1) * sizeof(xChar));

			GetData()->sizeData = lengthData;
		}
	}

	void TrimLeft(const xChar* s)
	{
		if (!GetSafeLength(s))
		{
			return;
		}

		Copy();

		const xChar* temp = m_string;

		while (*temp != X_CHAR('\0'))
		{
			// TODO: Multibyte handling

			const xChar* next = temp++;

			if (next > temp + 1)
			{
				if (!CharNext(s, *temp, *(temp + 1)))
				{
					break;
				}
			}
			else
			{
				if (!wcsrchr(s, *temp))
				{
					break;
				}
			}

			temp = next;
		}

		if (temp != m_string)
		{
			const uint32_t lengthData = GetData()->sizeData - (uint32_t) (uintptr_t) (temp - m_string);

			xMemoryMove(m_string, temp, (lengthData + 1) * sizeof(xChar));

			GetData()->sizeData = lengthData;
		}
	}

	void TrimRight(const xChar s)
	{
		Copy();

		xChar* temp = m_string;

		xChar* last = NULL;

		while (*temp != X_CHAR('\0'))
		{
			if (*temp == s)
			{
				if (!last)
				{
					last = temp;
				}
			}
			else
			{
				last = NULL;
			}

			// TODO: Multibyte handling

			temp++;
		}

		if (last)
		{
			*last = xChar('\0');

			GetData()->sizeData = (int32_t) (uintptr_t) (last - m_string);
		}
	}

	void TrimRight(const xChar* s)
	{
		Copy();

		xChar* temp = m_string;

		xChar* last = NULL;

		while (*temp != X_CHAR('\0'))
		{
			// TODO: Multibyte handling

			xChar* next = temp++;

			if (next > temp + 1)
			{
				if (CharNext(s, *temp, *(temp + 1)))
				{
					if (!last)
					{
						last = temp;
					}
				}
				else
				{
					last = NULL;
				}
			}
			else
			{
				if (wcsrchr(s, *temp))
				{
					if (!last)
					{
						last = temp;
					}
				}
				else
				{
					last = NULL;
				}
			}

			temp = next;
		}

		if (last)
		{
			*last = xChar('\0');

			GetData()->sizeData = (uint32_t) (uintptr_t) (last - m_string);
		}
	}

	uint32_t Replace(const xChar replacee, const xChar replacer)
	{
		uint32_t count = 0;

		if (replacee != replacer)
		{
			Copy();

			xChar* temp = m_string;

			xChar* end = temp + GetData()->sizeData;

			while (temp < end)
			{
				if (*temp == replacee)
				{
					*temp = replacer;

					count++;
				}

				// TODO: Multibyte handling

				temp++;
			}
		}

		return count;
	}

	int32_t Replace(const xChar* replacee, const xChar* replacer)
	{
		const uint32_t length = GetSafeLength(replacee);

		if (!length)
		{
			return 0;
		}

		const uint32_t lengthReplace = GetSafeLength(replacer);

		uint32_t count = 0;

		xChar* start = m_string;

		xChar* end = m_string + GetData()->sizeData;

		xChar* target = NULL;

		while (start < end)
		{
			while ((target = (xChar*) wcsstr(start, replacee)))
			{
				start = target + length;
				count++;
			}

			start += xCharLength(start) + 1;
		}

		if (count > 0)
		{
			Copy();

			uint32_t lengthOld = GetData()->sizeData;
			
			const uint32_t lengthNew = lengthOld + (lengthReplace - length) * count;
			
			if (GetData()->sizeMem < lengthNew || GetData()->countRef > 1)
			{
				Data* data = GetData();

				xChar* s = m_string;

				if (!Alloc(lengthNew))
				{
					return -1;
				}

				xMemoryCopy(m_string, s, data->sizeData * sizeof(xChar));

				Release(data);
			}

			start = m_string;

			end = m_string + GetData()->sizeData;

			while (start < end)
			{
				while ((target = (xChar*) wcsstr(start, replacee)))
				{
					const uint32_t balance = lengthOld - ((uint32_t) (uintptr_t) (target - m_string) + length);

					xMemoryMove(target + lengthReplace, target + length, balance * sizeof(xChar));

					xMemoryCopy(target, replacer, lengthReplace * sizeof(xChar));

					start = target + lengthReplace;

					start[balance] = xChar('\0');

					lengthOld += (lengthReplace - length);
				}

				start += xCharLength(start) + 1;
			}

			X_ASSERT(m_string[lengthNew] == xChar('\0'));

			GetData()->sizeData = lengthNew;
		}

		return count;
	}

	int32_t Remove(const xChar s)
	{
		Copy();

		xChar* start = m_string;

		xChar* temp = m_string;

		while (start < (m_string + GetData()->sizeData))
		{
			if (*start != s)
			{
				*temp = *start;

				// TODO: Multibyte handling

				temp++;
			}

			// TODO: Multibyte handling

			start++;
		}

		*temp = xChar('\0');

		const uint32_t count = (uint32_t) (uintptr_t) (start - temp);

		GetData()->sizeData -= count;

		return count;
	}

	int32_t Insert(const uint32_t index, const xChar s)
	{
		Copy();

		uint32_t lengthNew = GetData()->sizeData;

		uint32_t indexInsert = index < lengthNew ? index : lengthNew;

		lengthNew++;

		if (GetData()->sizeMem < lengthNew)
		{
			Data* data = GetData();

			xChar* temp = m_string;

			if (!Alloc(lengthNew))
			{
				return -1;
			}

			xMemoryCopy(m_string, temp, (data->sizeData + 1) * sizeof(xChar));

			Release(data);
		}

		xMemoryMove(m_string + indexInsert + 1, m_string + indexInsert, (lengthNew - indexInsert) * sizeof(xChar));

		m_string[indexInsert] = s;

		GetData()->sizeData = lengthNew;

		return lengthNew;
	}

	int32_t Insert(const uint32_t index, const xChar* s)
	{
		uint32_t lengthInsert = GetSafeLength(s);

		uint32_t lengthNew = GetData()->sizeData;

		if (lengthInsert > 0)
		{
			Copy();

			const uint32_t indexInsert = index < lengthNew ? index : lengthNew;

			lengthNew += lengthInsert;

			if (GetData()->sizeMem < lengthNew)
			{
				Data* data = GetData();

				xChar* temp = m_string;

				if (!Alloc(lengthNew))
				{
					return -1;
				}

				xMemoryCopy(m_string, temp, (data->sizeData + 1) * sizeof(xChar));

				Release(data);
			}

			xMemoryMove(m_string + indexInsert + lengthInsert, m_string + indexInsert, (lengthNew - indexInsert - lengthInsert + 1) * sizeof(xChar));

			xMemoryCopy(m_string + indexInsert, s, lengthInsert * sizeof(xChar));

			GetData()->sizeData = lengthNew;
		}

		return lengthNew;
	}

	uint32_t Delete(const uint32_t index, const uint32_t count = 1)
	{
		uint32_t length = GetData()->sizeData;

		if (count > 0 && index < length)
		{
			const uint32_t countDelete =  (index + count) > length ? length - index : count;

			Copy();

			const uint32_t countCopy = length - (index + countDelete) + 1;

			xMemoryMove(m_string + index, m_string + index + countDelete, countCopy * sizeof(xChar));

			length -= count;

			GetData()->sizeData = length;
		}

		return length;
	}

	int32_t Find(const xChar s, const uint32_t start) const
	{
		X_ASSERT(start <= GetData()->sizeData);

		if (start > GetData()->sizeData)
		{
			return -1;
		}

		const xChar* temp = wcschr(m_string + start, (uint8_t) s);

		return temp ? (int32_t) (temp - m_string) : -1;
	}

	int32_t Find(const xChar s) const
	{
		return Find(s, 0);
	}

	int32_t ReverseFind(xChar s) const
	{
		const xChar* temp = wcsrchr(m_string, (uint8_t) s);

		return temp ? (int32_t) (temp - m_string) : -1;
	}

	int32_t FindOneOf(const xChar* charSet) const
	{
		X_ASSERT(IsValid(charSet));

		if (!IsValid(charSet))
		{
			return -1;
		}

		const xChar* temp = wcspbrk(m_string, charSet);

		return temp ? (int32_t) (temp - m_string) : -1;
	}

	int32_t Find(const xChar* sub, const uint32_t start) const
	{
		X_ASSERT(IsValid(sub));

		if (!IsValid(sub))
		{
			return -1;
		}

		if (start >= GetData()->sizeData)
		{
			return -1;
		}

		const xChar* temp = wcsstr(m_string + start, sub);

		return temp ? (int32_t) (temp - m_string) : -1;
	}

	int32_t Find(const xChar* sub) const
	{
		X_ASSERT(IsValid(sub));

		if (!IsValid(sub))
		{
			return -1;
		}

		return Find(sub, 0);
	}

	int32_t ReverseFind(const xChar* sub, int32_t start = -1, const bool noCase = false) const
	{
		X_ASSERT(sub);

		if (!sub)
		{
			return -1;
		}

		const int32_t length = GetLength();

		const int32_t lengthSub = (int32_t) xCharLength(sub);

		if (0 < lengthSub && 0 < length)
		{
			if (start == -1 || start >= length)
			{
				start = length - 1;
			}

			for (xChar* reverse = m_string + start; reverse >= m_string; reverse--)
			{
				if (noCase ? _wcsnicmp(sub, reverse, lengthSub) : wcsncmp(sub, reverse, lengthSub) == 0)
				{
					return (int32_t)(reverse - m_string);
				}
			}
		}

		return -1;
	}

	bool __X_CDECL Format(const xChar* format, ...)
	{
		X_ASSERT(IsValid(format));

		if (!IsValid(format))
		{
			return false;
		}

		va_list args;

		va_start(args, format);

		bool result = FormatV(format, args);

		va_end(args);

		return result;
	}

	bool FormatV(const xChar* format, va_list args)
	{
		X_ASSERT(IsValid(format));

		if (!IsValid(format))
		{
			return false;
		}

		uint32_t length = (uint32_t) ((2 * wcslen(format)) * sizeof(xChar));

		xChar* temp = GetBuffer(length);

		if (!temp)
		{
			return false;
		}

		while (length && ((uint32_t) _vsnwprintf(temp, length, format, args)) >= length)
		{
			ReleaseBuffer();

			length *= 2;

			if (!(temp = GetBuffer(length)))
			{
				return false;
			}
		}

		ReleaseBuffer();

		va_end(args);

		return true;
	}

	xChar* Clone() const
	{
		const uint32_t length = GetLength() + 1;

		xChar* temp = (xChar*) xMemoryAlloc(length * sizeof(xChar));

		if (!temp)
		{
			return NULL;
		}

		xCharCopyLength(temp, GetString(), length);

		return temp;
	}

	xChar* GetBuffer(const uint32_t lengthMin)
	{
		if (GetData()->countRef > 1 || lengthMin > GetData()->sizeMem)
		{
			Data* data = GetData();

			const uint32_t lengthOld = GetData()->sizeData;

			if (!Alloc(lengthMin > lengthOld ? lengthMin : lengthOld))
			{
				return NULL;
			}

			xMemoryCopy(m_string, data->_GetData(), (lengthOld + 1) * sizeof(xChar));

			GetData()->sizeData = lengthOld;

			Release(data);
		}

		X_ASSERT(GetData()->countRef <= 1);

		X_ASSERT(m_string);

		return m_string;
	}

	const xChar* GetString() const
	{
		X_ASSERT(m_string);

		return m_string;
	}

	xChar* GetString()
	{
		X_ASSERT(m_string);

		return m_string;
	}

	void ReleaseBuffer(int32_t length = -1)
	{
		Copy();

		uint32_t lengthNew = length == -1 ? (uint32_t) xCharLength(m_string) : (uint32_t) length ;

		X_ASSERT(lengthNew <= GetData()->sizeMem);

		GetData()->sizeData = lengthNew;

		m_string[lengthNew] = xChar('\0');
	}

	xChar* GetBufferSetLength(const uint32_t lengthNew)
	{
		if (!GetBuffer(lengthNew))
		{
			return NULL;
		}

		GetData()->sizeData = lengthNew;

		m_string[lengthNew] = xChar('\0');

		return m_string;
	}

	void Compact()
	{
		X_ASSERT(GetData()->sizeData <= GetData()->sizeMem);

		if (GetData()->sizeData != GetData()->sizeMem)
		{
			Data* data = GetData();

			if (Alloc(GetData()->sizeData))
			{
				xMemoryCopy(m_string, data->_GetData(), data->sizeData * sizeof(xChar));

				X_ASSERT(m_string[GetData()->sizeData] == xChar('\0'));

				Release(data);
			}
		}

		X_ASSERT(GetData());
	}

	xChar* LockBuffer()
	{
		xChar* temp = GetBuffer(0);

		if (temp)
		{
			GetData()->countRef = -1;
		}

		return temp;
	}

	void UnlockBuffer()
	{
		X_ASSERT(GetData()->countRef == -1);

		if (GetData() != s_dataTemp)
		{
			GetData()->countRef = 1;
		}
	}

	int32_t GetAllocLength() const
	{
		return GetData()->sizeMem;
	}

	static bool __X_STDCALL IsValid(const xChar* s, const int32_t length = -1)
	{
		return !!s;
	}

	void ToUpper()
	{
		const uint32_t length = GetLength();

		for (uint32_t ui = 0; ui < length; ui++)
		{
			SetAt(ui, towupper(GetAt(ui)));
		}
	}

	void ToLower()
	{
		const uint32_t length = GetLength();

		for (uint32_t ui = 0; ui < length; ui++)
		{
			SetAt(ui, towlower(GetAt(ui)));
		}
	}

private:
	xString::Data* GetData() const
	{
		return ((Data*) m_string) - 1;
	}

	void Init()
	{
		m_string = GetEmptyString().m_string;
	}

	bool Alloc(const uint32_t length)
	{
		X_ASSERT(length <= INT32_MAX - 1);

		if (!length)
		{
			Init();
		}
		else
		{
			Data* data = (Data*) xMemoryAlloc(sizeof(Data) + (length + 1) * sizeof(xChar));
			
			if (!data)
			{
				return false;
			}

			data->countRef = 1;

			data->_GetData()[length] = xChar('\0');

			data->sizeData = length;

			data->sizeMem = length;

			m_string = data->_GetData();
		}

		return true;
	}

	void AllocCopy(xString& s, const int32_t lengthCopy, const int32_t indexCopy, const int32_t lengthExtra) const
	{
		int32_t lengthNew = lengthCopy + lengthExtra;

		if (!lengthNew)
		{
			s.Init();
		}
		else
		{
			if (s.Alloc(lengthNew))
			{
				xMemoryCopy(s.m_string, m_string + indexCopy, lengthCopy * sizeof(xChar));
			}
		}
	}

	bool AllocWrite(const uint32_t length)
	{
		bool result = true;

		if (GetData()->countRef > 1 || length > GetData()->sizeMem)
		{
			Release();

			result = Alloc(length);
		}

		X_ASSERT(GetData()->countRef <= 1);

		return result;
	}

	void Copy()
	{
		if (GetData()->countRef > 1)
		{
			Data* data = GetData();

			Release();

			if (Alloc(data->sizeData))
			{
				xMemoryCopy(m_string, data->_GetData(), (data->sizeData + 1) * sizeof(xChar));
			}
		}

		X_ASSERT(GetData()->countRef <= 1);
	}

	void AssignCopy(const int32_t length, const xChar* s)
	{
		if (AllocWrite(length))
		{
			xMemoryCopy(m_string, s, length * sizeof(xChar));

			GetData()->sizeData = length;

			m_string[length] = '\0';
		}
	}

	bool ConcatCopy(const uint32_t length1, const xChar* s1, const uint32_t length2, const xChar* s2)
	{
		bool result = true;

		const uint32_t lengthNew = length1 + length2;

		if (lengthNew && (result = Alloc(lengthNew)))
		{
			xMemoryCopy(m_string, s1, length1 * sizeof(xChar));

			xMemoryCopy(m_string + length1, s2, length2 * sizeof(xChar));
		}

		return result;
	}

	void ConcatInPlace(const int32_t length, const xChar* psz)
	{
		if (!length)
		{
			return;
		}

		if (GetData()->countRef > 1 || GetData()->sizeData + length > GetData()->sizeMem)
		{
			Data* data = GetData();

			if (ConcatCopy(GetData()->sizeData, m_string, length, psz))
			{
				X_ASSERT(data);

				xString::Release(data);
			}
		}
		else
		{
			xMemoryCopy(m_string + GetData()->sizeData, psz, length * sizeof(xChar));

			GetData()->sizeData += length;

			X_ASSERT(GetData()->sizeData <= GetData()->sizeMem);

			m_string[GetData()->sizeData] = '\0';
		}
	}

	void Release()
	{
		if (GetData() != s_dataTemp)
		{
			X_ASSERT(GetData()->countRef != 0);

			if (--(GetData()->countRef) <= 0)
			{
				xMemoryFree(GetData());
			}

			Init();
		}
	}

	static uint32_t __X_STDCALL GetSafeLength(const xChar* psz)
	{
		return psz ? (uint32_t) xCharLength(psz) : 0;
	}

	static void __X_STDCALL Release(Data* data)
	{
		if (data != s_dataTemp)
		{
			X_ASSERT(data->countRef != 0);

			if (--(data->countRef) <= 0)
			{
				xMemoryFree(data);
			}
		}
	}

	static const xString& __X_STDCALL GetEmptyString()
	{
		return *(xString*) &s_stringTemp;
	}

	static const xChar* CharNext(const xChar* s1, const xChar s2, const xChar s3)
	{
		X_ASSERT(IsValid(s1));

		if (!IsValid(s1))
		{
			return NULL;
		}

		const xChar* temp = NULL;

		while (*s1 != 0)
		{
			if (*s1 == s2 && *(s1 + 1) == s3)
			{
				temp = s1;
				break;
			}

			// TODO: Multibyte handling

			s1++;
		}

		return temp;
	}

protected:
	xChar* m_string;

private:
	static int32_t s_dataInit[];
	static const xChar* s_stringTemp;
	static Data* s_dataTemp;
};

__X_SELECTANY int32_t xString::s_dataInit[] = {-1, 0, 0, 0};
__X_SELECTANY const xChar* xString::s_stringTemp = (const xChar*) (((uint8_t*) &s_dataInit) + sizeof(xString::Data));
__X_SELECTANY xString::Data* xString::s_dataTemp = (xString::Data*) &s_dataInit;



static bool __X_STDCALL operator==(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) == 0;
}



static bool __X_STDCALL operator==(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) == 0;
}



static bool __X_STDCALL operator==(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) == 0;
}

static bool __X_STDCALL operator!=(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) != 0;
}



static bool __X_STDCALL operator!=(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) != 0;
}



static bool __X_STDCALL operator!=(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) != 0;
}



static bool __X_STDCALL operator<(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) < 0;
}



static bool __X_STDCALL operator<(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) < 0;
}



static bool __X_STDCALL operator<(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) > 0;
}



static bool __X_STDCALL operator>(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) > 0;
}



static bool __X_STDCALL operator>(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) > 0;
}



static bool __X_STDCALL operator>(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) < 0;
}



static bool __X_STDCALL operator<=(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) <= 0;
}



static bool __X_STDCALL operator<=(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) <= 0;
}



static bool __X_STDCALL operator<=(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) >= 0;
}



static bool __X_STDCALL operator>=(const xString& s1, const xString& s2)
{
	return s1.Compare(s2) >= 0;
}



static bool __X_STDCALL operator>=(const xString& s1, const xChar* s2)
{
	return s1.Compare(s2) >= 0;
}



static bool __X_STDCALL operator>=(const xChar* s1, const xString& s2)
{
	return s2.Compare(s1) <= 0;
}

