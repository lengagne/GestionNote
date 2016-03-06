#include "examples2.hpp"
#include "util.hpp"

#include <ods/ods>

void
Lesson11_CreateFormulaWithPercentage()
{
	const int col = 2;
	int row = 0;
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");

	sheet->CreateRow(row++)->CreateCell(0)->SetValue("My weight");
	sheet->CreateRow(row++)->CreateCell(0)->SetValue("My preferred weight");
	auto *cell = sheet->CreateRow(row++)->CreateCell(0);
	cell->SetValue("Percentage I need to lose");
	cell->SetRowColSpan(1, 2);

	row = 0;
	auto *cell1 = sheet->row(row++)->CreateCell(col);
	auto *cell2 = sheet->row(row++)->CreateCell(col);
	auto *cell3 = sheet->row(row++)->CreateCell(col);
	cell1->SetValue(100);
	cell2->SetValue(80);

	auto *formula = new ods::Formula();
	formula->Add(ods::Grouping::Open);
	formula->Add(cell1);
	formula->Add(ods::Op::Subtract);
	formula->Add(cell2);
	formula->Add(ods::Grouping::Close);
	formula->Add(ods::Op::Divide);
	formula->Add(cell1);

	cell3->SetFormula(formula);
	// Notes:
	// SetPercentageType(..) just like SetPercentageValue(..) under the hood
	// apply a style to the cell so after calling SetFormula(..) you
	// should use the existing cell style instead of setting a new one.
	// SetPercentageType(1) sets cell type to "percentage", one decimal place
	// after the dot, defaults to zero
	cell3->SetPercentageType(1);
	auto *style = cell3->style();
	// also make the cell's text bold with a yellow background
	style->SetBold();
	style->SetBackgroundColor(QColor(255, 255, 0));

	// setting a style on a non-percentage cell:
	style = book.CreateCellStyle();
	cell1->SetStyle(style);
	style->SetBold();
	style->SetBackgroundColor(QColor(215, 255, 215));

	Save(book);
}

void
Lesson12_ReadCurrency()
{
	auto path = QDir(QDir::homePath()).filePath("Currency.ods");
	QFile file(path);
	if (!file.exists())
	{
		qDebug() << "No such file:" << path;
		return;
	}
	ods::Book book(path);

	auto *sheet = book.sheet(0);
	if (sheet == nullptr)
	{
		qDebug() << "No sheet at 0";
		return;
	}
	auto *row = sheet->row(0);
	if (row == nullptr)
	{
		qDebug() << "No such row at 0";
		return;
	}
	auto *cell = row->cell(2);
	if (cell == nullptr)
	{
		qDebug() << "No such cell at 2";
		return;
	}

	auto &value = cell->value();
	if (!value.IsCurrency())
	{
		printf("Not a currency cell\n");
		return;
	}
	qDebug() << "currency value:" << *value.AsCurrency(); // returns a double

	// next test the type of currency
	auto *style = cell->style();
	if (style == nullptr)
	{
		qDebug() << "style = nullptr";
		return;
	}
	ods::style::Currency *curr_style = style->GetCurrencyStyle();
	if (curr_style == nullptr)
	{
		qDebug() << "currency style = nullptr";
		return;
	}

	const ods::CurrencyInfo *info = curr_style->info();
	if (info->IsUSD())
		qDebug() << "The currency is US Dollar";
	else if (info->IsEUR())
		qDebug() << "The currency is Euro";
	 else
		qDebug() << "The currency is something else";
}

void
Lesson13_WriteCurrency()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(0);

	auto *cell = row->CreateCell(0);
	cell->SetCurrencyValue(524); // defaults to "EUR" (euros)

	// set to euros (shows "€" instead of "EUR"), Germany,
	// German, with 3 decimal places
	// see file i18n.hxx for details.
	ods::CurrencyInfo info;
	info.currency_set(ods::i18n::kEuro);
	info.show_symbol_set(true);
	info.country_set(ods::i18n::kGermany);
	info.language_set(ods::i18n::kGerman);
	info.decimal_places_set(3);

	ods::Style *style = book.CreateStyle(info);
	row->CreateCell(1)->SetCurrencyValue(1008.94, style);
	row->CreateCell(2)->SetCurrencyValue(0.402, style);

	// set to "USD" (shows "USD" instead of "$"), USA, English
	// with 1 decimal place
	info.currency_set(ods::i18n::kUSD);
	info.show_symbol_set(false);
	info.country_set(ods::i18n::kUSA);
	info.language_set(ods::i18n::kEnglish);
	info.decimal_places_set(1);

	auto *style2 = book.CreateStyle(info);
	row = sheet->CreateRow(1);
	row->CreateCell(0)->SetCurrencyValue(4.2, style2);
	row->CreateCell(1)->SetCurrencyValue(102.3, style2);
	row->CreateCell(2)->SetCurrencyValue(60, style2);

	row = sheet->CreateRow(2);
	// add a few non-currency values:
	row->CreateCell(0)->SetValue("Non currency cells: ");
	row->CreateCell(1)->SetValue("Hello");
	row->CreateCell(2)->SetValue(40.3);

	Save(book);
}

