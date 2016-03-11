#include "structure_cours.hpp"
#define START_LINE 5
#define START_COL 5

#define PROJECT_NAME "GestionNoteProject.xml"

structure_cours::structure_cours()
{
	std::cout<<"Creation de l'objet strucutre cours"<<std::endl;

}

structure_cours::~structure_cours()
{
	std::cout<<"Destruction de l'objet strucutre cours"<<std::endl;
}

void structure_cours::add_main_sheet_line( matiere * tree, ods::Row * row, int level, ods::Cell* previous)
{
    qDebug()<<"La matiere "<< tree->alias_<<" a "<< tree->dads_.size()<<" parents";
	auto *style = book_master_.CreateCellStyle();
	style->SetBold(true);
	style->SetHAlignment(ods::HAlign::Center);
//	style->SetBackgroundColor(QColor(255,255,0,100));

    auto *border = new ods::style::Border();
	border->sides_set(ods::BorderSideTop | ods::BorderSideLeft | ods::BorderSideRight);
	style->SetBorder(border);

    std::cout<<  tree->alias_.toStdString() <<" cellule de début = "<< tree->col_debut_<<std::endl;
    ods::Cell * cell;

    if (tree->tree_level_ < level)
    {
        cell = row->CreateCell(tree->col_debut_);
        std::cout<<tree->alias_.toStdString()<<"  une seule cellule "<< tree->col_debut_ <<std::endl;
        cell->SetValue( tree->alias_);
        cell->SetStyle(style);
    }else if (tree->tree_level_ == level)
    {
        cell = row->CreateCell(tree->col_debut_);
        std::cout<<tree->alias_.toStdString()<<"  plusieurs cellules "<< tree->get_nb_dep() <<std::endl;
        //if (tree->alias_ == "MIE")
//            cell->SetRowColSpan(1, 2);
        if (tree->col_fin_ - tree->col_debut_ > 0)
        {
            std::cout<<"We expand cell for "<< tree->alias_.toStdString()<<std::endl;
            cell->SetRowColSpan(1, tree->col_fin_ - tree->col_debut_+1);
        }

        //cell->SetRowColSpan(1, 2);
        cell->SetValue( tree->alias_);
        cell->SetStyle(style);
    }

    for (int i=0;i<tree->dep_matiere_.size();i++)
    {
        if ( tree->dep_matiere_[i]->dads_.size()>0 && tree->dep_matiere_[i]->dads_[0]->alias_ == tree->alias_)
            add_main_sheet_line(tree->dep_matiere_[i],row,level,cell);
    }
    std::cout<<  tree->alias_.toStdString() <<" cellule de fin = "<< tree->col_fin_<<std::endl;

}

