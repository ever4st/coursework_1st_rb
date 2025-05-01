#pragma once

#include <openssl/evp.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <memory>

class MD5Hasher {
public:
	MD5Hasher() = default;
	~MD5Hasher() = default;

	static std::string hash(const std::string& value);

	template <typename T>
	static std::string hash(const T& value) {
		return hash(toString(value));
	}

	// Удаляем копирование и присваивание
	MD5Hasher(const MD5Hasher&) = delete;
	MD5Hasher& operator=(const MD5Hasher&) = delete;

private:
	template <typename T>
	static std::string toString(const T& value) {
		std::ostringstream oss;
		oss << value;

		return oss.str();
	}

	static std::string toString(const std::string& value) {
		return value;
	}

	/*struct Impl; // Предварительное объявление Pimpl
	std::unique_ptr<Impl> pimpl; */
};

