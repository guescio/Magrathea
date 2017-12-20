#include "gluehandler.h"
#include <QtMessageHandler>
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <tchar.h>
#include <strsafe.h>

GlueHandler::GlueHandler()
{

    // Change settings here if desired
    // Could move to config file later.

    // Port settings
    m_baudRate = CBR_38400;
    m_byteSize = 8;
    m_stopBits = ONESTOPBIT;
    m_parity = NOPARITY;

    // Glue machine settings
    m_pressure = 35.0; // unit: psi
    m_time = 1.2340; // unit: seconds
    m_vaccuum = 2.00; //unit: kPa


    OpenAndConfigurePort();
    ConfigureGlueMachine();


}

GlueHandler::~GlueHandler() {

    CloseHandle(m_comm);

}

void GlueHandler::OpenAndConfigurePort() {

  // Open port
  m_comm = CreateFileA("COM1",
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      0,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      0);
  if(m_comm==INVALID_HANDLE_VALUE) {

      if(GetLastError()==ERROR_FILE_NOT_FOUND) {
          qFatal("That's not a real port!");
      } else {
          qInfo("Some error occurred!");
          InterpretError(LPTSTR("CreateFileA"));
      }

  }

  // Configure baud etc.
  DCB dcbSerialParams = {0};
  if (!GetCommState(m_comm, &dcbSerialParams)) {
      qFatal("Could not get state of port!");
  }

  dcbSerialParams.BaudRate = m_baudRate;
  dcbSerialParams.ByteSize = m_byteSize;
  dcbSerialParams.StopBits = m_stopBits;
  dcbSerialParams.Parity = m_parity;

  if(!SetCommState(m_comm, &dcbSerialParams)) {
      qFatal("Could not configure port!");
  }

  // Set timeouts
  COMMTIMEOUTS timeouts = {0};

  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;

  if(!SetCommTimeouts(m_comm,&timeouts)) {
      qFatal("Cannot set timeout information!");
  }

  qInfo("Glue machine successfully configured");

}

void GlueHandler::ConfigureGlueMachine() {

    // Set timed mode
    std::string command1 = "TT  ";
    SendCommand(command1);

    // Set each setting
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(4) << int(10.0*m_pressure);
    std::string command2 = "PS  " + stream.str();
    SendCommand(command2);
    stream.clear();
    stream.str(std::string());

    stream << std::setfill('0') << std::setw(5) << int(1e4*m_time);
    std::string command3 = "DS  T" + stream.str();
    SendCommand(command3);
    stream.clear();
    stream.str(std::string());

    stream << std::setfill('0') << std::setw(4) << int(100.0*m_vaccuum);
    std::string command4 = "VS  " + stream.str();
    SendCommand(command4);
    stream.clear();
    stream.str(std::string());


}

void GlueHandler::SendCommand(std::string command) {

    // Start with checking port is open
    HandshakeProtocol();

    std::cout  << "Sending command: " << command.c_str() << std::endl;

    std::string packet = ArtisinalHandFormattedPacket(command);
    unsigned char data[99] = {0};
    memcpy(data,packet.c_str(),packet.length());

    // Can't use size of data for this.  Data is extra
    // long to hold variable width packet.
    // total length should be:
    // length of string which is packet,
    // since each char is 1 byte
    //DWORD dwBytesToWrite = (DWORD) sizeof(data);
    DWORD dwBytesToWrite = packet.length();
    DWORD dwBytesWritten = 0;
    BOOL errorFlag = FALSE;

    errorFlag = WriteFile(m_comm,
                          data,
                          dwBytesToWrite,
                          &dwBytesWritten,
                          NULL);

    if (errorFlag==FALSE) {
        qFatal("Did not send command successfully!!");
    } else if (dwBytesWritten != dwBytesToWrite) {
        qFatal("Did not write correct number of bytes!");
    }

    // Read command result.
    ReadResult();

    // Send end of transmission symbol.
    unsigned char EOT = (unsigned char)0x04;
    std::fill_n(data,99,0);
    data[1] = {EOT};
    errorFlag = WriteFile(m_comm,
                               data,
                               1,
                               &dwBytesWritten,
                               NULL);
    if (errorFlag==FALSE) {
        qFatal("Did not send terminate command successfully!!");
    } else if (dwBytesWritten != 1) {
        qFatal("Did not write correct number of bytes!");
    }

    return;
}