void structure_cours::create_project( const QString & cours_xml,
                                    const QString& student_ods,
                                    const QString& output)
{
    std::cout<<" Creating project"<<std::endl;
    if (read_project())
    {
        std::cerr<<"Error the project already exists you cannot create  a new one here"<<std::endl;
        exit(0);
    }

    std::cout<<" Actually create project"<<std::endl;
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
	auto *style_center = book_master_.CreateCellStyle();
	style_center->SetHAlignment(ods::HAlign::Center);

    main_sheet_ = book_master_.CreateSheet(output_);
    create_sub_sheet( tree_matiere_ );
    create_main_sheet(  );
/*
	int max_level = get_biggest_level();
// 	std::cout<<"max_level = "<< max_level <<std::endl;
	for (int i=0;i<max_level+1;i++)
	{
		for (int j=0;j<liste_cours.size();j++)	if (liste_cours[j].tree_level_ == i)
		{
			liste_cours[j].sheet = book_master_.CreateSheet(liste_cours[j].alias);

			// ajouter les etudiants
			int start=START_LINE;
			auto *row = liste_cours[j].sheet->CreateRow(start++); // create first/top row
			int main_it = START_COL;
			auto *cell = row->CreateCell(main_it);
			cell->SetValue(liste_cours[j].alias);
			cell->SetStyle(style_center);

			for (int k=0;k<liste_cours[j].dependance.size();k++)
			{
				cell = row->CreateCell(main_it+1+k);
				cell->SetValue(liste_cours[j].dependance[k]);
				cell->SetStyle(style_center);
			}


			row = liste_cours[j].sheet->CreateRow(start++); // create first/top row
			cell = row->CreateCell(0);
			cell->SetRowColSpan(1, 2);
			cell->SetValue("NOM");
			cell = row->CreateCell(2);
			cell->SetRowColSpan(1, 2);
			cell->SetValue("PRENOM");
			cell = row->CreateCell(4);
			cell->SetValue("OPTION");
			cell->SetStyle(style_center);


			auto *row_coeff = row;
			for (int k=0;k<liste_cours[j].dependance.size();k++)
			{
				cell = row_coeff->CreateCell(6+k);
				cell->SetValue(liste_cours[j].coeff[k]);
				cell->SetStyle(style_center);
			}

			for (int k=0;k<liste_etudiant.size();k++)
			{
				row = liste_cours[j].sheet->CreateRow(start++); // create first/top row
				cell = row->CreateCell(0);
				cell->SetRowColSpan(1, 2);
				cell->SetValue(liste_etudiant[k].nom);
				cell = row->CreateCell(2);
				cell->SetRowColSpan(1, 2);
				cell->SetValue(liste_etudiant[k].prenom);
				cell = row->CreateCell(4);
				cell->SetValue(liste_etudiant[k].option);
				cell->SetStyle(style_center);

				cell = row->CreateCell(5);
				if(liste_cours[j].dependance.size())
				{
					auto *formula = new ods::Formula();
					formula->Add(ods::Grouping::Open);
					for (int k=0;k<liste_cours[j].dependance.size();k++)
					{
						ods::Cell* cell_coeff = row_coeff->cell(6+k);
						ods::Cell* cell_value = row->CreateCell(6+k);
						formula->Add(cell_coeff);
						formula->Add(ods::Op::Mult);
						formula->Add(cell_value);

						if (k!=liste_cours[j].dependance.size()-1)
							formula->Add(ods::Op::Add);
					}
					formula->Add(ods::Grouping::Close);
					formula->Add(ods::Op::Divide);
					formula->Add(ods::Grouping::Open);
					for (int k=0;k<liste_cours[j].dependance.size();k++)
					{
						ods::Cell* cell_coeff = row_coeff->cell(6+k);
						formula->Add(cell_coeff);
						if (k!=liste_cours[j].dependance.size()-1)
							formula->Add(ods::Op::Add);
					}
					formula->Add(ods::Grouping::Close);
					cell->SetFormula(formula);
				}else
				{
					cell->SetValue(0.0);
				}
				cell->SetStyle(style_center);
			}
		}
	}

	for (int j=0;j<liste_cours.size();j++)
	{
		for (int k=0;k<liste_cours[j].dependance.size();k++)
		{
			auto *dep_sheet = book_master_.sheet(liste_cours[j].dependance[k]);
			for (int kk=0;kk<liste_etudiant.size();kk++)
			{
				auto* dep_row = dep_sheet->row(START_LINE+2+kk);
				auto* dep_cell = dep_row->cell(START_COL);

				auto *sheet = book_master_.sheet(liste_cours[j].alias);
				auto *row = sheet->row(START_LINE+2+kk);
				auto *cell = row->cell(START_COL+1+k);
				auto *formula = new ods::Formula();
				formula->Add(dep_sheet,dep_cell);
				cell->SetFormula(formula);
				cell->SetStyle(style_center);
			}
		}
	}

*/
	auto path = output_ + ".ods";
	QFile target(path);
	QString err = book_master_.Save(target);
	if (!err.isEmpty())
		qDebug() << "Error saving ods file:" << err;
	else
		qDebug() << "Saved to" << target.fileName();

/*	// create prof files
	for (int i=0;i<liste_prof.size();i++)
	{
		ods::Book book;
		for (int j=0;j<liste_prof[i].matiere.size();j++)
			ods::Sheet *sheet = book.CreateSheet(liste_prof[i].matiere[j]);


		auto path = liste_prof[i].nom + ".ods";
		QFile target(path);
		QString err = book.Save(target);
		if (!err.isEmpty())
			qDebug() << "Error saving ods file:" << err;
		else
			qDebug() << "Saved to" << target.fileName();
	}
*/
}

void structure_cours::create_main_sheet(  )
{
    /*/ writing columns*/
    int start = 0;
	auto *style = book_master_.CreateCellStyle();
	style->SetBold(true);
	style->SetHAlignment(ods::HAlign::Center);
	style->SetBackgroundColor(QColor(255,255,0,100));
	//style->SetBorder(ods::BorderSideAll);

    auto *border = new ods::style::Border();
	border->sides_set(ods::BorderSideTop | ods::BorderSideLeft | ods::BorderSideRight);
	style->SetBorder(border);
//	style->SetFontSize(18);

    unsigned int nbm = get_biggest_level();
    std::cout<<"nbm = "<< nbm <<std::endl;
    for (int i=0;i<nbm+1;i++)
    {
        auto *row = main_sheet_->CreateRow(start++);
        auto* cell = row->CreateCell(2);
        add_main_sheet_line( tree_matiere_, row, i,cell);
    }



}

