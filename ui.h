#ifndef GTKMMPROJECT_UI_H
#define GTKMMPROJECT_UI_H

#include "resources.h"
#include <gtkmm/listbox.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>
#include <mutex>
#include <set>

std::mutex mu;

bool populateActive(){
    for(int i=0; i< alivePeers[0].size(); i++){
        std::string name;
        name = alivePeers[0].at(i);
        if(name.length() > 0) {
            Gtk::ListBox *listBox = userListBox;
            Gtk::ListBoxRow *myRow;
            Gtk::Button *myButton;
            Gtk::Box *chatRow;
            Gtk::Label *l2;
            Gtk::Image *avatar;
            myRow = new Gtk::ListBoxRow;
            myButton = new Gtk::Button();
            l2 = new Gtk::Label(name);
            l2->set_alignment(0.1, 0.5);
            avatar = new Gtk::Image;
            int x = 1;
            std::string imagePath = "../" + userImages[x];
            avatar->set(imagePath);
            avatar->set_size_request(40, 40);
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
    myLabel->set_alignment(0, 0.5);
    myLabel->set_padding(10, 10);
    myLabel->set_line_wrap(true);
    myLabel->set_margin_right(300);
    myLabel->set_margin_top(10);
    myLabel->set_margin_bottom(10);

    Gdk::RGBA *myColor;
    myColor = new Gdk::RGBA;
    myColor->set_rgba(0, 0.603, 0.108, 1);
    myLabel->override_background_color(*myColor);

    myColor->set_rgba(1, 1, 1, 1);
    myLabel->override_color(*myColor);

    messageItemIn->add(*myLabel);
    listBox->append(*messageItemIn);
    listBox->show_all();
    mu.unlock();
}

bool updateMessageBoard(){

    if (messages.size() > 0) {
        g_print("> Update function called for in, %d messages in queue\n", messages.size());
        std::string getMessage = messages.front();
        messages.pop();
        if (strlen(getMessage.c_str()) > 0)
            addReceivedMessage(messageViewer, getMessage);
    }
    return true;
}
bool updateOutMessageBoard(){
    if (outmessages.size() > 0) {
        g_print("> Update function called for out, %d out messages in queue\n", messages.size());
        std::string getMessage = outmessages.front();
        outmessages.pop();
        if (strlen(getMessage.c_str()) > 0)
            g_print("%s", getMessage.c_str());
            addSentMessage(messageViewer, getMessage, true);
    }
    return true;
}

#endif