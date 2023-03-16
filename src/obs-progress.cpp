#include <obs-module.h>
#include <QAction>
#include <obs-frontend-api.h>
#include "ProgressDockWidget.h"
#include <QtWidgets/QMainWindow>
#include <obs.h>
#include <ctime>
#include <QTimer>
#include <QObject>
#include <chrono>
#include <date/date.h>
#include <QLabel>
#include "Globals.h"
#include "ProgressSlider.h"

using namespace std::chrono;

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-progress", "en-US")

ProgressDockWidget* _progressDockWidget;
std::map<obs_media_state, QString> media_states;
void startTimer();
void stopTimer();
void timerHit();
void updateSceneInfo();

QTimer* _timer;
QMap<obs_source_t*, ProgressSlider*> _videoSources;
QMap<obs_source_t*, QWidget*> _slideshowSources;

QString progressBarTitleFormat = "'%1' Source   %2 / %3 / -%4";
QString slideshowFormat = "'%1' Slideshow Source   %2 / %3";

typedef bool (*scene_items_callback)(obs_scene_t*, obs_sceneitem_t*, void*);

void obs_module_unload(void)
{
	blog(LOG_INFO, "obs-progress: Unloading");

	stopTimer();
}

bool obs_module_load(void)
{
	blog(LOG_INFO, "obs-progress: Loading");

	Globals::initialize();
	
	_videoSources = QMap<obs_source_t*, ProgressSlider*>();
	_slideshowSources = QMap<obs_source_t*, QWidget*>();

	QMainWindow* mainWindow = static_cast<QMainWindow*>(obs_frontend_get_main_window());
	_progressDockWidget = new ProgressDockWidget(mainWindow);
	obs_frontend_add_dock(_progressDockWidget);

	startTimer();
	updateSceneInfo();

	// Setup event handler to start the server once OBS is ready
	auto eventCallback = [](enum obs_frontend_event event, void* param) {
		if (event == OBS_FRONTEND_EVENT_SCENE_CHANGED) {
			updateSceneInfo();
		}
		else if (event == OBS_FRONTEND_EVENT_SCENE_COLLECTION_CLEANUP) {
			//blog(LOG_INFO, "obs-progress: Got scene collection cleanup event. Had %d source(s). Clearing.", _sources.count());
			//_sources.clear();
		}
	};
	obs_frontend_add_event_callback(eventCallback, static_cast<void*>(static_cast<obs_frontend_event_cb>(eventCallback)));

	blog(LOG_INFO, "obs-progress: Successfully loaded");

	return true;
}

void startTimer()
{
	QMainWindow* mainWindow = static_cast<QMainWindow*>(obs_frontend_get_main_window());
	_timer = new QTimer(mainWindow);
	QObject::connect(_timer, &QTimer::timeout, timerHit);
	_timer->start(500);
}

void stopTimer()
{
	try
	{
		//_timer->stop();
	}
	catch (...)
	{
		// Empty
	}
}

