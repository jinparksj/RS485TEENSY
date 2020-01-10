//
//  Created by Jin on 8/12/19.
//  Author: Jin
//

#include "RS485TEENSY.h"

RS485TEENSY::RS485TEENSY(int BPS, int Teensy_ID) {
    _BPS = BPS;
    _Teensy_ID = Teensy_ID;
    if (_Teensy_ID == 1) {
        ch_Teensy_RS485_ID = '0';
        Teensy_RS485_ID = "0";
    }
    else if (_Teensy_ID == 2) {
        ch_Teensy_RS485_ID = '7';
        Teensy_RS485_ID = "7";
    }
    packet_storage_mega0.setStorage(storage_array_0);
    packet_storage_mega1.setStorage(storage_array_1);
    packet_storage_mega2.setStorage(storage_array_2);
    packet_storage_mega3.setStorage(storage_array_3);
    packet_storage_mega4.setStorage(storage_array_4);
    packet_storage_mega5.setStorage(storage_array_5);
    packet_storage_mega6.setStorage(storage_array_6);
    packet_storage_mega7.setStorage(storage_array_7);

    parity_storage_mega0.setStorage(storage_array_parity_0);
    parity_storage_mega1.setStorage(storage_array_parity_1);
    parity_storage_mega2.setStorage(storage_array_parity_2);
    parity_storage_mega3.setStorage(storage_array_parity_3);
    parity_storage_mega4.setStorage(storage_array_parity_4);
    parity_storage_mega5.setStorage(storage_array_parity_5);
    parity_storage_mega6.setStorage(storage_array_parity_6);
    parity_storage_mega7.setStorage(storage_array_parity_7);

    if (_Teensy_ID == 1) {
        echo_storage_mega6.setStorage(storage_echo_6);
        echo_storage_mega6.push_back(EchoGenerator('6', 'e', 'T'));
        echo_storage_mega6.push_back(EchoGenerator('6', 'e', 'G'));

        enzyme_tmb_echo_mega4 = EchoGenerator('4', 'e', 'T');
        enzyme_tmb_echo_mega5 = EchoGenerator('5', 'e', 'T');
    }
    else {
        process_storage.setStorage(storage_array_process);
    }
}

void RS485TEENSY::InitializeCommunication() {
    Serial1.transmitterEnable(_TEENSY_TE_PIN_PC);
    Serial1.begin(_BPS);
    while(!Serial1);

    Serial2.transmitterEnable(_TEENSY_TE_PIN_MEGA);
    Serial2.begin(_BPS);
    while(!Serial2);

    if (_Teensy_ID == 1) {
        Serial4.begin(_BPS_PRINTER);
    }
    else {
        Serial4.begin(_BPS);
        while(!Serial4 && (millis() < 10000));
    }


    Serial.println("%Teensy RS-485 & UART initialization completed");
}

void RS485TEENSY::SendSerialPacket(char ID, String Packet, bool isConfirmParity = true) {
    String count_packet = Packet;
    switch (ID) {
        case 'B':
            String temp_parity_packet = ParityGenerator(Packet);
            Packet = Packet + temp_parity_packet + _end_delimiter;
            _latest_sending_packet = Packet;
            break;
        case '0':
        case '3':
        case '4':
        case '5':
            Packet = Packet + _end_delimiter;
            break;
    }

    switch (ID) {
        case '0':
            Serial1.print(Packet);
            break;
        case 'B':
            Serial2.print(Packet);
            break;
        case '3':
            Serial3.print(Packet);
            break;
        case '4':
            Serial4.print(Packet);
            break;
        case '5':
            Serial5.print(Packet);
            break;
    }
    delay(_mainDelay);

    Serial.print("Sent : ");
    Serial.println(Packet);

    if (isConfirmParity && (ID == 'B')) {
        while (!ConfirmParity(Packet[1])) {
            Serial.println("%Confirm Parity Echo Process...");
            delay(1000);
        }
    }
}

void RS485TEENSY::SendSerialPacketB1(char ID, String Packet, bool isConfirmParity) {
    switch (ID) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            String parity_packet = ParityGenerator(Packet);
            Packet = Packet + parity_packet + _end_delimiter;
            _latest_sending_packet = Packet;
            break;
        case '0':
        case 'R':
        case 'S':
            Packet = Packet + _end_delimiter;
            break;
    }
    switch (ID) {
        case '0':
            Serial1.print(Packet);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            Serial2.print(Packet);
            break;
        case 'R':
            Serial3.print(Packet);
            break;
        case 'P':
            if (Packet[0] == '0') {
                Serial4.print(Packet.substring(1));
            }
            else if (Packet[0] == '1') {
                Serial4.println(Packet.substring(1));
            }
            break;
        case 'S':
            Serial5.print(Packet);
            break;
    }
    delay(_mainDelay);
    Serial.print("Sent : "); Serial.println(Packet);
    if (isConfirmParity) {
        Serial.println("%Confirm Parity Echo Process...");
        while(!ConfirmParity(Packet[1])){
            delay(1000);
        }
    }
}

void RS485TEENSY::SendSerialPacketB2(char ID, String Packet, bool isConfirmParity = true) {
    switch (ID) {
        case '4':
        case '5':
        case '6':
            String parity_packet = ParityGenerator(Packet);
            Packet = Packet + parity_packet + _end_delimiter;
            _latest_sending_packet = Packet;
            break;
        case '0':
        case 'T':
        case 'H':
        case 'M':
            Packet = Packet + _end_delimiter;
            break;
    }

    switch (ID) {
        case '0':
            Serial1.print(Packet);
            break;
        case '4':
        case '5':
        case '6':
            Serial2.print(Packet);
            break;
        case 'T':
            Serial3.print(Packet);
            break;
        case 'H':
            Serial4.print(Packet);
            break;
        case 'M':
            Serial5.print(Packet);
            break;
    }
    delay(_mainDelay);
    Serial.print("Sent : "); Serial.println(Packet);
    if (isConfirmParity) {
        Serial.println("%Confirm Parity Echo Process...");
        while(!ConfirmParity(Packet[1])){
            delay(1000);
        }
    }

}

//For NicuMax
void RS485TEENSY::SendSerialPacketN(char ID, String Packet, bool isConfirmParity = true) {
    switch (ID) {
        case '1':
        case '2':
        case '3':
            String parity_packet = ParityGenerator(Packet);
            Packet = Packet + parity_packet + _end_delimiter;
            _latest_sending_packet = Packet;
            break;
        case '0':
        case 'R':
        case 'T':
            Packet = Packet + _end_delimiter;
            break;
    }

    switch (ID) {
        case '0':
            Serial1.print(Packet);
            break;
        case '1':
        case '2':
        case '3':
            Serial2.print(Packet);
            break;
        case 'R':
            Serial3.print(Packet);
            break;
        case 'T':
            Serial4.print(Packet);
            break;
        case 'P':
            if (Packet[0] == '0') {
                Serial5.print(Packet.substring(1));
            }
            else if (Packet[0] == '1') {
                Serial5.println(Packet.substring(1));
            }
            break;
    }
    delay(_mainDelay);
    Serial.print("Sent : "); Serial.println(Packet);

    if (isConfirmParity) {
        Serial.println("%Confirm Parity Echo Process...");
        while(!ConfirmParity(Packet[1])){
            delay(1000);
        }
    }
}

