
#ifndef GTKMMPROJECT_RESOURCES_H
#define GTKMMPROJECT_RESOURCES_H
#include <iostream>
#include "network.h"
#include <vector>
#include <queue>
#include <set>

std::queue<std::string> messages;
std::queue<std::string> outmessages;

std::vector<std::string> alivePeers[2];

std::string activeIP;

Gtk::Window  *mainWindow;
Gtk::Button  *connectButton;
Gtk::Entry   *myName;
Gtk::ListBox *userListBox;
Gtk::ListBox *messageViewer;
Gtk::Entry   *myMessage;

int clients[30];

std::string userImages[] = {
    "elf.png",
    "santa.png",
    "snowman.png",
    "reindeer.png",
    "hancy.png",
    "gingerbread.png"
};
#endif