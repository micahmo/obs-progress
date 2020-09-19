#include <QStatusBar>
#include "ProgressDockWidget.h"

ProgressDockWidget::ProgressDockWidget(QWidget* parent) 
	: QDockWidget(parent)
{
	setObjectName("ProgressDockWidget");
	setWindowTitle("Media Progress");
	setFloating(false);

	// I guess you can't "new" using the constructor syntax??
	container = new QWidget();
	layout = new ProgressContainerLayout();

	container->setLayout(layout);
	setWidget(container);
}

QProgressBar* ProgressDockWidget::addProgress(QString text) const
{
	return layout->addProgressBar(text);
}

void ProgressDockWidget::clearProgressBars() const
{
	for (QWidget* widget : container->findChildren<QWidget*>(QString{}, Qt::FindDirectChildrenOnly))
	{
		delete widget;
	}
}

ProgressDockWidget::~ProgressDockWidget()
{
	//delete statusBar;
}
