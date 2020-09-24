#include "ProgressSlider.h"
#include "SliderStyle.h"

ProgressSlider::ProgressSlider(QWidget* parent)
	: QSlider(parent), canChange(true), lastTime(-1), time(-1), previousState(OBS_MEDIA_STATE_NONE)
{
	setStyle(new SliderStyle());
}