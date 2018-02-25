#include <gtkmm.h>
#include <iostream>
#include "network.h"
#include "block.h"
#include "instance.h"
#include "ui.h"

Network *mynet;

void connectNetwork(Glib::ustring const &in){
    using namespace Gtk;
    const char *x = myName->get_text().c_str();
    if(strlen(x) <= 3)
        errorMessage("Error! The username must exceed three characters!");
    else{
        mynet = new Network(); // Setup the network.
        int interfaces = mynet->getInterfaceCount();
        if(interfaces == -1){
            errorMessage("Exiting the application");
            gtk_main_quit();
        }
        else{

        }
    }
}
void sendMessage(){
    g_print("Call to send message.\n");
    if(mynet->sendMessage(myMessage->get_text())){
        g_print("Successful sending.\n");
//        addSentMessage(messageViewer, myMessage->get_text(), true);
    }
    else{
        g_print("Un successful sending.\n");
//        addSentMessage(messageViewer, myMessage->get_text(), false);
    }
    myMessage->set_text("");
}
void quitApplication(){
    messageMutex.unlock();
    std::string handshake = "d|bhusal";
    mynet->broadcastAvailability(false, handshake);
    g_print("Exiting the application...");
    exit(0);
}
int main(int argc, char *argv[]) {
    using namespace Gtk;
    using namespace std;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
//    auto builder = Gtk::Builder::create_from_file("../mainscreen.glade");

//    builder->get_widget("loginWindow", mainWindow);
//    builder->get_widget("connectButton", connectButton);
//    builder->get_widget("username", myName);
//    connectButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&connectNetwork), "string"));

    auto builder = Gtk::Builder::create_from_file("../mainWindow.glade");
    builder->get_widget("mainWindow", mainWindow);

    builder->get_widget("listbox1", userListBox);

    populateActive(userListBox, "sandesh");

    builder->get_widget("messageEdit", myMessage);
    builder->get_widget("messageList", messageViewer);
    messageViewer->set_border_width(0);
    messageViewer->set_selection_mode(SelectionMode::SELECTION_NONE);

    addReceivedMessage(messageViewer, "General Kenobi");

    myMessage->signal_activate().connect(sigc::ptr_fun(&sendMessage));
    mynet = new Network;

    sigc::slot<bool> my_slot = sigc::ptr_fun(&updateMessageBoard);
    Glib::signal_timeout().connect(my_slot, 100);

    sigc::slot<bool> new_slot = sigc::ptr_fun(&updateOutMessageBoard);
    Glib::signal_timeout().connect(new_slot, 100);


    app->signal_shutdown().connect(sigc::ptr_fun(quitApplication));
    app->run(*mainWindow);
    broadcastListener->join();
    messageListener->join();
    return 0;
}