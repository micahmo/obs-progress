#include "Globals.h"
#include <QPainter>

QIcon Globals::pauseIcon;
QIcon Globals::playIcon;
QIcon Globals::stopIcon;
QIcon Globals::loopIcon;
QIcon Globals::previousIcon;
QIcon Globals::nextIcon;
QIcon Globals::restartIcon;

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

	QPixmap loopImage(":/qt-project.org/styles/commonstyle/images/refresh-24.png");
	QPainter loopImagePainter(&loopImage);
	loopImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	loopImagePainter.fillRect(loopImage.rect(), Qt::lightGray);
	loopImagePainter.end();
	Globals::loopIcon = QIcon(loopImage);

	QPixmap previousImage(":/qt-project.org/styles/commonstyle/images/media-seek-backward-16.png");
	QPainter previousImagePainter(&previousImage);
	previousImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	previousImagePainter.fillRect(previousImage.rect(), Qt::lightGray);
	previousImagePainter.end();
	Globals::previousIcon = QIcon(previousImage);

	QPixmap nextImage(":/qt-project.org/styles/commonstyle/images/media-seek-forward-16.png");
	QPainter nextImagePainter(&nextImage);
	nextImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	nextImagePainter.fillRect(nextImage.rect(), Qt::lightGray);
	nextImagePainter.end();
	Globals::nextIcon = QIcon(nextImage);

	QPixmap restartImage(":/qt-project.org/styles/commonstyle/images/media-skip-backward-16.png");
	QPainter restartImagePainter(&restartImage);
	restartImagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	restartImagePainter.fillRect(restartImage.rect(), Qt::lightGray);
	restartImagePainter.end();
	Globals::restartIcon = QIcon(restartImage);
}