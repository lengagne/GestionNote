#ifndef STRUCTURE_COURS_HPP_
#define STRUCTURE_COURS_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>
#include "Item.hpp"

#include <ods/ods>

typedef struct cours {
	QString nom;
	QString alias;
	std::vector<cours*> dep_cours;
	std::vector<QString> dependance;
	std::vector<double> coeff;
	QString apogee; 
	QString responsable; // si pas dependance
	QString option;
	int tree_level;
	ods::Sheet *sheet;
	
}cours;

typedef struct prof{
	QString nom;
	std::vector<QString> matiere;
}prof;

typedef struct etudiant{
	QString nom;
	QString prenom;
	QString option;
	int row;
}etudiant;


class structure_cours
{
public:
	structure_cours();
	
	~structure_cours();
	
	void add_matiere(QString prof, QString matiere);
	
	void create_file();
	
	void find_cell(int * col, int * row, QString name, QString ods_file);
	
	int get_biggest_level();
	
	void read_student(QString ods);
	
	void read_xml( QString input);
	
	cours* get_cours(QString alias);
	
	void set_dependancy(cours * C, int level);
	
private:
	std::vector<cours> liste_cours;
	std::vector<prof> liste_prof;
	std::vector<etudiant> liste_etudiant;
	cours* tree;
	
	ods::Book book_master_;	
};


#endif
