/*
 * This file cotains the network routines
 * and data about current interfaces, along with
 * the functions to push and pull data from the blockchain
 * network.
 *
 * Author: sandesh Bhusal.
 *
 */
#ifndef GTKMMPROJECT_NETWORK_H
#define GTKMMPROJECT_NETWORK_H

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <cstdio>
#include <cctype>
#include <cstring>
#include "arpa/inet.h"
#include "ui.h"
#include "resources.h"
#include <ifaddrs.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <mutex>
#include <netinet/tcp.h>
#include "block.h"

std::mutex messageMutex;

const int udpListenPort = 8888;
const int tcpTransferPort = 7777;

sockaddr_in inputStation;
int udpListenSocket;
int tcpTransferSocket;

bool firstRun = true;
std::thread *broadcastListener;
std::thread *messageListener;
std::thread *updateMessageBoardThread;
fd_set readfds;
int max_sd;
int activity;
int new_socket;

int numConnectedClients = 0;

class Network {
private:
    typedef struct sockaddr_in station;
    typedef struct sockaddr stationBase;
public:
    Network() {
        if (firstRun) {
            int accessTrigger = 1;
            udpListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            tcpTransferSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            setsockopt(udpListenSocket, SOL_SOCKET, SO_BROADCAST, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEPORT, &accessTrigger, sizeof(accessTrigger));
            setsockopt(tcpTransferSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
            setsockopt(tcpTransferSocket, SOL_SOCKET, SO_REUSEPORT, &accessTrigger, sizeof(accessTrigger));
            setsockopt(tcpTransferSocket, SOL_SOCKET, TCP_NODELAY, &accessTrigger, sizeof(accessTrigger));
            std::string handshake = "c|"+myUserName;
            broadcastAvailability(true, handshake);
            handshake = "p|"+myUserName;
            sendPacketRequest(std::string("255.255.255.255"), handshake);
            bindBroadcastPort();
            bindMessagePort();
            broadcastListener = new std::thread(listenPeerBroadcast);
            memset(&clients, 0, sizeof(clients));
            myIP = getOwnIP();
            messageListener = new std::thread(listenMessage);
            g_print("NETWORK SETUP COMPLETED.\n");
            g_print("---------\n");
            genesisBlock GENESIS;
            blockChain.push_back(GENESIS);
            alivePeers[0].push_back("127.0.0.3");
            alivePeers[1].push_back("Third Person");

            std::string encMessage = encryptMessage(std::string("hello there"), "192.168.1.1", "192.168.1.2");
            g_print(">>%s\n", encMessage.c_str());
            std::string decMessage = decryptMessage(encMessage, "192.168.1.1", "192.168.1.2");
            g_print(">>%s\n", decMessage.c_str());
        }
    }
    static char *getChars(const std::string &input) {
        auto retVal = new char[input.length()];
        strcpy(retVal, input.c_str());
        return retVal;
    }
    static void broadcastAvailability(bool available, std::string &handshake) {
        messageMutex.lock();
        if (available) {
            int accessTrigger = 1;
            int udpListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            setsockopt(udpListenSocket, SOL_SOCKET, SO_BROADCAST, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEPORT, &accessTrigger, sizeof(accessTrigger));

            std::cout << "Broadcasting availability as " << available << " to peers..." << std::endl;

            station motherShip;
            memset(&motherShip, 0, sizeof(motherShip));

            motherShip.sin_port = htons(udpListenPort);
            motherShip.sin_family = AF_INET;
            motherShip.sin_addr.s_addr = inet_addr("255.255.255.255");

            int pings = 30;
            while (pings--) {
                if (sendto(udpListenSocket, getChars(handshake), strlen(getChars(handshake)), 0,
                           (stationBase *) &motherShip, sizeof(station)) == -1) {
                    std::cout << "Could not broadcast address. Retrying for another " << pings << " attempts"
                              << std::endl;
                    std::cout << errno;
                } else {
                    g_print("\rSuccessfully broadcast connection request #%d ", pings);
                    if(pings == 3){
                        g_print("\n");
                        return;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            std::cout << "Failed to broadcast. Are you connected?" << std::endl;
        } else {
            int accessTrigger = 1;
            int udpListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            setsockopt(udpListenSocket, SOL_SOCKET, SO_BROADCAST, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
            setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEPORT, &accessTrigger, sizeof(accessTrigger));

            std::cout << "Broadcasting availability as " << available << " to peers..." << std::endl;
            station motherShip;
            memset(&motherShip, 0, sizeof(motherShip));

            motherShip.sin_port = htons(udpListenPort);
            motherShip.sin_family = AF_INET;
            motherShip.sin_addr.s_addr = inet_addr("255.255.255.255");

            int pings = 30;
            while (pings--) {
                if (sendto(udpListenSocket, getChars(handshake), strlen(getChars(handshake)), 0,
                           (stationBase *) &motherShip, sizeof(station)) == -1) {
                    std::cout << "Could not broadcast address. Retrying for another " << pings << " attempts"
                              << std::endl;
                    std::cout << errno;
                } else {
                    std::cout << "Successfully broadcast disconnection request!" << std::endl;
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            std::cout << "Failed to broadcast. Are you connected?" << std::endl;
        }
        messageMutex.unlock();
    }
    static bool bindBroadcastPort() {
        udpListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        station me;
        memset(&me, 0, sizeof(me));
        me.sin_port = htons(udpListenPort);
        me.sin_family = AF_INET;
        me.sin_addr.s_addr = INADDR_ANY;

        if (bind(udpListenSocket, (stationBase *) &me, sizeof(me)) == -1) {
            std::cout << "Could not bind to interface for broadcasts. " << std::endl;
            g_print("%d", errno);
            return false;
        } else {
            std::cout << "BINDING BROADCAST PORT... DONE" << std::endl;
            return true;
        }
    }
    static bool bindMessagePort() {
        inputStation.sin_family = AF_INET;
        inputStation.sin_addr.s_addr = INADDR_ANY;
        inputStation.sin_port = htons(tcpTransferPort);
        class bindException {
        };
        class listenException {
        };
        try {
            int out = bind(tcpTransferSocket, (stationBase *) &inputStation, sizeof(inputStation));
            if (out < 0)
                throw new bindException;
            int x = listen(tcpTransferSocket, 3);
            if (x < 0)
                throw new listenException;
            g_print("BINDING MESSAGE PORT... DONE\n");
        }
        catch (bindException e) {
            g_print("Could not bind to 192.168.1.1");
            return false;
        }
        catch (listenException e) {
            g_print("Could not listen to anything...");
            return false;
        }
        return true;
    }
    static void listenPeerBroadcast() {
        while (true) {
            station incoming;
            std::string incomingIP;
            std::string incomingName;
            char buffer[1024];
            unsigned int len = sizeof(station);
            g_print("\nListening who is getting connected\n");
            auto readBytes = recvfrom(udpListenSocket, buffer, 1024, 0, (stationBase *) &incoming, &len);
//            g_print("Read %d bytes \n", readBytes);
            if (strlen(buffer) > 0) {
//                g_print("got some data as: %s in UDP \n", buffer);
                if (buffer[0] == 'c') {
//                    g_print("Connected.\n");
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    std::string name = std::string(buffer);
                    incomingName = name.substr(2, name.size()-1);
                    if (std::find(alivePeers[0].begin(), alivePeers[0].end(), incomingIP) == alivePeers[0].end()) {
                        alivePeers[0].push_back(std::string(incomingIP));
                        alivePeers[1].push_back(incomingName);
                        g_print("New user connected! as %s\n", incomingName.c_str());
//                        populateActive();
                    } else {
                        g_print("Received broadcast but user already in user list.\n");
                    }
                } else if(buffer[0] == 'd'){
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    g_print("Disconnected: %s ", inet_ntoa(incoming.sin_addr));
                    std::string name = std::string(buffer);
                    incomingName = name.substr(2, name.size()-1);
                    if(int pos = (std::find(alivePeers[0].begin(), alivePeers[0].end(), incomingIP)) != alivePeers[0].end()){
                        alivePeers[0].erase(alivePeers[0].begin()+pos);
                        alivePeers[1].erase(alivePeers[1].begin()+pos);
//                        populateActive();
                    }
                }
                else if(buffer[0] == 'p'){
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    int udpMessenger = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                    if(udpMessenger == -1){
                        g_print("Could not create messenger for incoming packet\n");
                    }
                    else{
                        station receiver;
                        memset(&receiver, 0, sizeof(receiver));
                        receiver.sin_family = AF_INET;
                        receiver.sin_port   = htons(udpListenPort);
                        receiver.sin_addr.s_addr  = inet_addr(incomingIP.c_str());
                        std::string handshake = "c|"+myUserName;
                        if(sendto(udpMessenger, handshake.c_str(), handshake.length(), 0,
                                  (stationBase *) &receiver, sizeof(station)) == -1){
                            g_print("Could not send connection status packet :'(\n");
                            g_print("%d\n", errno);
                        }
                        else{
                            g_print("Connection status successfully\n");
                        }
                        handshake = "s|"+ to_string(blockChain.size());
                        if(sendto(udpMessenger, handshake.c_str(), handshake.length(), 0,
                                  (stationBase *) &receiver, sizeof(station)) == -1){
                            g_print("Could not send Size packet :'(\n");
                            g_print("%d\n", errno);
                        }
                        else{
                            g_print("Size packet sent successfully\n");
                        }
                    }
                }
                else if(buffer[0] = 's'){
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    std::string receivedBuffer = buffer;
                    std::string interest = receivedBuffer.substr(2, receivedBuffer.size()-1);
                    int thisPacketSize = stoi(interest);
                    packetSize newPacketSize({incomingIP, thisPacketSize});
                    // Check if the incoming IP is already in the packetsizestore.
                    bool flag = false;
                    for(int i=0; i<packetSizeStore.size(); i++){
                        packetSize searchItem = packetSizeStore.at(i);
                        if(searchItem.ipAddress == incomingIP){
                            flag = true;
                            break;
                        }
                    }
                    if(!flag){
                        g_print("This IP is new. I will store its packet size for future reference. as %d\n", thisPacketSize);
                        packetSizeStore.push_back(newPacketSize);
                        if(blockChain.size() < newPacketSize.size){
                            updateBlockChain(getLongestPeer());
                        }
                    }
                }
            }
        }
    }
    static std::string getOwnIP() {
        g_print("CHECKING MY IP ADDRESS...");
        int inet = 0, inet6 = 0;
        struct ifaddrs *interfaces, *iterator;
        int family, s, n;
        if (getifaddrs(&interfaces) == -1) {
            errorMessage("Could not bind connections!");
        } else {
            for (iterator = interfaces; iterator != NULL; iterator = iterator->ifa_next) {
                if (iterator->ifa_addr->sa_family == AF_INET) {
                    station *sa;
                    sa = (station*) iterator->ifa_addr;
                    std::string address = inet_ntoa(sa->sin_addr);
                    if(address != "127.0.0.1") {
                        g_print("DONE\n");
                        return address;
                    }
                }
            }
            g_print("DONE\n");
            return "127.0.0.1";
        }
    }
    static void listenMessage() {
        while (true) {
            int sd;
            FD_ZERO(&readfds);
            FD_SET(tcpTransferSocket, &readfds);
            max_sd = tcpTransferSocket;
            for (int i = 0; i < 30; i++) {
                sd = clients[i];
                if (sd > 0)
                    FD_SET(sd, &readfds);
                if (sd > max_sd)
                    max_sd = sd;
            }
            activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
            if ((activity < 0) && (errno != EINTR)) {
                listenMessage();
            }
            if (FD_ISSET(tcpTransferSocket, &readfds)) {
                int inputStationSize = sizeof(inputStation);
                if ((new_socket = accept(tcpTransferSocket,
                                         (struct sockaddr *) &inputStation, (socklen_t *) &inputStationSize)) < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                std::string a(inet_ntoa(inputStation.sin_addr));
                g_print("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket,
                        inet_ntoa(inputStation.sin_addr), ntohs(inputStation.sin_port));

                for (int i = 0; i < 30; i++) {
                    if (clients[i] == 0) {
                        clients[i] = new_socket;
//                        g_print("Adding to list of sockets as %d\n", i);
                        numConnectedClients++;
                        break;
                    }
                }
            }
            for (int i = 0; i < numConnectedClients; i++) {
                sd = clients[i];
                char buffer[1024];
                socklen_t size = sizeof(inputStation);
                if (FD_ISSET(sd, &readfds)) {
                    int valread;
                    memset(&buffer, 0, sizeof(buffer));
                    while ((valread = recv(sd, (char *) &buffer, 1024, 0)) > 0) {
                        std::thread *addMessageThread;
                        buffer[valread] = '\0';
                        std::string myStr = buffer;
                        g_print("\nBuffer sent : %d characters as %s\n", strlen(buffer), myStr.c_str());
                        if (strlen(buffer) > 0) {
                            // Check if the request is for the total blockchain?
                            if(myStr == "BLOCKCHAINREQUEST"){
                                g_print("Someone is asking for a copy of the blockchain. We are going to send one.\n");
                                for(int i=1; i<blockChain.size(); i++){
                                    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                                    station remote;
                                    remote.sin_family = AF_INET;
                                    remote.sin_addr.s_addr = inet_addr(inet_ntoa(inputStation.sin_addr));
                                    remote.sin_port = htons(tcpTransferPort);
                                    if (connect(sock , (struct sockaddr *)&remote , sizeof(remote)) < 0) {
                                        perror("connect failed to give blocks...\n");
                                    }
                                    std::string sendData = blockChain.at(i).getStringFormToSend();
                                    if(send(sock, sendData.c_str(), sendData.length(), 0) < 0){
                                        perror("Could not send Block packet on TCP\n");
                                    }
                                    else{
                                        g_print("SENT BLOCK #%d\n", i);
                                    }
                                    close(sock);
                                }
                                listenMessage();
                            }
                            else {
                                Block checkBlock(buffer);
                                if (checkBlock.validate()) {
                                    g_print("This block seems untampered. Passing this along...\n");
                                    g_print("Checking for hash mismatch...\n");
                                    if (blockChain.size() > 0) {
                                        Block mostRecent = blockChain.at(blockChain.size() - 1);
                                        if (checkBlock._prevHash == mostRecent.calculateHash()) {
                                            blockChain.push_back(checkBlock);
                                            g_print("COOL! A NEW BLOCK!!");
                                            std::string data = checkBlock._data;
                                            data = decryptMessage(checkBlock._data, "192.168.1.1", "192.168.1.2");
                                            if(activeIP == checkBlock._sender && myIP == checkBlock._receiver)
                                                inMessages.push(data);
                                        } else if(checkBlock._currentHash != mostRecent.calculateHash()) {
                                            g_print("Who keeps tampering with my blocks??\n");
                                            g_print("Got Hash: %s\n",checkBlock._prevHash.c_str());
                                            g_print("Expected: %s\n",mostRecent.calculateHash().c_str());
                                        }
                                    }
                                } else {
                                    g_print("Who tampered with this block???\n");
                                    g_print("SENDER: %s\n", checkBlock._sender.c_str());
                                    g_print("RECEIVER: %s\n", checkBlock._receiver.c_str());
                                    g_print("DATA: %s\n", checkBlock._data.c_str());
                                    g_print("Current hash: %s\n", checkBlock._currentHash.c_str());
                                    g_print("Calculated hash: %s\n", checkBlock.calculateHash().c_str());
                                }
                            }
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    static bool sendMessage(const std::string &message) {
        Block *myBlock;
        int successfulSents = 0;
        std::string encMessage = encryptMessage(message, "192.168.1.1", "192.168.1.2");
        if(blockChain.size() > 0){
            int blockChainIndex = blockChain.size();
            myBlock = new Block(myIP, activeIP, encMessage, to_string(time(0)), blockChain[blockChainIndex-1].calculateHash());
//            blockChain.push_back(*myBlock);
            g_print("Now I will create a new block\n");
            g_print("%s\n", myBlock->getStringFormToSend().c_str());
        }

        for(int i=0; i < alivePeers[0].size(); i++){
            int accessTrigger = 1;
            int sendMessageSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            setsockopt(sendMessageSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
            setsockopt(sendMessageSocket, SOL_SOCKET, TCP_NODELAY, &accessTrigger, sizeof(accessTrigger));

            station sendStation;
            sendStation.sin_addr.s_addr = inet_addr(alivePeers[0][i].c_str());
            sendStation.sin_family = AF_INET;
            sendStation.sin_port = htons(tcpTransferPort);
            class bindException {
            };
            class sendException {
            };
            try {
                int b = connect(sendMessageSocket, (stationBase*) &sendStation, sizeof(station));
                if (b < 0) {
                    g_print("Binding exception, host bind error: %s\n", alivePeers[0][i].c_str());
                    g_print("%d", errno);
                    throw *(new bindException);
                }
//                g_print("\n\n%s\n\n", myBlock->getStringFormToSend().c_str());
                char *message = new char[myBlock->getStringFormToSend().length()];
                strcpy(message, myBlock->getStringFormToSend().c_str());
                message[myBlock->getStringFormToSend().length()] = '\0';
                ssize_t a = send(sendMessageSocket, message, strlen(message), 0);
                if (a == -1)
                    throw *(new sendException);
                g_print("Sent to: %s \n", alivePeers[0][i].c_str());
                successfulSents++;
            }
            catch (bindException e) {
                if (errno == 106)
                    g_print("Already bound port being recalled..\n");
            }
            catch (sendException e) {
                g_print("Error sending the message to the receiver...\n");
                g_print("%d", errno);
            }
            catch (...) {
                g_print("%d", errno);
                return false;
            }
            close(sendMessageSocket);
        }
        if(successfulSents > 1 && activeIP == myBlock->_receiver){    // At least one computer must receive the block.
            std::string data = myBlock->_data;
            outmessages.push(decryptMessage(myBlock->_data, "192.168.1.1", "192.168.1.2"));
            return true;
        }
        return false;
    }
    static void sendPacketRequest(std::string IPADDR, std::string handshake){
        g_print("SENDING PACKET SIZE QUERY REQUEST.");
        int accessTrigger = 1;
        int udpListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        setsockopt(udpListenSocket, SOL_SOCKET, SO_BROADCAST, &accessTrigger, sizeof(accessTrigger));
        setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
        setsockopt(udpListenSocket, SOL_SOCKET, SO_REUSEPORT, &accessTrigger, sizeof(accessTrigger));

        station motherShip;
        memset(&motherShip, 0, sizeof(motherShip));

        motherShip.sin_port = htons(udpListenPort);
        motherShip.sin_family = AF_INET;
        motherShip.sin_addr.s_addr = inet_addr(IPADDR.c_str());

        int pings = 20;
        while (pings--) {
            if (sendto(udpListenSocket, handshake.c_str(), handshake.length(), 0,
                       (stationBase *) &motherShip, sizeof(station)) == -1) {
                std::cout << "Could not broadcast address. Retrying for another " << pings << " attempts"
                          << std::endl;
                std::cout << errno;
            } else {
                g_print("\rSuccessfully broadcast Packet query request #%d ", pings);
                if(pings == 3){
                    g_print("\n");
                    return;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "Failed to broadcast. Are you connected?" << std::endl;
    }
    static std::string getLongestPeer(){
        int longest = -1;
        std::string peer;
        for(int i=0; i<packetSizeStore.size(); i++){
            if(packetSizeStore.at(i).size > longest){
                longest = packetSizeStore.at(i).size;
                peer    = packetSizeStore.at(i).ipAddress;
            }
        }
        if(longest > 1)
            return (peer);
        else
            return "";
    }
    static void updateBlockChain(std::string peerName){
        g_print("Someone has a more recent copy of the blockchain than me!\n");
        g_print("I will ask %s for the remaining blockchain..\n", peerName.c_str());
        // TCP Update == '' request to pull correct blockchain.

        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock == -1){
            g_print("Could not make a socket to pull blockchain...");
        }
        else{
            station remote;
            remote.sin_family = AF_INET;
            remote.sin_addr.s_addr = inet_addr(getLongestPeer().c_str());
            remote.sin_port = htons(tcpTransferPort);
            if (connect(sock , (struct sockaddr *)&remote , sizeof(remote)) < 0) {
                perror("connect failed to update blockchain...\n");
            }
            if(send(sock, "BLOCKCHAINREQUEST", strlen("BLOCKCHAINREQUEST"), 0) < 0){
                perror("Could not send blockchain request packet on TCP\n");
            }
            else{
                g_print("SENT BLOCKCHAIN REQUEST PACKET.. PACKETS MUST BE ARRIVING ANY TIME NOW\n");
            }
        }
    }
};

#endif