

#include <getopt.h>
#include <iostream>
#include <string>

using namespace std;


/**
 Start the main loop of this module
*/
bool startConnectivity(string bootstrap_IP, int port_local, int port_remote){
  bool success_spawn = true;
  // TODO spawnConnectivityThread

  return success_spawn;
}

/**
  Start the Raft Server Process
*/
bool startRaft(string bootstrap_IP, int port_local, int port_remote){
  bool success = true;


  return success;
}

/**
  Start the local APC Process, i.e. the control software controlled by the
  Regional Controller (RC)
*/
bool startAPC(string bootstrap_IP, int port_local, int port_remote){
  bool success = true;


  return success;
}

/**
 Start the Regional Controller (RC) Process. The RC controlls the APCs.
*/
bool startRC(int port_local){
  bool success = true;


  return success;
}



int
main(int argc, char** argv)
{
  // TODO these variables should be set by passed args or config file
  string selfIP;
  string bootstrap_IP;
  int connectivity_port_local = 0;
  int raft_port_local = 0;
  int apc_port_local = 0;
  int rc_port_local = 0;
  int connectivity_port_boot = 0;
  int raft_port_boot = 0;
  int apc_port_boot = 0;

  // Start connectivity
  if(!startConnectivity(bootstrap_IP, connectivity_port_local, connectivity_port_boot)){
    return -1;
  }

  // Start Raft
  if(!startRaft(bootstrap_IP, raft_port_local, raft_port_boot)){
    return -1;
  }

  if (bootstrap_IP.empty()){
    // If this node is NOT bootstrapper
    if(!startAPC(bootstrap_IP, apc_port_local, apc_port_boot)){
      return -1;
    }
  }
  else{
    /* This node is the bootstrapper. The bootstapper will not be an AP.
    It will be a dedicated controller at system startup*/
    if(!startRC(rc_port_local)){
      return -1;
    }
  }

  std::cout << "SUCCESS" << std::endl;
  return 0;
}
