#include <gtkmm.h>
#include <iostream>
#include "network.h"
#include "block.h"
#include "instance.h"
#include "ui.h"

Gtk::Window *mainWindow;
Gtk::Button *connectButton;
Gtk::Entry  *myName;
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

int main(int argc, char *argv[]) {
    using namespace Gtk;
    using namespace std;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
    auto builder = Gtk::Builder::create_from_file("../mainscreen.glade");
//
//    builder->get_widget("loginWindow", mainWindow);
//    builder->get_widget("connectButton", connectButton);
//    builder->get_widget("username", myName);
//    connectButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&connectNetwork), "string"));

    builder = Gtk::Builder::create_from_file("../mainWindow.glade");
    builder->get_widget("mainWindow", mainWindow);

    Gtk::ListBox *listBox;
    builder->get_widget("listbox1", listBox);

    populateActive(listBox);

    Gtk::ListBox *messageViewer;
    builder->get_widget("messageList", messageViewer);
    messageViewer->set_border_width(0);
    messageViewer->set_selection_mode(SelectionMode::SELECTION_NONE);
    for(int i=0; i<12; i++){
        addSentMessage(messageViewer, "hello there!");
        addReceivedMessage(messageViewer, "General Kenobi");
    }

    return app->run(*mainWindow);
    return 0;

}