//
// Created by prasanga on 2/19/18.
//

#ifndef MINIP_BLOCK_H
#define MINIP_BLOCK_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <regex>
#include "sha256.h"
#include "resources.h"
#include "time.h"

using namespace std;

// On request of blockchain, iterate the loop of blockchain,
// run the sendData function over TCP.

class Block{
private:
public:
    std::string _sender, _receiver, _data, _timestamp, _prevHash, _currentHash;
    Block(){};
    Block(std::string sender, std::string receiver, std::string data, std::string timestamp, std::string prevHash){
        _sender = sender;
        _receiver = receiver;
        for(int i=0; i<data.length(); i++)
            if(data.at(i) == ' ')
                data.at(i) = '~';
        _data = data;
        _prevHash = prevHash;
        _timestamp = timestamp;
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
        _timestamp = vstrings[3];
        _prevHash = vstrings[4];
        _currentHash = vstrings[5];
    }
    std::string calculateHash(){
        return picosha2::hash256_hex_string(getStringFormToCalculate());
    }
    std::string calculateHash(std::string inputString){
        return picosha2::hash256_hex_string(inputString);
    }
    std::string getStringFormToCalculate(){
        std::string toReturn =(_sender+" "+_receiver+" "+_data+ " "+ _timestamp+" " + _prevHash);
        return toReturn;
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
        return *(new Block("0.0.0.0", "0.0.0.0", "And then god said, let there be light", to_string(time(0)),"0000000000000000000000000000000000000000000000000000000000000000"));
    }
    ~Block(){

    }
};
class genesisBlock : public Block {
public:
    std::string _sender, _receiver, _data, _timestamp, _prevHash, _currentHash;
    genesisBlock() {
        _sender = "0.0.0.0";
        _receiver = "0.0.0.0";
        _data = "And then god said, let there be light";
        _prevHash = "0000000000000000000000000000000000000000000000000000000000000000";
        _timestamp = "0";
        _currentHash = "e325a0dea8867506b9d5ff2c2a9b291884b9b82af58834d96c6cf3a14cbf6cef";
        g_print("Created genesis block.\n");
    }
};
std::vector<Block> blockChain;


#endif

