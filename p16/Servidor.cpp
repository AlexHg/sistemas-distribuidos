#include "SocketDatagrama.h"
#include "Mensaje.h"
#include "Respuesta.hpp"
#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char*argv[])
{
    int nbd = 0;
    Respuesta respuesta(7200);
    struct mensaje *msg = respuesta.getRequest();
    string nbdstring;

    while(true) {
    if(msg->operationId == 1){
        nbdstring = to_string(nbd);
        respuesta.sendReply((char *)nbdstring.c_str(), msg->IP, msg->puerto);
    }else if(msg->operationId == 2){
        nbd++;
        nbdstring = to_string(nbd);
        respuesta.sendReply((char *)nbdstring.c_str(), msg->IP, msg->puerto);
    }
}
}
