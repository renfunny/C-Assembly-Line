#include "LineManager.h"

namespace seneca
{
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream f(file);
        if (!f) 
        {
            throw std::string("Err: Unable to open the file!");
        }

        Utilities util;
        std::string line;
        std::set<std::string> currentStations;
        std::set<std::string> nextStations;

        while (std::getline(f, line)) 
        {
            if (!line.empty()) 
            {
                size_t next_pos = 0;
                bool more = true;

                std::string item = util.extractToken(line, next_pos, more);
                std::string nextItem = more ? util.extractToken(line, next_pos, more) : "";

                currentStations.insert(item);
                if (!nextItem.empty()) 
                {
                    nextStations.insert(nextItem);
                }

                auto itCurrent = std::find_if(stations.begin(), stations.end(),
                    [&item](const Workstation* st) 
                    {
                        return st->getItemName() == item;
                    });

                if (itCurrent != stations.end()) 
                {
                    Workstation* st1 = *itCurrent;

                    if (!nextItem.empty()) 
                    {
                        auto itNext = std::find_if(stations.begin(), stations.end(),
                            [&nextItem](const Workstation* st) 
                            {
                                return st->getItemName() == nextItem;
                            });
                        if (itNext != stations.end()) 
                        {
                            st1->setNextStation(*itNext);
                        }
                    }
                    else 
                    {
                        st1->setNextStation(nullptr);
                    }

                    m_activeLine.push_back(st1);
                }
            }
        }
        f.close();

        std::set<std::string> difference;
        std::set_difference(currentStations.begin(), currentStations.end(),
            nextStations.begin(), nextStations.end(),
            std::inserter(difference, difference.begin()));

        if (!difference.empty()) 
        {
            auto itFirstStation = std::find_if(stations.begin(), stations.end(),
                [&difference](const Workstation* st) 
                {
                    return st->getItemName() == *difference.begin();
                });
            if (itFirstStation != stations.end()) 
            {
                m_firstStation = *itFirstStation;
            }
        }

        m_cntCustomerOrder = g_pending.size();
    }

	void LineManager::reorderStations() 
    {
		Workstation* it = m_firstStation;
		std::vector<Workstation*> orderedStations;
		while (it) 
        {
			orderedStations.push_back(it);
			it = it->getNextStation();
		}
		m_activeLine = orderedStations;
	}

    bool LineManager::run(std::ostream& os) 
    {
		static size_t count = 0;
		os << "Line Manager Iteration: " << ++count << std::endl;
		if (!g_pending.empty()) 
        {
			*m_firstStation += std::move(g_pending.front());
			g_pending.pop_front();
		}
		for (auto station : m_activeLine)
			station->fill(os);

		for (auto station : m_activeLine)
			station->attemptToMoveOrder();

		return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;
    }

	void LineManager::display(std::ostream& os) const
	{
		for (auto station : m_activeLine)
		{
			station->display(os);
		}
	}
}