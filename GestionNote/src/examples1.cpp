#include "examples1.hpp"
#include "Invoice.hpp"
#include "util.hpp"

void
Lesson01_CreateEmptyBook()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(0); // create first/top row

	// Column (cell) count is zero-based, e.g. 0 = 1, 4 = 5.
	// Cells and rows must always be created in ascending order.

	row->CreateCell(1)->SetValue("At 1");
	row->CreateCell(4)->SetValue("At 4");

	Save(book);
}

void
Lesson02_CreateCellsOfDifferentTypes()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(0);

	auto *cell = row->CreateCell(0);
	// will hold a string value
	cell->SetValue("string");

	cell = row->CreateCell(1);
	// will hold a float value
	// Note: internally the API stores/uses numbers as doubles, but the
	// ODF standard seems to use/store as floats
	cell->SetValue(3.14);

	// Cell with percentage value
	cell = row->CreateCell(2);
	cell->SetPercentageValue(0.8, 2); // 0.8 = 80.00%, second param (2) is
	// the number of leading zeroes after the dot, in this case
	// it's ".00" from "80.00%"

	// Cell with another percentage value
	cell = row->CreateCell(3);
	cell->SetPercentageValue(17.2); // 17.2 = 1720%, the second param
	// by default is zero, hence it will show up as "1720%" and not
	// as "1720.0%" or "1720.00%".

	Save(book);
}

void
Lesson03_UseFontsAndStyles()
{
	/** Note: when applying same style to many cells - don't create a new
	style for each cell, instead use the same style for them. For example:

	WRONG:
	for(int i=0; i<10; i++)
	{
		auto *style = book.CreateCellStyle();
		style->SetBackground(QColor(200, 0, 0));
		auto *cell = row->CreateCell(i);
		cell->SetStyle(style);
	}

	RIGHT:
	auto *style = book.CreateCellStyle();
	style->SetBackground(QColor(200, 0, 0));
	for(int i=0; i<10; i++)
	{
		auto *cell = row->CreateCell(i);
		cell->SetStyle(style);
	}
	**/

	ods::Book book;
	auto *sheet = book.CreateSheet("name");
	auto *row = sheet->CreateRow(0);

	auto *cell = row->CreateCell(0);
	cell->SetValue("string");

	// Note: styles are managed by ods::Book, so never delete styles yourself.
	auto *style = book.CreateCellStyle();
	style->SetFontName("Verdana");
	style->SetFontSize(20.0);
	style->SetTextColor(QColor(200, 0, 0));
	cell->SetStyle(style);

	// We got a big font size in a cell, so when done creating all the cells
	// of the row tell the row to resize itself vertically for the contents
	// of the cell with the biggest font size to be fully visible by default
	// when opened with an office suite like LibreOffice or MS Office.
	row->SetOptimalHeightStyle();

	cell = row->CreateCell(1);
	//derive a new style from parent style and set yellow background
	style = style->Derive();
	style->SetBackgroundColor(QColor(255, 255, 0));
	cell->SetValue(3.14f);
	cell->SetStyle(style);

	Save(book);
}

void
Lesson04_Alignment()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("name");
	auto *row = sheet->CreateRow(0);

	auto *cell = row->CreateCell(0);
	cell->SetValue("string");
	auto *style = book.CreateCellStyle();
	style->SetHAlignment(ods::HAlign::Center);
	//aligns the string value "string" of the cell to the center
	cell->SetStyle(style);

	cell = row->CreateCell(1);
	cell->SetValue(3.14);
	style = book.CreateCellStyle();
	//aligns the number value "3.14" of the cell to the right
	style->SetHAlignment(ods::HAlign::Right);
	cell->SetStyle(style);

	Save(book);
}

