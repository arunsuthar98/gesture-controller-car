#define irout D6
#define rightclk D0
#define rightack D5
#define leftclk D6
#define leftack D7

void aforwad(){
  digitalWrite(rightclk,1);
  digitalWrite(leftclk,1);
  digitalWrite(rightack,0);
  digitalWrite(leftack,0);
}
void aback(){
  digitalWrite(rightclk,0);
  digitalWrite(leftclk,0);
  digitalWrite(rightack,1);
  digitalWrite(leftack,1);
}
void aleft(){
  digitalWrite(rightclk,1);
  digitalWrite(leftclk,0);
  digitalWrite(rightack,0);
  digitalWrite(leftack,1);
}
void aright(){
  digitalWrite(rightclk,0);
  digitalWrite(leftclk,1);
  digitalWrite(rightack,1);
  digitalWrite(leftack,0);
}
void asleft(){
  digitalWrite(rightclk,1);
  digitalWrite(leftclk,0);
  digitalWrite(rightack,0);
  digitalWrite(leftack,0);
}
void asright(){
  digitalWrite(rightclk,0);
  digitalWrite(leftclk,1);
  digitalWrite(rightack,0);
  digitalWrite(leftack,0);
}
void astop(){
  digitalWrite(rightclk,0);
  digitalWrite(leftclk,0);
  digitalWrite(rightack,0);
  digitalWrite(leftack,0);
}
//void f(){
//  forwad();
//  stop();
//  forwad();
//}
//void b(){
//  back();
//  stop();
//  back();
//}
//void l(){
//  left();
//  stop();
//  left();
//}
//void r(){
//  right();
//  stop();
//  right();
//}
