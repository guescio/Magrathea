#include "MessageHandler.h"


MessageHandler::MessageHandler(msgLevel level)
{
	m_level = level;
}


MessageHandler::~MessageHandler()
{

}

void MessageHandler::SendToSTDOUT(std::string message) {

	std::cout << message.c_str() << std::endl;

}