void structure_cours::create_sub_sheet( matiere * m )
{
    std::cout<<" Creating sheet for "<< m->alias_.toStdString()<<std::endl;
    auto *style_center = book_master_.CreateCellStyle();
    style_center->SetHAlignment(ods::HAlign::Center);

    m->sheet_ = book_master_.CreateSheet(m->alias_);
    // create the sheet of the child
    for (int i=0;i< m->dep_matiere_.size();i++)
    {
        create_sub_sheet(m->dep_matiere_[i]);
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

    for (int i=0;i<liste_etudiant.size();i++)
    {
        student& e = liste_etudiant[i];
        //qDebug()<<"Working on student "<< e.name_;
        if (m->option_ == "all" || m->option_ == e.option_ )
        {
            row = m->sheet_->CreateRow(start++);
            cell = row->CreateCell(0);
            cell->SetValue(e.name_);
            cell = row->CreateCell(1);
            cell->SetValue(e.first_name_);
            cell = row->CreateCell(2);
            cell->SetValue(e.option_);
            cell->SetStyle(style_center);

            cell = row->CreateCell(3);
            //std::cout<<"matiere = "<< m->name_.toStdString()<<" nombre de dépendance = "<< m->dep_matiere_.size()<<std::endl;
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
                        row->CreateCell(4+k);
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
                            auto* sheet_dep = book_master_.sheet(m->dep_matiere_[k]->alias_);
                            auto *row_dep = sheet_dep->row(p.row);
                            auto *cell_dep = row_dep->cell(p.col);
                            auto *formula1 = new ods::Formula();
                            formula1->Add(sheet_dep, cell_dep );
                            cell->SetFormula(formula1);
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
}

/*cours* structure_cours::get_cours(QString alias)
{
	for (int i=0;i<liste_cours.size();i++)
	{
		if (liste_cours[i].alias == alias.trimmed())
		{
			return & liste_cours[i];
		}
	}
	std::cerr<<" Cannot find dependency of "<< alias.toStdString()<<std::endl;
	exit(0);
}*/

bool  structure_cours::find_cell(int * COL, int * ROW, QString name,QString ods_file)
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
	if (sheet_in == nullptr)
	{
		qDebug() << "No sheet at 0";
		return false;
	}

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

matiere* structure_cours::get_master_of_tree()
{
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

void structure_cours::print_students()
{
    for (int i=0;i<liste_etudiant.size();i++)
    {
        liste_etudiant[i].print();
    }
}

void structure_cours::print_tree()
{
    if (tree_matiere_)
        tree_matiere_->print();
    else{
        std::cerr<<" Error the tree is not defined yet"<<std::endl;
        exit(0);
    }
}

bool structure_cours::read_project()
{
	QDomDocument document;
	QFile file(PROJECT_NAME);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		// qDebug() << "Failed to open the file for reading.";
		// qDebug() << "You must create a project please refer to : GestionNote help";
		return false;
	}
	else
	{	// loading
		if(!document.setContent(&file))
		{
			// qDebug() << "Failed to load the file for reading.";
			return false;
		}
		file.close();
	}
	QDomElement root = document.firstChildElement();

//	QDomElement nodes = root.elementsByTagName("project");
/*// 	qDebug() << "# nodes = " << nodes.count();
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
// 			qDebug() << new_cours.nom;

			QDomElement alias = elm.namedItem("alias").toElement();
			if ( !alias.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.alias_ = alias.text().trimmed();
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
				new_cours.option_ = "";

			new_cours.tree_level_ = -1;
//			new_cours.sheet =  nullptr;

			liste_cours.push_back(new_cours);
		}
	}*/


	return true;
}

void structure_cours::read_student(QString ods_file)
{
	std::cout<<"Start to read the student file."<<std::endl;
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
    //std::cout<<" find_option = "<< find_option <<std::endl;
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
			if (find_option)
            {
                const ods::Value &value4 = cell_option->value();
                if( !value4.NoValue())
                    tmp.option_ = *value4.AsString();
            }

			qDebug()<<tmp.name_ << tmp.first_name_ << tmp.email_;
			liste_etudiant.push_back(tmp);
		}
		current_row = sheet_in->row(nom_row++);
	}
	std::cout<<"il y a "<< nb_etu <<" etudiants"<<std::endl;
}

void structure_cours::read_xml( QString input)
{
	qDebug().nospace()<<" reading "<< input;
	QDomDocument document;
	QFile file(input);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open the file : "<< input<<" for reading.";
		return;
	}
	else
	{	// loading
		if(!document.setContent(&file))
		{
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
// 			qDebug() << new_cours.nom;

			QDomElement alias = elm.namedItem("alias").toElement();
			if ( !alias.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.alias_ = alias.text().trimmed();
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

			new_cours.tree_level_ = -1;
//			new_cours.sheet =  nullptr;

			liste_cours.push_back(new_cours);
		}
	}
	tree_matiere_ = get_master_of_tree();
	tree_matiere_->update_dads();
	tree_matiere_->update_level();
	tree_matiere_->update_col(3);

}

