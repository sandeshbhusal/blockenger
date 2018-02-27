#ifndef GTKMMPROJECT_UI_H
#define GTKMMPROJECT_UI_H

#include "resources.h"
#include <gtkmm/listbox.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <mutex>
#include <set>
#include <gtkmm/togglebutton.h>
#include "block.h"

std::mutex mu;
static void buttonClicked(Gtk::Button *mybutton){
    std::string myName = mybutton->get_name();
    if(activeIP != myName){
        Gtk::ListBox *clearBox = messageViewer;
        std::vector<Gtk::Widget* > rows= clearBox->get_children();
        for(int i=0; i<rows.size(); i++)
            clearBox->remove(*rows.at(i));
        //Now scan the blockchain for occurence of the given IP and do stuff :D
        for(int i=0; i<blockChain.size(); i++){
            Block thisBlock = blockChain.at(i);
            if(thisBlock._sender == myName) {
                outmessages.push(decryptMessage(thisBlock._data, "192.168.1.1", "192.168.1.2"));
            }
            if(thisBlock._receiver == myName) {
                inMessages.push(decryptMessage(thisBlock._data, "192.168.1.1", "192.168.1.2"));
            }
        }
    }
    g_print("Active IP set to : %s \n", myName.c_str());
    activeIP = myName;
}

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

bool populateActive(){
    Gtk::ListBox *clearBox = userListBox;
    std::vector<Gtk::Widget* > rows= clearBox->get_children();
    for(int i=0; i<rows.size(); i++)
        clearBox->remove(*rows.at(i));

    for(int i=0; i< alivePeers[0].size(); i++){
        std::string name;
        name = alivePeers[1].at(i);
        if(name.length() > 0) {
            Gtk::ListBox *listBox = userListBox;
            Gtk::ListBoxRow *myRow;
            Gtk::Button *myButton;
            Gtk::Box *chatRow;
            Gtk::Label *l2;
            Gtk::Image *avatar;
            myRow = new Gtk::ListBoxRow;
            myButton = new Gtk::Button();
            myButton->set_name(alivePeers[0].at(i));
            myButton->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&buttonClicked), myButton));
            l2 = new Gtk::Label(name);
            l2->set_alignment(0.1, 0.5);
            avatar = new Gtk::Image;
            int x = i%6;
            std::string imagePath = "../" + userImages[x];
            avatar->set(imagePath);
            avatar->set_size_request(40, 40);
            avatar->set_alignment(0, 0.5);
            chatRow = new Gtk::Box();
            chatRow->set_size_request(0, 40);
            chatRow->pack_start(*avatar);
            chatRow->pack_start(*l2);
            myButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);
            myButton->add(*chatRow);
            myRow->add(*myButton);
            listBox->append(*myRow);
            listBox->show_all();
        }
    }
}

void addSentMessage(Gtk::ListBox *listBox, std::string message, bool sent){
    mu.lock();
    if(sent){
        Gtk::ListBoxRow *messageItemOut = new Gtk::ListBoxRow();

        Gtk::Label *hisLabel = new Gtk::Label(message);
        hisLabel->set_alignment(1, 0.5);
        hisLabel->set_margin_left(300);
        hisLabel->set_margin_top(10);
        hisLabel->set_margin_right(10);
        hisLabel->set_margin_bottom(10);
        hisLabel->set_padding(10, 10);
        hisLabel->set_line_wrap(true);


        Gdk::RGBA *myColor;
        myColor = new Gdk::RGBA;
        myColor->set_rgba(0, 0.631, 1, 1);
        hisLabel->override_background_color(*myColor);

        myColor->set_rgba(1, 1, 1, 1);
        hisLabel->override_color(*myColor);

        messageItemOut->add(*hisLabel);
        listBox->append(*messageItemOut);
        listBox->show_all();
    }
    else{
        Gtk::ListBoxRow *messageItemOut = new Gtk::ListBoxRow();

        Gtk::Label *hisLabel = new Gtk::Label(message);
        hisLabel->set_alignment(1, 0.5);
        hisLabel->set_margin_left(300);
        hisLabel->set_margin_top(10);
        hisLabel->set_margin_right(10);
        hisLabel->set_margin_bottom(10);
        hisLabel->set_padding(10, 10);
        hisLabel->set_line_wrap(true);


        Gdk::RGBA *myColor;
        myColor = new Gdk::RGBA;
        myColor->set_rgba(1, 0, 0, 1);
        hisLabel->override_background_color(*myColor);

        myColor->set_rgba(1, 1, 1, 1);
        hisLabel->override_color(*myColor);

        messageItemOut->add(*hisLabel);
        listBox->append(*messageItemOut);
        listBox->show_all();
    }
    mu.unlock();
}
void addReceivedMessage(Gtk::ListBox *listBox, std::string message){
    mu.lock();
    Gtk::ListBoxRow *messageItemIn = new Gtk::ListBoxRow();
    Gtk::Label *myLabel = new Gtk::Label(message);
    messageItemIn->set_size_request(100, 0);
    myLabel->set_alignment(0, 0.5);
    myLabel->set_padding(10, 10);
    myLabel->set_line_wrap(true);
    myLabel->set_margin_right(300);
    myLabel->set_margin_top(10);
    myLabel->set_margin_bottom(10);

    Gdk::RGBA *myColor;
    myColor = new Gdk::RGBA;
    myColor->set_rgba(0.22, 0.7, 0.73, 1);
    myLabel->override_background_color(*myColor);

    myColor->set_rgba(1, 1, 1, 1);
    myLabel->override_color(*myColor);

    messageItemIn->add(*myLabel);
    listBox->append(*messageItemIn);
    listBox->show_all();
    mu.unlock();
}

bool updateMessageBoard(){
    if (inMessages.size() > 0) {
//        g_print("> Update function called for in, %d messages in queue\n", inMessages.size());
        std::string getMessage = inMessages.front();
        inMessages.pop();
        if (strlen(getMessage.c_str()) > 0)
            addReceivedMessage(messageViewer, getMessage);
    }
    return true;
}
bool updateOutMessageBoard(){
    if (outmessages.size() > 0) {
        g_print("> Update function called for out, %d out messages in queue\n", outmessages.size());
        std::string getMessage = outmessages.front();
        outmessages.pop();
        if (strlen(getMessage.c_str()) > 0)
            addSentMessage(messageViewer, getMessage, true);
            g_print("%s", getMessage.c_str());
    }
    return true;
}

#endif