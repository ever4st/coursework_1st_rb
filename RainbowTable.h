#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class RainbowTable {
public:
	RainbowTable(size_t chain_length, size_t num_chains, size_t checkpoint_step = 100000);

	void generate(const std::vector<std::string>& start_points);
	bool saveToFile(const std::string& filename) const;
	bool loadFormFile(const std::string& filename);
	
	std::string crackHash(const std::string& target_hash) const;

private:
	struct Chain {
		std::string start;
		std::string end;
		std::vector<std::string> checkpoints;
	};

	size_t chain_length;
	size_t num_chains;
	size_t checkpoint_step;

	std::vector<Chain> chains;
	std::unordered_map<std::string, std::string> endpoint_map;
	std::unordered_map<std::string, size_t> checkpoint_map;


	std::string reduce(const std::string& hash, size_t step) const;
	std::string restoreChain(const std::string& start, const std::string& target_hash) const;
	
};


