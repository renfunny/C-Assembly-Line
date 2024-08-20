#include "CustomerOrder.h"

namespace seneca
{
	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder()
		: m_name{ "" }
		, m_product{ "" }
		, m_cntItem{ 0 }
		, m_lstItem{ nullptr }
	{
	}

	CustomerOrder::CustomerOrder(const std::string& str)
	{
		Utilities util;
		size_t next_pos = 0;
		bool more = true;

		m_name = util.extractToken(str, next_pos, more);
		m_product = util.extractToken(str, next_pos, more);
		m_cntItem = std::count(str.begin(), str.end(), util.getDelimiter()) - 1;
		m_lstItem = new Item * [m_cntItem];

		for (size_t i = 0; i < m_cntItem; i++)
		{
			m_lstItem[i] = new Item(util.extractToken(str, next_pos, more));
		}

		if (util.getFieldWidth() > m_widthField)
		{
			m_widthField = util.getFieldWidth();
		}
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& src)
	{
		throw std::invalid_argument("No copies allowed");
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept
		: m_name{ src.m_name }
		, m_product{ src.m_product }
		, m_cntItem{ src.m_cntItem }
		, m_lstItem{ src.m_lstItem }
	{
		src.m_name = "";
		src.m_product = "";
		src.m_cntItem = 0;
		src.m_lstItem = nullptr;
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept
	{
		if (this != &src)
		{
			for (size_t i = 0; i < m_cntItem; ++i)
			{
				delete m_lstItem[i];
			}
			delete[] m_lstItem;

			m_name = std::move(src.m_name);
			m_product = std::move(src.m_product);
			m_cntItem = src.m_cntItem;
			m_lstItem = src.m_lstItem;

			src.m_cntItem = 0;
			src.m_lstItem = nullptr;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder()
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			delete m_lstItem[i];
		}

		delete[] m_lstItem;
	}

	bool CustomerOrder::isOrderFilled() const
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (!m_lstItem[i]->m_isFilled)
			{
				return false;
			}
		}

		return true;
	}

	bool CustomerOrder::isItemFilled(const std::string& itemName) const
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled)
			{
				return false;
			}
		}

		return true;
	}

	void CustomerOrder::fillItem(Station& station, std::ostream& os)
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == station.getItemName())
			{
				if (!m_lstItem[i]->m_isFilled)
				{
					if (station.getQuantity() > 0)
					{
						m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
						station.updateQuantity();
						m_lstItem[i]->m_isFilled = true;
						os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
						return;
					}
					else
					{
						os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
					}
				}
			}
		}
	}

	void CustomerOrder::display(std::ostream& os) const
	{
		os << m_name << " - " << m_product << std::endl;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			os << "[" << std::right << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber << "] ";
			os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_lstItem[i]->m_itemName;
			os << " - " << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
		}
	}
}