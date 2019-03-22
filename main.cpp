#include "mbed.h"
#include "EthernetInterface.h"
#include "Settimino.h"


#define ADDRESS     "192.168.0.5"
#define SUBNET      "255.255.255.0"
#define GATEWAY     "192.168.0.11"
#define PLC_ADDRESS "192.168.0.1"

Serial pc(PA_9, PA_10, 115200);
EthernetInterface eth;
S7Client s7;
SocketAddress plc;

int _s7_err;
int _s7_status;
uint16_t    db_num = 1;

uint8_t     db_byte_buffer[256];
uint16_t    db_byte_size = 17;

uint16_t    db_word_buffer[256];
uint16_t    db_word_size = 17;

bool        db_bool_buffer[256];
uint16_t    db_bool_size = 17;

int demo_read_byte();
int demo_write_byte();
int demo_read_bool();
int demo_write_bool();

int main()
{
    pc.printf("==============================\r\r\n");
    pc.printf("= F746 - S7-1200 with S7comm =\r\r\n");
    pc.printf("==============================\r\r\n");
    /*Ethernet*/
    pc.printf("Initializing EthernetInterface.\r\r\n");

    eth.set_network(ADDRESS,SUBNET,GATEWAY);
    eth.connect();
    pc.printf("MAC: %s\r\r\n", eth.get_mac_address());
    pc.printf("IP: %s\r\r\n", eth.get_ip_address());
    pc.printf("Gateway: %s\r\r\n", eth.get_gateway());

    /*S7 Connect*/
    plc.set_ip_address(PLC_ADDRESS);

    pc.printf("Starting S7Client......");
    _s7_err = s7.Start(&eth);
    pc.printf("%s\r\r\n", _s7_err == 0 ? "OK" : "ERROR");

    pc.printf("Connecting to PLC [ %s ] ......",PLC_ADDRESS);
    _s7_err = s7.ConnectTo(plc, 0, 1);
    pc.printf("%s\r\r\n", _s7_err == 0 ? "OK" : "ERROR");

    pc.printf("Getting PLC Status......");
    _s7_err = s7.GetPlcStatus(&_s7_status);
    switch (_s7_status)
    {
    case S7CpuStatusRun:
        pc.printf("Running\r\r\n");
        // pc.printf("Stopping PLC\r\r\n");
        // s7.PlcStop();
        break;
    case S7CpuStatusStop:
        pc.printf("Stop\r\r\n");
        // pc.printf("Starting PLC\r\r\n");
        // s7.PlcStart();
        break;
    case S7CpuStatusUnknown:
        pc.printf("Unknown\r\r\n");
        break;
    default:
        break;
    }

    // demo_read_byte();
    // demo_write_byte();
    demo_read_bool();
    while(1)
    {
        demo_write_bool();
        Thread::wait(100);
    }

    pc.printf("\r\r\nEND OF PROGRAM\r\r\n");
    return 0;
}

int demo_read_byte()
{
    pc.printf("\r\r\n");
    pc.printf("Demo: Read DB1 from PLC\r\r\n");
    pc.printf("Getting DB1......");
    _s7_err = s7.ReadArea(S7AreaDB,
                        db_num,
                        0,
                        db_byte_size,
                        S7WLByte,
                        &db_byte_buffer);
    if (_s7_err != 0)
        pc.printf("ERROR 0x%04X\r\r\n",_s7_err);
    else
    {
        pc.printf("OK\r\r\n");
        for (int i=0;i<db_byte_size;i++)
        {
            pc.printf("val %d: %d\r\r\n",i,db_byte_buffer[i]);
        }
        pc.printf("\r\r\n");
    }

    return 0;
}

int demo_write_byte()
{
    pc.printf("\r\r\n");
    pc.printf("Demo: Write DB1 to PLC\r\r\n");
    pc.printf("Writing some random value to DB1......");
    time_t t;
    srand((unsigned) time(&t));

    for(int i=0;i<db_byte_size;i++)
    {
        db_byte_buffer[i] = rand()%255;
    }
    _s7_err = s7.WriteArea(S7AreaDB,
                            db_num,
                            0,
                            db_byte_size,
                            S7WLByte,
                            &db_byte_buffer);
    if(_s7_err !=0)
        pc.printf("ERROR 0x%04X\r\r\n",_s7_err);
    else
    {
        pc.printf("OK\r\r\n");
        pc.printf("Bytes written to DB1: \r\r\n[ ");
        for(int i=0;i<=db_byte_size;i++)
        {
            pc.printf("0x%02X  ",db_byte_buffer[i]);
        }
        pc.printf("]\r\r\n");
    }
    
    return 0;
}
int demo_read_bool()
{
    pc.printf("\r\r\n");
    pc.printf("Demo: Read DB Bits from PLC\r\r\n");
    pc.printf("Getting DB Bits......");
    for(int i=0;i<db_bool_size;i++)
    {
         _s7_err = s7.ReadBit(S7AreaDB,
                            1,
                            i,
                            db_bool_buffer[i]);
    }
    if (_s7_err != 0)
        pc.printf("ERROR 0x%04X\r\r\n",_s7_err);
    else
    {
        pc.printf("OK\r\r\n");
        for (int i=0;i<db_bool_size;i++)
        {
            pc.printf("bit %d: %d\r\r\n",i,db_bool_buffer[i]);
        }
        pc.printf("\r\r\n");
    }

    return 0;
}

/*Not run yet*/
int demo_write_bool()
{
    pc.printf("\r\r\n");
    pc.printf("Demo: Write DB1 Bool to PLC\r\r\n");
    pc.printf("Writing some random value to DB1......");
    
    time_t t;
    srand((unsigned) time(&t));

    for(int i=0;i<db_bool_size;i++)
    {
        db_bool_buffer[i] = rand() & 1;
    }

    for(int i=0;i<db_bool_size;i++)
    {
        _s7_err = s7.WriteBit(S7AreaDB,
                            1,
                            i,
                            db_bool_buffer[i]);
    }
    if(_s7_err !=0)
        pc.printf("ERROR 0x%04X\r\r\n",_s7_err);
    else
    {
        pc.printf("OK\r\r\n");
        // pc.printf("Bools written to DB1: \r\r\n[ ");
        // for(int i=0;i<=db_bool_size;i++)
        // {
        //     pc.printf("%d  ",db_bool_buffer[i]);
        // }
        // pc.printf("]\r\r\n");
    }
    
    return 0;
}