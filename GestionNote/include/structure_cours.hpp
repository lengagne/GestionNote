#ifndef STRUCTURE_COURS_HPP_
#define STRUCTURE_COURS_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>
#include "Item.hpp"
#include "matiere.hpp"
#include "student.hpp"
#include <ods/ods>


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

//	void add_matiere(QString prof, QString matiere);

	void create_file();

	bool find_cell(int * col, int * row, QString name, QString ods_file);

	int get_biggest_level();

    void print_students();

	void print_tree();

	void read_student(QString ods);

	void read_xml( QString input);

    // return the master and make the tree
    matiere* get_master_of_tree();



private:
	std::vector<matiere> liste_cours;
	std::vector<prof> liste_prof;
	std::vector<student> liste_etudiant;
	matiere* tree_matiere_;

	ods::Book book_master_;
};


#endif
