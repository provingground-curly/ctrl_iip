

/*
 * This file contains the implementation for the archiver_SummaryState receive test.
 *
 ***/

#include <string>
#include <sstream>
#include <iostream>
#include "SAL_archiver.h"
#include "ccpp_sal_archiver.h"
#include "os.h"
#include <stdlib.h>

#include "example_main.h"
#include "SimplePublisher.h" 
#include <yaml-cpp/yaml.h> 
using namespace YAML; 

using namespace DDS;
using namespace archiver;

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
  OS_API_EXPORT
  int test_archiver_SummaryState_Log();
}

int test_archiver_SummaryState_Log()
{ 
  os_time delay_10ms = { 0, 10000000 };
  int status = -1;

  archiver_logevent_SummaryStateC SALInstance;
#ifdef SAL_SUBSYSTEM_ID_IS_KEYED
  int archiverID = 1;
  if (getenv("LSST_archiver_ID") != NULL) {
     sscanf(getenv("LSST_archiver_ID"),"%d",&archiverID);
  } 
  SAL_archiver mgr = SAL_archiver(archiverID);
#else
  SAL_archiver mgr = SAL_archiver();
#endif
  mgr.salEvent("archiver_logevent_SummaryState");
  Node n = LoadFile("../../tests/yaml/L1SystemCfg_Test_ocs_bridge.yaml");
  string broker = n["ROOT"]["BASE_BROKER_ADDR"].as<string>(); 
  ostringstream url; 
  url << "amqp://EVN_1:EVN_1@" << broker; 
  cout << url.str() << endl; 
  SimplePublisher *publisher = new SimplePublisher(url.str()); 
  cout << "=== Event SummaryState logger ready = " << endl;

  while (1) {
  // receive event
    status = mgr.getEvent_SummaryState(&SALInstance);
    if (status == SAL__OK) {
      cout << "=== Event SummaryState received = " << endl;

    cout << "    SummaryStateValue : " << SALInstance.SummaryStateValue << endl;
    cout << "    priority : " << SALInstance.priority << endl;
     publisher->publish_message("test_dmcs_ocs_publish", "{ MSG_TYPE: SUMMARY_STATE_EVENT, DEVICE: AR, CURRENT_STATE: None }");  

    }
    os_nanoSleep(delay_10ms);
  }

  /* Remove the DataWriters etc */
  mgr.salShutdown();

  return 0;
}

int OSPL_MAIN (int argc, char *argv[])
{
  return test_archiver_SummaryState_Log();
}

