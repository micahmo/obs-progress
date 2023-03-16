#pragma once

#include <obs.h>
#include <QHboxLayout>
#include <QLabel>
#include <qmap.h>
#include <QPushButton>
#include "ProgressSlider.h"

class ProgressContainerLayout : public QVBoxLayout
{
	Q_OBJECT

public:
	explicit ProgressContainerLayout(QWidget* = 0);
	~ProgressContainerLayout();
	ProgressSlider* addProgressBar(obs_source_t*);
	QWidget* addSlideshow(obs_source_t*);
	std::vector<QWidget*> getWidget(ProgressSlider*);
	QLabel* getLabel(ProgressSlider*);
	QLabel* getLabel(QWidget*);
	QPushButton* getPlayPauseButton(ProgressSlider*);
	QPushButton* getLoopToggleButton(ProgressSlider*);

private:
	QMap<ProgressSlider*, std::vector<QWidget*>> videoWidgets;
	QMap<QWidget*, std::vector<QWidget*>> slideshowWidgets;
	QPushButton* addPlayPauseButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addStopButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addLoopToggleButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addRestartButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addPreviousButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addNextButton(QHBoxLayout*, obs_source_t*);
};
