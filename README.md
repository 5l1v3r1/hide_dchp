# hide_dchp

This repository contains a proof-of-concept implementation of a covert channel in DHCP. This software was developed a few years ago and is based on the Internet Systems Consortium (ISC) DHCP open software implementation version 4.1.1-P1. Changes to the original code can be easily found by searching for the string "CCC".

The covert channel exploits the sname and file fields in the DHCP protocol header to convey information from a client to a server. Two more channels based on the xid and options fields, respectively, were developed but they are not provided here. More information about these channels can be found in the following paper:

    R. Rios, J. A. Onieva, and J. Lopez, "Covert Communications through Network Configuration Messages", 
    In Computers & Security, vol. 39, Part A, Elsevier, pp. 34 - 46, Nov 2013 
  
If you are interested in the other 2 versions drop a line to the corresponding author. Anyway, I will try to upload them some time soon.

----------------------------------------------------------------------------------------------

Configuration and running (tested on Debian):

0)	Install development packages to compile C code (if not done yet)

    $ sudo apt-get install build-essential

1)	Optionally, modify the transmission delimiters (DELIM), whose default value is defined in ./includes/dhcpd.h and it currently set to “/st”. This helps to enhance the stealthiness of the transmission.

2)	Configure, compile (and install) the code
      
      $ ./configure
      $ make
      $ sudo make install

Installing is not completely necessary but hereon we assume that the reader has installed the software (needs to be root). Otherwise, the reader might choose to indicate the path to the configuration files to the program by using command line options.
Uninstalling can be done with "sudo make uninstall" from the same folder.

3)	Configure the client in a dedicated machine

   a.	Edit the client’s configuration file dhclient.conf to enable the inclusion of the client’s name in the packets, which allows the server to recognize it as a colluding client. Uncomment and edit this line:

      send host-name "colluding_client_name";

   A sample client configuration file can be found at ./sample-config
   If not provided to the client via the –cf flag, the client will look for the configuration file at the path defined by _PATH_DHCLIENT_CONF, which by default points at /etc/dhclient.conf. This constant is defined at ./includes/dhcpd.h.

   b.	Moreover, the client needs to know which is the colluding server waiting for covert data because there might be several DHCP servers in the network. This is defined by means of the variable FAKE_SERVER in the client’s source file (./client/dhclient.c). By default, this variable is set to “fake.dhcpd”. Note that if the source file is modified, it would be necessary to recompile the code (step 4).

4) Configure the server

   a.	A sample configuration file for the server can be found at ./sample-config . The location of this file can be latter indicated using the –cf flag.

   b.	Edit the configuration file according to your network needs. It is important to include the name of the server (server-name) to allow the client to detect it. The value should be the same as the one used in step 5b (by default “fake.dhcpd”). 

   c.	Create the file containing the list of clients allowed to send covert data to the server. The path to this file is declared by the variable FAKE_CLIENTS_FILE, which is defined within ./includes/dhcpd.h .

      $ echo covert_client_name >> /etc/clients.lst

5)	Run the DHCP server in machine A

   a.	Machine A must belong to a subnet whose IP address is according to the data provided in dhcpd.conf

   b.	In order to start the server for the first time, it is necessary to create the clients’ database dhcpd.leases, which by default is located at /var/db/. 

      $ mkdir /var/db
      $ touch /var/db/dhcpd.leases

   To indicate the server a different location for this file, use the –lf option.

   c.	Run the server including the –cc to enable the retrieval of covert data. The –d option allows the trace to be flushed to the standard output. 

      # dhcpd –d –cc received_file

6)	Run a DHCP client in machine B

   a.	Run the client with the –cc option to indicate which file to covertly send. More information about the client’s options can be read from the manual pages ($ man dhclient).

      # dhclient –d –cc sent_file
