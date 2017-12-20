#pragma once

#include <iostream>
#include <cstdarg>
#include <typeinfo>
#include <string>
#include <sstream>

enum msgLevel { DEBUG, INFO, ERROR };

class MessageHandler
{
public:


	MessageHandler(msgLevel level = INFO);

	~MessageHandler();

	////////////////////////////////
	// Sinks for pointing output to different
	// locations
	void SendToSTDOUT(std::string message);

	////////////////////////////////
	// Template classes for handling different
	// degrees of output

	template<typename... Args>
	inline void Info(Args... args)
	{
		if (m_level > 1) return;
		std::string myString = Stringify("INFO:",args...);
		SendToSTDOUT(myString);
	}
	
	template<typename... Args>
	inline void Error(Args... args)
	{
		if (m_level > 2) return;
		std::string myString = Stringify("ERROR:", args...);
		SendToSTDOUT(myString);

	}

	template<typename... Args>
	inline void Fatal(Args... args)
	{
		if (m_level > 3) return;
		std::string myString = Stringify("FATAL:",args...);
		SendToSTDOUT(myString);
	}
	
	template<typename... Args>
	inline void Debug(Args... args)
	{
		if (m_level > 0) return;
		std::string myString = Stringify("DEBUG:", args...);
		SendToSTDOUT(myString);
	}

	void SetLevel(msgLevel level)
	{
		m_level = level; 
	}
	
	template<typename... Args>
	std::string Stringify(Args... args) {
		std::stringstream ss;
		std::initializer_list<int> unused{ (ss << " " << args, 0)... };

		return ss.str();

	}

private :

	msgLevel m_level;

};

