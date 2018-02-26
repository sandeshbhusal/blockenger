//
// Created by prasanga on 2/19/18.
//

#ifndef MINIP_BLOCK_H
#define MINIP_BLOCK_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

// On request of blockchain, iterate the loop of blockchain,
// run the sendData function over TCP.

class Block{
private:
    std::string _sender;
    std::string _receiver;
    std::string _data;
    std::string _prevHash;
    long long _index;

public:
    long long _size;
    std::string _genesisHash;
    std::string _currentHash;

    Block(std::string data){
        popData(data);
    };

    Block(std::string sender, std::string receiver, std::string data) {
        _sender = sender;
        _receiver = receiver;
        _data = data;
        _prevHash = "asdasd";
    }

    bool verify() { // Kaam chaldai xa

    };

    std::string getData(std::string privateKey) { // Message herna use garne
        return _data;
    }

    std::string sendData() { // network maa string send garna use garne.
        return _genesisHash + " " + _currentHash + " " + _sender + " " + _receiver + " " + _prevHash + " " + _data;
    }

    void popData(string dat) {  // network bata aako string lai parse garne
        std::stringstream ss(dat);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        _genesisHash = vstrings[0];
        _currentHash = vstrings[1];
        _sender = vstrings[2];
        _receiver = vstrings[3];
        _prevHash = vstrings[4];
        _data = vstrings[5];
    }

    ~Block(){

    }

    static Block generateGenesis(){
        return *(new Block("0.0.0.0", "0.0.0.0", "And god said, let there be light XD"));
    }
};

std::vector<Block> blockChain;


#endif

