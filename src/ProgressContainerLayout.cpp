#include "ProgressContainerLayout.h"
#include <QWidget>
#include <QLabel>
#include <QStatusBar>
#include <qmap.h>
#include "Globals.h"
#include <QDebug>
#include <QTimer>

#include "ProgressSlider.h"

ProgressContainerLayout::ProgressContainerLayout(QWidget* parent)
	: QVBoxLayout(parent)
{
	addStretch();
	addStretch();
	setContentsMargins(10, 10, 10, 10);
}

ProgressSlider* ProgressContainerLayout::addProgressBar(obs_source_t* source)
{
	QHBoxLayout* mediaControlsLayout = new QHBoxLayout();
	QStatusBar* statusBar = new QStatusBar();

	ProgressSlider* progressBar = new ProgressSlider();
	progressBar->setOrientation(Qt::Horizontal);
	progressBar->setStyleSheet("QSlider::groove:horizontal { height: 25 } QSlider::handle:horizontal { height: 25; width: 10 }");

	progressBar->setTracking(false);
	
	QTimer* seekTimer = new QTimer(this);
	connect(seekTimer, &QTimer::timeout, [=]()
	{
		if (progressBar->previousState == OBS_MEDIA_STATE_ENDED)
		{
			obs_source_media_restart(source);
			obs_source_media_play_pause(source, true);
			progressBar->previousState = OBS_MEDIA_STATE_PLAYING;
		}
		
		if (progressBar->time != progressBar->lastTime)
		{
			obs_source_media_set_time(source, progressBar->time);
			qDebug() << "Timer: Media time being set to " << progressBar->time;
		}
		progressBar->lastTime = progressBar->time;
	});
	
	connect(progressBar, &QSlider::sliderPressed, [=]()
	{
		progressBar->canChange = false;
		
		progressBar->previousState = obs_source_media_get_state(source);

		if (progressBar->previousState == OBS_MEDIA_STATE_PLAYING)
		{
			obs_source_media_play_pause(source, true);
		}

		progressBar->time = progressBar->value();
		seekTimer->start(100);
	});

	connect(progressBar, &QSlider::sliderReleased, [=]()
	{
		if (seekTimer->isActive())
		{
			seekTimer->stop();
			if (progressBar->time != progressBar->lastTime)
			{
				if (progressBar->previousState == OBS_MEDIA_STATE_ENDED)
				{
					obs_source_media_restart(source);
				}
				
				obs_source_media_set_time(source, progressBar->time);
				qDebug() << "Released: Media time being set to " << progressBar->time;
			}
			progressBar->time = progressBar->lastTime = -1;
		}

		if (progressBar->previousState != OBS_MEDIA_STATE_PAUSED)
		{
			obs_source_media_play_pause(source, false);
		}

		progressBar->canChange = true;
	});
	
	connect(progressBar, &QSlider::sliderMoved, [=](int newSliderValue)
	{
		if (seekTimer->isActive())
		{
			progressBar->time = newSliderValue;
			qDebug() << "Time variable is set to " << progressBar->time;
		}
	});
	
	statusBar->addPermanentWidget(progressBar, 1);
	
	QLabel* label = new QLabel();
	insertWidget(count() - 1, label); // Insert above the last stretch

	QWidget* mediaControls = new QWidget();
	mediaControlsLayout->setContentsMargins(0, 0, 0, 10);
	mediaControls->setLayout(mediaControlsLayout);
	insertWidget(count() - 1, mediaControls);

	QPushButton* playPauseButton = addPlayPauseButton(mediaControlsLayout, source);	
	addStopButton(mediaControlsLayout, source);

	mediaControlsLayout->addWidget(statusBar);

	QPushButton* loopToggleButton = addLoopToggleButton(mediaControlsLayout, source);

	videoWidgets[progressBar] = { label, playPauseButton, loopToggleButton };

	return progressBar;
}

QWidget* ProgressContainerLayout::addSlideshow(obs_source_t* source)
{
	const char* name = obs_source_get_name(source);
	
	QLabel* label = new QLabel();
	insertWidget(count() - 1, label); // Insert above the last stretch

	QHBoxLayout* mediaControlsLayout = new QHBoxLayout();
	addRestartButton(mediaControlsLayout, source);
	addPreviousButton(mediaControlsLayout, source);
	addNextButton(mediaControlsLayout, source);
	mediaControlsLayout->addStretch();

	QWidget* mediaControls = new QWidget();
	mediaControlsLayout->setContentsMargins(0, 0, 0, 10);
	mediaControls->setLayout(mediaControlsLayout);
	insertWidget(count() - 1, mediaControls);

	slideshowWidgets[mediaControls] = { label };
	
	return mediaControls;
}

QPushButton* ProgressContainerLayout::addRestartButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* restartButton = new QPushButton();
	restartButton->setMaximumWidth(25);
	restartButton->setIcon(Globals::restartIcon);
	restartButton->setToolTip("First slide");

	connect(restartButton, &QPushButton::clicked, [=]()
	{
		obs_source_media_restart(source);
	});

	layout->addWidget(restartButton);
	return restartButton;
}

QPushButton* ProgressContainerLayout::addPreviousButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* previousButton = new QPushButton();
	previousButton->setMaximumWidth(25);
	previousButton->setIcon(Globals::previousIcon);
	previousButton->setToolTip("Previous slide");

	connect(previousButton, &QPushButton::clicked, [=]()
	{
		obs_source_media_previous(source);
	});

	layout->addWidget(previousButton);
	return previousButton;
}

