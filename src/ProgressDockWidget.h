#pragma once

#include <QDockWidget>
#include <QWidget>
#include "ProgressContainerLayout.h"

class ProgressDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	explicit ProgressDockWidget(QWidget* parent = 0);
	~ProgressDockWidget();
	ProgressSlider* addProgress(obs_source_t*) const;
	ProgressSlider* addSlideshow(obs_source_t*) const;
	void clearProgressBars() const;

	ProgressContainerLayout* layout;

private:
	QWidget* container;
};
