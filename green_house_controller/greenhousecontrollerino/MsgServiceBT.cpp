#include "Arduino.h"
#include "MsgServiceBT.h"


MsgServiceBT::MsgServiceBT(int rxPin, int txPin){
    this->channel = new SoftwareSerial(rxPin, txPin);
}

void MsgServiceBT::init(){
  content.reserve(256);
  this->channel->begin(9600);
}

bool MsgServiceBT::sendMsg(Msg *msg){
  this->channel->print(msg->getContent());
}

bool MsgServiceBT::isMsgAvailable(){
  return this->channel->available();
}

Msg* MsgServiceBT::receiveMsg(){
  while (this->channel->available()) {
    char ch = (char) this->channel->read();
    if (ch == '\n'){
      Msg* msg = new Msg(content);
      content = "";
      return msg;
    } else {
      content += ch;
    }
  }
  return NULL;
}
