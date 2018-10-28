import sys
sys.path.append("../")
from SimplePublisher import SimplePublisher
import toolsmod

def main():
  sp1 = SimplePublisher('amqp://DMCS:DMCS@140.252.32.128:5672/%2Ftest_at', "YAML")

  IMAGE_ID = "bunny"

  msg = {}
  msg['MSG_TYPE'] = "DMCS_AT_START_INTEGRATION"
  msg['IMAGE_ID'] = IMAGE_ID
  msg['IMAGE_INDEX'] = '2'
  msg['IMAGE_SEQUENCE_NAME'] = 'MAIN'
  msg['IMAGES_IN_SEQUENCE'] = '3'
  msg['ACK_ID'] = 'START_INT_ACK_76'
  msg['REPLY_QUEUE'] = "dmcs_ack_consume"
  sp1.publish_message("ocs_dmcs_consume", msg)
  print("START_INTEGRATION Message")
  time.sleep(2)

  msg = {}
  msg['MSG_TYPE'] = "DMCS_AT_END_READOUT"
  msg['IMAGE_ID'] = IMAGE_ID
  msg['IMAGE_INDEX'] = '2'
  msg['IMAGE_SEQUENCE_NAME'] = 'MAIN'
  msg['IMAGES_IN_SEQUENCE'] = '3'
  msg['RESPONSE_QUEUE'] = "dmcs_ack_consume"
  msg['ACK_ID'] = 'READOUT_ACK_77'
  sp1.publish_message("ocs_dmcs_consume", msg)
  print("READOUT Message")
  time.sleep(2)

  msg = {}
  msg["MSG_TYPE"] = "DMCS_AT_HEADER_READY"
  msg["IMAGE_ID"] = IMAGE_ID 
  msg["FILENAME"] = "http://localhost:8000/visitJune-28.header"
  sp1.publish_message("ocs_dmcs_consume", msg)
  print("HEADER_READY Message")
  time.sleep(2)

  print("Sender done")

if __name__ == "__main__":  main()
