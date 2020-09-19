#pragma once

#include <QDockWidget>
#include <QWidget>
#include <QProgressBar>
#include <QVboxLayout>
#include "ProgressContainerLayout.h"

class ProgressDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit ProgressDockWidget(QWidget* parent = 0);
	~ProgressDockWidget();
	QProgressBar* addProgress(QString text = "") const;
	void clearProgressBars() const;

	ProgressContainerLayout* layout;

private:
	QWidget* container;
};