//RS-485
String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, String string_command) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + string_command;
    return stringPacket;
}
String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char char_command, String string_command){
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(char_command) + string_command;
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char char_command) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(char_command);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char char_command) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(char_command);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char char_command, char char_value) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(char_command) + String(char_value);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char set_command, int int_value) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(set_command) + String(int_value);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char set_command1, char set_command2, int int_value) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(set_command1) + String(set_command2) + String(int_value);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char set_command1, char set_command2, float float_value){
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(set_command1) + String(set_command2) + String(float_value);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator_485(char ReceiverID, char MotorID, char set_command, float float_value) {
    String stringPacket = _start_delimiter + String(ReceiverID) + _start_delimiter + Teensy_RS485_ID + String(MotorID) + String(set_command)  + String(float_value);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator(char BoardID, char set) {
    String stringPacket = String(BoardID) + String(set);
    return stringPacket;
}

String RS485TEENSY::PacketGenerator(char BoardID, String strValue) {
    String stringPacket = String(BoardID) + strValue;
    return stringPacket;
}

String RS485TEENSY::PacketGenerator(int line_space, String strValue) {
    String stringPacket = String(line_space) + strValue;
    return stringPacket;
}

String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, String string_command) {
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + string_command;
    return stringPacket;
}

String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, char char_command, String string_command){
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + String(char_command) + string_command;
    return stringPacket;
}


String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, char char_command) {
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + String(char_command);
    return stringPacket;
}
String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, char char_command, char char_value){
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + String(char_command) + String(char_value);
    return stringPacket;
}

String RS485TEENSY::EchoGenerator(char sender_id, char char_command) {
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(char_command);
    return stringPacket;
}

String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, char set_command1, char set_command2, int int_value){
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + String(set_command1) + String(set_command2) + String(int_value);
    return stringPacket;
}

String RS485TEENSY::EchoGenerator(char sender_id, char motor_id, char set_command1, char set_command2, float float_value){
    String stringPacket = _start_delimiter + Teensy_RS485_ID + _start_delimiter + String(sender_id) + String(motor_id) + String(set_command1) + String(set_command2) + String(float_value);
    return stringPacket;
}

void RS485TEENSY::FlushEcho(int _mega_id) {
    switch (_mega_id) {
        case 0:
            serial_input_Mega1 = "";
            serial_input_Mega2 = "";
            serial_input_Mega3 = "";
            serial_input_Mega4 = "";
            serial_input_Mega5 = "";
            serial_input_Mega6 = "";
            break;
        case 1:
            serial_input_Mega1 = "";
            break;
        case 4:
            serial_input_Mega4 = "";
            break;
        case 2:
            serial_input_Mega2 = "";
            break;
        case 5:
            serial_input_Mega5 = "";
            break;
        case 3:
            serial_input_Mega3 = "";
            break;
        case 6:
            serial_input_Mega6 = "";
            break;
    }
    delay(_mainDelay);
}

// Use for CheckSerialPacket
String RS485TEENSY::CheckParityPacket(String expected_echo_packet) {
    Serial.print("CheckParityPacket - Parity Check Received ID : "); Serial.println(expected_echo_packet[3]);
    while(!ReadPacketsBUS(expected_echo_packet, true, false)); // should be false ConfirmParityOrCheckSerial
    String result_packet = check_serial_echo_packet;
    check_serial_echo_packet = "";
    return result_packet;
}

bool RS485TEENSY::CheckInitializeUART(String Echo) {
    String strEcho;
    unsigned long _start_time = millis();
    while (Serial5.available() == 0) {
        if ((millis() - _start_time) > 5000) {
            return false;
        }
    }
    strEcho = Serial5.readStringUntil('/');
    if (strEcho != Echo) {
        Serial.print("Echo wrong : "); Serial.print("Expected Echo - "); Serial.print(Echo); Serial.print(" / ");
        Serial.print("Echo from Teensy 2 - "); Serial.println(strEcho);
        return false;
    }
    else {
        Serial.println("Echo is fine");
        return true;
    }
}

//Teensy-use
bool RS485TEENSY::CheckSerialPacket(char ID, String strExpectedEcho){
    Serial.println("CheckSerialPacket");
    //return true; //Debug
    String strEcho;
    switch (ID) {
        case '0':
            while (Serial1.available() == 0);
            strEcho = Serial1.readStringUntil('/');
            break;
        case 'B':
            strEcho = CheckParityPacket(strExpectedEcho); // strEcho has no parity packet already being through packet processing
            break; //no need parity processing
        case '3':
            while(Serial3.available() == 0);
            strEcho = Serial3.readStringUntil('/');
            break;
        case '4':
            while(Serial4.available() == 0);
            strEcho = Serial4.readStringUntil('/');
            break;
        case '5':
            while (Serial5.available() == 0);
            strEcho = Serial5.readStringUntil('/');
            break;
        default:
            break;
    }

    if (ID == 'B') {
        String output = "Received from ";
        String colon = ": ";
        output = output + ID + colon + strEcho;

//        Serial.print("Expected Echo : "); Serial.print(strExpectedEcho);
//        Serial.print(" / ");
//        Serial.print("After Parity Check, Echo : "); Serial.println(output);


        if (strEcho != strExpectedEcho) {
            return false;
        }

    }
    else {
        if (strEcho != strExpectedEcho) {
            Serial.print("Coming in Echo : "); Serial.print(strEcho); Serial.print(" / ");
            Serial.print("Expected Echo : "); Serial.print(strExpectedEcho);
            return false;
        }
    }
    return true;
}

String RS485TEENSY::EchoManagement_Enzyme_TMB_4(){
    Serial.println("Library EchoManagement_Enzyme_TMB - 4");
    while(!ReadPacketsBUS("*0*4", false, false));

    Serial.println("Library EchoManagement_Enzyme_TMB - 4");
    Serial.println("Packet Storage Check");
    Serial.print("EchoManagement_Enzyme_TMB_4 - Packet Storage 4 : ");
    Serial << packet_storage_mega4 << endl;

    if (!packet_storage_mega4.empty()) {
        Serial.print("EchoManagement_Enzyme_TMB 4: ");
        bool isExisting = false;
        String temp_packet_with_parity = "";
        String temp_packet_without_parity = "";
        for (int i = 0; i < packet_storage_mega4.size(); i++) {
            temp_packet_with_parity = packet_storage_mega4.at(i);
            temp_packet_without_parity = temp_packet_with_parity.substring(0, temp_packet_with_parity.length() - 3);
            if (temp_packet_without_parity == enzyme_tmb_echo_mega4) {
                Serial.print("EchoManagement_Enzyme_TMB_4 - there is a same packet : ");
                Serial.println(temp_packet_with_parity);
                Serial << packet_storage_mega4 << endl;
                packet_storage_mega4.remove(i);
                Serial << packet_storage_mega4 << endl;
                isExisting = true;
                break;
            }
        }
        if (isExisting) {
            for (int k = 0; k < packet_storage_mega4.size(); k++) {
                if (temp_packet_with_parity == packet_storage_mega4.at(k)) {
                    packet_storage_mega4.remove(k);
                    Serial << packet_storage_mega4 << endl;
                }
            }
            return temp_packet_without_parity;
        }
        else {
            return "";
        }
    }
    else {
        return "";
    }
}


