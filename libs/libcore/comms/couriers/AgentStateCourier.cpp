#include "AgentStateCourier.hpp"

#include "comms/SyncParameter.hpp"

#include "comms/couriers/CourierMandate.hpp"

#include <QDebug>

#include <QDataStream>


#include <QDateTime>


const quint32 AgentStateCourier::AGENT_STATE_COURIER_ID=(Courier::FIRST_USER_ID + 5);

// Agent side constructor
AgentStateCourier::AgentStateCourier(QDataStream *initialization, QObject *parent)
	: Courier("AgentState", AGENT_STATE_COURIER_ID, parent)
	, mAgentSide(nullptr!=initialization)
	, mParams(100)
	, mLastSendTime(0)
	, mLastChangeTime(0)
	, mFlags(nullptr)
	, mMode(nullptr)
	, mTargetPosition(nullptr)
	, mTargetOrientation(nullptr)
	, mTargetPose(nullptr)
{
	initParams(initialization);
	mParams.flush(QDateTime::currentMSecsSinceEpoch());
}



void AgentStateCourier::setFlags(quint8 flags)
{
	mFlags->setLocalValue(flags);
}



void AgentStateCourier::setMode(AgentMode mode)
{
	mMode->setLocalValue(mode);
}



QString AgentStateCourier::toString() const
{
	return "AgentStateCourier{ mandate="+mandate().toString()+", params="+mParams.toString()+" }";
}

QDebug &AgentStateCourier::toDebug(QDebug &d) const
{
	d << "AgentStateCourier{ mandate="+mandate().toString()+", params=";
	d << mParams;
	d << "}";
	return d;
}

void AgentStateCourier::initParams(QDataStream *initialization)
{
	qDebug()<<"REGISTERING PARAMETERS with "<<(nullptr!=initialization?"DATA":"NULL");
	quint8 flags=0;
	mFlags=mParams.registerParameter(flags);
	if(nullptr==mFlags) {
		qWarning()<<"ERROR: could nota allocate flags parameter";
	} else {
		qDebug()<<"REGISTERED PARAMETER #1: "<<mParams;
	}

	AgentMode mode=OFFLINE;
	mMode=mParams.registerParameter(mode);
	if(nullptr==mMode) {
		qWarning()<<"ERROR: could nota allocate agent mode parameter";
	} else {
		qDebug()<<"REGISTERED PARAMETER #2: "<<mParams;
	}

	QGeoCoordinate targetPosition;
	mTargetPosition=mParams.registerParameter(targetPosition);
	if(nullptr==mTargetPosition) {
		qWarning()<<"ERROR: could nota allocate target position parameter";
	}
	else {
		qDebug()<<"REGISTERED PARAMETER #3: "<<mParams;
	}
	qreal targetOrientation=0;//Asume North at startup
	mTargetOrientation=mParams.registerParameter(targetOrientation);
	if(nullptr==mTargetOrientation) {
		qWarning()<<"ERROR: could nota allocate target orientation parameter";
	} else {
		qDebug()<<"REGISTERED PARAMETER #4: "<<mParams;
	}
	Pose targetPose;
	mTargetPose=mParams.registerParameter(targetPose);
	if(nullptr==mTargetPose) {
		qWarning()<<"ERROR: could nota allocate target pose parameter";
	} else {
		qDebug()<<"REGISTERED PARAMETER #5: "<<mParams;
	}
//Read initial data into parameters

//TODO: Fix and re-enable this. TIP: logging indicates the problem, ack and sync bit arrays are destroyed after load
	if(nullptr!=initialization) {
		/*
		qDebug()<<"BEFORE Reading in initial values for agent side parameters:"<<mParams;
		QDataStream &ds=*initialization;
		ds >> mParams;
		qDebug()<<"AFTER Reading in initial values for agent side parameters:"<<mParams;
		*/
	}

}

const QDebug &operator<<(QDebug &d, const AgentStateCourier &a)
{
	return a.toDebug(d);
}

////////////////////////////////////////////////////////////////////////////////
// Courier Interface ///////////////////////////////////////////////////////////

// Update courier state when channel has opportunity
void AgentStateCourier::update()
{
	mParams.update(QDateTime::currentMSecsSinceEpoch());
}


// Let the CommChannel know what we want
CourierMandate AgentStateCourier::mandate() const
{
	CourierMandate mandate(mParams.bytesSentIdeal() , 10, 100, true, mParams.hasPendingSyncs());
	return mandate;
}

// Act on sending opportunity.
// Return nubmer of bytes sent ( >0 ) if you took advantage of the opportunity
quint16 AgentStateCourier::sendingOpportunity(QDataStream &ds)
{
	if(mandate().sendActive) {
		ds << mParams;
		quint16 bytes=mParams.bytesSent();
		qDebug()<<"Spent sending opportunity for agent status data with "<<bytes<<" bytes";
		return  bytes;
	} else {
		qWarning()<<"ERROR: sendingOpportunity while sendActive=false";
	}
	return 0;
}


quint16 AgentStateCourier::dataReceived(QDataStream &ds, quint16 availableBytes)
{
	if(mandate().receiveActive) {
		ds >> mParams;
		quint16 bytes=mParams.bytesRead();
		qDebug()<<"Receiving "<<bytes<<" of "<<availableBytes<<" bytes for agent status data";
		return  bytes;
	} else {
		qWarning()<<"ERROR: dataReceived while receiveActive=false";
	}
	return 0;
}
