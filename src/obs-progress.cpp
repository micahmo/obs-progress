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

using namespace std::chrono;

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-progress", "en-US")

ProgressDockWidget* _progressDockWidget;
std::map<obs_media_state, QString> media_states;
void Initialize();
void startTimer();
void stopTimer();
void timerHit();
void updateSceneInfo();

QTimer* _timer;
QMap<obs_source_t*, QProgressBar*> _sources;

QString progressBarTitleFormat = "'%1' Source   %2 / %3 / -%4";

typedef bool (*scene_items_callback)(obs_scene_t*, obs_sceneitem_t*, void*);

void obs_module_unload(void)
{
	stopTimer();
}

bool obs_module_load(void)
{
	_sources = QMap<obs_source_t*, QProgressBar*>();

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
	};
	obs_frontend_add_event_callback(eventCallback, static_cast<void*>(static_cast<obs_frontend_event_cb>(eventCallback)));

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
		if (_sources.isEmpty() == false)
		{
			QMapIterator<obs_source_t*, QProgressBar*> i(_sources);
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

				// Set the times on the 
				i.value()->setRange(0, duration);
				i.value()->setValue(time);
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
	try
	{
		obs_source_t* currentSceneSource = obs_frontend_get_current_scene(); // This is the only call that increments the count, so we have to release it
		const char* name = obs_source_get_name(currentSceneSource);

		obs_scene_t* currentScene = obs_scene_from_source(currentSceneSource);
		obs_source_release(currentSceneSource);

		auto sceneItemsCallback = [](obs_scene_t* currentScene, obs_sceneitem_t* currentSceneItem, void* param)
		{
			obs_source_t* currentSceneItemSource = obs_sceneitem_get_source(currentSceneItem);

			if (obs_source_media_get_duration(currentSceneItemSource) > 0)
			{
				const QString progressBarTitle = progressBarTitleFormat.arg(obs_source_get_name(currentSceneItemSource)); 
				_sources[currentSceneItemSource] = _progressDockWidget->addProgress(progressBarTitle);
			}

			return true;
		};

		_sources.clear();
		_progressDockWidget->clearProgressBars();
		obs_scene_enum_items(currentScene, sceneItemsCallback, static_cast<void*>(static_cast<scene_items_callback>(sceneItemsCallback)));

		_progressDockWidget->setWindowTitle("Progress of '" + QString(name) + "'");
	}
	catch (...)
	{
		
	}
}