String RS485TEENSY::EchoManagement_Enzyme_TMB_5(){
    Serial.println(" Library EchoManagement_Enzyme_TMB - 5");
    //DEBUG 4 -> 5
    while(!ReadPacketsBUS("*0*5", false, false));

    Serial.println(" Library EchoManagement_Enzyme_TMB - 5");
    Serial.println(" Packet Storage Check ");
    Serial.print("Packet Storage 5 : ");
    Serial << packet_storage_mega5 << endl;

    if (!packet_storage_mega5.empty()) {
        Serial.print("EchoManagement_Enzyme_TMB 5: ");
        bool isExisting = false;
        String temp_packet_with_parity = "";
        String temp_packet_without_parity = "";
        for (int i = 0; i < packet_storage_mega5.size(); i++) {
            temp_packet_with_parity = packet_storage_mega5.at(i);
            temp_packet_without_parity = temp_packet_with_parity.substring(0, temp_packet_with_parity.length() - 3);
            if (temp_packet_without_parity == enzyme_tmb_echo_mega5) {
                Serial.print(" EchoManagement_Enzyme_TMB_5 - there is a same packet : ");
                Serial.println(temp_packet_with_parity);
                Serial << packet_storage_mega5 << endl;
                packet_storage_mega5.remove(i);
                Serial << packet_storage_mega5 << endl;
                isExisting = true;
                break;
            }
        }
        if (isExisting) {
            for (int k = 0; k < packet_storage_mega5.size(); k++) {
                if (temp_packet_with_parity == packet_storage_mega5.at(k)) {
                    packet_storage_mega5.remove(k);
                    Serial << packet_storage_mega5 << endl;
                }
            }
            return temp_packet_without_parity;
        }
        else {
            return "";
        }
    }
    else {
        return "";
    }
}

String RS485TEENSY::EchoManagement_Enzyme_TMB_6(){
    Serial.println(" Library EchoManagement_Enzyme_TMB - 6");
    while(!ReadPacketsBUS("*0*6", false, false));

    Serial.println(" Library EchoManagement_Enzyme_TMB - 6");
    Serial.println(" Packet Storage Check ");
    Serial.print("EchoManagement_Enzyme_TMB_6 - Packet Storage 6 : ");
    Serial << packet_storage_mega6 << endl;


    if (!packet_storage_mega6.empty()) {
        Serial.print("EchoManagement_Enzyme_TMB 6: ");
        bool isExisting = false;
        for (int i = 0; i < packet_storage_mega6.size(); i++) {
            for (int j = 0; j < echo_storage_mega6.size(); j++) {
                String temp_packet_with_parity = packet_storage_mega6.at(i);
                String temp_packet_without_parity = temp_packet_with_parity.substring(0, temp_packet_with_parity.length() - 3);
                if (temp_packet_without_parity == echo_storage_mega6.at(j)) {
                    Serial.print(" EchoManagement_Enzyme_TMB_6 - there is a same packet : ");
                    Serial.println(temp_packet_with_parity);
                    Serial << packet_storage_mega6 << endl;
                    packet_storage_mega6.remove(i);
                    Serial << packet_storage_mega6 << endl;
                    for (int k = 0; k < packet_storage_mega6.size(); k++) {
                        if (temp_packet_with_parity == packet_storage_mega6.at(k)) {
                            packet_storage_mega6.remove(k);
                            Serial << packet_storage_mega6 << endl;
                        }
                    }
                    return temp_packet_without_parity;
                }
            }
        }
        return "";
    }
    else {
        return "";
    }
}

//Teensy-use
bool RS485TEENSY::CheckSerialPacketB1(char ID, String strExpectedEcho){
    //return true; //Debug
    String strEcho;
    switch (ID) {
        case '0':
            while (Serial1.available() == 0);
            strEcho = Serial1.readStringUntil('/');
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            while (Serial2.available() == 0);
            strEcho = Serial2.readStringUntil('/');
            delay(20);
            if (strEcho[1] != ch_Teensy_RS485_ID) {
                Serial.println(strEcho);
                return false;
            }
            strEcho = strEcho.substring(0, strEcho.length() - 3);
            break;
        case 'R':
            while(Serial3.available() == 0);
            strEcho = Serial3.readStringUntil('/');
            break;
        case 'S':
            while (Serial5.available() == 0);
            strEcho = Serial5.readStringUntil('/');
            break;
        default:
            break;
    }
    delay(_mainDelay);

    Serial.print("ID : "); Serial.print(ID);
    Serial.print(" - "); Serial.print("Expected : "); Serial.println(strExpectedEcho);

    String output = "Received from ";
    String colon = ": ";
    output = output + ID + colon + strEcho;

    if (strEcho != strExpectedEcho) {
        if (strEcho[3] == '1') {
            serial_input_Mega1 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega1);
        }
        else if (strEcho[3] == '2') {
            serial_input_Mega2 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega2);
        }
        else if (strEcho[3] == '3') {
            serial_input_Mega3 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega3);
        }

        Serial.print("DIFFERENT : ");
        Serial.print(strExpectedEcho);
        Serial.print(" / ");
        Serial.println(output);
        return false;
    }
    return true;
}

//Teensy-use
bool RS485TEENSY::CheckSerialPacketB2(char ID, String strExpectedEcho){
    //return true; //Debug
    String strEcho;
    switch (ID) {
        case '0':
            while (Serial1.available() == 0);
            strEcho = Serial1.readStringUntil('/');
            break;
        case '4':
        case '5':
        case '6':
            while (Serial2.available() == 0);
            strEcho = Serial2.readStringUntil('/');
            delay(20);
            if (strEcho[1] != ch_Teensy_RS485_ID) {
                Serial.println(strEcho);
                return false;
            }
            strEcho = strEcho.substring(0, strEcho.length() - 3);
            break;
        case 'T':
            while(Serial3.available() == 0);
            strEcho = Serial3.readStringUntil('/');
            break;
        case 'H':
            while (Serial4.available() == 0);
            strEcho = Serial4.readStringUntil('/');
            break;
        default:
            break;
    }
    delay(_mainDelay);

    Serial.print("ID : "); Serial.print(ID);
    Serial.print(" - "); Serial.print("Expected : "); Serial.println(strExpectedEcho);

    String output = "Received from ";
    String colon = ": ";
    output = output + ID + colon + strEcho;

    if (strEcho != strExpectedEcho) {
        if (strEcho[3] == '4') {
            serial_input_Mega1 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega1);
        }
        else if (strEcho[3] == '5') {
            serial_input_Mega2 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega2);
        }
        else if (strEcho[3] == '6') {
            serial_input_Mega3 = strEcho;
            Serial.print("Another Input : ");
            Serial.println(serial_input_Mega3);
        }

        Serial.print("DIFFERENT : ");
        Serial.print(strExpectedEcho);
        Serial.print(" / ");
        Serial.println(output);
        return false;
    }
    return true;
}



//Teensy-use
bool RS485TEENSY::CheckSerialPacketN(char ID, String strExpectedEcho){
    //return true; //Debug
    String strEcho;
    switch (ID) {
        case '0':
            while (Serial1.available() == 0);
            strEcho = Serial1.readStringUntil('/');
            break;
        case '1':
        case '2':
        case '3':
            while (Serial2.available() == 0);
            strEcho = Serial2.readStringUntil('/');
            if (strEcho[1] != ch_Teensy_RS485_ID) {
                Serial.println(strEcho);
                return false;
            }
            strEcho = strEcho.substring(0, strEcho.length() - 3);
            break;
        case 'R':
            while(Serial3.available() == 0);
            strEcho = Serial3.readStringUntil('/');
            break;
        case 'T':
            while (Serial4.available() == 0);
            strEcho = Serial4.readStringUntil('/');
            break;
        default:
            break;
    }

    Serial.print("ID : "); Serial.print(ID);
    Serial.print(" - "); Serial.print("Expected : "); Serial.println(strExpectedEcho);

    String output = "Received from ";
    String colon = ": ";
    output = output + ID + colon + strEcho;

    if (strEcho != strExpectedEcho) {
        Serial.println("DIFFERENT!");
        Serial.println(output);
        return false;
    }
    return true;
}

