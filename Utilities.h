#ifndef SENECA_UTILITIES_H
#define SENECA_UTILITIES_H
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <deque>
#include <vector>
#include <fstream>
#include <set>
#include <string>

namespace seneca
{
	class Utilities
	{
		//Instance Variable
		size_t m_widthField = 1;

		//Class Variable
		static char m_delimiter;

	public:
		//Member Functions
		void setFieldWidth(size_t newWidth);
		size_t getFieldWidth() const;
		std::string extractToken(const std::string& str, size_t& next_pos, bool& more);

		//Class Functions
		static void setDelimiter(char newDelimiter);
		static char getDelimiter();
	};
}

#endif // !SENECA_UTILITIES_H