void
Lesson14_ReadDate()
{
	auto path = QDir(QDir::homePath()).filePath("ReadDate.ods");
	QFile file(path);
	if (!file.exists())
	{
		qDebug() << "No such file:" << path;
		return;
	}
	ods::Book book(path);
	auto *sheet = book.sheet(0);

	if (sheet == nullptr)
	{
		qDebug() << "No sheet at 0";
		return;
	}
	const int kRowIndex = 0;
	auto *row = sheet->row(kRowIndex);

	if (row == nullptr)
	{
		qDebug() << "No row at " << kRowIndex;
		return;
	}
	const int kColIndex = 0;
	auto *cell = row->cell(kColIndex);

	if (cell == nullptr)
	{
		qDebug() << "No cell at " << kColIndex;
		return;
	}
	auto &value = cell->value();

	if (!value.IsDate())
	{
		qDebug() << "cell at (" << kRowIndex << "," << kColIndex
			<< ") is not a date cell";
		return;
	}
	QDateTime *dt = value.AsDate();

	if (dt == nullptr)
	{
		qDebug() << "QDateTime = nullptr";
		return;
	}
	const QLatin1String format("dd.MM.yyyy");
	qDebug().nospace() << "date (" << format << "): " << dt->toString(format);
}

void
Lesson15_WriteDate()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(0);

	qint32 col = 0;
	auto *cell = row->CreateCell(col++);
	// when no style specified, defaults to DAY_MONTH_YEAR for order
	// and to dot as the separator string
	cell->SetValue(QDateTime(QDate(1910, 10, 27)));

	// now, set order and/or the separator string explicitly
	ods::DateInfo info;
	info.order_set(ods::dateinfo::Order::DAY_MONTH_YEAR);
	info.separator_set(ods::dateinfo::Separator::DOT);
	ods::Style *style = book.CreateStyle(info);

	cell = row->CreateCell(col++);
	cell->SetValue(QDateTime(QDate(1981, 1, 18)), style);

	cell = row->CreateCell(col++);
	cell->SetValue(QDateTime(QDate(1811, 2, 26)), style);

	ods::DateInfo info2;
	info2.order_set(ods::dateinfo::Order::MONTH_DAY_YEAR);
	info2.separator_set(ods::dateinfo::Separator::FWD_SLASH);
	auto *style2 = book.CreateStyle(info2);

	cell = row->CreateCell(col++);
	cell->SetValue(QDateTime(QDate(1933, 9, 20)), style2);

	ods::DateInfo info3;
	info3.order_set(ods::dateinfo::Order::YEAR_MONTH_DAY);
	auto *style3 = book.CreateStyle(info3);

	cell = row->CreateCell(col++);
	cell->SetValue(QDateTime(QDate(1924, 7, 22)), style3);

	Save(book);
}

void
Lesson16_ReadDuration()
{
	auto path = QDir(QDir::homePath()).filePath("ReadDuration.ods");
	QFile file(path);
	if (!file.exists())
	{
		qDebug() << "No such file:" << path;
		return;
	}
	ods::Book book(path);
	auto *sheet = book.sheet(0);

	if (sheet == nullptr)
	{
		qDebug() << "No sheet at 0";
		return;
	}
	const int kRowIndex = 0;
	auto *row = sheet->row(kRowIndex);

	if (row == nullptr)
	{
		qDebug() << "No row at " << kRowIndex;
		return;
	}
	const int kColIndex = 0;
	auto *cell = row->cell(kColIndex);

	if (cell == nullptr)
	{
		qDebug() << "No cell at " << kColIndex;
		return;
	}
	auto &value = cell->value();

	if (!value.IsDuration())
	{
		qDebug() << "cell at (" << kRowIndex << "," << kColIndex
			<< ") is not a duration cell";
		return;
	}
	ods::Duration *t = value.AsDuration();

	if (t == nullptr)
	{
		qDebug() << "ods::Duration = nullptr";
		return;
	}
	qDebug().nospace() << "Duration: " << t->ToString();
}

void
Lesson17_WriteDuration()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(0);

	qint32 col = 0;
	auto *cell = row->CreateCell(col++);
	// if no style specified, defaults to "HH:mm" (no seconds displayed)
	cell->SetValue(ods::Duration(50, 12, 40)); // 50 hours, 12 min, 40 sec

	// now, set order explicitly to show seconds as well:
	ods::DurationInfo info;
	info.order_set(ods::duration::Order::HOURS_MINUTES_SECONDS);
	info.truncate_on_overflow_set(true); // wraps by 24 hours, so 32 hours
	// will show up as 8 hours, default is false.
	ods::Style *style = book.CreateStyle(info);

	cell = row->CreateCell(col++);
	cell->SetValue(ods::Duration(32, 59, 50), style); // 32h, 59m, 50s

	Save(book);
}
