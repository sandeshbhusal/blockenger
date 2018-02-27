
#ifndef GTKMMPROJECT_RESOURCES_H
#define GTKMMPROJECT_RESOURCES_H
#include <iostream>
#include <vector>
#include <queue>
#include <set>

std::queue<std::string> inMessages;
std::queue<std::string> outmessages;

std::vector<std::string> alivePeers[2];
std::string activeIP = "127.0.0.1";
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

std::string encryptMessage(std::string message, std::string myIP, std::string hisIP){
    std::string key = myIP+hisIP+"!%^%^&!@*#123123";
    int keySum = 0;
    for(int i=0; i<key.size(); i++){
        keySum += key.at(i);
    }
    keySum %= 100;
    g_print("Key sum is %d \n", keySum);
    std::string output;
    for(int i=0; i<message.size(); i++){
        char *ch = new char[2];
        sprintf(ch, "%x", message.at(i)+keySum);
        output += ch;
    }
    return output;
}
std::string decryptMessage(std::string message, std::string myIP, std::string hisIP){
    std::string key = myIP+hisIP+"!%^%^&!@*#";
    int keySum = 0;
    for(int i=0; i<key.size(); i++){
        keySum += key.at(i);
    }
    keySum %= 100;
    g_print("Key sum is %d \n", keySum);
    int len = message.length();
    std::string newString;
    for(int i=0; i< len; i+=2)
    {
        std::string byte = message.substr(i,2);
        g_print("%s ", byte.c_str());
        char chr = (char) (int)strtol(byte.c_str(), nullptr, 16);
        chr = chr-keySum;
        newString.push_back(chr);
    }
    return newString;
}
#endif