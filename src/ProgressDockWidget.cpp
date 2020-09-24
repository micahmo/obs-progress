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

ProgressSlider* ProgressDockWidget::addProgress(obs_source_t* source) const
{
	return layout->addProgressBar(source);
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
