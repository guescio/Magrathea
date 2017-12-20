#ifndef GLUEHANDLER_H
#define GLUEHANDLER_H

#include "windows.h"
#include <iostream>

class GlueHandler
{
public:
    GlueHandler();
    ~GlueHandler();

    void GlueDot();

protected:

    void OpenAndConfigurePort();

    void ConfigureGlueMachine();
    void SendCommand(std::string command);
    void HandshakeProtocol();
    std::string ArtisinalHandFormattedPacket(std::string command);
    std::string CheckSum(int len, std::string command);
    void ReadResult();

    void InterpretError(LPTSTR function);

    HANDLE m_comm;

    DWORD m_baudRate;
    BYTE m_byteSize;
    BYTE m_stopBits;
    BYTE m_parity;

    double m_pressure;
    double m_time;
    double m_vaccuum;

};

#endif // GLUEHANDLER_H
