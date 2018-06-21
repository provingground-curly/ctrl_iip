

/*
 * This file contains the implementation for the atArchiver_exitControl commander test.
 *
 ***/

#include <string>
#include <sstream>
#include <iostream>
#include "SAL_atArchiver.h"
#include "ccpp_sal_atArchiver.h"
#include "os.h"
#include <stdlib.h>
#include "SimplePublisher.h" 
#include <yaml-cpp/yaml.h> 
using namespace YAML; 
using namespace DDS;
using namespace atArchiver;

int main (int argc, char *argv[])
{ 
  int cmdId;
  int timeout=10;
  int status=0;

  atArchiver_command_exitControlC myData;
  if (argc < 2 ) {
     printf("Usage :  input parameters...\n");
     printf("  boolean	state;\n");
     exit(1);
  }
  SAL_atArchiver mgr = SAL_atArchiver();

  mgr.salCommand("atArchiver_command_exitControl");

  Node n = LoadFile("../../tests/yaml/L1SystemCfg_Test_ocs_bridge.yaml");
  string broker = n["ROOT"]["BASE_BROKER_ADDR"].as<string>(); 
  Node user_file = LoadFile("userConfig.yaml"); 
  string user_name = user_file["atArchiver"]["exitControl"].as<string>(); 
  ostringstream url; 
  url << "amqp://" 
      << user_name 
      << ":"
      << user_name
      << "@" 
      << broker; 
  cout << url.str() << endl; 
  SimplePublisher *publisher = new SimplePublisher(url.str()); 

  myData.device   = "controller";
  myData.property = "command";
  myData.action   = "exit";
    sscanf(argv[1], "%d", &myData.state);

  // generate command
  cmdId = mgr.issueCommand_exitControl(&myData);
  cout << "=== command exitControl issued = " << endl;
  status = mgr.waitForCompletion_exitControl(cmdId, timeout);

  if (status == 303) { // completed_ok
     publisher->publish_message("test_dmcs_ocs_publish", "{ MSG_TYPE: EXIT_CONTROL_ACK, DEVICE: AT, CMD_ID: None, ACK_ID: None, ACK_BOOL: None, ACK_STATEMENT: None }");  
  } 

  /* Remove the DataWriters etc */
  mgr.salShutdown();
  if (status != SAL__CMD_COMPLETE) {
     exit(1);
  }
  exit(0);
}

