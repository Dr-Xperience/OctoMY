#ifndef DISCOVERYPARTICIPANT_HPP
#define DISCOVERYPARTICIPANT_HPP

#include "DiscoveryRole.hpp"
#include <QString>
#include <QVariantMap>


struct DiscoveryParticipant{

		QString publicKey;
		QString ID;
		QStringList pins;
		QStringList trusts;

		QString publicAddress;
		quint16 publicPort;
		QString localAddress;
		quint16 localPort;

		DiscoveryRole role;
		DiscoveryType type;

		quint64 lastTime;


		DiscoveryParticipant();

		DiscoveryParticipant(QString publicKey, QString publicAddress, quint16 publicPort, QString localAddress, quint16 localPort, DiscoveryRole role, DiscoveryType type);
		DiscoveryParticipant(QVariantMap map);

		DiscoveryParticipant(const DiscoveryParticipant &o);

		void clearTrust();
		void clearPins();

		void addTrust(QString pin);
		void addPin(QString pin);

		void generateID();

		bool operator==(const DiscoveryParticipant &o) const;

		bool isSet();

		bool isValidServer();
		bool isValidClient();

		QVariantMap toVariantMap();

};


#endif // DISCOVERYPARTICIPANT_HPP