String RS485TEENSY::ParityGenerator(String Packet){
    int temp_ascii_value = 0;
    int temp_ascii_sum = 0;
    int temp_ascii_mod = 0;
    String parity_packet = "";

    for (int i = 0; i < Packet.length(); i++) {
        temp_ascii_value = Packet[i];
        temp_ascii_sum += temp_ascii_value;
    }
    temp_ascii_mod = temp_ascii_sum % 255;
    if (temp_ascii_mod < 16) {
        parity_packet = String(temp_ascii_mod, HEX);
        parity_packet = "0" + parity_packet;
    }
    else {
        parity_packet = String(temp_ascii_mod, HEX);
    }
    parity_packet = "*" + parity_packet;
    //Serial.print("Parity : "); Serial.println(parity_packet);
    return parity_packet;
}

bool RS485TEENSY::ParityChecker(String Packet) {
    int temp_ascii_value = 0;
    int temp_ascii_sum = 0;
    int temp_ascii_mod = 0;
    String temp_parity_string = "";
    char temp_parity_buffer[3] = "";
    int temp_parity_integer = 0;

    int temp_location_delimiter = Packet.length() - 3;
    //  1. First Check : '*'?
    if (Packet[temp_location_delimiter] != '*') {
        return false;
    } //location '*': length - 3

    // 2. Second Check : Ascii sum and mod
    for (int i = 0; i < temp_location_delimiter; i++) {
        temp_ascii_value = Packet[i];
        temp_ascii_sum += temp_ascii_value;
    }
    temp_ascii_mod = temp_ascii_sum % 255;

    // 3. String Hex to Int
    temp_parity_string = Packet.substring(temp_location_delimiter + 1);
    temp_parity_string.toCharArray(temp_parity_buffer, 3);
    temp_parity_integer = strtol(temp_parity_buffer, 0, 16);
    // 4. Third Check: ascii_mod == parity_buffer ?
    if (temp_parity_integer == temp_ascii_mod) {
        return true;
    }
    else {
        return false;
    }
}


