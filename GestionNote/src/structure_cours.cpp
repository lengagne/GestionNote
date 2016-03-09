#include "structure_cours.hpp"
#define START_LINE 5
#define START_COL 5


structure_cours::structure_cours()
{
	std::cout<<"Creation de l'objet strucutre cours"<<std::endl;

}

structure_cours::~structure_cours()
{
	std::cout<<"Destruction de l'objet strucutre cours"<<std::endl;
}

void structure_cours::create_file()
{
/*	auto *style_center = book_master_.CreateCellStyle();
	style_center->SetHAlignment(ods::HAlign::Center);

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


	auto path = "./master.ods";
	QFile target(path);
	QString err = book_master_.Save(target);
	if (!err.isEmpty())
		qDebug() << "Error saving ods file:" << err;
	else
		qDebug() << "Saved to" << target.fileName();
*/
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

int structure_cours::get_biggest_level()
{
	int max = -1;
	for (int i=0;i<liste_cours.size();i++)
		if (liste_cours[i].tree_level_>max)
		{
			max = liste_cours[i].tree_level_;
		}
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

    find_option = find_cell( &option_col, &option_row, "OPTION",ods_file);

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
                if( value4.NoValue())
                    tmp.options_ .push_back(*value4.AsString());
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
		qDebug() << "Failed to open the file for reading.";
		return;
	}
	else
	{	// loading
		if(!document.setContent(&file))
		{
			qDebug() << "Failed to load the file for reading.";
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
				new_cours.option_ = "";

			new_cours.tree_level_ = -1;
//			new_cours.sheet =  nullptr;

			liste_cours.push_back(new_cours);
		}
	}
	tree_matiere_ = get_master_of_tree();
}

