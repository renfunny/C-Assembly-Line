#include "Workstation.h"

namespace seneca
{
	std::deque<CustomerOrder> g_pending{};
	std::deque<CustomerOrder> g_completed{};
	std::deque<CustomerOrder> g_incomplete{};

	void Workstation::fill(std::ostream& os)
	{
		if (!m_orders.empty())
		{
			m_orders.front().fillItem(*this, os);
		}
	
	}

	bool Workstation::attemptToMoveOrder()
	{
		if (!m_orders.empty()) 
		{
			bool isFilled = m_orders.front().isItemFilled(getItemName());

			bool noInventory = getQuantity() <= 0;

			if (isFilled || noInventory) 
			{
				if (m_pNextStation) 
				{
					*m_pNextStation += std::move(m_orders.front());
				}
				else 
				{
					if (m_orders.front().isOrderFilled()) 
					{
						g_completed.push_back(std::move(m_orders.front()));
					}
					else 
					{
						g_incomplete.push_back(std::move(m_orders.front()));
					}
				}
				m_orders.pop_front();
				return true;
			}
		}
		return false;
	}

	void Workstation::setNextStation(Workstation* station)
	{
		m_pNextStation = station;
	}

	Workstation* Workstation::getNextStation() const
	{
		return m_pNextStation;
	}

	void Workstation::display(std::ostream& os) const
	{
		os << this->getItemName() << " --> ";
		if (this->getNextStation() == nullptr) {
			os << "End of Line\n";
		}
		else {
			os << this->getNextStation()->getItemName() << '\n';
		}
	}

	Workstation& Workstation::operator+=(CustomerOrder&& order)
	{
		m_orders.push_back(std::move(order));
		return *this;
	}
}