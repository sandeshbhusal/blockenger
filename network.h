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

std::mutex messageMutex;

const int udpListenPort = 8888;
const int tcpTransferPort = 7777;
sockaddr_in inputStation;
int udpListenSocket;
int tcpTransferSocket;

bool firstRun = true;
void errorMessage(std::string err) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK_CANCEL, err.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void infoMessage(std::string err) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, err.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

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
            std::string handshake = "c|me";
            broadcastAvailability(true, handshake);
            bindBroadcastPort();
            broadcastListener = new std::thread(listenPeerBroadcast);
            bindMessagePort();
            messageListener = new std::thread(listenMessage);
            memset(&clients, 0, sizeof(clients));
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
                    std::cout << "Successfully broadcast connection request!" << std::endl;
                    if(pings == 3) return;
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
            std::cout << "Successfully bound to UDP port for broadcasts. " << std::endl;
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
            g_print("Listening who is getting connected\n");
            auto readBytes = recvfrom(udpListenSocket, buffer, 1024, 0, (stationBase *) &incoming, &len);
            g_print("Read %d bytes \n", readBytes);
            if (strlen(buffer) > 0) {
                g_print("got some data as: %s in UDP \n", buffer);
                if (buffer[0] == 'c') {
                    g_print("Connected.\n");
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    std::string name = std::string(buffer);
                    incomingName = name.substr(2, name.size()-1);
                    if (std::find(alivePeers[0].begin(), alivePeers[0].end(), incomingIP) == alivePeers[0].end()) {
                        alivePeers[0].push_back(std::string(incomingIP));
                        alivePeers[1].push_back(incomingName);
                        g_print("New user connected! as %s\n", incomingName.c_str());
                        populateActive();
                    } else {
                        g_print("Received broadcast but user already in user list.\n");
                    }
                } else {
                    incomingIP = inet_ntoa(incoming.sin_addr);
                    g_print("Disconnected: %s ", inet_ntoa(incoming.sin_addr));
                    std::string name = std::string(buffer);
                    incomingName = name.substr(2, name.size()-1);
                    if(int pos = (std::find(alivePeers[0].begin(), alivePeers[0].end(), incomingIP)) != alivePeers[0].end()){
                        alivePeers[0].erase(alivePeers[0].begin()+pos);
                        alivePeers[1].erase(alivePeers[1].begin()+pos);
                        populateActive();
                    }
                }
            }

        }
    }

    static int getInterfaceCount() {
        int inet = 0, inet6 = 0;
        struct ifaddrs *interfaces, *iterator;
        int family, s, n;
        if (getifaddrs(&interfaces) == -1) {
            errorMessage("Could not bind connections!");
            return -1;
        } else {
            for (iterator = interfaces; iterator != NULL; iterator = iterator->ifa_next) {
                if (iterator->ifa_addr->sa_family == AF_INET) {
                    inet++;
                } else if (iterator->ifa_addr->sa_family == AF_INET6) {
                    inet6++;
                }
            }
            return inet;
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
                        g_print("Adding to list of sockets as %d\n", i);
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
                        g_print("Buffer sent : %d characters as %s\n", strlen(buffer), myStr.c_str());
                        if (strlen(buffer) > 0) {
                            messages.push(std::string(buffer));
                            g_print("Pushed message to queue\n");
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    static bool sendMessage(const std::string &message) {
        int accessTrigger = 1;
        int sendMessageSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(sendMessageSocket, SOL_SOCKET, SO_REUSEADDR, &accessTrigger, sizeof(accessTrigger));
        setsockopt(sendMessageSocket, SOL_SOCKET, TCP_NODELAY, &accessTrigger, sizeof(accessTrigger));

        station sendStation;
        sendStation.sin_addr.s_addr = inet_addr("192.168.0.103");
        sendStation.sin_family = AF_INET;
        sendStation.sin_port = htons(tcpTransferPort);
        class bindException {
        };
        class sendException {
        };
        try {
            int b = connect(sendMessageSocket, (stationBase *) &sendStation, sizeof(station));
            if (b < 0) {
                g_print("Binding exception\n");
                throw *(new bindException);
            }
        }
        catch (bindException e) {
            if (errno != 106)
                g_print("Error binding to the port to send data...\n");
        }
        catch (...) {
            g_print("%d", errno);
            return false;
        }
        try {
            ssize_t a = send(sendMessageSocket, message.c_str(), message.length(), 0);
            if (a == -1)
                throw *(new sendException);
            outmessages.push(message);
        }
        catch (sendException e) {
            g_print("Error sending the message to the receiver...\n");
            g_print("%d", errno);
            return false;
        }
        close(sendMessageSocket);
        return true;
    }
};

#endif