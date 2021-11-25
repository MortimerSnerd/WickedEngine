#pragma once
#include "CommonInclude.h"
#include <stdint.h>

#include <string>
#include <vector>

class wiArchive
{
private:
	uint64_t version = 0; // the version number is used for maintaining backwards compatibility with earlier archive versions
	bool readMode = false; // archive can be either read or write mode, but not both
	size_t pos = 0; // position of the next memory operation, relative to the data's beginning
	std::vector<uint8_t> DATA; // data suitable for read/write operations
	const uint8_t* data_ptr = nullptr; // this can either be a memory mapped pointer (read only), or the DATA's pointer

	std::string fileName; // save to this file on closing if not empty
	std::string directory; // the directory part from the fileName

	void CreateEmpty(); // creates new archive in write mode

public:
	// Create empty arhive for writing
	wiArchive();
	wiArchive(const wiArchive&) = default;
	wiArchive(wiArchive&&) = default;
	// Create archive from a file.
	//	If readMode == true, the whole file will be loaded into the archive in read mode
	//	If readMode == false, the file will be written when the archive is destroyed or Close() is called
	wiArchive(const std::string& fileName, bool readMode = true);
	// Creates a memory mapped archive in read mode
	wiArchive(const uint8_t* data);
	~wiArchive() { Close(); }

	wiArchive& operator=(const wiArchive&) = default;
	wiArchive& operator=(wiArchive&&) = default;

	const uint8_t* GetData() const { return data_ptr; }
	constexpr uint64_t GetVersion() const { return version; }
	constexpr bool IsReadMode() const { return readMode; }
	// This can set the archive into either read or write mode, and it will reset it's position
	void SetReadModeAndResetPos(bool isReadMode);
	// Check if the archive has any data
	bool IsOpen() const { return data_ptr != nullptr; };
	// Close the archive.
	//	If it was opened from a file in write mode, the file will be written at this point
	//	The data will be deleted, the archive will be empty after this
	void Close();
	// Write the archive contents to a specific file
	//	The archive data will be written starting from the beginning, to the current position
	bool SaveFile(const std::string& fileName);
	// Write the archive contents into a C++ header file
	//	dataName : it will be the name of the byte data array in the header, that can be memory mapped
	bool SaveHeaderFile(const std::string& fileName, const std::string& dataName);
	// If the archive was opened from a file, this will return the file's directory
	const std::string& GetSourceDirectory() const;
	// If the archive was opened from a file, this will return the file's name
	//	The file's name will include the directory as well
	const std::string& GetSourceFileName() const;

	// It could be templated but we have to be extremely careful of different datasizes on different platforms
	// because serialized data should be interchangeable!
	// So providing exact copy operations for exact types enforces platform agnosticism

	// Write operations
	inline wiArchive& operator<<(bool data)
	{
		_write((uint32_t)(data ? 1 : 0));
		return *this;
	}
	inline wiArchive& operator<<(char data)
	{
		_write((int8_t)data);
		return *this;
	}
	inline wiArchive& operator<<(unsigned char data)
	{
		_write((uint8_t)data);
		return *this;
	}
	inline wiArchive& operator<<(int data)
	{
		_write((int64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(unsigned int data)
	{
		_write((uint64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(long data)
	{
		_write((int64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(unsigned long data)
	{
		_write((uint64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(long long data)
	{
		_write((int64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(unsigned long long data)
	{
		_write((uint64_t)data);
		return *this;
	}
	inline wiArchive& operator<<(float data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(double data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT2& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT3& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT4& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT3X3& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT4X3& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMFLOAT4X4& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMUINT2& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMUINT3& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const XMUINT4& data)
	{
		_write(data);
		return *this;
	}
	inline wiArchive& operator<<(const std::string& data)
	{
		uint64_t len = (uint64_t)data.length();
		_write(len);
		_write(*data.data(), len);
		return *this;
	}
	template<typename T>
	inline wiArchive& operator<<(const std::vector<T>& data)
	{
		// Here we will use the << operator so that non-specified types will have compile error!
		(*this) << data.size();
		for (const T& x : data)
		{
			(*this) << x;
		}
		return *this;
	}

	// Read operations
	inline wiArchive& operator>>(bool& data)
	{
		uint32_t temp;
		_read(temp);
		data = (temp == 1);
		return *this;
	}
	inline wiArchive& operator>>(char& data)
	{
		int8_t temp;
		_read(temp);
		data = (char)temp;
		return *this;
	}
	inline wiArchive& operator>>(unsigned char& data)
	{
		uint8_t temp;
		_read(temp);
		data = (unsigned char)temp;
		return *this;
	}
	inline wiArchive& operator>>(int& data)
	{
		int64_t temp;
		_read(temp);
		data = (int)temp;
		return *this;
	}
	inline wiArchive& operator>>(unsigned int& data)
	{
		uint64_t temp;
		_read(temp);
		data = (unsigned int)temp;
		return *this;
	}
	inline wiArchive& operator>>(long& data)
	{
		int64_t temp;
		_read(temp);
		data = (long)temp;
		return *this;
	}
	inline wiArchive& operator>>(unsigned long& data)
	{
		uint64_t temp;
		_read(temp);
		data = (unsigned long)temp;
		return *this;
	}
	inline wiArchive& operator>>(long long& data)
	{
		int64_t temp;
		_read(temp);
		data = (long long)temp;
		return *this;
	}
	inline wiArchive& operator>>(unsigned long long& data)
	{
		uint64_t temp;
		_read(temp);
		data = (unsigned long long)temp;
		return *this;
	}
	inline wiArchive& operator>>(float& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(double& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT2& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT3& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT4& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT3X3& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT4X3& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMFLOAT4X4& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMUINT2& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMUINT3& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(XMUINT4& data)
	{
		_read(data);
		return *this;
	}
	inline wiArchive& operator>>(std::string& data)
	{
		uint64_t len;
		_read(len);
		data.resize(len);
		_read(*data.data(), len);
		if (!data.empty() && GetVersion() < 73)
		{
			// earlier versions of archive saved the strings with 0 terminator
			data.pop_back();
		}
		return *this;
	}
	template<typename T>
	inline wiArchive& operator>>(std::vector<T>& data)
	{
		// Here we will use the >> operator so that non-specified types will have compile error!
		size_t count;
		(*this) >> count;
		data.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			(*this) >> data[i];
		}
		return *this;
	}



private:

	// This should not be exposed to avoid misaligning data by mistake
	// Any specific type serialization should be implemented by hand
	// But these can be used as helper functions inside this class

	// Write data using memory operations
	template<typename T>
	inline void _write(const T& data, uint64_t count = 1)
	{
		assert(!readMode);
		assert(!DATA.empty());
		if (count == 0)
			return;
		size_t _size = (size_t)(sizeof(data)*count);
		size_t _right = pos + _size;
		if (_right > DATA.size())
		{
			DATA.resize(_right * 2);
			data_ptr = DATA.data();
		}
		std::memcpy(DATA.data() + pos, &data, _size);
		pos = _right;
	}

	// Read data using memory operations
	template<typename T>
	inline void _read(T& data, uint64_t count = 1)
	{
		assert(readMode);
		assert(data_ptr != nullptr);
		if (count == 0)
			return;
		std::memcpy(&data, data_ptr + pos, (size_t)(sizeof(data)*count));
		pos += (size_t)(sizeof(data)*count);
	}
};

