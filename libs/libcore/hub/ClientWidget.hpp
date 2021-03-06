#ifndef CLIENTWINDOW_HPP
#define CLIENTWINDOW_HPP

#include <QWidget>
#include <QTimer>

#include "comms/CommsSession.hpp"

#include "widgets/TryToggle.hpp"
#include "basic/Node.hpp"


#include <QSharedPointer>

class WaitingSpinnerWidget;
class SensorsCourier;
class AgentStateCourier;
class BlobCourier;
class ISyncParameter;

namespace Ui
{
class ClientWidget;
}

class ClientWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::ClientWidget *ui;
	QTimer updateTimer;
	QSharedPointer<Node> mController;
	QSharedPointer<NodeAssociate> mNodeAssoc;
	WaitingSpinnerWidget *mSpinner;

	// TODO: Look at possibility of reusing the CourierSet class in Agent.cpp
	AgentStateCourier *mAgentStateCourier;
	SensorsCourier *mSensorsCourier;
	BlobCourier *mBlobCourier;

public:
	explicit ClientWidget(QSharedPointer<Node> controller, QSharedPointer<NodeAssociate> nodeAssoc, QWidget *parent=nullptr);
	virtual ~ClientWidget();

private:
	bool eventFilter(QObject *object, QEvent *event);

	// Spinner
	void prepareSpinner();
	void prepareSteering();
	void setSpinnerActive(bool active);

	void init();

	void updateOnlineStatus();

	bool courierRegistration();
	void setCourierRegistration(bool reg);



public:
	CommsChannel *comms();
	QSharedPointer<NodeAssociate> nodeAssoc() const;

	void updateControlLevel(int level);

	// Agent State Courier slots
public slots:
	void onSyncParameterChanged(ISyncParameter *);

	// CommsChannel slots
private slots:
	void onCommsError(QString);
	void onCommsClientAdded(CommsSession *);
	void onCommsConnectionStatusChanged(bool);



	// Internal slots
public slots:
	void onUpdateTimer();
	void appendLog(const QString& text);


	// Internal custom UI slots
private slots:

	void onConnectButtonStateChanged(const TryToggleState, const TryToggleState);
	void onSteeringChanged(qreal throttle, qreal steeringAngle);

	// Internal UI slots
public slots:

	void on_checkBoxShowEyes_toggled(bool checked);
	void on_checkBoxShowStats_toggled(bool checked);
	void on_checkBoxShowLog_toggled(bool checked);
	void on_checkBoxShowOnlineButton_toggled(bool checked);
	void on_widgetPanic_toggled(bool checked);


};

#endif // CLIENTWINDOW_HPP
