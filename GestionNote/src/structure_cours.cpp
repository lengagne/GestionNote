#include "structure_cours.hpp"
#define PROJECT_NAME "GestionNoteProject.xml"

#include <time.h>

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



structure_cours::structure_cours()
{
//	std::cout<<"Creation de l'objet strucutre cours"<<std::endl;
}

structure_cours::~structure_cours()
{
//	std::cout<<"Destruction de l'objet strucutre cours"<<std::endl;
}

void structure_cours::add_main_sheet_line( ods::Book& book, matiere * tree, ods::Row * row, int level, ods::Cell* previous)
{
	if(tree->sheet_created_){
        //std::cout<<" return "<<std::endl;
        return;
	}

	//tree->sheet_created_ = true;

   //qDebug()<<"La matiere "<< tree->alias_<<" a "<< tree->dads_.size()<<" parents";
	auto *style = book.CreateCellStyle();
	style->SetBold(true);
	style->SetHAlignment(ods::HAlign::Center);
    auto *border = new ods::style::Border();


    //std::cout<<  tree->alias_.toStdString() <<" cellule de début = "<< tree->col_debut_<<std::endl;
    ods::Cell * cell;

    if (tree->tree_level_ < level)
    {
        //std::cout<<" < "<<std::endl;
        if (level == max_level_)
            border->sides_set(ods::BorderSideBottom | ods::BorderSideLeft |ods::BorderSideRight);
        else
            border->sides_set(ods::BorderSideLeft | ods::BorderSideRight);

        style->SetBorder(border);
        cell = row->CreateCell(tree->col_debut_);
//        std::cout<<tree->alias_.toStdString()<<"  une seule cellule "<< tree->col_debut_ <<std::endl;
        cell->SetValue( tree->alias_);
        cell->SetStyle(style);
    }else if (tree->tree_level_ == level)
    {
        //std::cout<<" == "<<std::endl;
        if (level == max_level_)
            border->sides_set(ods::BorderSideTop | ods::BorderSideBottom | ods::BorderSideLeft |ods::BorderSideRight);
        else
            border->sides_set(ods::BorderSideTop | ods::BorderSideLeft | ods::BorderSideRight);

        style->SetBorder(border);

        cell = row->CreateCell(tree->col_debut_);
        //std::cout<<tree->alias_.toStdString()<<"s'etend sur plusieurs cellules "<< tree->get_nb_dep() <<std::endl;

        if (tree->col_fin_ - tree->col_debut_ > 0)
        {
            //std::cout<<"We expand cell for "<< tree->alias_.toStdString()<<std::endl;
            cell->SetRowColSpan(1, tree->col_fin_ - tree->col_debut_+1);
        }

        //cell->SetRowColSpan(1, 2);
        cell->SetValue( tree->alias_);
        cell->SetStyle(style);
    }

    for (int i=0;i<tree->dep_matiere_.size();i++)
    {
        if ( tree->dep_matiere_[i]->dads_.size()>0 && tree->dep_matiere_[i]->dads_[0]->alias_ == tree->alias_)
            add_main_sheet_line(book, tree->dep_matiere_[i],row,level,cell);
    }
//    std::cout<<  tree->alias_.toStdString() <<" cellule de fin = "<< tree->col_fin_<<std::endl;
}

void structure_cours::create_project( const QString & cours_xml,
                                    const QString& student_ods,
                                    const QString& output)
{
    //std::cout<<" Creating project"<<std::endl;
    if (read_project())
    {
        std::cerr<<"Error the project already exists you cannot create  a new one here"<<std::endl;
        exit(0);
    }

    //std::cout<<" Actually create project"<<std::endl;
	QDomDocument document;
	QFile file(PROJECT_NAME);

	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open the file for writing.";
		qDebug() << "You must create a project please refer to : GestionNote help";
		exit(0);
	}

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();

	xmlWriter.writeStartElement("GestionNoteProject");

	xmlWriter.writeTextElement("cours", cours_xml );
	xmlWriter.writeTextElement("students", student_ods);
	xmlWriter.writeTextElement("output", output);
	xmlWriter.writeTextElement("referent", "");
	xmlWriter.writeTextElement("referent_email", "");

	xmlWriter.writeEndElement();
    file.close();

    cours_xml_ = cours_xml;
	student_ods_ = student_ods;
	output_ = output;

	read_student(student_ods_);
	read_xml(cours_xml);

    create_files();
}

void structure_cours::create_files()
{
    ods::Book book_master;
	auto *style_center = book_master.CreateCellStyle();
	style_center->SetHAlignment(ods::HAlign::Center);

    main_sheet_ = book_master.CreateSheet(output_);
	qDebug()<<"Creating subsheets";
    create_sub_sheet( book_master,tree_matiere_ );
	qDebug()<<"Creating main sheet";
    create_main_sheet( book_master  );
	qDebug()<<"Create main sheet done";
	auto path = output_ + ".ods";
	QFile target(path);
	QString err = book_master.Save(target);
	if (!err.isEmpty())
		qDebug() << "Error saving ods file:" << err;
	else
		qDebug() << "Saved to" << target.fileName();

    // create individual sheet
    int dummy = system("mkdir notes mem_ods");

    for (int i=0;i<liste_cours.size();i++)
    {
        if (liste_cours[i].dep_matiere_.size() == 0)
        {
            ods::Book book;
            create_sub_sheet( book,&liste_cours[i] );
            auto path = "notes/" +liste_cours[i].alias_ + ".ods";
            QFile target(path);
            QString err = book.Save(target);
            if (!err.isEmpty())
                qDebug() << "Error saving ods file:" << err;
            else
                qDebug() << "Saved to" << target.fileName();
        }
    }
}

