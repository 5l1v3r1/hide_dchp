/** hide.h
 *
 *  Authors: Ruben Rios and Jose A. Onieva 
 *	
 *
 *  Definitions for the creation of the covert channel... 
 *
 */

//Path to the server's file with the possible fake clients
//from which information can be received.
#ifndef FAKE_CLIENTS_FILE
  #define FAKE_CLIENTS_FILE	"/etc/clients.lst"
#endif


//Indicates the start/stop of a covert transmission
//	- Exactly 3 bytes long
#ifndef DELIM
  #define DELIM	"/st" 
#endif

typedef enum {
  OFF,   //Trans not started
  INI ,  //Found nothing
  BAR ,  //Found 'DELIM[0]'
  ES  ,  //Found 'DELIM[0]DELIM[1]'
  END    //Found 'DELIM'
}delim_state; 


//Structure containing fake client's info
struct fake_info{
  struct fake_info * next; //Next client info
  char * name;             //Client's name
  delim_state currentstate;//State of transm
  FILE * file;             //File to write in
};

