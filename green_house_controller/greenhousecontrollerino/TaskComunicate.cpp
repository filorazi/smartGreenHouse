#include "TaskComunicate.h"

TaskComunicate::TaskComunicate(MsgServiceBT *msgSBT, Light *la, Light *lm, ServoTimer2 *servo, LevelIndicator *lp){
  this->msgSBT = msgSBT;
  this->msgSBT->init();
  this->la=la;
  this->la->switchOn();
  this->lm=lm;
  this->lm->switchOff();
  this->servo=servo;
  this->lp=lp;
  this->lp->setLevel(map(GLOBAL_CLASS.getFlow(),0,100, MIN_LVL, MAX_LVL));
}
void TaskComunicate::init(int period){
  MsgService.init();

  Task::init(period);
}

void TaskComunicate::move(){
  Serial.println("move");
  this->lp->setLevel(map(GLOBAL_CLASS.getFlow(),0,100, MIN_LVL, MAX_LVL));
  this->servo->write(map(GLOBAL_CLASS.getFlow(),0,100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
}

void TaskComunicate::tick(){
  String msg;
  Serial.println("comm");
  if(MsgService.isMsgAvailable()){
    msg = MsgService.receiveMsg()->getContent();
    switch (msg[0]) {
      case 'l':{
        if(GLOBAL_CLASS.isAutoMode()){
          GLOBAL_CLASS.setFlow(LOWFLOW);
          this->move();
        }
        break;
      }
      case 'h':{
        if(GLOBAL_CLASS.isAutoMode()){
          GLOBAL_CLASS.setFlow(HIGHFLOW);
          this->move();
        }
        break;
      }
      case 'm':{
        if(GLOBAL_CLASS.isAutoMode()){
          GLOBAL_CLASS.setFlow(MEDIUMFLOW);
          this->move();
        }
        break;
      }
      case 'z':{
        if(GLOBAL_CLASS.isAutoMode()){
          GLOBAL_CLASS.setFlow(NOFLOW);
          this->move();
        }
        break;
      }
      case 'H':{
        int h = msg.substring(1).toInt();
        GLOBAL_CLASS.setHumidity(h);
        break;
      }
    }
  }
  if(GLOBAL_CLASS.isConnected()){
    if(msgSBT->isMsgAvailable()){
      msg = msgSBT->receiveMsg()->getContent();
      switch (msg[0]) {
        case 'l':{
          if(!GLOBAL_CLASS.isAutoMode()){
            GLOBAL_CLASS.setFlow(LOWFLOW);
            this->move();
          }
          break;
        }
        case 'h':{
          if(!GLOBAL_CLASS.isAutoMode()){
            GLOBAL_CLASS.setFlow(HIGHFLOW);
            this->move();
          }
          break;
        }
        case 'm':{
          if(!GLOBAL_CLASS.isAutoMode()){
            GLOBAL_CLASS.setFlow(MEDIUMFLOW);
            this->move();
          }
          break;
        }
        case 'z':{
          if(!GLOBAL_CLASS.isAutoMode()){
            GLOBAL_CLASS.setFlow(NOFLOW);
            this->move();
          }
          break;
        }
        case 't':{
          GLOBAL_CLASS.toggleAutomode();
          la->toggle();
          lm->toggle();
          break;
        }
      }
    }
  msgSBT.sendMsg(new Msg("A-H" + GLOBAL_CLASS.getHumidity()))
  }
}