void structure_cours::create_main_sheet(  ods::Book& book)
{
    /*/ writing columns*/
    int start = 0;
	auto *style = book.CreateCellStyle();
	style->SetBold(true);
	style->SetHAlignment(ods::HAlign::Center);
	style->SetBackgroundColor(QColor(255,255,0,100));
	//style->SetBorder(ods::BorderSideAll);

    auto *border = new ods::style::Border();
	border->sides_set(ods::BorderSideBottom | ods::BorderSideTop | ods::BorderSideLeft | ods::BorderSideRight);
	style->SetBorder(border);
//	style->SetFontSize(18);

    unsigned int max_level_ = get_biggest_level();
    //std::cout<<"max_level_ = "<< max_level_ <<std::endl;
    for (int i=0;i<max_level_+1;i++)
    {
        //std::cout<<"line "<< i<<" / "<< max_level_<<std::endl;
        auto *row = main_sheet_->CreateRow(start++);
        auto* cell = row->CreateCell(2);
        add_main_sheet_line( book,tree_matiere_, row, i,cell);
    }


    auto *row = main_sheet_->CreateRow(start++);
    row = main_sheet_->CreateRow(start++);
    auto *cell = row->CreateCell(0);
    cell->SetValue("Nom ");
    cell->SetStyle(style);
    cell = row->CreateCell(1);
    cell->SetValue("Prenom ");
    cell->SetStyle(style);
    cell = row->CreateCell(2);
    cell->SetValue("Option ");
    cell->SetStyle(style);


    auto *style_center = book.CreateCellStyle();
    style_center->SetHAlignment(ods::HAlign::Center);
    ods::Row* row_start= row;
    ods::Row* row_end= row;

    for (int i=0;i<liste_etudiant.size();i++)
    {
        student& e = liste_etudiant[i];
		qDebug()<<"student = "<< e.name_ ;
        row = main_sheet_->CreateRow(start++);
        if (i==0)
            row_start = row;
        row_end = row;

        cell = row->CreateCell(0);
        cell->SetValue(e.name_);
        cell = row->CreateCell(1);
        cell->SetValue(e.first_name_);
        cell = row->CreateCell(2);
        cell->SetValue(e.option_);

        cell->SetStyle(style_center);

        for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
        {
            cell = row->CreateCell(i);
            QString current_matiere = get_matiere_from_col(i);
            place p = e.get_dep_cell( current_matiere );
            if (p.row != -1)
            {
                auto* sheet_dep = book.sheet(current_matiere);
                auto *row_dep = sheet_dep->row(p.row);
                if (row_dep)
                {
                    auto *cell_dep = row_dep->cell(p.col);
                    if(!cell_dep)   cell_dep  =  row_dep->CreateCell(p.col);
                    auto *formula1 = new ods::Formula();
                    formula1->Add(sheet_dep, cell_dep );
                    cell->SetFormula(formula1);
                }
                cell->SetStyle(style_center);
            }else
            {
                cell->SetValue(" ");
            }
        }
    }
    row = main_sheet_->CreateRow(start++);
    row = main_sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MOYENNE ");
    cell->SetStyle(style_center);
    for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
    {
        cell = row->CreateCell(i);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(i);
        auto* cell2 = row_end->cell(i);
        formula->Special("AVERAGE",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = main_sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MAX ");
    cell->SetStyle(style_center);
    for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
    {
        cell = row->CreateCell(i);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(i);
        auto* cell2 = row_end->cell(i);
        formula->Special("MAx",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = main_sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MIN ");
    cell->SetStyle(style_center);
    for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
    {
        cell = row->CreateCell(i);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(i);
        auto* cell2 = row_end->cell(i);
        formula->Special("MIN",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = main_sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("ECART TYPE ");
    cell->SetStyle(style_center);
    for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
    {
        cell = row->CreateCell(i);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(i);
        auto* cell2 = row_end->cell(i);
        formula->Special("STDEV",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = main_sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MEDIANE ");
    cell->SetStyle(style_center);
    for(unsigned int i=tree_matiere_->col_debut_;i<tree_matiere_->col_fin_+1;i++)
    {
        cell = row->CreateCell(i);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(i);
        auto* cell2 = row_end->cell(i);
        formula->Special("MEDIAN",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }
}

void structure_cours::create_sub_sheet(ods::Book & book, matiere * m )
{
    //std::cout<<" Creating sheet for "<< m->alias_.toStdString()<<std::endl;
    auto *style_center = book.CreateCellStyle();
    style_center->SetHAlignment(ods::HAlign::Center);

    m->sheet_ = book.CreateSheet(m->alias_);
    // create the sheet of the child
    for (int i=0;i< m->dep_matiere_.size();i++)
    {
        create_sub_sheet(book, m->dep_matiere_[i]);
    }

    // write info on the head of the page
    int start = 0;
    // set name
    auto  *row = m->sheet_->CreateRow(start++);
    auto *cell = row->CreateCell(0);
    cell->SetValue("Matiere : ");
    cell = row->CreateCell(1);
    cell->SetValue( m->name_);

    // set alias :
    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(0);
    cell->SetValue("Alias : ");
    cell = row->CreateCell(1);
    cell->SetValue( m->alias_);

    // set professor :
    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(0);
    cell->SetValue("Responsable : ");
    cell = row->CreateCell(1);
    cell->SetValue( m->referent_);

    // set option :
    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(0);
    cell->SetValue("Option : ");
    cell = row->CreateCell(1);
    cell->SetValue( m->option_);

    // set apogee :
    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(0);
    cell->SetValue("Apogee : ");
    cell = row->CreateCell(1);
    cell->SetValue( m->apogee_);

    row = m->sheet_->CreateRow(start++);
    row = m->sheet_->CreateRow(start++);
    // set the dependancy
    cell = row->CreateCell(3);
    cell->SetValue(m->alias_);
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size();k++)
    {
        cell = row->CreateCell(4+k);
        cell->SetValue(m->dep_matiere_[k]->alias_);
        cell->SetStyle(style_center);
    }


    row = m->sheet_->CreateRow(start++);
    auto *row_coeff = row;
    cell = row->CreateCell(0);
    cell->SetValue("Nom ");
    cell = row->CreateCell(1);
    cell->SetValue("Prenom ");
    cell = row->CreateCell(2);
    cell->SetValue("Option ");
    cell->SetStyle(style_center);

     for (int k=0;k<m->dep_matiere_.size();k++)
    {
        cell = row->CreateCell(4+k);
        cell->SetValue(m->coeff_[k]);
        cell->SetStyle(style_center);
    }

    auto* row_start = row;
    auto* row_end = row;
    bool first = true;
    first_row_ = start;
    //std::cout<<" first_row_ = "<< first_row_ <<std::endl;
    for (int i=0;i<liste_etudiant.size();i++)
    {
        student& e = liste_etudiant[i];
        //qDebug()<<"Working on student "<< e.name_;
        if (m->option_ == "all" || m->option_ == e.option_ )
        {
            row = m->sheet_->CreateRow(start++);
            if (first)
            {
                row_start = row;
                first = false;
            }

            row_end = row;
            cell = row->CreateCell(0);
            cell->SetValue(e.name_);
            cell = row->CreateCell(1);
            cell->SetValue(e.first_name_);
            cell = row->CreateCell(2);
            cell->SetValue(e.option_);
            cell->SetStyle(style_center);

            cell = row->CreateCell(3);
            cell->SetStyle(style_center);
            if (m->dep_matiere_.size()>0)
            {
                //std::cout<<"in the loop"<<std::endl;
                // mise en place de l'équation
                auto *formula = new ods::Formula();
                formula->Add(ods::Grouping::Open);
                for (int k=0;k<m->dep_matiere_.size();k++)
                    if (m->dep_matiere_[k]->option_ == "all" || m->dep_matiere_[k]->option_ == e.option_ )
                    {
                        if (k!=0)
                            formula->Add(ods::Op::Add);

                        ods::Cell* cell_coeff = row_coeff->cell(4+k);
                        ods::Cell* cell_value = row->CreateCell(4+k);
                        formula->Add(cell_coeff);
                        formula->Add(ods::Op::Mult);
                        formula->Add(cell_value);
                    }else
                    {
                        ods::Cell* cell = row->CreateCell(4+k);
                        cell->SetValue(" ");

                    }
                formula->Add(ods::Grouping::Close);
                formula->Add(ods::Op::Divide);
                formula->Add(ods::Grouping::Open);
                for (int k=0;k<m->dep_matiere_.size();k++)   if (m->dep_matiere_[k]->option_ == "all" || m->dep_matiere_[k]->option_ == e.option_ )
                {
                    if (k!=0)
                        formula->Add(ods::Op::Add);

                    ods::Cell* cell_coeff = row_coeff->cell(4+k);
                    formula->Add(cell_coeff);

                }
                formula->Add(ods::Grouping::Close);
                cell->SetFormula(formula);
                cell->SetStyle(style_center);

                // copie des feuilles précédentes
                for (int k=0;k<m->dep_matiere_.size();k++)
                {
                    cell = row->cell(4+k);
                    cell->SetStyle(style_center);
                    if (m->dep_matiere_[k]->option_ == "all" || m->dep_matiere_[k]->option_ == e.option_ )
                    {
                        if (e.get_dep(m->dep_matiere_[k]->alias_))
                        {
                            place p = e.get_dep_cell( m->dep_matiere_[k]->alias_);
                            auto* sheet_dep = book.sheet(m->dep_matiere_[k]->alias_);
                            auto *row_dep = sheet_dep->row(p.row);
                            if (row_dep)
                            {
                                auto *cell_dep = row_dep->cell(p.col);
                                if(!cell_dep)   cell_dep  =  row_dep->CreateCell(p.col);
                                auto *formula1 = new ods::Formula();
                                formula1->Add(sheet_dep, cell_dep );
                                cell->SetFormula(formula1);
                            }
                            cell->SetValue(m->dep_matiere_[k]->alias_);
                        }
                    }
                }
            }

            //qDebug()<<" matiere = "<< m->alias_<<"  student = "<< e.name_;
            // save value of current sheet;
            place p;
            p.col = 3;
            p.row = start-1;
            e.set_cell( p , m->sheet_, m->alias_);
        }
    }

    row = m->sheet_->CreateRow(start++);
    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MOYENNE ");
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size()+1;k++)
    {
        cell = row->CreateCell(3+k);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(3+k);
        auto* cell2 = row_end->cell(3+k);
        if( cell1 != cell2)
            formula->Special("AVERAGE",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MAX ");
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size()+1;k++)
    {
        cell = row->CreateCell(3+k);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(3+k);
        auto* cell2 = row_end->cell(3+k);
        if( cell1 != cell2)
            formula->Special("MAX",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MIN ");
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size()+1;k++)
    {
        cell = row->CreateCell(3+k);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(3+k);
        auto* cell2 = row_end->cell(3+k);
        if( cell1 != cell2)
            formula->Special("MIN",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("ECART TYPE ");
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size()+1;k++)
    {
        cell = row->CreateCell(3+k);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(3+k);
        auto* cell2 = row_end->cell(3+k);
        if( cell1 != cell2)
            formula->Special("STDEV",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }

    row = m->sheet_->CreateRow(start++);
    cell = row->CreateCell(2);
    cell->SetValue("MEDIANE");
    cell->SetStyle(style_center);

    for (int k=0;k<m->dep_matiere_.size()+1;k++)
    {
        cell = row->CreateCell(3+k);
        auto *formula = new ods::Formula();
        auto* cell1 = row_start->cell(3+k);
        auto* cell2 = row_end->cell(3+k);
        if( cell1 != cell2)
            formula->Special("MEDIAN",cell1, cell2 );
        cell->SetFormula(formula);
        cell->SetStyle(style_center);
    }
}

QString structure_cours::get_matiere_from_col(unsigned int in)
{
    for (int i=0;i<liste_cours.size();i++)
        if(liste_cours[i].col_debut_ == in)
            return liste_cours[i].alias_;

    return "not_found";
}

bool structure_cours::find_cell(int * COL, int * ROW, QString student, QString alias, ods::Sheet * s)
{
//    qDebug()<<" find_cell "<< student <<" "<< alias;
    int cs, rs, ca, ra;
    if (!find_cell(&cs,&rs,student,s,1))
    {
        std::cerr<<"Error cannot find "<< student.toStdString()<<std::endl;
        return false;
    }
//    std::cout<<" looking for "<<alias.toStdString()<<std::endl;
    if (!find_cell(&ca,&ra,alias,s,2))
    {
        std::cerr<<"Error cannot find "<< alias.toStdString()<<std::endl;
        return false;
    }
    *COL = ca;
    *ROW = rs;
//    std::cout<<" col ="<< ca <<"\t row = "<<rs <<"\t\t"<<ra<<" "<< cs <<std::endl;
    return true;
}

bool  structure_cours::find_cell(int * COL, int * ROW, QString name,QString ods_file, int count)
{
	name.trimmed();
	QFile file(ods_file);
	if (!file.exists())
	{
		qDebug() << "No such file: " << ods_file;
		return false;
	}
	ods::Book book_in(ods_file);
	auto *sheet_in = book_in.sheet(0);
	return find_cell(COL,ROW,name,sheet_in,count);
}

bool  structure_cours::find_cell(int * COL, int * ROW, QString name,ods::Sheet * sheet_in, int count)
{
    if (sheet_in == nullptr)
	{
		qDebug() << "No sheet at 0";
		return false;
	}
    int cpt = 0;
	ods::Cell *cell_read = nullptr;
	for (int i=0;i<100;i++)
	{
		auto *row = sheet_in->row(i);
		if (row == nullptr)
		{
			break;
		}

		for (int j=0;j<100;j++)
		{
			cell_read = row->cell(j);
			if (cell_read == nullptr) {
// 				qDebug() << "No cell at " << QString::number(0);
				break;
			}
			const ods::Value &value = cell_read->value();
			if (value.IsString())
			{
				QString value_as_string = *value.AsString();
				if (value_as_string == name)
				{
					*COL = j;
					*ROW = i;
					cpt++;
					if (cpt  == count)
                        return true;
				}
			}
		}
	}
	return false;
}

unsigned int structure_cours::get_biggest_level()
{
	unsigned int max = 0;
	for (int i=0;i<liste_cours.size();i++)
    {
        //std::cout<<"liste_cours["<<i<<"].tree_level_ = "<< liste_cours[i].tree_level_ <<std::endl;
        //std::cout<<" max = "<<max <<std::endl;
		if (liste_cours[i].tree_level_ > max)
		{
		    //std::cout<<"modif max = "<<max <<std::endl;
			max = liste_cours[i].tree_level_;
		}
	}
	//std::cout<<" max = "<<max <<std::endl;
	return max;
}

bool structure_cours::get_cell_value( ods::Sheet * sheet, const place & p, double * out)
{
    //std::cout<<" row = "<<p.row<<"  col = "<< p.col<<std::endl;
    auto * row = sheet->row(p.row);
    auto* cell = row->cell(p.col);

    if (cell->HasFormula())
    {
        auto* f = cell->formula();
        f->UpdateValue();
        const auto &value = cell->value();

        if (value.IsDouble())
        {
            //std::cout<<" read formula value ="<<*value.AsDouble()<<std::endl;
            *out =  *value.AsDouble();
            return true;
        }
    }
    {
        const ods::Value &value =  cell->value();
        if (value.IsDouble())
        {
         //   std::cout<<" read value"<<std::endl;
            *out =  *value.AsDouble();
            return true;
        }
    }
    *out = 0;
    return false;
}

matiere* structure_cours::get_master_of_tree()
{
    //std::cout<<"get master of tree"<<std::endl;
    int nb = liste_cours.size();
    std::vector<bool> might_be_master(nb);
    for (int i=0;i<nb;i++)
        might_be_master[i] = true;

    for (int i=0;i<nb;i++)
    {
        //check the dependancy
        for (int j=0;j<liste_cours[i].dep_matiere_name_.size();j++)
        {
            bool find_dep = false;
            // find the id of the dep_matiere
            for (int k=0;k<nb;k++)
            {
                // std::cout<<"matiere = "<<liste_cours[i].dep_matiere_name_[j].toStdString()<<"  "<< liste_cours[k].name_.toStdString()<<std::endl;
                if(liste_cours[i].dep_matiere_name_[j] == liste_cours[k].name_ || liste_cours[i].dep_matiere_name_[j] == liste_cours[k].alias_)
                {
                    might_be_master[k] = false;
                    liste_cours[i].dep_matiere_.push_back( & liste_cours[k] );
                    find_dep = true;
                    //std::cout<<"found !!"<<std::endl;
                    break;
                }
            }
            if (!find_dep)
            {
                std::cerr<<" Error in the xml "<< liste_cours[i].name_.toStdString()<<" cannot find dependancy : "<< liste_cours[i].dep_matiere_name_[j].toStdString()<<std::endl;
                exit(0);
            }
        }
    }

    int id = -1;
    for (int i=0;i<nb;i++)
    {
        if ( might_be_master[i] && id == -1)
            id = i;
        else if ( might_be_master[i] && id != -1)
        {
            std::cerr<<"Error there must be only one master !!"<<std::endl;
            std::cerr<<liste_cours[id].name_.toStdString()<< " and "<< liste_cours[i].name_.toStdString()<<" are two possible candidates."<<std::endl;
            exit(0);
        }
    }
    if (id ==-1)
    {
        std::cerr<<" No root found in the xml" <<std::endl;
        exit(0);
    }

    return &liste_cours[id];
}

void structure_cours::import_note(QString& alias_matiere, const QString & output)
{
    std::ofstream outfile ("send_mail_student.sh");
    QString tmp ;

	if(output == "")
    {
        tmp  = output_;
    }else
    {
        tmp  = output;
    }
    auto path = tmp + ".ods";

	QFile target(path);
    ods::Book book_in(path);
    ods::Book book("./notes/"+alias_matiere+".ods");
    auto *sheet = book.sheet(0);
    for (int i=0;i<liste_etudiant.size();i++)   if(liste_etudiant[i].get_dep(alias_matiere))
    {
        double note_value = 0;
        int c,r;
        find_cell(&c,&r,liste_etudiant[i].name_,alias_matiere, sheet);
        place p;
        p.col = c;
        p.row = r;
        if( !get_cell_value( sheet, p, &note_value))
        {
            std::cerr<<"Cannot find student "<< liste_etudiant[i].name_.toStdString() <<" in "<< alias_matiere.toStdString()<<std::endl;
            exit(0);
        }
        //std::cout<<"La note de "<< alias_matiere.toStdString()<<" de "<< liste_etudiant[i].name_.toStdString()<<" est de "<< note_value<<std::endl;

        set_notes( liste_etudiant[i], alias_matiere, book_in,note_value);
        liste_etudiant[i].mail_notes(outfile, referent_, email_,tree_matiere_);
    }
    QString err = book_in.Save(target);
	if (!err.isEmpty())
		qDebug() << "Error saving ods file:" << err;
	else
		qDebug() << "Saved to" << target.fileName();

    outfile.close();
}

void structure_cours::print_cours()
{
    for (int i=0;i<liste_cours.size();i++)  if(liste_cours[i].dep_matiere_.size() == 0)
        std::cout<<" "<< liste_cours[i].alias_.toStdString();
    std::cout<<std::endl;
}

void structure_cours::print_students()
{
    for (int i=0;i<liste_etudiant.size();i++)
    {
        liste_etudiant[i].print();
    }
}

void structure_cours::print_students_name()
{
    for (int i=0;i<liste_etudiant.size();i++)
    {
        std::cout<<liste_etudiant[i].name_.toStdString()<<" ";
    }
    std::cout<<std::endl;
}

void structure_cours::print_profs()
{
    for (int i=0;i<liste_profs.size();i++)
    {
        liste_profs[i].print();
    }
}

void structure_cours::print_tree()
{
    //std::cout<<" structure_cours::print_tree()"<<std::endl;
    if (tree_matiere_)
        tree_matiere_->print();
    else{
        std::cerr<<" Error the tree is not defined yet"<<std::endl;
        exit(0);
    }
}

void structure_cours::read_ods()
{
    std::string command = "cp "+output_.toStdString()+".ods mem_ods/" + output_.toStdString() +"_"+currentDateTime()+".ods";
    int dummy = system(command.c_str());
	QFile file(output_+".ods");
	if (!file.exists())
	{
		qDebug() << "No such file: " << output_+".ods";
		return;
	}
	ods::Book book_in(output_+".ods");
	for (int i=0;i<liste_etudiant.size();i++)
    {
        student &e = liste_etudiant[i];
        for (int j=0;j<liste_cours.size();j++)
        {
            matiere &m = liste_cours[j];
            read_notes( e, m, book_in);
        }
    }
}

void structure_cours::read_notes(student &e , matiere & m, ods::Book & book_in)
{
    //std::cout<<"read_notes debut  m alias  "<<  m.alias_.toStdString()<<"  option = "<< m.option_.toStdString()<<" "<< e.option_.toStdString()<<std::endl;
    if ( m.option_ == "all" || m.option_ == e.option_ ) // )&& m.dep_matiere_.size() == 0)
    {
        //std::cout<<" Searching for cell of "<< m.alias_.toStdString()<<" student "<< e.name_.toStdString()<<std::endl;
        auto *sheet = book_in.sheet(m.alias_);
        int c,r;
        if ( !find_cell(&c,&r, e.name_,m.alias_,sheet))
        {
            std::cerr<<"Cannot find the student "<< e.name_.toStdString()<<" in "<< m.alias_.toStdString()<<std::endl;
            exit(0);
        }

        place p;
        p.col = c;
        p.row = r;

        note n;
        n.name = m.alias_;
        n.sheet = sheet;
        n.cell = p;
//        std::cout<<"Youhou col = "<< c<<" row = "<< r <<std::endl;

        n.defined = get_cell_value( sheet, p, & (n.value));
//        std::cout<<"col = "<< c<<" row = "<< r <<"  value = "<< n.value<<std::endl;
        e.notes_.push_back(n);
    }
//    std::cout<<"read_notes fin  m alias  "<<  m.alias_.toStdString()<<"  option = "<< m.option_.toStdString()<<" "<< e.option_.toStdString()<<std::endl;
}

bool structure_cours::read_project()
{
	QDomDocument document;
	QFile file(PROJECT_NAME);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		 qDebug() << "Failed to open the file for reading.";
		 qDebug() << "You must create a project please refer to : GestionNote help";
		return false;
	}
	else
	{	// loading
		if(!document.setContent(&file))
		{
			 qDebug() << "Failed to load the file for reading.";
			return false;
		}
		file.close();
	}
	QDomElement root = document.firstChildElement();
	QDomElement cours = root.namedItem("cours").toElement();
	cours_xml_ = cours.text().trimmed();

	QDomElement students = root.namedItem("students").toElement();
	student_ods_ = students.text().trimmed();

    QDomElement output = root.namedItem("output").toElement();
	output_ = output.text().trimmed();

	QDomElement referent = root.namedItem("referent").toElement();
	referent_ ="";
    if ( !referent.isNull() ) { // We have a <name>..</name> element in the set
        referent_ = referent.text().trimmed();
    }else
    {
        std::cerr<<"You must specify a referent in the xml"<<std::endl;
        exit(0);
    }

    if(referent_=="")
    {
        std::cerr<<"Please edit the file " << PROJECT_NAME<<" and add the referent name"<<std::endl;
        exit(0);
    }

	QDomElement email = root.namedItem("referent_email").toElement();
	email_ ="";
    if ( !email.isNull() ) { // We have a <name>..</name> element in the set
        email_ = email.text().trimmed();
    }
    if(email_=="")
    {
        std::cerr<<"Please edit the file " << PROJECT_NAME<<" and add the email of the referent. found = "<< email_.toStdString()<<std::endl;
        exit(0);
    }
	read_xml(cours_xml_);
	read_student(student_ods_);
	read_ods();
	return true;
}

void structure_cours::read_student(QString ods_file)
{
//	std::cout<<"Start to read the student file."<<std::endl;
	/// read a file

	int nom_col, nom_row;
	int prenom_col, prenom_row;
    int email_col, email_row;
    int option_col, option_row;
    bool find_option;
	if( !find_cell( &nom_col, &nom_row, "NOM",ods_file))
    {
        std::cerr<<" Cannot find column NOM in your file"<<std::endl;
        exit(0);
    }
	if( !find_cell( &prenom_col, &prenom_row, "PRENOM",ods_file) && !find_cell( &prenom_col, &prenom_row, "PRÉNOM",ods_file))
    {
        std::cerr<<" Cannot find column PRENOM in your file"<<std::endl;
        exit(0);
    }
	if( !find_cell( &email_col, &email_row, "MESSAGERIE",ods_file) && !find_cell( &email_col, &email_row, "EMAIL",ods_file))
    {
        std::cerr<<" Cannot find column MESSAGERIE in your file"<<std::endl;
        exit(0);
    }

    find_option = find_cell( &option_col, &option_row, "OPTION",ods_file) || find_cell( &option_col, &option_row, "option",ods_file) || find_cell( &option_col, &option_row, "Option",ods_file);;
//    std::cout<<" find_option = "<< find_option << "  option_col = "<< option_col<<std::endl;
	QFile file(ods_file);
	if (!file.exists())
	{
		qDebug() << "No such file: " << ods_file;
		return;
	}
	ods::Book book_in(ods_file);
	auto *sheet_in = book_in.sheet(0);
	if (sheet_in == nullptr)
	{
		qDebug() << "No sheet at 0";
		return;
	}

	nom_row ++;
	auto *current_row = sheet_in->row(nom_row++);
	ods::Cell *cell_nom = nullptr;
	ods::Cell *cell_prenom = nullptr;
	ods::Cell *cell_mail = nullptr;
	ods::Cell *cell_option = nullptr;

	int nb_etu = 0;
	while(current_row != nullptr)
	{

		cell_nom = current_row->cell(nom_col);
		cell_prenom = current_row->cell(prenom_col);
		cell_mail = current_row->cell(email_col);
		if (find_option)    cell_option = current_row->cell(option_col);
		const ods::Value &value1 = cell_nom->value();
		const ods::Value &value2 = cell_prenom->value();
		const ods::Value &value3 = cell_mail->value();

		if (value1.NoValue())
		{
			break;
		}else
		{
			student tmp;
			nb_etu ++;
			tmp.name_ = *value1.AsString();
			tmp.first_name_ = *value2.AsString();
			tmp.email_ = *value3.AsString();
			tmp.option_ = "not_defined";
			if (find_option)
            {
                const ods::Value &value4 = cell_option->value();
                if( !value4.NoValue())
                    tmp.option_ = *value4.AsString();
            }
			// qDebug()<<tmp.name_ << tmp.first_name_ << tmp.email_;
			liste_etudiant.push_back(tmp);
		}
		current_row = sheet_in->row(nom_row++);
	}
//	std::cout<<"il y a "<< nb_etu <<" etudiants"<<std::endl;
}

void structure_cours::read_xml( QString input)
{
//	qDebug()<<" reading "<< input;
	QDomDocument document;
	QFile file(input);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open the file : "<< input<<" for reading.";
		return;
	}
	else
	{	// loading
	    QString errorStr;
        int errorLine;
        int errorColumn;
		if(!document.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
		{
		     qDebug() << errorStr << " at line "<< errorLine <<" column : "<< errorColumn;
			qDebug() << "Failed to load the file : "<< input <<" for reading.";
			return;
		}
		file.close();
	}
	QDomElement root = document.firstChildElement();

	QDomNodeList nodes = root.elementsByTagName("cours");
// 	qDebug() << "# nodes = " << nodes.count();
	for(int i = 0; i < nodes.count(); i++)
	{
		QDomNode elm = nodes.at(i);
		if(elm.isElement())
		{
			matiere new_cours;
			new_cours.referent_ = "none";
			// lecture du nom
			QDomElement name = elm.namedItem("nom").toElement();
			if ( !name.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.name_ = name.text().trimmed();
			}else
			{
				std::cerr<<"nom not found "<<std::endl;
				exit(0);
			}
// 			qDebug() << new_cours.name_;

			QDomElement alias = elm.namedItem("alias").toElement();
			if ( !alias.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.alias_ = alias.text().trimmed();
			}

            if (new_cours.alias_ == new_cours.name_)
            {
                std::cerr<<"Error the name and the alias must be different : "<< new_cours.alias_.toStdString()<<std::endl;
                exit(0);
            }

			// lecture des dépendances
			QDomElement e = elm.firstChildElement( "dependance" );
			while ( !e.isNull() ) {
				new_cours.dep_matiere_name_.push_back(e.text().trimmed());
				new_cours.coeff_.push_back(e.attribute("coeff").toDouble());
// 				qDebug() <<"coeff = "<< e.attribute("coeff");
// 				qDebug() <<"dependance = "<< e.text().trimmed();
				e = e.nextSiblingElement( "dependance" );
			}

			QDomElement responsable = elm.namedItem("responsable").toElement();
			if ( !responsable.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.referent_ = responsable.text().trimmed();
// 				qDebug() <<"responsable = "<< responsable.text().trimmed();
			}

			QDomElement apogee = elm.namedItem("apogee").toElement();
			if ( !apogee.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.apogee_ = apogee.text().trimmed();
			}

			QDomElement option = elm.namedItem("option").toElement();
			if ( !option.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.option_ = option.text().trimmed();
			}else
				new_cours.option_ = "all";


			QDomElement to_validate = elm.namedItem("to_validate").toElement();
			if ( !to_validate.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.must_validate_ =  ( to_validate.text().trimmed() =="yes" || to_validate.text().trimmed() == "oui");
			}else
				new_cours.must_validate_ = false;

			new_cours.tree_level_ = -1;
//			new_cours.sheet =  nullptr;

			liste_cours.push_back(new_cours);
		}
	}
	tree_matiere_ = get_master_of_tree();
	tree_matiere_->update_dads();
	tree_matiere_->update_level();
	tree_matiere_->update_col(3);

	QDomNodeList prof_node = root.elementsByTagName("prof");
	for(int i = 0; i < prof_node.count(); i++)
	{
		QDomNode elm = prof_node.at(i);
		if(elm.isElement())
		{
			profs next_prof;
			// lecture du nom
			QDomElement name = elm.namedItem("nom").toElement();
			if ( !name.isNull() ) { // We have a <name>..</name> element in the set
				next_prof.name_ = name.text().trimmed();
			}else
			{
				std::cerr<<"nom not found for prof"<<std::endl;
				exit(0);
			}
// 			qDebug() << new_cours.nom;

			QDomElement first_name = elm.namedItem("prenom").toElement();
			if ( !first_name.isNull() ) { // We have a <name>..</name> element in the set
				next_prof.first_name_ = first_name.text().trimmed();
			}else
			{
				std::cerr<<"prenom not found for prof"<<std::endl;
				exit(0);
			}

			QDomElement email = elm.namedItem("email").toElement();
			if ( !email.isNull() ) { // We have a <name>..</name> element in the set
				next_prof.email_ = email.text().trimmed();
// 				qDebug() <<"responsable = "<< responsable.text().trimmed();
			}else
			{
				std::cerr<<"email not found for prof"<<std::endl;
				exit(0);
			}
			liste_profs.push_back(next_prof);
		}
	}

    for (int i=0;i<liste_cours.size();i++)
    {
        if (liste_cours[i].dep_matiere_.size() == 0)
        {
            bool found = false;
            for (int j=0;j<liste_profs.size();j++)
                if( liste_cours[i].referent_ == liste_profs[j].name_)
                {
                    found = true;
                    liste_profs[j].matieres_.push_back(& liste_cours[i]);
                    break;
                }
            if(!found)
            {
                std::cerr<<" Error cannot find the definition of profs "<< liste_cours[i].referent_.toStdString()<<" for "<<  liste_cours[i].alias_.toStdString()<<std::endl;
                exit(0);
            }
        }
    }
//    qDebug()<<" reading "<< input<<"  done";
}

void structure_cours::send_mail_profs()
{
//echo -e "Bonjour Bénédicte Bousset \n Je te joins les fichiers à compléter" | mutt  -s "Fichier pour notes GEA" sebastienlengagne@yahoo.fr -a ./notes/AURO5.ods
    std::ofstream outfile ("send_mail_prof.sh");

    for (int i=0;i<liste_profs.size();i++)  if(liste_profs[i].matieres_.size()>0)
    {
        outfile << "echo \"Bonjour "<<liste_profs[i].first_name_.toStdString()<<" "<< liste_profs[i].name_.toStdString()<<", \n \n";
        outfile <<"Je te joins les fichiers à compléter concernant les matières : \\n";
        for (int j=0;j<liste_profs[i].matieres_.size();j++)
            outfile << liste_profs[i].matieres_[j]->alias_.toStdString()<<" \\n";
        outfile <<"\n Désolé en cas de réception multiple";
        outfile <<"\n \n Bonne journée \n";
        outfile << referent_.toStdString() <<"\n \n";
        outfile <<"---- Mail généré automatiquement par GestionNote :  https://github.com/lengagne/GestionNote ---\n ";
        outfile << " \" | mutt ";
        outfile<<" -s \"[GE4A] Fichier pour notes GE4A\" -c "<<email_.toStdString() <<" "<< liste_profs[i].email_.toStdString()<<" ";
        for (int j=0;j<liste_profs[i].matieres_.size();j++)
            outfile << " -a ./notes/"<< liste_profs[i].matieres_[j]->alias_.toStdString()<<".ods";
        outfile<<"  "<<std::endl<<std::endl;
    }
    outfile.close();
}

void structure_cours::send_mail_student(QString & name)
{
    std::ofstream outfile ("send_mail_student.sh");
    for (int i = 0;i<liste_etudiant.size();i++)
    {
        if (liste_etudiant[i].name_ == name)
        {
            liste_etudiant[i].mail_notes(outfile, referent_, email_,tree_matiere_);
            break;
        }
    }
    outfile.close();
}

void structure_cours::send_mail_students( const std::string & alias)
{
    std::ofstream outfile ("send_mail_student.sh");
    for (int i = 0;i<liste_etudiant.size();i++) if (liste_etudiant[i].get_dep(QString::fromUtf8(alias.c_str())) || alias =="")
    {
        liste_etudiant[i].mail_notes(outfile, referent_, email_, tree_matiere_, alias);
    }
    outfile.close();
}

void structure_cours::set_notes(student &e , QString &alias, ods::Book & book_in, double note_value)
{
    // std::cout<<" Searching for cell of "<< m.alias_.toStdString()<<std::endl;
    auto *sheet = book_in.sheet(alias);
    int c,r;
    if ( !find_cell(&c,&r, e.name_,alias,sheet))
    {
        std::cerr<<"Cannot find the student "<< e.name_.toStdString()<<" in "<< alias.toStdString()<<std::endl;
        exit(0);
    }

    for (int i=0;i<e.notes_.size();i++) if(e.notes_[i].name == alias)
    {
        e.notes_[i].value = note_value;
        e.notes_[i].defined = true;
        break;
    }
    auto* row = sheet->row(r);
    auto* cell = row->cell(c);
    cell->SetValue(note_value);
    sheet->PreSave();
}

void structure_cours::show_rattrapage()
{
    std::vector<student> etudiants_rattrapage;
    for (int i=0;i<liste_etudiant.size();i++)
    {
        for (int j=0;j<liste_cours.size();j++)
        {
            if ( ( liste_cours[j].option_ == liste_etudiant[i].option_  || liste_cours[j].option_ == "all" )&& liste_cours[j].must_validate_ )
            {
                if ( liste_etudiant[i].get_note(liste_cours[j].alias_) < 10 )
                {
                    // check already in the list
                    bool test = true;
                    for (int k=0;k<etudiants_rattrapage.size();k++)
                    {
                        if(etudiants_rattrapage[k].name_ == liste_etudiant[i].name_)
                        {
                            test = false;
                            break;
                        }
                    }
                    if (test)
                    {
                        for (int k=0;k<liste_etudiant[i].notes_.size();k++)
                            liste_etudiant[i].notes_[k].cell.row = first_row_ + 1+etudiants_rattrapage.size();
                        etudiants_rattrapage.push_back(liste_etudiant[i]);
                    }
                    qDebug()<<liste_etudiant[i].name_<<" doit rattraper : "<< liste_cours[j].name_<< " car il a "<< liste_etudiant[i].get_note(liste_cours[j].alias_);
                }
            }
        }
    }

//    liste_etudiant = etudiants_rattrapage;
//
//    // create the file rattrapage
//    ods::Book book_master;
//	auto *style_center = book_master.CreateCellStyle();
//	style_center->SetHAlignment(ods::HAlign::Center);
//
//    main_sheet_ = book_master.CreateSheet(output_+ "_rattrapage.ods");
//    create_sub_sheet( book_master,tree_matiere_ );
//    create_main_sheet( book_master  );
//
//	auto path = output_ + "_rattrapage.ods";
//	QFile target(path);
//	QString err = book_master.Save(target);
//	if (!err.isEmpty())
//		qDebug() << "Error saving ods file:" << err;
//	else
//		qDebug() << "Saved to" << target.fileName();

}