void timerHit()
{
	try
	{
		if (!_videoSources.isEmpty())
		{
			QMapIterator<obs_source_t*, ProgressSlider*> i(_videoSources);
			while (i.hasNext())
			{
				i.next();

				auto* currentSceneItemSource = i.key();

				// Get the times
				auto time = obs_source_media_get_time(currentSceneItemSource);
				auto duration = obs_source_media_get_duration(currentSceneItemSource);

				// Convert the times to a date format
				auto timeTimePoint = floor<seconds>(time_point_cast<milliseconds>(system_clock::time_point(duration_cast<milliseconds>(milliseconds(time)))));
				auto durationTimePoint = floor<seconds>(time_point_cast<milliseconds>(system_clock::time_point(duration_cast<milliseconds>(milliseconds(duration)))));
				auto remainingTimePoint = ceil<seconds>(time_point_cast<milliseconds>(system_clock::time_point(duration_cast<milliseconds>(milliseconds(duration - time)))));
				QString progressBarText = progressBarTitleFormat.arg(obs_source_get_name(currentSceneItemSource), 
					date::format("%T", timeTimePoint).c_str(),
					date::format("%T", durationTimePoint).c_str(),
					date::format("%T", remainingTimePoint).c_str()
				);

				// Update the times on the label
				QLabel* labelToUpdate = _progressDockWidget->layout->getLabel(i.value());
				labelToUpdate->setText(progressBarText);

				if (i.value()->canChange)
				{
					// Set the times on the progress bar
					i.value()->setRange(0, duration);
					i.value()->setValue(time);
				}

				// Get the state of the source and see if it's ended
				obs_media_state state = obs_source_media_get_state(currentSceneItemSource);
				if (state == OBS_MEDIA_STATE_ENDED || state == OBS_MEDIA_STATE_PAUSED)
				{
					QPushButton* playPauseButton = _progressDockWidget->layout->getPlayPauseButton(i.value());
					playPauseButton->setIcon(Globals::playIcon);
				}
				else if (state == OBS_MEDIA_STATE_PLAYING)
				{
					// Sometimes the playback can start automatically (e.g., when settings are updated)
					// so we need to handle this case and update the play/pause button
					QPushButton* playPauseButton = _progressDockWidget->layout->getPlayPauseButton(i.value());
					playPauseButton->setIcon(Globals::pauseIcon);
				}

				// Lastly, make sure the LoopToggle button is in the right state
				obs_data_t* settings = obs_source_get_settings(currentSceneItemSource);
				const bool loop = obs_data_get_bool(settings, "looping");
				obs_data_release(settings);
				QPushButton* loopToggleButton = _progressDockWidget->layout->getLoopToggleButton(i.value());
				loopToggleButton->setChecked(loop);
			}
		}

		if (!_slideshowSources.isEmpty())
		{
			QMapIterator<obs_source_t*, QWidget*> i(_slideshowSources);
			while (i.hasNext())
			{
				i.next();

				auto* currentSceneItemSource = i.key();

				// From: https://github.com/obsproject/obs-studio/blob/bff7928b5069beea674274d42afab33528924dfd/UI/media-controls.cpp#L509-L525
				proc_handler_t* ph = obs_source_get_proc_handler(currentSceneItemSource);
				calldata_t cd = {};

				proc_handler_call(ph, "current_index", &cd);
				int slide = calldata_int(&cd, "current_index");

				proc_handler_call(ph, "total_files", &cd);
				int total = calldata_int(&cd, "total_files");
				calldata_free(&cd);

				QString progressBarText;
				if (total < 0)
				{
					progressBarText = slideshowFormat.arg(obs_source_get_name(currentSceneItemSource), "-", "-");
				}
				else
				{
					progressBarText = slideshowFormat.arg(obs_source_get_name(currentSceneItemSource), QString::number(slide + 1), QString::number(total));
				}

				QLabel* labelToUpdate = _progressDockWidget->layout->getLabel(i.value());
				labelToUpdate->setText(progressBarText);
			}
		}
	}
	catch (...)
	{
		// Empty
	}
}

void updateSceneInfo()
{
	blog(LOG_INFO, "obs-progress: Got scene changed, rebuilding sources");

	try
	{
		obs_source_t* currentSceneSource = obs_frontend_get_current_scene(); // This is the only call that increments the count, so we have to release it
		const char* name = obs_source_get_name(currentSceneSource);

		obs_scene_t* currentScene = obs_scene_from_source(currentSceneSource);
		obs_source_release(currentSceneSource);

		auto sceneItemsCallback = [](obs_scene_t* currentScene, obs_sceneitem_t* currentSceneItem, void* param)
		{
			obs_source_t* currentSceneItemSource = obs_sceneitem_get_source(currentSceneItem);
			const char* id = obs_source_get_unversioned_id(currentSceneItemSource);
			const bool isSlideshow = strcmp(id, "slideshow") == 0;

			// These are handy signals to potentially use in the future if we want to get notified of source changes.
			//signal_handler_t* handler = obs_source_get_signal_handler(currentSceneItemSource);
			//signal_handler_connect(handler, "destroy", source_destroyed, 0);

			//handler = obs_source_get_signal_handler(currentSceneItemSource);
			//signal_handler_connect(handler, "deactivate", source_deactivated, 0);

			if (obs_source_media_get_duration(currentSceneItemSource) > 0)
			{
				// Media source
				_videoSources[currentSceneItemSource] = _progressDockWidget->addProgress(currentSceneItemSource);
			}
			else if (isSlideshow)
			{
				// Slideshow source
				_slideshowSources[currentSceneItemSource] = _progressDockWidget->addSlideshow(currentSceneItemSource);
			}

			return true;
		};

		_videoSources.clear();
		_slideshowSources.clear();
		_progressDockWidget->clearProgressBars();
		obs_scene_enum_items(currentScene, sceneItemsCallback, static_cast<void*>(static_cast<scene_items_callback>(sceneItemsCallback)));

		blog(LOG_INFO, "obs-progress: Tracking %d video source(s)", _videoSources.count());
		blog(LOG_INFO, "obs-progress: Tracking %d slideshow source(s)", _slideshowSources.count());

		_progressDockWidget->setWindowTitle("Progress of '" + QString(name) + "'");
	}
	catch (...)
	{
		
	}
}
