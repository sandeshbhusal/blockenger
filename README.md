# blockenger

Blockenger is a peer to peer local network blockchain based messaging application developed by Prasanga Dhungel (073bct527)
and Sandesh Bhusal(073bct539) as a part of third semester project under the guidance of Mr. Daya Sagar Baral, submitted to 
the Department of Electronics and Computer Engineering. This project has been made entirely in C++, and contains the following
dependencies:

1. Gtkmm (libgtkmm-3.0)

All the code for this project, including the code for the network is hand-coded and developed from scratch, so there might be 
a few glitches here and there, but the project is mostly stable. 
The compiler flag for the project:

`pkg-config --cflags --libs gtkmm-3.0`

On a debian architecture, the libraries are simple to install, and can be installed as easily as typing 

sudo apt install libgtkmm-3.0-*

The above code downloads all the dependencies for the project and we are ready to go. The preferred IDE of choice is Clion, but 
the code runs on virtually anything.

Platform restriction:
At the time of writing this code, the code runs only on the linux platfform, because the socket implementation for the underlying
network is coded entirely in for linux.

This code is hereby free to be modified, distributed, copied and be used for commercial purposes without any prior written permission
The authors of this code release the code under the GNU GPL license, which means this software is a freeware and available for hacking.

Thank you.
~Authors
    Prasanga Dhungel 
    Sandesh Bhusal
