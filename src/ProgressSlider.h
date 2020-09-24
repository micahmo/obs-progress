#pragma once

#include <obs.h>
#include <QSlider>

class ProgressSlider : public QSlider
{
	Q_OBJECT

public:
	explicit ProgressSlider(QWidget* parent = 0);
	bool canChange;
	int64_t lastTime;
	int64_t time;
	obs_media_state previousState;
};
