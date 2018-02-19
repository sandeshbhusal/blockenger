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
//
//    builder->get_widget("loginWindow", mainWindow);
//    builder->get_widget("connectButton", connectButton);
//    builder->get_widget("username", myName);
//    connectButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&connectNetwork), "string"));

    builder = Gtk::Builder::create_from_file("../mainWindow.glade");
    builder->get_widget("mainWindow", mainWindow);

    Gtk::ListBox *listBox;
    builder->get_widget("listbox1", listBox);
    std::string myArray[] = {
            "Sandesh",
            "Sushant",
            "Sobit",
            "Prasanga",
            "Prashant",
            "Srijana",
            "Manish",
            "Aananda"
    };
    std::string myUsers[] = {
            "elf.png",
            "santa.png",
            "snowman.png",
            "reindeer.png",
            "hancy.png",
            "gingerbread.png"
    };
    for(int i=0; i<3; i++){
        int totalImages = sizeof(myUsers)/ sizeof(myUsers[1]);
        int totalUsers  = sizeof(myArray)/sizeof(myArray[0]);
        Gtk::ListBoxRow *myRow;
        Gtk::Button *myButton;
        Gtk::Box *chatRow;
        Gtk::Label *l2;
        Gtk::Image *avatar;

        myRow = new Gtk::ListBoxRow;
        myButton = new Gtk::Button();
        l2    = new Gtk::Label(myArray[i%totalUsers]);
        avatar = new Gtk::Image;
        std::string imagePath = "../"+myUsers[i%totalImages];
        avatar->set(imagePath);
        avatar->set_size_request(40, 40);
        l2->set_alignment(0.1, 0.5);
        l2->set_size_request(100, 40);
        chatRow = new Gtk::Box();
        chatRow->set_size_request(0, 40);
        chatRow->pack_start(*avatar);
        chatRow->pack_start(*l2);
        myButton->set_relief(ReliefStyle::RELIEF_NONE);
        myButton->override_background_color(Gdk::RGBA("10, 10, 10, 10"));
        myButton->add(*chatRow);
        myRow->add(*myButton);
        listBox->append(*myRow);
        listBox->show_all();
    }
    Gtk::ListBox *messageViewer;
    builder->get_widget("messageList", messageViewer);
    messageViewer->set_border_width(0);
    messageViewer->set_selection_mode(SelectionMode::SELECTION_NONE);
    for(int i=0; i<12; i++){
        Gtk::ListBoxRow *messageItemIn = new Gtk::ListBoxRow();
        Gtk::ListBoxRow *messageItemOut = new Gtk::ListBoxRow();
        Gtk::Label *myLabel = new Gtk::Label("Incoming Message Outgoing MessageOutgoing MessageOutgoing MessageOutgoing MessageOutgoing MessageOutgoing MessageOutgoing MessageOutgoing Message");
        myLabel->set_alignment(0, 0.5);
        myLabel->set_padding(10, 10);
        myLabel->set_line_wrap(true);
        myLabel->set_margin_right(300);
        myLabel->set_margin_top(10);
        myLabel->set_margin_bottom(10);
        Gdk::RGBA myColor;
        myColor.set_rgba(0, 0.729, 0.408, 1);
        myLabel->override_background_color(myColor);
        Gdk::RGBA myColorFore;
        myColor.set_rgba(1, 1, 1, 1);
        myLabel->override_color(myColor);

        Gtk::Label *hisLabel = new Gtk::Label("Outgoing Message");
        hisLabel->set_alignment(1, 0.5);
        hisLabel->set_margin_left(300);
        hisLabel->set_margin_top(10);
        hisLabel->set_margin_right(10);
        hisLabel->set_margin_bottom(10);
        hisLabel->set_padding(10, 10);
        Gdk::RGBA myColor2;
        myColor2.set_rgba(0, 0.631, 1, 1);
        hisLabel->override_background_color(myColor2);
        hisLabel->override_color(myColor);
        
        messageItemOut->add(*myLabel);
        messageItemIn->add(*hisLabel);

        messageViewer->append(*messageItemOut);
        messageViewer->append(*messageItemIn);
        messageViewer->show_all();
    }

    return app->run(*mainWindow);
    return 0;

}