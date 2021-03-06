#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"


class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }

  String getContent(){
    return content;
  }
};

class Pattern {
public:
  virtual boolean match(const Msg& m) = 0;
};

/*Classe utilizzata per lo scambio di messaggi sulla seriale, utilizza la classe
Msg come pacchetto per i messaggi*/
class MsgServiceClass {

public:

  Msg* currentMsg;
  bool msgAvailable;

  void init();

  bool isMsgAvailable();
  Msg* receiveMsg();

  void sendMsg(const String& msg);
};

extern MsgServiceClass MsgService;

#endif
