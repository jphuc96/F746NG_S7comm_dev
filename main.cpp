#include "mbed.h"
#include "EthernetInterface.h"
#include "Settimino.h"

Serial pc(PA_9,PA_10,115200);
EthernetInterface eth;
S7Client s7;
SocketAddress plc;

int _err_s7;

int main() {
    pc.printf("==============================\r\r\n");
    pc.printf("= F746 - S7-1200 with S7comm =\r\r\n");
    pc.printf("==============================\r\r\n");
    /*Ethernet*/
    pc.printf("Initializing EthernetInterface.\r\r\n");

    eth.connect();
    pc.printf("MAC: %s\r\r\n",eth.get_mac_address());
    pc.printf("IP: %s\r\r\n",eth.get_ip_address());
    pc.printf("Gateway: %s\r\r\n",eth.get_gateway());

    /*S7 Connect*/
    pc.printf("Connecting to PLC.\r\r\n");
    plc.set_ip_address("10.10.0.2");
    _err_s7 = s7.Start(&eth);
    pc.printf("Starting status: %d\r\r\n",_err_s7);
    _err_s7 = s7.ConnectTo(plc,0,1);
    pc.printf("Connect status: %d\r\r\n",_err_s7);
    return 1;
}