bool RS485TEENSY::ReadPacketsBUS(String desired_packet = "", bool is_desired_packet_mode = false, bool ConfirmParityOrCheckSerial = false) {
    String bus_packet;
    int packet_count = 0;
    int first_packet_integer = 0;
    // * Resend is used only for ConfirmParity. CheckSerialPacket is not used/
    unsigned long start_time = millis();
    unsigned long interval_echo = interval_echo_resending; // if there is no serial packet about parity echo, it resends the lastest packet again.
    bool is_return_true = false;
    String _latest_coming_packet = ""; //Protect duplicated conditions.
    char expected_sender_id = desired_packet[3];
    // Waiting until Serial Packets come in.
    // ConfirmParityOrCheckSerial try to get *0*6O
    while (Serial2.available() == 0){
        if (ConfirmParityOrCheckSerial) {
            if ((millis() - start_time) > interval_echo) {
                Serial.print("ReadPacketsBUS/Confirm Parity - Resend Again : ");
                Serial.println(_latest_sending_packet);
                Serial2.print(_latest_sending_packet);
                start_time = millis();
            }
        }
        else if (!is_desired_packet_mode && !ConfirmParityOrCheckSerial){
            if ((millis() - start_time) > (interval_echo / 15)) {
                is_return_true = true;
                break;
            }
        }
        else {
            if ((millis() - start_time) > (interval_echo / 3)) {
                if (expected_sender_id == '0') {
                    if (!packet_storage_mega0.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega0 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '1') {
                    if (!packet_storage_mega1.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega1 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '2') {
                    if (!packet_storage_mega2.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega2 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '3') {
                    if (!packet_storage_mega3.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega3 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '4') {
                    if (!packet_storage_mega4.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega4 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '5') {
                    if (!packet_storage_mega5.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega5 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '6') {
                    if (!packet_storage_mega6.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega6 << endl;
                        break;
                    }
                }
                else if (expected_sender_id == '7') {
                    if (!packet_storage_mega7.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        Serial << packet_storage_mega7 << endl;
                        break;
                    }
                }
            }
        }
    }

    while (Serial2.available() > 0){
        //IF - BREAK for Null, \n, \r -> they mean being 'empty'
//        Serial.print("ReadPacketsBUS - "); Serial.println(packet_count);
        packet_count++;
        delay(50);
        bus_packet = Serial2.readStringUntil('/');
        delay(50);

        first_packet_integer = bus_packet[0];
        char id_packet = bus_packet[1];
        char sender_packet = bus_packet[3];
        char parity_confirm_packet = bus_packet[4];
        delay(5);
        //1. bus_packet length is less than 5, it is
        if (bus_packet[0] != '*' || bus_packet[2] != '*' || bus_packet.length() < 8) {
            Serial.print("ReadPacketsBUS - Corrupted Data : "); Serial.println(bus_packet);
        }

        //2. bus_packet corruption case 1
        if (first_packet_integer == -1) {
//            Serial.print("ReadPacketsBUS - Corrupted Data (-1) : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //3. bus_packet corruption case 2
        if (first_packet_integer <= 32) {
//            Serial.print("ReadPacketsBUS - Null or Line Space, Carriage Return : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //4. / case
        if (first_packet_integer == 47) {
//            Serial.print("ReadPacketsBUS - Delimiter '/' : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //5. not my packet
        if (id_packet != ch_Teensy_RS485_ID) {
//            Serial.print("ReadPacketsBUS - This Serial Packet is not belong to this device : "); Serial.println(bus_packet);
            bus_packet = "";
            continue;
        }

        //6. Protect Duplicated Serial Packet Case
        if (_latest_coming_packet == bus_packet) {
//            Serial.print("ReadPacketsBUS - Duplicated Case : "); Serial.print(_latest_coming_packet); Serial.print(" : "); Serial.println(bus_packet);
            bus_packet = "";
            delay(200);
            continue;
        }

        bool is_parity_processing = true;
        //7. Parity Processing - only for CheckSerialPacket, No <ConfirmParity -> ConfirmParityOrCheckSerial: true>
        if (bus_packet[4] == 's' || bus_packet[4] == 'e' || bus_packet[4] == '1' || bus_packet[4] == '2' || bus_packet[4] == '3' \
        || bus_packet[4] == '4' || bus_packet[4] == '5' || bus_packet[4] == '6' || bus_packet[4] == '7' || bus_packet[4] == '0') {
            is_parity_processing = false;
            if(!ParityProcessing(bus_packet)){
//                Serial.print("ReadPacketsBUS - Parity Packet Error : "); Serial.println(bus_packet);
                bus_packet = "";
                continue;
            }
        }

        //8. Protect falling into infinite loop due to communication corrupted error
        if (ConfirmParityOrCheckSerial && is_desired_packet_mode && is_parity_processing){
            if(!ConfirmParityProcessing(bus_packet)) {
                bus_packet = desired_packet;
                bus_packet = bus_packet + ParityGenerator(bus_packet);
                first_packet_integer = bus_packet[0];
                id_packet = bus_packet[1];
                sender_packet = bus_packet[3];
                parity_confirm_packet = bus_packet[4];
            }
        }




        delay(20);
        String bus_packet_wo_parity = bus_packet.substring(0, bus_packet.length() - 3);
        //8. Distribution to Parity and Packet Storages
        //  1) sender ID
        switch (sender_packet) {
            case '0':
                //  2)  packet_storage_mega or parity_storage_mega
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega0.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    //  3) N case: resend latest sending packet and wait 0.5S, Go To the beginning of while loop
//                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                    //  4) Check Serial Packet case
                else {
                    if (bus_packet_wo_parity != desired_packet) {
//                        Serial.println("ReadPacketsBUS - Not Desired Packet"); Serial.println(bus_packet);
//                        Serial.println("ReadPacketsBUS - Desired Packet : "); Serial.println(desired_packet);
                        packet_storage_mega0.push_back(bus_packet);
                        Serial << packet_storage_mega0 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega0.size(); i++) {
                            if (bus_packet == packet_storage_mega0.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega0 << endl;
                                packet_storage_mega0.remove(i);
                                Serial << packet_storage_mega0 << endl;
                            }
                        }
                    }
                }
                break;
            case '1':
                //  2)  packet_storage_mega or parity_storage_mega
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega1.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    //  3) N case: resend latest sending packet and wait 0.5S, Go To the beginning of while loop
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;//////TOMORROW CHECK IT WORKS??!!
                }
                //  4) Check Serial Packet case
                else {
                    if (bus_packet_wo_parity != desired_packet) {
                        Serial.println("ReadPacketsBUS - Not Desired Packet"); Serial.println(bus_packet);
                        Serial.println("ReadPacketsBUS - Desired Packet : "); Serial.println(desired_packet);
                        packet_storage_mega1.push_back(bus_packet);
                        Serial << packet_storage_mega1 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega1.size(); i++) {
                            if (bus_packet == packet_storage_mega1.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega1 << endl;
                                packet_storage_mega1.remove(i);
                                Serial << packet_storage_mega1 << endl;
                            }
                        }
                    }
                }
                break;
            case '2':
                if (parity_confirm_packet == 'O') {
                    Serial.println("ReadPacketsBUS/Confirm Parity 2 - O Case");
                    parity_storage_mega2.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity 2 - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 2 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega2.push_back(bus_packet);
                        Serial << packet_storage_mega2 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega2.size(); i++) {
                            if (bus_packet == packet_storage_mega2.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega2 << endl;
                                packet_storage_mega2.remove(i);
                                Serial << packet_storage_mega2 << endl;
                            }
                        }
                    }
                }
                break;
            case '3':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega3.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 3 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega3.push_back(bus_packet);
                        Serial << packet_storage_mega3 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega3.size(); i++) {
                            if (bus_packet == packet_storage_mega3.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega3 << endl;
                                packet_storage_mega3.remove(i);
                                Serial << packet_storage_mega3 << endl;
                            }
                        }
                    }
                }
                break;
            case '4':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega4.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 4 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega4.push_back(bus_packet);
                        Serial << packet_storage_mega4 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega4.size(); i++) {
                            if (bus_packet == packet_storage_mega4.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega4 << endl;
                                packet_storage_mega4.remove(i);
                                Serial << packet_storage_mega4 << endl;
                            }
                        }
                    }
                }
                break;
            case '5':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega5.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 5 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega5.push_back(bus_packet);
                        Serial << packet_storage_mega5 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega5.size(); i++) {
                            if (bus_packet == packet_storage_mega5.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega5 << endl;
                                packet_storage_mega5.remove(i);
                                Serial << packet_storage_mega5 << endl;
                            }
                        }
                    }
                }
                break;
            case '6':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega6.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 6 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega6.push_back(bus_packet);
                        Serial << packet_storage_mega6 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega6.size(); i++) {
                            if (bus_packet == packet_storage_mega6.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega6 << endl;
                                packet_storage_mega6.remove(i);
                                Serial << packet_storage_mega6 << endl;
                            }
                        }
                    }
                }
                break;
            case '7':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega7.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 7 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega7.push_back(bus_packet);
                        Serial << packet_storage_mega7 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega7.size(); i++) {
                            if (bus_packet == packet_storage_mega7.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega7 << endl;
                                packet_storage_mega7.remove(i);
                                Serial << packet_storage_mega7 << endl;
                            }
                        }
                    }
                }
                break;
        }

        //9. Print bus serial packets
        if ((parity_confirm_packet == 'O') || (parity_confirm_packet == 'N')) {
//            Serial.print("$$$ ReadPacketsBUS - Parity Packet : "); Serial.println(bus_packet);
        }
        else {
//            Serial.print("$$$ ReadPacketsBUS - Serial Packet : "); Serial.println(bus_packet);
        }
        //10. Compare desired packets, is_return_true
        if (is_desired_packet_mode) {
            if(desired_packet == bus_packet_wo_parity) {
                Serial.print("ReadPacketsBUS - It is desired packet : "); Serial.println(desired_packet);
                is_return_true |= true;
                if (!ConfirmParityOrCheckSerial) {
                    check_serial_echo_packet = bus_packet_wo_parity;
                }
            }
            else {
                is_return_true |= false;
            }
        }
        else {
            is_return_true = true;
        }
        _latest_coming_packet = bus_packet;
    }

    //11. ConfirmParity Case: There is a case that check serial echo comes in earlier than start of check serial packet function
    //Need to check whether it is in packet_storage
    //It is activated, when is_return_true is false
    if (!ConfirmParityOrCheckSerial && !is_return_true) {
        String packet_without_parity = "";
        bool isExisting = false;
        switch(expected_sender_id) {
            case '0':
                if (packet_storage_mega0.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 1, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega0.size(); i++) {
                        packet_without_parity = packet_storage_mega0.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega0 << endl;
                            packet_storage_mega0.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '1':
                if (packet_storage_mega1.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 1, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega1.size(); i++) {
                        packet_without_parity = packet_storage_mega1.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega1 << endl;
                            packet_storage_mega1.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '2':
                if (packet_storage_mega2.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 2, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega2.size(); i++) {
                        packet_without_parity = packet_storage_mega2.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega2 << endl;
                            packet_storage_mega2.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '3':
                if (packet_storage_mega3.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 3, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega3.size(); i++) {
                        packet_without_parity = packet_storage_mega3.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega3 << endl;
                            packet_storage_mega3.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }

                break;
            case '4':
                if (packet_storage_mega4.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 4, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega4.size(); i++) {
                        packet_without_parity = packet_storage_mega4.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega4 << endl;
                            packet_storage_mega4.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '5':
                if (packet_storage_mega5.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 5, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega5.size(); i++) {
                        packet_without_parity = packet_storage_mega5.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega5 << endl;
                            packet_storage_mega5.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '6':
                if (packet_storage_mega6.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 6, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega6.size(); i++) {
                        packet_without_parity = packet_storage_mega6.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega6 << endl;
                            packet_storage_mega6.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '7':
                if (packet_storage_mega7.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 7, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega7.size(); i++) {
                        packet_without_parity = packet_storage_mega7.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega7 << endl;
                            packet_storage_mega7.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
        }
        if (isExisting) {
            check_serial_echo_packet = packet_without_parity;
            is_return_true = true;
        }
    }

    //11. If it is not desired packet mode, it always return true. If it is, it returns is_return_true
    if (is_desired_packet_mode) {
        return is_return_true;
    }
    else {
        return true;
    }
}


bool RS485TEENSY::ParityProcessing(String packet_with_parity) {
    //It is only for normal serial packet, not parity confirmation packet.
    String temp_packet_parity_confirm = "";
    String temp_parity = "";
    char receiver_id = packet_with_parity[3];

    if (ParityChecker(packet_with_parity)) {
        Serial.println("ParityProcessing - Parity is Good");
        temp_packet_parity_confirm = PacketGenerator_485(receiver_id, 'O');
        temp_parity = ParityGenerator(temp_packet_parity_confirm);
        temp_packet_parity_confirm = temp_packet_parity_confirm + temp_parity + _end_delimiter;
        Serial2.print(temp_packet_parity_confirm);
        delay(100);
        return true;
    }
    else {
        Serial.println("ParityProcessing - Parity is Bad");
        temp_packet_parity_confirm = PacketGenerator_485(receiver_id, 'N');
        temp_parity = ParityGenerator(temp_packet_parity_confirm);
        temp_packet_parity_confirm = temp_packet_parity_confirm + temp_parity + _end_delimiter;
        Serial2.print(temp_packet_parity_confirm);
        delay(100);
        return false;
    }
}


bool RS485TEENSY::ConfirmParityProcessing(String packet_with_parity) {
    if (ParityChecker(packet_with_parity)) {
        Serial.print("ConfirmParityProcessing - Parity is Good : "); Serial.println(packet_with_parity);
        delay(100);
        return true;
    }
    else {
        Serial.print("ConfirmParityProcessing - Parity is Bad : "); Serial.println(packet_with_parity);
        delay(100);
        return false;
    }
}

bool RS485TEENSY::ConfirmParity(char receiver_id) {
    String expected_parity_echo = EchoGenerator(receiver_id, 'O'); //*1*2O
    while(!ReadPacketsBUS(expected_parity_echo, true, true)); //Until Parity is exactly same with expected_parity_echo
//    Serial.print("ConfirmParity ID : "); Serial.println(receiver_id);
    delay(200);

    switch (receiver_id) {
        case '0':
            if (parity_storage_mega0.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega0.size(); i++) {
                    confirm_parity_echo = parity_storage_mega0.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega0 << endl;
                        parity_storage_mega0.remove(i);
//                        Serial << parity_storage_mega0 << endl;
                        break;
                    }
                }
            }
            break;
        case '1':
            if (parity_storage_mega1.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega1.size(); i++) {
                    confirm_parity_echo = parity_storage_mega1.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega1 << endl;
                        parity_storage_mega1.remove(i);
//                        Serial << parity_storage_mega1 << endl;
                        break;
                    }
                }
            }
            break;
        case '2':
            if (parity_storage_mega2.empty()) {
//                Serial.println("ConfirmParity - Empty 2");
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega2.size(); i++) {
//                    Serial.println("ConfirmParity - Storage");
                    confirm_parity_echo = parity_storage_mega2.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega2 << endl;
                        parity_storage_mega2.remove(i);
//                        Serial << parity_storage_mega2 << endl;
                        break;
                    }
                }
            }
            break;
        case '3':
            if (parity_storage_mega3.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega3.size(); i++) {
                    confirm_parity_echo = parity_storage_mega3.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega3 << endl;
                        parity_storage_mega3.remove(i);
//                        Serial << parity_storage_mega3 << endl;
                        break;
                    }
                }
            }
            break;
        case '4':
            if (parity_storage_mega4.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega4.size(); i++) {
                    confirm_parity_echo = parity_storage_mega4.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega4 << endl;
                        parity_storage_mega4.remove(i);
//                        Serial << parity_storage_mega4 << endl;
                        break;
                    }
                }
            }
            break;
        case '5':
            if (parity_storage_mega5.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega5.size(); i++) {
                    confirm_parity_echo = parity_storage_mega5.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega5 << endl;
                        parity_storage_mega5.remove(i);
//                        Serial << parity_storage_mega5 << endl;
                        break;
                    }
                }
            }
            break;
        case '6':
            if (parity_storage_mega6.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega6.size(); i++) {
                    confirm_parity_echo = parity_storage_mega6.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega6 << endl;
                        parity_storage_mega6.remove(i);
//                        Serial << parity_storage_mega6 << endl;
                        break;
                    }
                }
            }
            break;
        case '7':
            if (parity_storage_mega7.empty()) {
                return false;
            }
            else {
                for (int i = 0; i < parity_storage_mega7.size(); i++) {
                    confirm_parity_echo = parity_storage_mega7.at(i);
                    confirm_parity_echo_wo_parity = confirm_parity_echo.substring(0, confirm_parity_echo.length() - 3);
                    if (confirm_parity_echo_wo_parity == expected_parity_echo) {
//                        Serial.print(" SendSerialPacket - ConfirmParity - there is a same packet : ");
//                        Serial.println(confirm_parity_echo_wo_parity);
//                        Serial << parity_storage_mega7 << endl;
                        parity_storage_mega7.remove(i);
//                        Serial << parity_storage_mega7 << endl;
                        break;
                    }
                }
            }
            break;
    }
    Serial.print("ConfirmParity - Parity Storage - "); Serial.print(receiver_id); Serial.print(" : "); Serial.println(confirm_parity_echo);

    //Strange Case - unlikely happened
    if (confirm_parity_echo_wo_parity != expected_parity_echo) {
        Serial.print("??? ConfirmParity - STR ECHO: ");
        Serial.println(confirm_parity_echo);
        Serial.print("??? ConfirmParity - STR ECHO - 3: ");
        Serial.println(confirm_parity_echo_wo_parity);
        Serial.print("??? ConfirmParity - Expected Parity ECHO : ");
        Serial.println(expected_parity_echo);
        return false;
    }
    return true;
}

