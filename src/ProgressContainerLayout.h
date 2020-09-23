#pragma once

#include <obs.h>
#include <QHboxLayout>
#include <QProgressBar>
#include <QLabel>
#include <qmap.h>
#include <QPushButton>

class ProgressContainerLayout : public QVBoxLayout
{
	Q_OBJECT

public:
	explicit ProgressContainerLayout(QWidget* = 0);
	~ProgressContainerLayout();
	QProgressBar* addProgressBar(obs_source_t*);
	std::vector<QWidget*> getWidget(QProgressBar*);
	QLabel* getLabel(QProgressBar*);
	QPushButton* getPlayPauseButton(QProgressBar*);
	QPushButton* getLoopToggleButton(QProgressBar*);

private:
	QMap<QProgressBar*, std::vector<QWidget*>> widgets;
	QPushButton* addPlayPauseButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addStopButton(QHBoxLayout*, obs_source_t*);
	QPushButton* addLoopToggleButton(QHBoxLayout*, obs_source_t*);
};
