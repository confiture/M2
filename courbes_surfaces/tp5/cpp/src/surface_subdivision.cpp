// test de triangulation
#include<iomanip>
#include<string>
#include"maillage_subdivision.hpp"

using namespace std;

//////////////////////////////////////////////////////
// partie déformation
int partie_deformation(char * nom_fichier_donnees,int k)
{
	//char nom_fichier_donnees[] = "DONNEES/cube.off";

	//// A - le modèle initial est subdivisé 6 fois
	MaillageSubdivision M0(nom_fichier_donnees);
	for (int i=1; i<=6; i++)
		M0.subdivision_catmull_clark();
	Point3D B = M0.barycentre();

	// centrer le maillage
	M0.translater(-B);
	M0.write_geomview("res0.off.o");

	//// B - le modèle initial est subdivisé k fois
	//// puis les coordonnées d'un sommet sont modifiées
	//// en enfin le maillage est subdivisé 6-k fois
	//int k=0;
	Point3D C(1,1,1); // le point à modifier
	Point3D C2(0,0,1); // le point à modifier
	Point3D T(0.5,0.5,0.5); // la translation à effectuer

	// k subdivisions
	MaillageSubdivision M1(nom_fichier_donnees);
	for (int i=1; i<=k; i++)
		M1.subdivision_catmull_clark();

	// récupérer le sommet s le plus proche de C
	PtrSommet s = M1.sommet(C);
	PtrSommet s2 = M1.sommet(C2);
	// et le modifier
	s->translater(T);
	s2->translater(T);

	// 6-k subdivisions
	for (int i=k+1; i<=6; i++)
		M1.subdivision_catmull_clark();

	// recentrer le maillage par rapport à M0
	M1.translater(-B);
	M1.write_geomview("res1.off.o");
}

//////////////////////////////////////////////////////
// partie méthodes subdivision
const char *nom_methode_subdiv[] = {
  "doo_sabin",
  "catmull_clark",
  "midedge",
  "sqrt3",
  "butterfly",
  "loop"
};

void partie_subdivision(char * nom,int choix,int nb_subdivision)
{
	/*char nom[200],*/ char nom_res[200];

	// choix du fichier à lire
	cout << "Nom du fichier OFF : ";
	//cin >> nom;

	// création du maillage initial
	MaillageSubdivision M(nom);
	M.centrer();

	// choix de la méthode
	/*
	int choix;
	cout << "Choix de la méthode de subdivision :" << endl;
	cout << " Doo-Sabin     : 0" << endl;
	cout << " Catmull-Clark : 1" << endl;
	cout << " MidEdge       : 2" << endl;
	cout << " Sqrt(3)       : 3" << endl;
	cout << " Butterfly     : 4" << endl;
	cout << " Loop          : 5" << endl;
	do {
		cout << "Entrer votre choix : ";
		cin >> choix;
	} while (choix<0 || choix>5);
	*/

	// triangulation si nécessaire et sauvegarde du maillage initial
	if (choix>=3 && choix<=5) M.trianguler();
	sprintf(nom_res, "res%02d.off.o", 0);
	M.write_geomview(nom_res);

	sprintf(nom_res, "res%02d.off.o", 0);
	M.write_geomview(nom_res);
	//unsigned int nb_subdivision;
	//cout << "Entrer le nombre de subdivision : ";
	//cin >> nb_subdivision;

	for (unsigned int i=1; i<=nb_subdivision; i++)
	{
		cout << " subdivision " << i << " ..." << endl;
		switch(choix)
		{
			case 0 : M.subdivision_doo_sabin()     ; break;
			case 1 : M.subdivision_catmull_clark() ; break;
			case 2 : M.subdivision_midedge()       ; break;
			case 3 : M.subdivision_sqrt3()         ; break;
			case 4 : M.subdivision_butterfly()     ; break;
			case 5 : M.subdivision_loop()          ; break;
		}
		sprintf(nom_res, "res-%s-%02d.off.o", nom_methode_subdiv[choix], i);
		M.write_geomview(nom_res);
	}
}


int main(int argc,char * argv[])
{
	//	partie_subdivision(argv[1],atoi(argv[2]),atoi(argv[3]));
	partie_deformation(argv[1],atoi(argv[2]));
	return 0;
}