void
Lesson05_Borders()
{
	// Borders are used by creating a border class and then applying it
	// to a style which then gets applied to the cell

	ods::Book book;
	auto *sheet = book.CreateSheet("name");
	auto *row = sheet->CreateRow(1);

	auto *cell = row->CreateCell(1);
	cell->SetValue("string");

	auto *style = book.CreateCellStyle();

	auto *base_border = new ods::style::Border();// thin solid black by default
	base_border->SetColor(QColor(0, 0, 255)); // make the border blue
	base_border->SetStyle(ods::BorderStyle::Solid);
	base_border->SetWidth(ods::BorderWidth::Medium);
	style->SetBorder(base_border);
	// cell which contains "string" will have solid blue border all around it
	cell->SetStyle(style);

	cell = row->CreateCell(5);
	cell->SetValue(3.14);

	auto *top_border = base_border->Derive();
	top_border->sides_set(ods::BorderSideTop);
	top_border->SetColor(QColor(0, 255, 0)); // (top) green border
	top_border->SetWidth("0.05in");

	style = book.CreateCellStyle();
	style->SetBorder(top_border);
	// cell which contains "3.14" will only have a (very) thick top green border
	cell->SetStyle(style);

	Save(book);
}

void
Lesson06_CellSpan()
{
	ods::Book book;
	auto *sheet = book.CreateSheet("name");
	auto *row = sheet->CreateRow(0);

	auto *cell = row->CreateCell(0);
	cell->SetValue("string");
	// By default a cell spans over 1 row and 1 column.
	// Make the cell span over 2 rows and 4 columns:
	cell->SetRowColSpan(2, 4);

	Save(book);
}

void
Lesson07_UsingImages()
{
	// Images are set on the sheet relative to a cell

	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");
	auto *row = sheet->CreateRow(3);
	auto *cell = row->CreateCell(2);

	//let's load a .png image "image.png" from the home dir into this cell
	QFile file(QDir(QDir::home()).filePath("image.png"));
	ods::DrawFrame *draw_frame = cell->CreateDrawFrame(file);
	if (draw_frame == nullptr)
	{
		qDebug() << "DrawFrame == nullptr";
		return;
	}
	// optional: resize it
	draw_frame->SetSize(QSize(40, 80));
	// optional: move the image by x,y points relative to the cell
	draw_frame->SetOffset(QPoint(60, 0));

	Save(book);
}

void
Lesson08_UsingFormulas()
{
	// only very basic formula functionality is supported
	ods::Book book;
	auto *sheet = book.CreateSheet("Sheet1");

	for (int i = 2; i < 10; i++)
	{
		auto *row = sheet->CreateRow(i);

		auto *cell1 = row->CreateCell(1);
		cell1->SetValue(15);

		auto *cell2 = row->CreateCell(2);
		cell2->SetValue(i);

		// Create a formula that adds the numbers from cell1 and cell2,
		// multiplies the resulting sum by two and applies the
		// resulting formula to cell3. Schematically the formula looks
		// like this: cell3 = "(cell1 + cell2) * 2"
		auto *formula = new ods::Formula();
		formula->Add(ods::Grouping::Open);
		formula->Add(cell1);
		formula->Add(ods::Op::Add);
		formula->Add(cell2);
		formula->Add(ods::Grouping::Close);
		formula->Add(ods::Op::Mult);
		formula->Add(2);

		auto *cell3 = row->CreateCell(3);
		cell3->SetFormula(formula);
	}

	Save(book);
}

void
Lesson09_CreateSampleInvoice()
{
	new app::Invoice();
}

void
Lesson10_ReadFile()
{
	auto path = QDir(QDir::homePath()).filePath("ReadFile.ods");
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

	//print out the values of the first 9 cells of the 3rd row:
	const int kRow = 2;
	for (int i=0; i <= 8; i++)
	{
		const int kColumn = i;
		ods::Cell *cell = nullptr;
		auto *row = sheet->row(kRow);
		if (row == nullptr)
		{
			qDebug() << "No row at " << QString::number(kRow);
			return;
		}
		cell = row->cell(kColumn);

		if (cell == nullptr) {
			qDebug() << "No cell at " << QString::number(kColumn);
			continue;
		}
		QString which_cell = "Cell [" + QString::number(kRow)
			+ ":" + QString::number(kColumn) + "]: ";
		QString value_as_string = GetCellValue(cell);
		qDebug() << which_cell << value_as_string;
	}
}

