//
// Created by Jin on 8/12/19.
//

#ifndef GFSERIAL_GFSERIAL_H
#define GFSERIAL_GFSERIAL_H

#include <Arduino.h>
#include <Vector.h>
#include <Streaming.h>
#define PACKET_COUNT_MAX 50
#define MEGA6_ECHO_COUNT 4

class RS485TEENSY {
public:
    RS485TEENSY(int BPS, int Teensy_ID);
    void InitializeCommunication();
    void SendSerialPacket(char ID, String Packet, bool isConfirmParity = true);
    String PacketGenerator_485(char ReceiverID, char MotorID, String string_command);
    String PacketGenerator_485(char ReceiverID, char MotorID, char char_command, String string_command);
    String PacketGenerator_485(char ReceiverID, char MotorID, char char_command);
    String PacketGenerator_485(char ReceiverID, char char_command);
    String PacketGenerator_485(char ReceiverID, char MotorID, char char_command, char char_value);
    String PacketGenerator_485(char ReceiverID, char MotorID, char set_command, int int_value);
    String PacketGenerator_485(char ReceiverID, char MotorID, char set_command1, char set_command2, int int_value);
    String PacketGenerator_485(char ReceiverID, char MotorID, char set_command1, char set_command2, float float_value);
    String PacketGenerator_485(char ReceiverID, char MotorID, char set_command, float float_value);
    String PacketGenerator(char BoardID, char set);
    String PacketGenerator(char BoardID, String strValue);
    String PacketGenerator(int line_space, String strValue);
    bool CheckInitializeUART(String Echo);
    bool CheckSerialPacket(char ID, String strExpectedEcho);ß
    String EchoGenerator(char sender_id, char motor_id, char char_command, char char_value);
    String EchoGenerator(char sender_id, char motor_id, String string_command);
    String EchoGenerator(char sender_id, char motor_id, char char_command, String string_command);
    String EchoGenerator(char sender_id, char motor_id, char char_command);
    String EchoGenerator(char sender_id, char char_command);
    String EchoGenerator(char sender_id, char motor_id, char set_command1, char set_command2, int int_value);
    String EchoGenerator(char sender_id, char motor_id, char set_command1, char set_command2, float float_value);
    bool ReadPacketsBUS(String desired_packet = "", bool is_desired_packet_mode = false, bool ConfirmParityOrCheckSerial = false);
    bool ReadPacketsSlaveBUS(String desired_packet = "", bool is_desired_packet_mode = false, bool ConfirmParityOrCheckSerial = false);
    String ReadBusSerialPacket();
    void FlushEcho(int _mega_id);
    String ParityGenerator(String Packet);
    bool ConfirmParity(char receiver_id);
    String CheckParityPacket(String expected_echo_packet);
    String EchoManagement_Enzyme_TMB_4();
    String EchoManagement_Enzyme_TMB_5();
    String EchoManagement_Enzyme_TMB_6();
    bool ParityProcessing(String packet_with_parity);
    bool ConfirmParityProcessing(String packet_with_parity);
    bool ParityChecker(String Packet);


public:
    String storage_array_0[PACKET_COUNT_MAX];
    String storage_array_1[PACKET_COUNT_MAX];
    String storage_array_2[PACKET_COUNT_MAX];
    String storage_array_3[PACKET_COUNT_MAX];
    String storage_array_4[PACKET_COUNT_MAX];
    String storage_array_5[PACKET_COUNT_MAX];
    String storage_array_6[PACKET_COUNT_MAX];
    String storage_array_7[PACKET_COUNT_MAX];

    String storage_echo_6[MEGA6_ECHO_COUNT];

    String storage_array_parity_0[PACKET_COUNT_MAX];
    String storage_array_parity_1[PACKET_COUNT_MAX];
    String storage_array_parity_2[PACKET_COUNT_MAX];
    String storage_array_parity_3[PACKET_COUNT_MAX];
    String storage_array_parity_4[PACKET_COUNT_MAX];
    String storage_array_parity_5[PACKET_COUNT_MAX];
    String storage_array_parity_6[PACKET_COUNT_MAX];
    String storage_array_parity_7[PACKET_COUNT_MAX];

    Vector<String> packet_storage_mega0;
    Vector<String> packet_storage_mega1;
    Vector<String> packet_storage_mega2;
    Vector<String> packet_storage_mega3;
    Vector<String> packet_storage_mega4;
    Vector<String> packet_storage_mega5;
    Vector<String> packet_storage_mega6;
    Vector<String> packet_storage_mega7;

    Vector<String> echo_storage_mega6;

    Vector<String> parity_storage_mega0;
    Vector<String> parity_storage_mega1;
    Vector<String> parity_storage_mega2;
    Vector<String> parity_storage_mega3;
    Vector<String> parity_storage_mega4;
    Vector<String> parity_storage_mega5;
    Vector<String> parity_storage_mega6;
    Vector<String> parity_storage_mega7;

    String storage_array_process[PACKET_COUNT_MAX];
    Vector<String> process_storage;

    String serial_input_Mega1 = "";
    String serial_input_Mega2 = "";
    String serial_input_Mega3 = "";
    String serial_input_Mega4 = "";
    String serial_input_Mega5 = "";
    String serial_input_Mega6 = "";

    String enzyme_tmb_echo_mega4 = "";
    String enzyme_tmb_echo_mega5 = "";
    String enzyme_tmb_echo_mega6 = "";

    String confirm_parity_echo = "";
    String confirm_parity_echo_wo_parity = "";
    String check_serial_echo_packet = "";
    String serial_packet_echo = "";
    String serial_packet_echo_4 = "";
    String serial_packet_echo_5 = "";
    String serial_packet_echo_6 = "";

    int count_6sT = 0;
    int count_6sG = 0;


private:
    //module_number: 0 - Teensy, 1 - Mega, 2 - Laptop
    //Teensy: Serial2, _TE: pin11, _RX: pin9, _TX: pin10
    //Mega: Serial3, _TE: pin7, _RX: 15, _TX: pin14
    int _mainDelay = 100;
    int _slaveDelay = 400;
    int _parity_echo_delay = 1000;
    char Serial_ID_0[7] = {'0', '1', '2', '3', 'R', 'T', 'P'}; //NicuMax
    char Serial_ID_1[7] = {'0', '1', '2', '3', 'R', 'P', 'S'}; //BsiMax 1
    char Serial_ID_2[7] = {'0', '4', '5', '6', 'T', 'H', 'M'}; //BsiMax 2
    const char ID_Serial_0 = Serial_ID_0[0];
    char ID_Serial_M1 = Serial_ID_0[1];
    char ID_Serial_M2 = Serial_ID_0[2];
    char ID_Serial_M3 = Serial_ID_0[3];
    char ID_Serial_U3 = Serial_ID_0[4];
    char ID_Serial_U4 = Serial_ID_0[5];
    char ID_Serial_U5 = Serial_ID_0[6];


private:
    const int _TEENSY_TE_PIN_MEGA = 11;
    const int _TEENSY_TE_PIN_PC = 2;
    String _latest_sending_packet = "";
    uint32_t _BPS = 9600;
    int _BPS_PRINTER = 19200;
    int _Teensy_ID = 0;
    String _start_delimiter = "*"; //ASCII 42
    String _end_delimiter = "/";
    String Teensy_RS485_ID = "0";
    char ch_Teensy_RS485_ID = '0';
    unsigned long interval_echo_resending = 30000;
};


#endif //GFSERIAL_GFSERIAL_H
