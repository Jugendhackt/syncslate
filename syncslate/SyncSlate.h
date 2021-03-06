#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SyncSlate.h"
#include <QMouseEvent>
#include <qsettings.h>

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


private slots:
	void onTimestamp();

	void on_videoButton_clicked();

    void on_runButton_clicked();

	void exit();

	void minimize();

	void reloadUi();

	void OpenSettings();

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
		int x_pos;
		int y_pos;

		const QPoint delta = evt->globalPos() - oldPos;
		if (locked) {
			x_pos = x() + delta.x();
			y_pos = y();
			move(x_pos, y_pos);
		}
		else {
			x_pos = x() + delta.x();
			y_pos = y() + delta.y();
			move(x_pos, y_pos);
		}
		oldPos = evt->globalPos();

		QSettings settings("JugendHackt","SyncSlate");
		settings.setValue("x_pos", x_pos);
		settings.setValue("y_pos", y_pos);
	}
private:
	bool locked = false;
	QPoint oldPos;
};
