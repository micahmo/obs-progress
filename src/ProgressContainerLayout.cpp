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
	addStopButton(mediaControlsLayout, source);

	mediaControlsLayout->addWidget(statusBar);

	QPushButton* loopToggleButton = addLoopToggleButton(mediaControlsLayout, source);

	widgets[progressBar] = { label, playPauseButton, loopToggleButton };

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

QPushButton* ProgressContainerLayout::getLoopToggleButton(QProgressBar* progressBar)
{
	if (widgets.contains(progressBar))
	{
		for (auto& widget : widgets[progressBar])
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