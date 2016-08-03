#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H
#include <QDir>
#include <QHash>

class SettingsHandler
{
public:
	SettingsHandler();
	~SettingsHandler();
	const QHash<QString, bool>& dicts() const;
	bool isDictEnabled(const QString& name) const;
	void setDictEnabled(const QString& name, bool);
	bool multipleChoiceMode() const;

private:
	static const QDir dictDir;

	void read();
	void write();
	void findDicts();

	QHash<QString, bool> dicts_;
	bool multipleChoiceMode_;
};

#endif // SETTINGSHANDLER_H
