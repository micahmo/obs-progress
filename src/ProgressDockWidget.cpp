#include <QStatusBar>
#include <QScrollArea>
#include "ProgressDockWidget.h"

ProgressDockWidget::ProgressDockWidget(QWidget* parent) 
	: QDockWidget(parent)
{
	setObjectName("ProgressDockWidget");
	setWindowTitle("Media Progress");
	setFloating(false);

	QScrollArea* scrollArea = new QScrollArea(this);

	container = new QWidget();
	scrollArea->setWidget(container);
	scrollArea->setWidgetResizable(true);

	layout = new ProgressContainerLayout();

	container->setLayout(layout);
	setWidget(scrollArea);
}

ProgressSlider* ProgressDockWidget::addProgress(obs_source_t* source) const
{
	return layout->addProgressBar(source);
}

QWidget* ProgressDockWidget::addSlideshow(obs_source_t* source) const
{
	return layout->addSlideshow(source);
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
