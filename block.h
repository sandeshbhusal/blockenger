#ifndef GTKMMPROJECT_BLOCK_H
#define GTKMMPROJECT_BLOCK_H
#include <iostream>
#include <vector>

namespace bitenger_blockchain{
    class Block{
    private:
        std::string _sender;
        std::string _receiver;
        std::string _prevHash;
        std::string _data;

    public:
        long long _size;
        std::string _genesisHash;
        std::string _currentHash;

        Block(){
            _size = sizeof(*this);
        }

        bool verify(){

        };
        bool checkRecipient(const std::string id){
            return (id==this->_receiver);
        }
        std::string getData(std::string privateKey){
            if(checkRecipient(privateKey))
                return this->_data;
            return nullptr;
        }
    };
    struct Node{
        Block data;
        Node *nextNode;
    };
    std::vector<struct Node> blockChain;
}
#endif