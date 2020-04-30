#include "ProgressContainerLayout.h"
#include <QWidget>
#include <QLabel>
#include <QStatusBar>
#include <qmap.h>

ProgressContainerLayout::ProgressContainerLayout(QWidget* parent)
	: QVBoxLayout(parent)
{
	
}

QProgressBar* ProgressContainerLayout::addProgressBar(QString text)
{
	QStatusBar* statusBar = new QStatusBar();
	QProgressBar* progressBar = new QProgressBar();
	statusBar->addPermanentWidget(progressBar, 1);

	QLabel* label = new QLabel(text);

	addWidget(label);
	addWidget(statusBar);

	widgets[progressBar] = { label };

	return progressBar;
}

std::vector<QWidget*> ProgressContainerLayout::getWidget(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		return widgets[progressBar]; 
	}
	else
	{
		return { };
	}
}

QLabel* ProgressContainerLayout::getLabel(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		for (auto& widget : widgets[progressBar])
		{
			if (typeid(*widget) == typeid(QLabel))
			{
				return (QLabel*)widget;
			}
		}
	}

	return new QLabel();
}

ProgressContainerLayout::~ProgressContainerLayout()
{
	
}