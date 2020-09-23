#include "ProgressContainerLayout.h"
#include <QWidget>
#include <QLabel>
#include <QStatusBar>
#include <qmap.h>
#include "Globals.h"

ProgressContainerLayout::ProgressContainerLayout(QWidget* parent)
	: QVBoxLayout(parent)
{
	addStretch();
	addStretch();
	setMargin(10);
}

QProgressBar* ProgressContainerLayout::addProgressBar(obs_source_t* source)
{
	QHBoxLayout* mediaControlsLayout = new QHBoxLayout();
	QStatusBar* statusBar = new QStatusBar();
	QProgressBar* progressBar = new QProgressBar();
	statusBar->addPermanentWidget(progressBar, 1);
	
	QLabel* label = new QLabel();
	insertWidget(count() - 1, label); // Insert above the last stretch

	QWidget* mediaControls = new QWidget();
	mediaControlsLayout->setContentsMargins(0, 0, 0, 10);
	mediaControls->setLayout(mediaControlsLayout);
	insertWidget(count() - 1, mediaControls);

	QPushButton* playPauseButton = addPlayPauseButton(mediaControlsLayout, source);	
	QPushButton* stopButton = addStopButton(mediaControlsLayout, source);

	mediaControlsLayout->addWidget(statusBar);

	widgets[progressBar] = { label, playPauseButton };

	return progressBar;
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

std::vector<QWidget*> ProgressContainerLayout::getWidget(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		return widgets[progressBar]; 
	}
	else
	{
		return { };
	}
}

QLabel* ProgressContainerLayout::getLabel(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		for (auto& widget : widgets[progressBar])
		{
			if (typeid(*widget) == typeid(QLabel))
			{
				return dynamic_cast<QLabel*>(widget);
			}
		}
	}

	return new QLabel();
}

QPushButton* ProgressContainerLayout::getPlayPauseButton(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		for (auto& widget : widgets[progressBar])
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

ProgressContainerLayout::~ProgressContainerLayout()
{
	
}