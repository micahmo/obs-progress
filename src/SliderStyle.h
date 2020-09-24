// From: https://github.com/obsproject/obs-studio/blob/0ba9b201a78b022b863db5aba84a6c34c348249d/UI/slider-absoluteset-style.hpp

#pragma once

#include <QProxyStyle>

class SliderStyle : public QProxyStyle {
public:
	SliderStyle(const QString& baseStyle);
	SliderStyle(QStyle* baseStyle = Q_NULLPTR);
	int styleHint(QStyle::StyleHint hint, const QStyleOption* option,
		const QWidget* widget,
		QStyleHintReturn* returnData) const;
};