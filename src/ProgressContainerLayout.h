#pragma once

#include <QVboxLayout>
#include <QProgressBar>
#include <QLabel>
#include <qmap.h>

class ProgressContainerLayout : public QVBoxLayout
{
	Q_OBJECT

public:
	explicit ProgressContainerLayout(QWidget* parent = 0);
	~ProgressContainerLayout();
	QProgressBar* addProgressBar(QString text = "");
	std::vector<QWidget*> getWidget(QProgressBar* progressBar);
	QLabel* getLabel(QProgressBar* progressBar);

private:
	QMap<QProgressBar*, std::vector<QWidget*>> widgets;
};
