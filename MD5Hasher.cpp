#include "MD5Hasher.h"

#include <format>


/*// Pimpl-структура для скрытия деталей реализации
struct MD5Hasher::Impl {
	EVP_MD_CTX* context;

	Impl() : context(EVP_MD_CTX_new()) {
		if (!context) {
			throw std::runtime_error("Failed to create EVP_MD_CTX");
		}
	}

	~Impl() {
		if (context) {
			EVP_MD_CTX_free(context);
		}
	}
};

MD5Hasher::MD5Hasher() : pimpl(std::make_unique<Impl>()) {}

MD5Hasher::~MD5Hasher() = default;
*/

std::string MD5Hasher::hash(const std::string& input) {

	EVP_MD_CTX* context = EVP_MD_CTX_new();						// выделяем память 
	const EVP_MD* md = EVP_md5();								// объект с указателем на алгоритм MD5
	unsigned char digest[EVP_MAX_MD_SIZE];						// массив для хранения полученного бинарного MD5 хеша
	unsigned int digestLength;									// длина конечного бинарного массива (16 байт для алгоритма md5)

	EVP_DigestInit_ex(context, md, nullptr);					// функция инициализации хеширования; аргументы: указатель класса, указатель на алгоритм, 
	EVP_DigestUpdate(context, input.c_str(), input.size());		// функция добавления данных хеширования; аругменты: указатель класса, , длина начальной строки в байтах
	EVP_DigestFinal_ex(context, digest, &digestLength);			// функция получения результата; аргументы: указатель класса, буфер для результата, указательЮ куда запишется длина хеша
	EVP_MD_CTX_free(context);									// освобждаем память после хеширования


	std::stringstream ss;
	for (size_t i = 0; i < digestLength; ++i) {
		ss << std::format("{:02x}", digest[i]);					// записываем массив байтов в строку
	}

	return ss.str();
}