void GlueHandler::HandshakeProtocol() {

    unsigned char ENQ = (unsigned char) 0x05;
    unsigned char ACK = (unsigned char) 0x06;
    unsigned char NAK = (unsigned char) 0x15;

    unsigned char data [1] = {ENQ};
    DWORD dwBytesWritten = 0;

    // send pre-amble
    BOOL errorFlag = WriteFile(m_comm,
                               data,
                               1,
                               &dwBytesWritten,
                               NULL);

    if (errorFlag==FALSE) qFatal("Failed to send ENQ");

    // Collect acknowledgement
    char received[2] = {0};
    DWORD dwBytesRead = 0;
    int counter = 0;
    while ((!(received[0] == ACK)) && (counter < 200)) {
        counter ++;
        errorFlag = ReadFile(m_comm,
                              received,
                              1,
                              &dwBytesRead,
                              NULL);
    }

    if (!errorFlag) qFatal("Failed to receive any response!");
    else {
        if (received[0]==ACK) {
            // Everything worked
        } else if (received[0]==NAK) {
            qInfo("Negative handshake acknowledgement!");
        } else {
            qInfo() << "Unidentified response!" << received ;
        }
    }


}

std::string GlueHandler::ArtisinalHandFormattedPacket(std::string command) {

    unsigned char STX = (unsigned char)0x02;
    unsigned char ETX = (unsigned char)0x03;

    // Use the stringstream to make our packet
    std::stringstream format;

    // First: packet begin symbol
    format << STX;

    // Second: 2 characters
    // encoding command length:
    // should be as a string, but first
    // zero if < 10 characters
    if (command.length() > 99 )
        qFatal("You cannot send a command longer than 99 chars!");
    int len = command.length();
    format << std::setfill('0') << std::setw(2) << len ;

    // Third: the command itself
    format << command;

    // Fourth: the CheckSum.
    // Should also be sent as two
    // digits in string, but this
    // time in hexadecimal.
    std::string checkSum = CheckSum(len,command);
    format << checkSum.c_str();

    // Finally, end of packet.
    format << ETX ;

    return format.str();

}

std::string GlueHandler::CheckSum(int len, std::string command) {

    int sum = 0;
    int d1 = int(double(len)/10.0);
    int d2 = len % 10;
    char d1c = '0'+d1;
    char d2c = '0'+d2;
    sum = sum - int(d1c) - int(d2c);
    for (char& c: command) {
        sum = sum - int((unsigned char)c);
    }
    sum = sum % 256;
    sum = sum + 256;

    std::ostringstream trimChecksum;
    trimChecksum << std::hex << std::setfill('0') << std::uppercase << std::setw(2) << sum;

    return trimChecksum.str();

}

void GlueHandler::ReadResult() {

    unsigned char ETX = (unsigned char)0x03;

    // Read number of bytes
    char received[100] = {0};
    DWORD dwBytesRead = 0;
    BOOL errorFlag = FALSE;
    // Get STX
    while (dwBytesRead < 1) {
        errorFlag = ReadFile(m_comm,
                                  received,
                                  1,
                                  &dwBytesRead,
                                  NULL);
    }

    // Get 6 bytes:
    // first two representing number of bytes to read
    // second two representing result
    // If this is working, 3rd & 4th should be A2 or A0.
    // Finally, checksum
    std::fill_n(received,100,0);
    errorFlag = ReadFile(m_comm,
                         received,
                         6,
                         &dwBytesRead,
                         NULL);

    // Get ETX and make sure it is
    std::fill_n(received,100,0);
    errorFlag = ReadFile(m_comm,
                         received,
                         1,
                         &dwBytesRead,
                         NULL);
    if (!errorFlag) qFatal("Failed to receive any response!");
    else if (!(received[0]==ETX)) {
        qInfo() << "Did not find ETX! " << received ;
    }

}

void GlueHandler::InterpretError(LPTSTR function) {

  LPVOID msgBuffer;
  LPVOID displayBuffer;
  DWORD dw = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                (LPTSTR) &msgBuffer,
                0,
                NULL);

  displayBuffer =
      (LPVOID)LocalAlloc( LMEM_ZEROINIT,
                          ( lstrlen((LPCTSTR)msgBuffer)
                            + lstrlen((LPCTSTR)function)
                            + 40) // account for format string
                          * sizeof(TCHAR) );

  if (FAILED( StringCchPrintf((LPTSTR)displayBuffer,
                   LocalSize(displayBuffer) / sizeof(TCHAR),
                   TEXT("%s failed with error code %d as follows:\n%s"),
                   function,
                   dw,
                   msgBuffer)))
  {
      printf("FATAL ERROR: Unable to output error code.\n");
  }

  _tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)displayBuffer);

  LocalFree(msgBuffer);
  LocalFree(displayBuffer);


  return;
}

void GlueHandler::GlueDot() {

    SendCommand("DI  ");
    return;
}
