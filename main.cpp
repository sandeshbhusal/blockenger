#include <gtkmm.h>
#include <iostream>
#include "network.h"
#include "block.h"
#include "instance.h"
#include "ui.h"

Network *mynet;

bool setupCompleted = false;

void sendMessage(){
    mynet->sendMessage(myMessage->get_text());
    myMessage->set_text("");
}
void quitApplication(){
    messageMutex.unlock();
    std::string handshake = "d|"+myUserName;
    mynet->broadcastAvailability(false, handshake);
    g_print("Exiting the application...");
    exit(0);
}

void connectNetwork(Glib::RefPtr<Gtk::Application> &app){
    using namespace Gtk;
    const char *x = myName->get_text().c_str();
    if(strlen(x) <= 3)
        errorMessage("Error! The username must exceed three characters!");
    else{
        myUserName = myName->get_text();
        mynet = new Network(); // Setup the network.
        if(myIP.length() == 0){
            errorMessage("Exiting the application");
            gtk_main_quit();
        }
        else{
            addReceivedMessage(messageViewer, "General Kenobi");
            myMessage->signal_activate().connect(sigc::ptr_fun(&sendMessage));
            sigc::slot<bool> my_slot = sigc::ptr_fun(&updateMessageBoard);
            Glib::signal_timeout().connect(my_slot, 50);
            sigc::slot<bool> new_slot = sigc::ptr_fun(&updateOutMessageBoard);
            Glib::signal_timeout().connect(new_slot, 50);
            sigc::slot<bool> user_slot = sigc::ptr_fun(&populateActive);
            Glib::signal_timeout().connect(user_slot, 1000);
            alivePeers[0].push_back("127.0.0.1");
            alivePeers[1].push_back("Me");
//            populateActive();
            mainWindow->show();
//            loginWindow->hide();
        }
    }
}

int main(int argc, char *argv[]) {
    using namespace Gtk;
    using namespace std;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
    auto builder0 = Gtk::Builder::create_from_file("../mainscreen.glade");

    builder0->get_widget("loginWindow", loginWindow);
    builder0->get_widget("connectButton", connectButton);
    builder0->get_widget("username", myName);
    connectButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&connectNetwork), app));

    auto builder = Gtk::Builder::create_from_file("../mainWindow.glade");
    builder->get_widget("mainWindow", mainWindow);

    builder->get_widget("listbox1", userListBox);

    builder->get_widget("messageEdit", myMessage);
    builder->get_widget("messageList", messageViewer);
    messageViewer->set_border_width(0);
    messageViewer->set_selection_mode(SelectionMode::SELECTION_NONE);

    app->add_window(*mainWindow);
    app->run(*loginWindow);
    broadcastListener->join();
    messageListener->join();
    return 0;
}