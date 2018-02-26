//
// Created by prasanga on 2/19/18.
//

#ifndef MINIP_BLOCK_H
#define MINIP_BLOCK_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include "sha256.h"
#include "resources.h"

using namespace std;

// On request of blockchain, iterate the loop of blockchain,
// run the sendData function over TCP.

class Block{
private:
public:
    std::string _sender, _receiver, _data, _prevHash, _currentHash;
    Block(std::string sender, std::string receiver, std::string data, std::string prevHash){
        _sender = sender;
        _receiver = receiver;
        _data = data;
        _prevHash = prevHash;
        _currentHash = calculateHash();
    }
    Block(std::string dataToParse){
        std::stringstream ss(dataToParse);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        _sender = vstrings[0];
        _receiver = vstrings[1];
        _data = vstrings[2];
        _prevHash = vstrings[3];
        _currentHash = vstrings[4];
    }
    std::string calculateHash(){
        return picosha2::hash256_hex_string(getStringFormToCalculate());
    }
    std::string getStringFormToCalculate(){
        return (_sender+" "+_receiver+" "+_data+" " + _prevHash);
    }
    std::string getStringFormToSend(){
        return getStringFormToCalculate() + " " + _currentHash;
    }
    bool validate(){
        return (picosha2::hash256_hex_string(getStringFormToCalculate()) == _currentHash);
    }
    std::string getData(){
        if(_receiver == myIP){
            return decryptMessage(_data, _receiver, _sender);
        }
    }
    static Block createGenesisBlock(){
        return *(new Block("0.0.0.0", "0.0.0.0", "And then god said, let there be light", "0000000000000000000000000000000000000000000000000000000000000000"));
    }
    ~Block(){

    }
};

std::vector<Block> blockChain;


#endif

