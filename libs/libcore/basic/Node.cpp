#include "Node.hpp"

#include "comms/CommsChannel.hpp"
#include "comms/Client.hpp"

#include "comms/discovery/DiscoveryClient.hpp"

#include "zoo/ZooClient.hpp"
#include "camera/CameraList.hpp"
#include "sensory/SensorInput.hpp"
#include "comms/messages/SensorsMessage.hpp"

#include <QCommandLineParser>
#include <QAccelerometerReading>
#include <QGyroscopeReading>
#include <QGeoPositionInfo>

Node::Node(QCommandLineParser &opts, QString base, QObject *parent)
	: QObject(parent)
	, opts(opts)
	, keystore (nullptr,base,false)
	, discovery (new DiscoveryClient)
	, comms (new CommsChannel(this))
	, zoo (new ZooClient(this))
	, sensors(new SensorInput(this))
	, hubPort(0)
	, cameras(new CameraList(this))
	, lastStatusSend(0)
	, sensorMessage(new SensorsMessage)

{
	setObjectName(base);
	hookSensorSignals(*this);
	if(nullptr!=comms){
		comms->hookSignals(*this);
	}
	//QByteArray OCID=UniquePlatformFingerprint::getInstance().platform().getHEX().toUtf8();
	if(nullptr!=zoo){
		zoo->setURL(QUrl("http://localhost:8123/api"));
		//zoo->setURL(QUrl("http://localhost/lennart/octomy/index.php"));
		//zoo->putNode(OCID); 		zoo->getNode(OCID);
	}

}


QCommandLineParser &Node::getOptions(){
	return opts;
}

Settings &Node::getSettings(){
	return settings;
}

KeyStore  &Node::getKeyStore(){
	return keystore;
}

DiscoveryClient *Node::getDiscoveryClient(){
	return discovery;
}

CommsChannel *Node::getComms(){
	return comms;
}

ZooClient *Node::getZooClient(){
	return zoo;
}

SensorInput *Node::getSensorInput(){
	return sensors;
}

CameraList *Node::getCameras(){
	return cameras;
}

QWidget *Node::showWindow(){
	return nullptr;
}

void Node::hookSensorSignals(QObject &o){
	if(nullptr!=sensors){
		sensors->hookSignals(o);
	}
	if(nullptr!=comms){
		comms->hookSignals(o);
	}
}


void Node::unHookSensorSignals(QObject &o){
	if(nullptr!=sensors){
		sensors->unHookSignals(o);
	}
	if(nullptr!=comms){
		comms->unHookSignals(o);
	}
}




void Node::sendStatus(){
	const qint64 now=QDateTime::currentMSecsSinceEpoch();
	const qint64 interval=now-lastStatusSend;
	if(interval>100){
		QByteArray datagram;
		QDataStream ds(&datagram,QIODevice::WriteOnly);
		ds<<sensorMessage;
		//TODO:Convert to use courier instead
		//		comms->sendPackage(datagram,QHostAddress(hubAddress),hubPort);
		lastStatusSend=now;
	}
}


void Node::onReceivePacket(QSharedPointer<QDataStream> ds, QHostAddress, quint16){
	qint32 magic=0;
	*ds>>magic;
	qDebug()<<"GOT PACKET WITH MAGIC: "<<magic;
}

void Node::onError(QString e){
	qDebug()<<"Comms error: "<<e;
}

void Node::onClientAdded(Client *c){
	qDebug()<<"Client added: "<<c->getHash() <<QString::number(c->getHash(),16);
}

void Node::onConnectionStatusChanged(bool s){
	qDebug() <<"New connection status: "<<s;
	if(s){

	}
	else{

	}
}


void Node::onPositionUpdated(const QGeoPositionInfo &info){
	if(nullptr!=sensorMessage){
		sensorMessage->gps=info.coordinate();
		sendStatus();
	}
}


void Node::onCompassUpdated(QCompassReading *r){
	if(nullptr!=r && nullptr!=sensorMessage){
		sensorMessage->compassAzimuth=r->azimuth();
		sensorMessage->compassAccuracy=r->calibrationLevel();
		sendStatus();
	}
}

void Node::onAccelerometerUpdated(QAccelerometerReading *r){
	if(nullptr!=r && nullptr!=sensorMessage){
		sensorMessage->accellerometer=QVector3D(r->x(), r->y(), r->z());
		sendStatus();
	}
}

void Node::onGyroscopeUpdated(QGyroscopeReading *r){
	if(nullptr!=r && nullptr!=sensorMessage){
		sensorMessage->gyroscope=QVector3D(r->x(), r->y(), r->z());
		sendStatus();
	}
}
