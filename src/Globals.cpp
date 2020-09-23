#include "Globals.h"
#include <QPainter>

QIcon Globals::pauseIcon;
QIcon Globals::playIcon;
QIcon Globals::stopIcon;

void Globals::initialize()
{
	QPixmap pauseImage(":/qt-project.org/styles/commonstyle/images/media-pause-16.png");
	QPainter pauseImagePainter(&pauseImage);
	pauseImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	pauseImagePainter.fillRect(pauseImage.rect(), Qt::gray);
	pauseImagePainter.end();
	Globals::pauseIcon = QIcon(pauseImage);

	QPixmap playImage(":/qt-project.org/styles/commonstyle/images/media-play-16.png");
	QPainter playImagePainter(&playImage);
	playImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	playImagePainter.fillRect(playImage.rect(), Qt::gray);
	playImagePainter.end();
	Globals::playIcon = QIcon(playImage);

	QPixmap stopImage(":/qt-project.org/styles/commonstyle/images/media-stop-16.png");
	QPainter stopImagePainter(&stopImage);
	stopImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	stopImagePainter.fillRect(stopImage.rect(), Qt::gray);
	stopImagePainter.end();
	Globals::stopIcon = QIcon(stopImage);
}