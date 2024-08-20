#include "Utilities.h"

namespace seneca
{
    char Utilities::m_delimiter = ',';

	void Utilities::setFieldWidth(size_t newWidth)
	{
		m_widthField = newWidth;
	}

	size_t Utilities::getFieldWidth() const
	{
		return m_widthField;
	}

	std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more)
	{
        // Check if next_pos is out of bounds
        if (next_pos >= str.length()) 
        {
            more = false;
            return "";
        }

        // Find the next delimiter
        size_t end_pos = str.find(m_delimiter, next_pos);

        // If the delimiter is found at next_pos, throw an exception
        if (end_pos == next_pos) 
        {
            more = false;
            throw std::invalid_argument("Delimiter found at next_pos");
        }

        std::string token;
        if (end_pos != std::string::npos) 
        {
            token = str.substr(next_pos, end_pos - next_pos);
            next_pos = end_pos + 1;
            more = true;
        }
        else 
        {
            token = str.substr(next_pos);
            next_pos = str.length();
            more = false;
        }

        // Trim leading and trailing spaces
        token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
        token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);

        if (token.empty()) 
        {
            throw std::invalid_argument("No token found");
        }

        // Update m_widthField
        m_widthField = std::max(m_widthField, token.length());

        return token;
	}

    void Utilities::setDelimiter(const char newDelimiter)
    {
		m_delimiter = newDelimiter;
	}

    char Utilities::getDelimiter()
    {
		return m_delimiter;
	}
}