#ifndef APP_UTIL_CPP_
#define APP_UTIL_CPP_

#include <QtCore>
#include <ods/ods>

QString
GetCellValue(ods::Cell *cell)
{
	if (cell->HasFormula())
	{
		auto *formula = cell->formula();
		if (formula->error())
			return QString("formula error: ") + formula->err();
		auto &value = formula->value();
		if (value.NoValue()) // should never happen
			return "formula has no value";
		if (value.IsDouble() || value.IsPercentage())
		{
			return QString("formula value: ")
				+ QString::number(*value.AsDouble());
		}
		// don't care, just print out as a string
		return QString("formula has a non-number value: ") + value.toString();
	}

	const ods::Value &value = cell->value();
	if (value.NoValue())
		return "cell value is empty";
	else if (value.IsDouble())
		return QString("cell value as double: ")
			 + QString::number(*value.AsDouble());
	else if (value.IsPercentage())
		return QString("cell value as percentage: ")
			+ QString::number(*value.AsPercentage());
	else if (value.IsString())
		return QString("cell value as string: ") + *value.AsString();

	return "unknown cell type";
}

void
Save(ods::Book &book)
{
	auto path = QDir(QDir::homePath()).filePath("file.ods");
	QFile target(path);
	QString err = book.Save(target);
	if (err.isEmpty())
		qDebug() << "Saved to" << target.fileName();
	else
		qDebug() << "Error saving ods file:" << err;
}

#endif
