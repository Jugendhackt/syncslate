#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SyncSlate.h"
#include <QMouseEvent>

namespace Ui
{
	class SyncSlate;
}

class SyncSlate : public QMainWindow
{
	Q_OBJECT

public:
	explicit SyncSlate(QWidget *parent = 0);
	~SyncSlate();

	//bool eventFilter(QObject *object, QEvent *event);
	//void mousePressEvent(QMouseEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);

	//QPoint current;
	//bool pressed;

private slots:
	void onTimestamp();

	void on_videoButton_clicked();

    void on_runButton_clicked();

	void exit();

	void minimize();

	void reloadUi();

	void lock() {
		locked = true;
		move(x(), 0); // move window to the top of the screen
	}
	void unlock() { locked = false; }

protected:
	Ui::SyncSlateClass *ui;
	void mousePressEvent(QMouseEvent *evt)
	{
		oldPos = evt->globalPos();
	}

	void mouseMoveEvent(QMouseEvent *evt)
	{
		const QPoint delta = evt->globalPos() - oldPos;
		if (locked)
			// if locked, ignore delta on y axis, stay at the top
			move(x() + delta.x(), y());
		else
			move(x() + delta.x(), y() + delta.y());
		oldPos = evt->globalPos();
	}
private:
	bool locked = false;
	QPoint oldPos;
};
