#include "RainbowTable.h"
#include "MD5Hasher.h"

#include <fstream>
#include <sstream>

RainbowTable::RainbowTable(size_t chain_length, size_t num_chains, size_t checkpoint_step)
	: chain_length(chain_length), num_chains(num_chains), checkpoint_step(checkpoint_step) {
};

void RainbowTable::generate(const std::vector<std::string>& start_points) {
	chains.clear();
	endpoint_map.clear();
	checkpoint_map.clear();

	MD5Hasher hasher;

	for (const auto& start : start_points) {

		Chain chain;
		chain.start = start;
		std::string current = start;


		for (size_t step = 0; step < chain_length; ++step) {
			std::string hash = hasher.hash(current);
			current = reduce(hash, step);

			if (!(step % checkpoint_step)) {
				chain.checkpoints.push_back(current);
			}
		}

		chain.end = current;
		chains.push_back(chain);
		endpoint_map[chain.end] = chain.start;
		checkpoint_map[chain.checkpoints.back()] = chains.size() - 1;


		chains.push_back({ start, current });
		endpoint_map[current] = start;
	}
}

std::string RainbowTable::crackHash(const std::string& target_hash) const {
	MD5Hasher hasher;
	std::string current = target_hash;

	for (size_t step = 0; step < chain_length; ++step) {
		current = reduce(hasher.hash(current), step);

		if (auto it = endpoint_map.find(current); it != endpoint_map.end()) {
			std::string found = restoreChain(it->second, target_hash);
			if (!found.empty()) {
				return found;
			}

		}

		if (!(step % checkpoint_step)) {
			if (auto it = checkpoint_map.find(current); it != checkpoint_map.end()) {
				std::string found = restoreChain(chains[it->second].start, target_hash);

				if (!found.empty()) {
					return found;
				}
			}
		}
	}

	return "cannot crack hash";
}

std::string RainbowTable::reduce(const std::string& hash, size_t step) const {
	uint64_t num = 0;

	for(size_t i = 0; i < 8 && i < hash.size(); ++i) {
		num = (num << 8) | static_cast<uint64_t>(hash[i]);
	}

	num += step;

	const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
	std::string result;

	for (int i = 0; i < 8; ++i) {
		result += alphabet[num % alphabet.size()];
		num /= alphabet.size();
	}

	return result;
}

std::string RainbowTable::restoreChain(const std::string& start, const std::string& target_hash) const {
	MD5Hasher hasher;
	std::string current = start;

	for (size_t step = 0; step < chain_length; ++step) {
		std::string hash = hasher.hash(current);

		if (hash == target_hash) {
			return current;
		}

		current = reduce(hash, step);
	}

	return "";
}

bool RainbowTable::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file) return false;

	for (const auto& chain : chains) {
		file << chain.start << " " << chain.end << " ";

		for (const auto& cp : chain.checkpoints) {
			file << cp << " ";
		}
	}

	return true;

}

bool RainbowTable::loadFormFile(const std::string& filename) {
	std::fstream file(filename);
	if (!file) return false;

	chains.clear();
	endpoint_map.clear();
	checkpoint_map.clear();

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		Chain chain;

		iss >> chain.start >> chain.end;
		std::string cp;

		while (iss >> cp) {
			chain.checkpoints.push_back(cp);
		}

		chains.push_back(chain);
		endpoint_map[chain.end] = chain.start;
		if (!chain.checkpoints.empty()) {
			checkpoint_map[chain.checkpoints.back()] = chains.size() - 1;
		}
	}

	return !chains.empty();
}