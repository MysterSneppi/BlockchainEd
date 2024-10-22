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
    time_t timestamp;
    int nonce;

    Block(int idx, const string& data, const string& prevHash)
        : index(idx), data(data), prevHash(prevHash),timestamp(time(nullptr)),nonce(0), hash(calculateHash()) {
    }

    void mineBlock(int difficualty) {
        string target(difficualty, '0');
        while (hash.substr(0, difficualty) != target) {
            nonce++;
            hash = calculateHash();
        }
        cout << "Block mined: " << hash << " with nonce: " << nonce << endl;
    }




private:
    string calculateHash() const {
        stringstream ss;
        ss << index << data << prevHash << timestamp << nonce;

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
    Blockchain(int difficulty) : difficulty(difficulty) {
        chain.push_back(createGenesisBlock());
    }

    void addBlock(const string& data) {
        const Block& prevBlock = chain.back();
        Block newBlock(prevBlock.index + 1, data, prevBlock.hash);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    void printChain() const {
        for (const auto& block : chain) {
            cout << "Block " << block.index << " [Hash: " << block.hash << ", Nonce: " << block.nonce << "]" << endl;
        }
    }

private:
    vector<Block> chain;
    int difficulty;

    Block createGenesisBlock() {
        Block genesisBlock(0, "GenesisBlock", "0");
        genesisBlock.mineBlock(difficulty);
        return genesisBlock;
    }
};

int main() {
    int difficulty = 4;
    Blockchain blockchain(difficulty);

    blockchain.addBlock("FirstBlock");
    blockchain.addBlock("SecondBlock");

    blockchain.printChain();

    return 0;
}
