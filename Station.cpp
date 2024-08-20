#include "Station.h"
#include "Utilities.h"

namespace seneca
{
	size_t Station::m_widthField = 0;
	int Station::id_generator = 0;

	Station::Station(const std::string& str)
	{
		Utilities util;
		size_t next_pos = 0;
		bool more = true;

		m_id = ++id_generator;

		m_name = util.extractToken(str, next_pos, more);
		m_serialNo = std::stoi(util.extractToken(str, next_pos, more));
		m_quantity = std::stoi(util.extractToken(str, next_pos, more));

		m_widthField = std::max(m_widthField, util.getFieldWidth());
		m_desc = util.extractToken(str, next_pos, more);
	}

	const std::string& Station::getItemName() const
	{
		return m_name;
	}

	size_t Station::getNextSerialNumber()
	{
		return m_serialNo++;
	}

	size_t Station::getQuantity() const
	{
		return m_quantity;
	}

	void Station::updateQuantity()
	{
		if (m_quantity > 0)
		{
			m_quantity--;
		}
	}

	void Station::display(std::ostream& os, bool full) const
	{
		os << std::right << std::setw(3) << std::setfill('0') << m_id << " | ";
		os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_name << " | ";
		os << std::right << std::setw(6) << std::setfill('0') << m_serialNo << " | ";

		if (full)
		{
			os << std::setw(4) << std::right << std::setfill(' ') << m_quantity << " | " << m_desc;
		}

		os << std::endl;
	}
}