QPushButton* ProgressContainerLayout::addNextButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* nextButton = new QPushButton();
	nextButton->setMaximumWidth(25);
	nextButton->setIcon(Globals::nextIcon);
	nextButton->setToolTip("Next slide");

	connect(nextButton, &QPushButton::clicked, [=]()
	{
		obs_source_media_next(source);
	});

	layout->addWidget(nextButton);
	return nextButton;
}

QPushButton* ProgressContainerLayout::addPlayPauseButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* playPauseButton = new QPushButton();
	playPauseButton->setObjectName("PlayPauseButton");
	playPauseButton->setIcon(Globals::pauseIcon);
	playPauseButton->setMaximumWidth(25);

	// Connect the clicked event on the button to the signal mapper
	connect(playPauseButton, &QPushButton::clicked, [=]()
	{
		obs_media_state playState = obs_source_media_get_state(source);

		bool nowPlaying;
		if (playState == OBS_MEDIA_STATE_ENDED)
		{
			obs_source_media_restart(source);
			nowPlaying = true;
		}
		else
		{
			obs_source_media_play_pause(source, playState == OBS_MEDIA_STATE_PLAYING);

			// If it was playing before we sent the command to toggle state, then it is not playing now.
			// Hence why nowPlaying is true if the previous playing state was not.
			nowPlaying = playState != OBS_MEDIA_STATE_PLAYING;
		}

		// Update the button text
		playPauseButton->setIcon(nowPlaying ? Globals::pauseIcon : Globals::playIcon);
	});

	layout->addWidget(playPauseButton);
	return playPauseButton;
}

QPushButton* ProgressContainerLayout::addStopButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* stopButton = new QPushButton();
	stopButton->setIcon(Globals::stopIcon);
	stopButton->setMaximumWidth(25);

	connect(stopButton, &QPushButton::clicked, [=]()
	{
		obs_media_state playState = obs_source_media_get_state(source);
		if (playState != OBS_MEDIA_STATE_ENDED)
		{
			obs_source_media_stop(source);
		}
	});
	
	layout->addWidget(stopButton);
	return stopButton;
}

QPushButton* ProgressContainerLayout::addLoopToggleButton(QHBoxLayout* layout, obs_source_t* source)
{
	QPushButton* loopToggleButton = new QPushButton();
	loopToggleButton->setObjectName("LoopToggleButton");
	loopToggleButton->setIcon(Globals::loopIcon);
	loopToggleButton->setToolTip("Toggle loop");
	loopToggleButton->setCheckable(true);
	loopToggleButton->setMaximumWidth(25);

	obs_data_t* initialSettings = obs_source_get_settings(source); // Must release
	const bool initialLoop = obs_data_get_bool(initialSettings, "looping");
	obs_data_release(initialSettings);
	
	loopToggleButton->setChecked(initialLoop);
	
	connect(loopToggleButton, &QPushButton::clicked, [=]()
	{
		obs_data_t* settings = obs_source_get_settings(source);
		obs_data_set_bool(settings, "looping", loopToggleButton->isChecked());

		// Important: Must persist the settings back to the source
		obs_source_update(source, settings);

		obs_data_release(settings);
	});

	layout->addWidget(loopToggleButton);
	return loopToggleButton;
}

std::vector<QWidget*> ProgressContainerLayout::getWidget(ProgressSlider* progressBar)
{
	if (videoWidgets.contains(progressBar))
	{
		return videoWidgets[progressBar]; 
	}
	else
	{
		return { };
	}
}

QLabel* ProgressContainerLayout::getLabel(ProgressSlider* progressBar)
{
	if (videoWidgets.contains(progressBar))
	{
		for (auto& widget : videoWidgets[progressBar])
		{
			if (typeid(*widget) == typeid(QLabel))
			{
				return dynamic_cast<QLabel*>(widget);
			}
		}
	}

	return new QLabel();
}

QLabel* ProgressContainerLayout::getLabel(QWidget* mediaControls)
{
	if (slideshowWidgets.contains(mediaControls))
	{
		for (auto& widget : slideshowWidgets[mediaControls])
		{
			if (typeid(*widget) == typeid(QLabel))
			{
				return dynamic_cast<QLabel*>(widget);
			}
		}
	}

	return new QLabel();
}

QPushButton* ProgressContainerLayout::getPlayPauseButton(ProgressSlider* progressBar)
{
	if (videoWidgets.contains(progressBar))
	{
		for (auto& widget : videoWidgets[progressBar])
		{
			if (typeid(*widget) == typeid(QPushButton))
			{
				QPushButton* button = dynamic_cast<QPushButton*>(widget);
				if (button->objectName() == "PlayPauseButton")
				{
					return button;
				}
			}
		}
	}

	return new QPushButton();
}

QPushButton* ProgressContainerLayout::getLoopToggleButton(ProgressSlider* progressBar)
{
	if (videoWidgets.contains(progressBar))
	{
		for (auto& widget : videoWidgets[progressBar])
		{
			if (typeid(*widget) == typeid(QPushButton))
			{
				QPushButton* button = dynamic_cast<QPushButton*>(widget);
				if (button->objectName() == "LoopToggleButton")
				{
					return button;
				}
			}
		}
	}

	return new QPushButton();
}


ProgressContainerLayout::~ProgressContainerLayout()
{
	
}