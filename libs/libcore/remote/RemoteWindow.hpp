#ifndef REMOTEWINDOW_HPP
#define REMOTEWINDOW_HPP

#include "widgets/TryToggle.hpp"
#include "basic/LogDestination.hpp"


#include <QWidget>
#include <QGeoPositionInfo>
#include <QHostAddress>
#include <QMenu>


class Remote;
class QAccelerometerReading;
class QCompassReading;
class QGyroscopeReading;
class CommsChannel;
class Client;


namespace Ui {
	class RemoteWindow;
}

class RemoteWindow : public QWidget, public LogDestination{
		Q_OBJECT

	private:
		Ui::RemoteWindow *ui;
		Remote *mRemote;
		QMenu mMenu;

	public:
		explicit RemoteWindow(Remote *mRemote, QWidget *parent = nullptr);
		virtual ~RemoteWindow();

		void appendLog(const QString& text);
		bool eventFilter(QObject *object, QEvent *event);


	private:
		void notifyAndroid(QString);
		void toastAndroid(QString);

		void updateControlLevel();
		void updateActiveAgent();
		void prepareMenu();
		void prepareMap();
		void homeMap();

		void updateIdentity();

		void addAgentToList(QString name, QString iconPath);

	public:

		virtual void keyReleaseEvent(QKeyEvent *);

	private slots:
		void onStartPairing();
		void onStartPlanEditor();
		void onStartShowBirthCertificate();

		//CommsChannel slots
	public slots:
		void onError(QString);
		void onClientAdded(Client *);
		void onConnectionStatusChanged(bool);

		//Internal Sensor slots
	private slots:
		void onPositionUpdated(const QGeoPositionInfo &info);
		void onCompassUpdated(QCompassReading *);
		void onAccelerometerUpdated(QAccelerometerReading *);
		void onGyroscopeUpdated(QGyroscopeReading *r);

		void onServoPositionChanged(int);

		//Internal UI slots
	private slots:
		void onTryToggleConnectionChanged(TryToggleState);
		void on_pushButtonConfirmQuit_clicked();
		void on_comboBoxAgent_currentIndexChanged(int index);
		void on_comboBoxControlLevel_activated(const QString &arg1);
		void on_pushButtonSay_clicked();
		void on_pushButtonMenu_clicked();
		void on_pushButtonBack_6_clicked();
};

#endif // REMOTEWINDOW_HPP
