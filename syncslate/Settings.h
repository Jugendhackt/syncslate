#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_settings.h"
#include <QMouseEvent>
#include <qsettings.h>

namespace Ui
{
	class Settings;
}

class Settings : public QMainWindow
{
	Q_OBJECT

public:
	explicit Settings(QWidget *parent = 0);
	~Settings();

private slots:

	void exit();

	void minimize();

	void closeForm();

	void open_fps_popup();

	void close_fps_popup();

	void change_fps_popup();

	void fps_popup_komma();

	void fps_popup_prozent();

	void fps_popup_fps();

	void fps_out_updated();

	void lock() {
		locked = true;
		move(x(), 0); // move window to the top of the screen
	}
	void unlock() { locked = false; }

protected:
	Ui::SettingsClass *ui;
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

		QSettings settings("JugendHackt", "SyncSlate");
		settings.setValue("x_pos", x_pos);
		settings.setValue("y_pos", y_pos);
	}
private:
	bool locked = false;
	QPoint oldPos;
};
