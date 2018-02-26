
#ifndef GTKMMPROJECT_RESOURCES_H
#define GTKMMPROJECT_RESOURCES_H
#include <iostream>
#include "block.h"
#include <vector>
#include <queue>
#include <set>

std::queue<std::string> inMessages;
std::queue<std::string> outmessages;

std::vector<std::string> alivePeers[2];

std::string activeIP = "127.0.0.2";
std::string myIP = "";
std::string myUserName = "";

Gtk::Window  *loginWindow;
Gtk::Window  *mainWindow;
Gtk::Button  *connectButton;
Gtk::Entry   *myName;
Gtk::ListBox *userListBox;
Gtk::ListBox *messageViewer;
Gtk::Entry   *myMessage;

int clients[30];

struct packetSize{
    std::string ipAddress;
    int size;
};
typedef struct packetSize packetSize;
std::vector<packetSize> packetSizeStore;

std::string userImages[] = {
    "elf.png",
    "santa.png",
    "snowman.png",
    "reindeer.png",
    "hancy.png",
    "gingerbread.png"
};

string XOR_Encryption(string toBeEncrypted, string sKey){
    string sEncrypted(toBeEncrypted);
    unsigned int iKey=sKey.length();
    int iIn=toBeEncrypted.length();
    int x=0;
    for(int i = 0; i < iIn; i++)
    {
        sEncrypted[i] = toBeEncrypted[i] ^ sKey[x];
        if(++x == iKey){ x = 0; }
    }
    return sEncrypted;
}
string encryptMessage(std::string message, std::string myIP, std::string hisIP){

}
string decryptMessage(std::string message, std::string myIP, std::string hisIP){

}
#endif