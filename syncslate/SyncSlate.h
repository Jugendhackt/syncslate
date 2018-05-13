#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SyncSlate.h"

namespace Ui
{
	class SyncSlate;
}

class SyncSlate : public QMainWindow
{
	Q_OBJECT

public:
	SyncSlate(QWidget *parent = 0);
	~SyncSlate();

private slots:
	void onTimestamp();

	void on_videoButton_clicked();

    void on_runButton_clicked();

protected:
	Ui::SyncSlateClass *ui;
};