String RS485TEENSY::ReadBusSerialPacket() {
    Serial.println("ReadBusSerialPacket");
    String temp_process_packet = "";
    if (process_storage.empty()) {
        while(!ReadPacketsSlaveBUS("", false, false));
        if (process_storage.empty()) {
            Serial.println("ReadBusSerialPacket - Empty - Return """);
            return "";
        }
        else {
            temp_process_packet = process_storage.at(0);
            Serial.print("ReadBusSerialPacket - After ReadPacketsSlave Bus, Not Empty : "); Serial.println(temp_process_packet);
            process_storage.remove(0);
        }
    }
    else {
        temp_process_packet = process_storage.at(0);
        Serial.print("ReadBusSerialPacket - Not Empty : "); Serial.println(temp_process_packet);
        process_storage.remove(0);
    }

    Serial.println("Escape From ReadPacketsSlaveBUS");


    String temp_process_packet_wo_parity = temp_process_packet.substring(0, temp_process_packet.length() - 3);
    switch(temp_process_packet[3]) {
        case '0':
            for (int i = 0; i < packet_storage_mega0.size(); i++) {
                if (packet_storage_mega0.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega0 << endl;
                    packet_storage_mega0.remove(i);
                    break;
                }
            }
            break;
        case '1':
            for (int i = 0; i < packet_storage_mega1.size(); i++) {
                if (packet_storage_mega1.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega1 << endl;
                    packet_storage_mega1.remove(i);
                    break;
                }
            }
            break;
        case '2':
            for (int i = 0; i < packet_storage_mega2.size(); i++) {
                if (packet_storage_mega2.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega2 << endl;
                    packet_storage_mega2.remove(i);
                    break;
                }
            }
            break;
        case '3':
            for (int i = 0; i < packet_storage_mega3.size(); i++) {
                if (packet_storage_mega3.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega3 << endl;
                    packet_storage_mega3.remove(i);
                    break;
                }
            }
            break;
        case '4':
            for (int i = 0; i < packet_storage_mega4.size(); i++) {
                if (packet_storage_mega4.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega4 << endl;
                    packet_storage_mega4.remove(i);
                    break;
                }
            }
            break;
        case '5':
            for (int i = 0; i < packet_storage_mega5.size(); i++) {
                if (packet_storage_mega5.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega5 << endl;
                    packet_storage_mega5.remove(i);
                    break;
                }
            }
            break;
        case '6':
            for (int i = 0; i < packet_storage_mega6.size(); i++) {
                if (packet_storage_mega6.at(i) == temp_process_packet) {
                    Serial.print(" ReadBusSerialPacket - there is a same packet : ");
                    Serial.println(temp_process_packet);
                    Serial << packet_storage_mega6 << endl;
                    packet_storage_mega6.remove(i);
                    break;
                }
            }
            break;
    }
    return temp_process_packet_wo_parity;
}

