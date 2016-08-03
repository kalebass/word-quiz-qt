#include "settingshandler.h"
#include <QDirIterator>
#include <QSettings>

const QDir SettingsHandler::dictDir{ "dict/", "*.txt" };

SettingsHandler::SettingsHandler()
{
	QSettings::setDefaultFormat(QSettings::IniFormat);
	findDicts();
	read();
}

SettingsHandler::~SettingsHandler()
{
	write();
}

void SettingsHandler::findDicts()
{
	QSettings settings;
	settings.beginGroup("dict");
	QDirIterator it{ dictDir };
	while (it.hasNext()) {
		it.next();
		if (!settings.contains(it.fileName())) {
			settings.setValue(it.fileName(), true);
		}
	}
}

void SettingsHandler::read()
{
	QSettings settings;
	multipleChoiceMode_ = settings.value("multiple choice mode").toBool();
	settings.beginGroup("dict");
	auto fileNames{ settings.childKeys() };
	for (auto& fileName : fileNames) {
		QFile file{ QString{ "dict/%1" }.arg(fileName) };
		if (file.exists()) {
			auto isEnabled{ settings.value(fileName).toBool() };
			dicts_.insert(fileName, isEnabled);
		}
	}
}

void SettingsHandler::write()
{
	QSettings settings;
	settings.setValue("multiple choice mode", { multipleChoiceMode_ });
	settings.beginGroup("dict");
	for (auto it{ dicts_.constBegin() }; it != dicts_.constEnd(); ++it) {
		settings.setValue(it.key(), it.value());
	}
}

bool SettingsHandler::isDictEnabled(const QString& name) const
{
	return dicts_.value(name);
}

void SettingsHandler::setDictEnabled(const QString& name, bool enabled)
{
	auto it{ dicts_.find(name) };
	it.value() = enabled;
}

const QHash<QString, bool>& SettingsHandler::dicts() const
{
	return dicts_;
}
