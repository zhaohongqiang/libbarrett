/*
 * puck_group-inl.h
 *
 *  Created on: Nov 1, 2010
 *      Author: dc
 */

#include <boost/thread/locks.hpp>


namespace barrett {


inline void PuckGroup::getProperty(enum Puck::Property prop, int* values, bool realtime) const
{
	getProperty<Puck::StandardParser>(prop, values, realtime);
}
template<typename Parser> void PuckGroup::getProperty(enum Puck::Property prop, typename Parser::result_type* values, bool realtime) const
{
	boost::unique_lock<thread::Mutex> ul(bus.getMutex(), boost::defer_lock);
	if (realtime) {
		ul.lock();
	}

	int ret;
	int propId = getPropertyId(prop);

	ret = Puck::sendGetPropertyRequest(bus, id, propId);
	if (ret != 0) {
		syslog(LOG_ERR, "%s: Puck::sendGetPropertyRequest() returned error %d.", __func__, ret);
		throw std::runtime_error("PuckGroup::getProperty(): Failed to send request. Check /var/log/syslog for details.");
	}

	for (size_t i = 0; i < numPucks(); ++i) {
		values[i] = Puck::receiveGetPropertyReply<Parser>(bus, pucks[i]->getId(), propId, true, realtime, &ret);
		if (ret != 0) {
			syslog(LOG_ERR, "%s: Puck::receiveGetPropertyReply() returned error %d while receiving message %d of %d for ID=%d.",
					__func__, ret, i+1, numPucks(), pucks[i]->getId());
			throw std::runtime_error("PuckGroup::getProperty(): Failed to receive reply. Check /var/log/syslog for details.");
		}
	}
}

inline void PuckGroup::setProperty(enum Puck::Property prop, int value) const
{
	Puck::setProperty(bus, id, getPropertyId(prop), value);
}


}
