#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h>
#include <vector>
#include <iomanip>

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")

using namespace std;


class Block {
public:
    int index;
    string data;
    string prevHash;
    string hash;

    Block(int idx, const string& data, const string& prevHash)
        : index(idx), data(data), prevHash(prevHash), hash(calculateHash()) {
    }

private:
    string calculateHash() const {
        stringstream ss;
        ss << index << data << prevHash;

        string input = ss.str();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

        stringstream hash_ss;
        for (unsigned char i : hash) {
            hash_ss << hex << setw(2) << setfill('0') << (int)i;
        }

        return hash_ss.str();
    }
};


class Blockchain {
public:
    Blockchain() {
        chain.push_back(createGenesisBlock());
    }

    void addBlock(const string& data) {
        const Block& prevBlock = chain.back();
        chain.push_back(Block(prevBlock.index + 1, data, prevBlock.hash));
    }

    void printChain() const {
        for (const auto& block : chain) {
            cout << "Block " << block.index << ": " << block.hash << endl;
        }
    }

private:
    vector<Block> chain;
    Block createGenesisBlock() {
        return Block(0, "GenesisBlock", "0");
    }
};

int main() {
    Blockchain blockchain;

    blockchain.addBlock("FirstBlock");
    blockchain.addBlock("SecondBlock");

    blockchain.printChain();

    return 0;
}