bool RS485TEENSY::ReadPacketsSlaveBUS(String desired_packet = "", bool is_desired_packet_mode = false, bool ConfirmParityOrCheckSerial = false) {
    //Teensy 2, ID: '7'
    String bus_packet;
    int packet_count = 0;
    int first_packet_integer = 0;
    // * Resend is used only for ConfirmParity. CheckSerialPacket is not used/
    unsigned long start_time = millis();
    unsigned long interval_echo = 10000; // if there is no serial packet about parity echo, it resends the lastest packet again.
    bool is_return_true = false;
    String _latest_coming_packet = ""; //Protect duplicated conditions.
    char expected_sender_id = desired_packet[3];
    // Waiting until Serial Packets come in.
    // ConfirmParityOrCheckSerial try to get *0*6O
    while (Serial2.available() == 0){
        if (ConfirmParityOrCheckSerial) {
            if ((millis() - start_time) > interval_echo) {
                Serial.print("ReadPacketsBUS/Confirm Parity - Resend Again : ");
                Serial.println(_latest_sending_packet);
                Serial2.print(_latest_sending_packet);
                start_time = millis();
            }
        }
        else {
            if ((millis() - start_time) > interval_echo) {
                if (expected_sender_id == '0') {
                    if (!packet_storage_mega0.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '1') {
                    if (!packet_storage_mega1.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '2') {
                    if (!packet_storage_mega2.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '3') {
                    if (!packet_storage_mega3.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '4') {
                    if (!packet_storage_mega4.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '5') {
                    if (!packet_storage_mega5.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '6') {
                    if (!packet_storage_mega6.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
                else if (expected_sender_id == '7') {
                    if (!packet_storage_mega7.empty()) {
                        Serial.print(" Not Wait - Break for Next : "); Serial.println(expected_sender_id);
                        break;
                    }
                }
            }
        }
        if (Serial.available() > 0) {
            Serial.println("!!! USB Mode");
            return true;
        }
        if (Serial5.available() > 0) {
            Serial.println("!!! UART Mode");
            return true;
        }
    }

    while (Serial2.available() > 0){
        //IF - BREAK for Null, \n, \r -> they mean being 'empty'
        Serial.print("Teensy2 Stuck Here");
        delay(100);
        bus_packet = Serial2.readStringUntil('/');
        delay(100);
        Serial.print("ReadPacketsBUS - "); Serial.print(packet_count); Serial.print(" - "); Serial.println(bus_packet);
        packet_count++;

        first_packet_integer = bus_packet[0];
        char id_packet = bus_packet[1];
        char sender_packet = bus_packet[3];
        char parity_confirm_packet = bus_packet[4];
        delay(10);
        //1. bus_packet length is less than 5, it is
        if (bus_packet[0] != '*' || bus_packet[2] != '*' || bus_packet.length() < 8) {
            Serial.print("ReadPacketsBUS - Corrupted Data : "); Serial.println(bus_packet);
        }

        //2. bus_packet corruption case 1
        if (first_packet_integer == -1) {
            Serial.print("ReadPacketsBUS - Corrupted Data (-1) : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //3. bus_packet corruption case 2
        if (first_packet_integer <= 32) {
            Serial.print("ReadPacketsBUS - Null or Line Space, Carriage Return : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //4. / case
        if (first_packet_integer == 47) {
            Serial.print("ReadPacketsBUS - Delimiter '/' : "); Serial.println(bus_packet);
            bus_packet = "";
            break;
        }

        //5. not my packet
        if (id_packet != ch_Teensy_RS485_ID) {
            Serial.print("ReadPacketsBUS - This Serial Packet is not belong to this device : "); Serial.println(bus_packet);
            bus_packet = "";
            continue;
        }

        //6. Protect Duplicated Serial Packet Case
        if (_latest_coming_packet == bus_packet) {
            Serial.print("ReadPacketsBUS - Duplicated Case : "); Serial.print(_latest_coming_packet); Serial.print(" : "); Serial.println(bus_packet);
            bus_packet = "";
            delay(200);
            continue;
        }

        //7. Parity Processing - only for CheckSerialPacket, No <ConfirmParity -> ConfirmParityOrCheckSerial: true>
        bool is_parity_processing = true;
        if (bus_packet[4] == 's' || bus_packet[4] == 'e' || bus_packet[4] == ch_Teensy_RS485_ID || bus_packet[4] == 't') {
            is_parity_processing = false;
            if(!ParityProcessing(bus_packet)){
                Serial.print("ReadPacketsBUS - Parity Packet Error : "); Serial.println(bus_packet);
                bus_packet = "";
                continue;
            }
            if (bus_packet[4] != 'e') {
                Serial.print("ReadPacketsBUS - Save in process_storage : "); Serial.println(bus_packet);
                process_storage.push_back(bus_packet);
            }
            Serial.print("ReadPacketsBUS - Process Stroage : ");
            Serial << process_storage << endl;
        }

        //8. Protect falling into infinite loop due to communication corrupted error
        if (ConfirmParityOrCheckSerial && is_desired_packet_mode && is_parity_processing){
            if(!ConfirmParityProcessing(bus_packet)) {
                bus_packet = desired_packet;
                bus_packet = bus_packet + ParityGenerator(bus_packet);
                first_packet_integer = bus_packet[0];
                id_packet = bus_packet[1];
                sender_packet = bus_packet[3];
                parity_confirm_packet = bus_packet[4];
            }
        }

        delay(20);
        String bus_packet_wo_parity = bus_packet.substring(0, bus_packet.length() - 3);
        //8. Distribution to Parity and Packet Storages
        //  1) sender ID
        switch (sender_packet) {
            case '0':
                //  2)  packet_storage_mega or parity_storage_mega
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega0.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    //  3) N case: resend latest sending packet and wait 0.5S, Go To the beginning of while loop
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;//////TOMORROW CHECK IT WORKS??!!
                }
                    //  4) Check Serial Packet case
                else {
                    if (bus_packet_wo_parity != desired_packet) {
                        Serial.println("ReadPacketsBUS - Not Desired Packet"); Serial.println(bus_packet);
                        packet_storage_mega0.push_back(bus_packet);
                        Serial << packet_storage_mega0 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega0.size(); i++) {
                            if (bus_packet == packet_storage_mega0.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega0 << endl;
                                packet_storage_mega0.remove(i);
                                Serial << packet_storage_mega0 << endl;
                            }
                        }
                    }
                }
                break;
            case '1':
                //  2)  packet_storage_mega or parity_storage_mega
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega1.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    //  3) N case: resend latest sending packet and wait 0.5S, Go To the beginning of while loop
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;//////TOMORROW CHECK IT WORKS??!!
                }
                    //  4) Check Serial Packet case
                else {
                    if (bus_packet_wo_parity != desired_packet) {
                        Serial.println("ReadPacketsBUS - Not Desired Packet"); Serial.println(bus_packet);
                        packet_storage_mega1.push_back(bus_packet);
                        Serial << packet_storage_mega1 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega1.size(); i++) {
                            if (bus_packet == packet_storage_mega1.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega1 << endl;
                                packet_storage_mega1.remove(i);
                                Serial << packet_storage_mega1 << endl;
                            }
                        }
                    }
                }
                break;
            case '2':
                if (parity_confirm_packet == 'O') {
                    Serial.println("ReadPacketsBUS/Confirm Parity 2 - O Case");
                    parity_storage_mega2.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity 2 - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 2 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega2.push_back(bus_packet);
                        Serial << packet_storage_mega2 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega2.size(); i++) {
                            if (bus_packet == packet_storage_mega2.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega2 << endl;
                                packet_storage_mega2.remove(i);
                                Serial << packet_storage_mega2 << endl;
                            }
                        }
                    }
                }
                break;
            case '3':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega3.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 3 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega3.push_back(bus_packet);
                        Serial << packet_storage_mega3 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega3.size(); i++) {
                            if (bus_packet == packet_storage_mega3.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega3 << endl;
                                packet_storage_mega3.remove(i);
                                Serial << packet_storage_mega3 << endl;
                            }
                        }
                    }
                }
                break;
            case '4':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega4.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 4 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega4.push_back(bus_packet);
                        Serial << packet_storage_mega4 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega4.size(); i++) {
                            if (bus_packet == packet_storage_mega4.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega4 << endl;
                                packet_storage_mega4.remove(i);
                                Serial << packet_storage_mega4 << endl;
                            }
                        }
                    }
                }
                break;
            case '5':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega5.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 5 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega5.push_back(bus_packet);
                        Serial << packet_storage_mega5 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega5.size(); i++) {
                            if (bus_packet == packet_storage_mega5.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega5 << endl;
                                packet_storage_mega5.remove(i);
                                Serial << packet_storage_mega5 << endl;
                            }
                        }
                    }
                }
                break;
            case '6':
                if (parity_confirm_packet == 'O') {
                    parity_storage_mega6.push_back(bus_packet);
                }
                else if (parity_confirm_packet == 'N') {
                    Serial.print("ReadPacketsBUS/Confirm Parity - N case - Resend Again : "); Serial.println(bus_packet);
                    Serial2.print(_latest_sending_packet);
                    delay(500);
                    continue;
                }
                else {
                    Serial.println("ReadPacketsBUS/Confirm Parity 6 - Serial Case");
                    if (bus_packet_wo_parity != desired_packet) {
                        packet_storage_mega6.push_back(bus_packet);
                        Serial << packet_storage_mega6 << endl;
                    }
                    else {
                        for (int i = 0; i < packet_storage_mega6.size(); i++) {
                            if (bus_packet == packet_storage_mega6.at(i)) {
                                Serial.println("ReadPacketsBUS - Duplicated : ");
                                Serial << packet_storage_mega6 << endl;
                                packet_storage_mega6.remove(i);
                                Serial << packet_storage_mega6 << endl;
                            }
                        }
                    }
                }
                break;
        }

        //9. Print bus serial packets
        if ((parity_confirm_packet == 'O') || (parity_confirm_packet == 'N')) {
//            Serial.print("$$$ ReadPacketsBUS - Parity Packet : "); Serial.println(bus_packet);
        }
        else {
//            Serial.print("$$$ ReadPacketsBUS - Serial Packet : "); Serial.println(bus_packet);
        }
        //10. Compare desired packets, is_return_true
        if (is_desired_packet_mode) {
//            Serial.print("$$$ ReadPacketsBUS - is_desired_packet_mode : "); Serial.println(desired_packet);
            if(desired_packet == bus_packet_wo_parity) {
                Serial.print("ReadPacketsBUS - It is desired packet : "); Serial.println(desired_packet);
                is_return_true |= true;
                if (!ConfirmParityOrCheckSerial) {
                    check_serial_echo_packet = bus_packet_wo_parity;
                }
            }
            else {
                is_return_true |= false;
            }
        }
        else {
            is_return_true = true;
        }
        _latest_coming_packet = bus_packet;
    }

    //11. ConfirmParity Case: There is a case that check serial echo comes in earlier than start of check serial packet function
    //Need to check whether it is in packet_storage
    //It is activated, when is_return_true is false
    if (!ConfirmParityOrCheckSerial && !is_return_true) {
        String packet_without_parity = "";
        bool isExisting = false;
        switch(expected_sender_id) {
            case '0':
                if (packet_storage_mega0.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 0, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega0.size(); i++) {
                        packet_without_parity = packet_storage_mega0.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
//                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
//                            Serial.println(packet_without_parity);
//                            Serial << packet_storage_mega0 << endl;
                            packet_storage_mega0.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '1':
                if (packet_storage_mega1.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 1, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega1.size(); i++) {
                        packet_without_parity = packet_storage_mega1.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega1 << endl;
                            packet_storage_mega1.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '2':
                if (packet_storage_mega2.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 2, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega2.size(); i++) {
                        packet_without_parity = packet_storage_mega2.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega2 << endl;
                            packet_storage_mega2.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '3':
                if (packet_storage_mega3.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 3, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega3.size(); i++) {
                        packet_without_parity = packet_storage_mega3.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega3 << endl;
                            packet_storage_mega3.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }

                break;
            case '4':
                if (packet_storage_mega4.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 4, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega4.size(); i++) {
                        packet_without_parity = packet_storage_mega4.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega4 << endl;
                            packet_storage_mega4.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '5':
                if (packet_storage_mega5.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 5, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega5.size(); i++) {
                        packet_without_parity = packet_storage_mega5.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega5 << endl;
                            packet_storage_mega5.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
            case '6':
                if (packet_storage_mega6.empty()) {
//                    Serial.print(" ReadPacketsBUS - CheckSerialPacket - Empty, 6, Return False : ");
                    return false;
                }
                else {
                    for (int i = 0; i < packet_storage_mega6.size(); i++) {
                        packet_without_parity = packet_storage_mega6.at(i);
                        packet_without_parity = packet_without_parity.substring(0, packet_without_parity.length() - 3);
                        if (packet_without_parity == desired_packet) {
                            Serial.print(" ReadPacketsBUS - CheckSerialPacket - there is a same packet : ");
                            Serial.println(packet_without_parity);
                            Serial << packet_storage_mega6 << endl;
                            packet_storage_mega6.remove(i);
                            isExisting = true;
                            break;
                        }
                    }
                }
                break;
        }
        if (isExisting) {
            check_serial_echo_packet = packet_without_parity;
            is_return_true = true;
        }
    }

    //11. If it is not desired packet mode, it always return true. If it is, it returns is_return_true
    if (is_desired_packet_mode) {
        return is_return_true;
    }
    else {
        return true;
    }
}

