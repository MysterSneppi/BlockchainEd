#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h>
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

class Block {
public:
    int index;
    std::string data;
    std::string prevHash;
    std::string hash;

    Block(int idx, const std::string& data, const std::string& prevHash)
        : index(idx), data(data), prevHash(prevHash), hash(calculateHash()) {
    }
    
private:
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << data << prevHash;

        std::string input = ss.str();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

        std::stringstream hash_ss;
        for (unsigned char i : hash) {
            hash_ss << std::hex << (int)i;
        }

        return hash_ss.str();
    }
};

int main() {
    Block genesisBlock(0, "Genesis Block", "0");
    Block secondBlock(1, "Second Block", genesisBlock.hash);

    std::cout << "Genesis Block Hash: " << genesisBlock.hash << std::endl;
    std::cout << "Second Block Hash: " << secondBlock.hash << std::endl;

    return 0;
}
