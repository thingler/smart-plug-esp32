#ifndef INIT_AWS_IOT
#define INIT_AWS_IOT

void connectIOT(void);
void publishIOT(char *message, char *topic);
void subscribeIOT(void (*subCallBack)(char *), char *topic);

#endif
