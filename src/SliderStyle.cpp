// From: https://github.com/obsproject/obs-studio/blob/0ba9b201a78b022b863db5aba84a6c34c348249d/UI/slider-absoluteset-style.cpp

#include "SliderStyle.h"

SliderStyle::SliderStyle(const QString& baseStyle)
	: QProxyStyle(baseStyle)
{
}
SliderStyle::SliderStyle(QStyle* baseStyle)
	: QProxyStyle(baseStyle)
{
}

int SliderStyle::styleHint(QStyle::StyleHint hint,
	const QStyleOption* option = 0,
	const QWidget* widget = 0,
	QStyleHintReturn* returnData = 0) const
{
	if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
		return (Qt::LeftButton | Qt::MiddleButton);
	return QProxyStyle::styleHint(hint, option, widget, returnData);
}
