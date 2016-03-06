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

void structure_cours::add_matiere(QString nom_prof, QString matiere)
{
	// check if new prof
	prof *p = NULL;
	for (int i=0;i<liste_prof.size();i++)
	{
		if(liste_prof[i].nom == nom_prof)
		{
			p = & liste_prof[i];
			break;
		}
	}
	if (!p)
	{
		prof tmp;
		tmp.nom = nom_prof;
		liste_prof.push_back(tmp);
		p = & liste_prof[liste_prof.size()-1];
	}
	
	// add the matiere
	p->matiere.push_back(matiere);
}

void structure_cours::create_file()
{
	auto *style_center = book_master_.CreateCellStyle();
	style_center->SetHAlignment(ods::HAlign::Center);
	
	int max_level = get_biggest_level();
// 	std::cout<<"max_level = "<< max_level <<std::endl;
	for (int i=0;i<max_level+1;i++)
	{
		for (int j=0;j<liste_cours.size();j++)	if (liste_cours[j].tree_level == i)
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

cours* structure_cours::get_cours(QString alias)
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
}

void structure_cours::find_cell(int * COL, int * ROW, QString name,QString ods_file)
{
	name.trimmed();
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
					break;
				}
			}
		}	
	}	
}

int structure_cours::get_biggest_level()
{
	int max = -1;
	for (int i=0;i<liste_cours.size();i++)
		if (liste_cours[i].tree_level>max)
		{
			max = liste_cours[i].tree_level;
		}
	return max;
}

void structure_cours::read_student(QString ods_file)
{
	std::cout<<"Start to read the student file."<<std::endl;
	/// read a file

	int nom_col, nom_row;
	int prenom_col, prenom_row;
	
	find_cell( &nom_col, &nom_row, "NOM",ods_file);
	
	std::cout<<" nom_col = "<< nom_col <<std::endl;
	std::cout<<" nom_row = "<< nom_row <<std::endl;
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
	auto *row_nom = sheet_in->row(nom_row++);
	ods::Cell *cell_nom = nullptr;
	ods::Cell *cell_prenom = nullptr;
	ods::Cell *cell_option = nullptr;
	int nb_etu = 0;
	while(row_nom != nullptr)
	{
		
		cell_nom = row_nom->cell(nom_col);
		cell_prenom = row_nom->cell(nom_col+1);
		cell_option = row_nom->cell(nom_col+2);
		const ods::Value &value1 = cell_nom->value();
		const ods::Value &value2 = cell_prenom->value();
		const ods::Value &value3 = cell_option->value();
		if (value1.NoValue())
		{
			break;
		}else
		{
			etudiant tmp;
			nb_etu ++;
			tmp.nom = *value1.AsString();
			tmp.prenom = *value2.AsString();
			if (value3.NoValue())
				tmp.option = "none";
			else
				tmp.option = *value3.AsString();
			qDebug()<<tmp.nom << tmp.prenom << tmp.option ;
			liste_etudiant.push_back(tmp);
		}		
		row_nom = sheet_in->row(nom_row++);
	}
	std::cout<<"il y a "<< nb_etu <<" etudiants"<<std::endl;
	
	
// 	ods::Cell *cell_read = nullptr;
// 	for (int i=0;i<100;i++)
// 	{
// 		auto *row = sheet_in->row(i);
// 		if (row == nullptr)
// 		{
// 			qDebug() << "No row at " << QString::number(0);
// 			return;
// 		}
// 		
// 		for (int j=0;j<100;j++)
// 		{
// 			cell_read = row->cell(j);
// 
// 			if (cell_read == nullptr) {
// 				qDebug() << "No cell at " << QString::number(0);
// 				break;
// 			}
// 			QString which_cell = "Cell [" + QString::number(i) + ":" + QString::number(j) + "]: ";
// 			QString value_as_string = GetCellValue(cell_read);
// 			qDebug() << which_cell << value_as_string <<" sans type = "<< GetCellValue(cell_read);
// 		}	
// 	}
// 	std::cout<<"End to read the student file."<<std::endl;	
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
			cours new_cours;
			new_cours.responsable = "none";
			// lecture du nom
			QDomElement name = elm.namedItem("nom").toElement();
			if ( !name.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.nom = name.text().trimmed();
			}else
			{
				std::cerr<<"nom not found "<<std::endl;
				exit(0);
			}
// 			qDebug() << new_cours.nom;
			
			QDomElement alias = elm.namedItem("alias").toElement();
			if ( !alias.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.alias = alias.text().trimmed();
			}
			
			// lecture des dépendances
			QDomElement e = elm.firstChildElement( "dependance" );
			while ( !e.isNull() ) {
				new_cours.dependance.push_back(e.text().trimmed());
				new_cours.coeff.push_back(e.attribute("coeff").toDouble());
// 				qDebug() <<"coeff = "<< e.attribute("coeff");
// 				qDebug() <<"dependance = "<< e.text().trimmed();
				e = e.nextSiblingElement( "dependance" );
			}
			
			QDomElement responsable = elm.namedItem("responsable").toElement();
			if ( !responsable.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.responsable = responsable.text().trimmed();
// 				qDebug() <<"responsable = "<< responsable.text().trimmed();
			}
			
			QDomElement apogee = elm.namedItem("apogee").toElement();
			if ( !apogee.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.apogee = apogee.text().trimmed();
			}
			
			QDomElement option = elm.namedItem("option").toElement();
			if ( !option.isNull() ) { // We have a <name>..</name> element in the set
				new_cours.option = option.text().trimmed();
			}else
				new_cours.option = "non specifique";
			
			new_cours.tree_level = -1;
			new_cours.sheet =  nullptr;
			
			liste_cours.push_back(new_cours);
		}
	}
	
	// mise à jour des dépendances
	std::cout<<" Construction des dépendances" <<std::endl;
	tree = get_cours("master");
	set_dependancy(tree,0);
	// check if some are out of the tree
	for (int i=0;i<liste_cours.size();i++)
	{
		// create the prof
		add_matiere(liste_cours[i].responsable, liste_cours[i].alias);
		
		if(liste_cours[i].tree_level == -1)
			std::cerr<<"Error the object : "<< liste_cours[i].alias.toStdString()<<" is not in the tree."<<std::endl;
	}
	std::cout<<" Dépendances faites" <<std::endl;
}

void structure_cours::set_dependancy(cours * C, int level)
{
	C->tree_level = level;
	for (int i=0;i<C->dependance.size();i++)
	{
		cours* tmp = get_cours(C->dependance[i]);
		C->dep_cours.push_back(tmp);
		set_dependancy(tmp,level+1);
	}
}

