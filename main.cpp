#include <gtkmm.h>
#include <iostream>
#include "network.h"
#include "block.h"
#include "instance.h"

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

//    builder->get_widget("loginWindow", mainWindow);
//    builder->get_widget("connectButton", connectButton);
//    builder->get_widget("username", myName);
//    connectButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&connectNetwork), "string"));

    builder = Gtk::Builder::create_from_file("../mainWindow.glade");
    builder->get_widget("mainWindow", mainWindow);

    Gtk::ListBox *listBox;
    builder->get_widget("listbox1", listBox);
    std::string myArray[] = {
            "sandesh",
            "Sushant",
            "Sobit",
            "Prasanga"
    };
    for(int i=0; i<100; i++){
        Gtk::ListBoxRow *myRow;
        Gtk::Button *myButton;
        Gtk::Box *chatRow;
        Gtk::Label *l2;

        myRow = new Gtk::ListBoxRow;
        myButton = new Gtk::Button();
        l2    = new Gtk::Label(myArray[i%4]);
        l2->set_alignment(0.1, 0.5);
        chatRow = new Gtk::Box();
        chatRow->set_size_request(0, 40);
        chatRow->pack_start(*l2);
        myButton->set_relief(ReliefStyle::RELIEF_NONE);
        myButton->add(*chatRow);
        myRow->add(*myButton);
        listBox->append(*myRow);
        listBox->show_all();
    }

    return app->run(*mainWindow);
    return 0;
}