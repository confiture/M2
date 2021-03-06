#include "image.hpp"

using namespace std;

int& image::operator()(int i,int j){
	assert(i<hauteur);
	assert(i>=0);
	assert(j<largeur);
	assert(j>=0);
	return buffer[i*largeur+j];
}

int image::operator()(int i,int j)const{
	assert(i<hauteur);
	assert(i>=0);
	assert(j<largeur);
	assert(j>=0);
	return buffer[i*largeur+j];
}

image::image(int hauteur, int largeur,int valmax){
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
	this->buffer=new int[hauteur*largeur];

	int n=hauteur*largeur;
	for(int i=0;i<n;i++)buffer[i]=valmax;
}

image::image(const char* nomFichier){
	/* Ouverture */
	FILE* ifp = fopen(nomFichier,"r");

	/* Lecture du Magic number */
	int ich1 = getc( ifp );
	if ( ich1 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	int ich2 = getc( ifp );
	if ( ich2 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	if(ich2 != '2'){
		pm_erreur(" wrong ifp format ");
		exit(-1);
	}


	/*Lecture des dimensions*/
	largeur = pm_getint( ifp );
	hauteur = pm_getint( ifp );
	valmax = pm_getint( ifp );

	buffer = new int[hauteur*largeur];

	/*Lecture*/
	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			(*this)(i,j)=pm_getint(ifp);
		}
	}
	/* fermeture */
	fclose(ifp);
}

image::image(const image & im){
	this->buffer = new int[im.hauteur*im.largeur];
	this->hauteur=im.hauteur;
	this->largeur=im.largeur;
	this->valmax=im.valmax;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=im(i,j);
		}
	}
}

void image::updateValmax(){
	valmax=0;
	for(int i=0;i<hauteur*largeur;i++){
		if(buffer[i]>valmax)valmax=buffer[i];
	}
}

image::image(const image & im,int i1,int i2,int j1,int j2,int** conn,int etiq){
	valmax=im.valmax;
	hauteur=i2-i1+1;
	largeur=j2-j1+1;
	buffer=new int[hauteur*largeur];
	for(int i=i1;i<=i2;i++){
		for(int j=j1;j<=j2;j++){
			if(conn[i][j]==etiq){
				(*this)(i-i1,j-j1)=im(i,j);
			}
			else{
				(*this)(i-i1,j-j1)=0;
			}
		}
	}
}

void image::recadre(double a,double b){
	double min;
	double max;
	min=max=(*this)(1,1);
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			if(min>(*this)(i,j))min=(*this)(i,j);
			if(max<(*this)(i,j))max=(*this)(i,j);
		}
	}

	for(int i=1;i<hauteur;i++){
		for(int j=1;j<largeur;j++){
			(*this)(i,j)=(((double)(*this)(i,j)-(double)min)/
			              ((double)max-(double)min))*(b-a)+(double)a;
		}
	}

	valmax=b;
}

int image::EcrireImagePGM(const char* nomFichier)const{
	/* Ouverture */
	filebuf fb;
	fb.open(nomFichier,ios::out);
	ostream os(&fb);

	/* Ecriture */
	os<<"P2"<<endl;
	os<<largeur<<" "<<hauteur<<endl;
	os<<valmax<<endl;

	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			os<<(*this)(i,j);
			os<<" ";
		}
		os<<endl;
	}
	fb.close();
	return 1;
}

int image::seuiller(int seuil){
	if(valmax>=seuil){valmax=255;}
	else{valmax=0;}

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if((*this)(i,j)<seuil){(*this)(i,j)=0;}
			else{(*this)(i,j)=255;}
		}
	}

	return 1;
}

int image::negatif(){
	int max=-1;
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=valmax-(*this)(i,j);
			if((*this)(i,j)>max)max=(*this)(i,j);
		}
	}

	valmax=max;
	return 1;
}



int** image::binConnexite4()const{
	int id=0;
	int** corres=new int*[hauteur];//l'image des étiquettes
	for(int i=0;i<hauteur;i++){
		corres[i]=new int[largeur];
		for(int j=0;j<largeur;j++)corres[i][j]=-1;
	}

	std::vector<int> equiv;

	for(int i=0;i<hauteur-1;i++){
		for(int j=0;j<largeur-1;j++){
			if((*this)(i,j)==255){
				//voisins noirs
				if((*this)(i+1,j)==0 && (*this)(i,j+1)==0){
					if(i==97 && j==0)std::cout<<"ici 1"<<endl;
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}
				}
				//un des voisins est blanc et appartient à un groupe
				else if(corres[i+1][j]!=-1 || corres[i][j+1]!=-1){
					if(i==97 && j==0)std::cout<<"ici 2"<<endl;
					if(corres[i+1][j]==255){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
						}
					}
					if(corres[i][j+1]!=255){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
						}
					}

					if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
					if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
				}
				//aucun n'appartient à un groupe
				else{
					if(i==97 && j==0)std::cout<<"ici 3"<<endl;
					//si le point courant ne fait pas partie d'un groupe on le crée
					if(corres[i][j]==-1){
						if(i==97 && j==0)cout<<"c'est le gros caca"<<endl;
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}

					if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
					if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
				}
			}
		}
	}


	//on traite la colonne sur la droite
	int j=largeur-1;
	for(int i=0;i<hauteur-1;i++){
		if((*this)(i,j)==255){
			//voisin d'en bas noir
			if((*this)(i+1,j)==0){
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}
			}
			//le voisin d'en bas n'est pas noir et appartient à un groupe
			else if(corres[i+1][j]!=-1){
				if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
				else{
					updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
				}
			}
			//le voisin d'en bas n'appartient pas à un groupe
			else{
				//si le point courant ne fait pas partie d'un groupe on le crée
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}

				if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
			}
		}
	}


	//on traite la dernière ligne
	int i=hauteur-1;
	for(int j=0;j<largeur-1;j++){
		if((*this)(i,j)==255){
			//voisin de droite est noir
			if((*this)(i,j+1)==0){
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}
			}
			//le voisin de droite n'est pas noir et appartient à un groupe
			else if(corres[i][j+1]!=-1){
				if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
				else{
					updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
				}
			}
			//le voisin de droite n'appartient pas à un groupe
			else{
				//si le point courant ne fait pas partie d'un groupe on le crée
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}

				if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
			}
		}
	}

	//mise à jour des groupes grâce au tableau des équivalences
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(corres[i][j]!=-1){corres[i][j]=equiv[corres[i][j]]+1;} // +1 pour rammener -1 à 0
			else{corres[i][j]=0;}
		}
	}

	return corres;
}

int** image::binConnexite8()const{
	int id=0;
	int** corres=new int*[hauteur];//l'image des étiquettes
	for(int i=0;i<hauteur;i++){
		corres[i]=new int[largeur];
		for(int j=0;j<largeur;j++)corres[i][j]=-1;
	}

	std::vector<int> equiv;

	for(int i=0;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			if((*this)(i,j)==255){
				//voisins noirs
				if((*this)(i+1,j)==0 && (*this)(i,j+1)==0 && (*this)(i+1,j+1)==0 && (*this)(i+1,j-1)==0){
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}
				}
				//un des voisins n'est pas noir et appartient à un groupe
				else if(corres[i+1][j]!=-1 || corres[i][j+1]!=-1 || corres[i+1][j-1]!=-1 || corres[i+1][j+1]!=-1){
					if(corres[i+1][j]!=-1){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
						}
					}
					if(corres[i][j+1]!=-1){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
						}
					}
					if(corres[i+1][j-1]!=-1){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j-1]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j-1]]);
						}
					}
					if(corres[i+1][j+1]!=-1){
						if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j+1]];}
						else{
							updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j+1]]);
						}
					}

					if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
					if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
					if((*this)(i+1,j-1)!=0){corres[i+1][j-1]=equiv[corres[i][j]];}
					if((*this)(i+1,j+1)!=0){corres[i+1][j+1]=equiv[corres[i][j]];}
				}
				//un des voisins est blanc et aucun voisin n'appartient à un groupe
				else{
					//si le point courant ne fait pas partie d'un groupe on le crée
					if(corres[i][j]==-1){
						corres[i][j]=id;
						equiv.push_back(id);
						id++;
					}

					if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
					if((*this)(i,j+1)!=0){corres[i][j+1]=equiv[corres[i][j]];}
					if((*this)(i+1,j-1)!=0){corres[i+1][j-1]=equiv[corres[i][j]];}
					if((*this)(i+1,j+1)!=0){corres[i+1][j+1]=equiv[corres[i][j]];}
				}
			}
		}
	}

	/////////////////////////////////////
	//COIN SUPERIEUR GAUCHE on v�rifie � droite seulement
	////////////////////////////////////
	if((*this)(0,0)==255){
		if((*this)(0,1)==0){
			if(corres[0][0]==-1){
				corres[0][0]=id;
				equiv.push_back(id);
				id++;
			}
		}
		else if(corres[0][1]!=-1){
			if(corres[0][0]==-1){corres[0][0]=equiv[corres[0][1]];}
			else{
				updateEquiv(equiv,id,equiv[corres[0][0]],equiv[corres[0][1]]);
			}
		}
		else{
			//si le point courant ne fait pas partie d'un groupe on le crée
			if(corres[0][0]==-1){
				corres[0][0]=id;
				equiv.push_back(id);
				id++;
			}

			if((*this)(0,1)!=0){corres[0][1]=equiv[corres[0][0]];}
		}
	}



	/////////////////////////////////////
	//COLONNE GAUCHE
	////////////////////////////////////
	int j=0;
	for(int i=0;i<hauteur-1;i++){
		if((*this)(i,j)==255){
			//voisins noirs
			if((*this)(i+1,j)==0 && (*this)(i+1,j+1)==0){
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}
			}
			//un des voisins n'est pas noir et appartient à un groupe
			else if(corres[i+1][j]!=-1 && corres[i+1][j+1]!=-1){
				if(corres[i+1][j]!=-1){
					if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
					else{
						updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
					}
				}
				if(corres[i+1][j+1]!=-1){
					if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j+1]];}
					else{
						updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j+1]]);
					}
				}

				if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
				if((*this)(i+1,j+1)!=0){corres[i+1][j+1]=equiv[corres[i][j]];}
			}
			//aucun n'appartient à un groupe
			else{
				//si le point courant ne fait pas partie d'un groupe on le crée
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}

				if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
				if((*this)(i+1,j+1)!=0){corres[i+1][j+1]=equiv[corres[i][j]];}
			}
		}
	}

	//////////////////////////////////////
	//DERNIERE LIGNE
	/////////////////////////////////////
	int i=hauteur-1;
	for(int j=1;j<largeur-1;j++){
		if((*this)(i,j)==255){
			//voisin noir
			if((*this)(i,j+1)==0){
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}
			}
			//le voisin n'est pas noir et appartient à un groupe
			else if(corres[i][j+1]!=-1){
				if(corres[i][j]==-1){corres[i][j]=equiv[corres[i][j+1]];}
				else{
					updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i][j+1]]);
				}
			}
			//le voisin n'est pas noir et n'appartient àaucun groupe
			else{
				//si le point courant ne fait pas partie d'un groupe on le crée
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}

				corres[i][j+1]=equiv[corres[i][j]];
			}
		}
	}

	//////////////////////////////////////
	//COLONNE DROITE
	//////////////////////////////////////
	j=largeur-1;
	for(i=0;i<hauteur-1;i++){
		if((*this)(i,j)==255){
			//voisin d'en bas noir
			if((*this)(i+1,j)==0 && (*this)(i+1,j-1)==0){
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}
			}
			//le voisin d'en bas n'est pas noir et appartient à un groupe
			else if(corres[i+1][j]!=-1 || corres[i+1][j-1]!=-1){
				if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j]];}
				else{
					updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j]]);
				}
				if(corres[i][j]==-1){corres[i][j]=equiv[corres[i+1][j-1]];}
				else{
					updateEquiv(equiv,id,equiv[corres[i][j]],equiv[corres[i+1][j-1]]);
				}
				if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
				if((*this)(i+1,j-1)!=0){corres[i+1][j-1]=equiv[corres[i][j]];}
			}
			//le voisin d'en bas n'appartient pas à un groupe
			else{
				//si le point courant ne fait pas partie d'un groupe on le crée
				if(corres[i][j]==-1){
					corres[i][j]=id;
					equiv.push_back(id);
					id++;
				}

				if((*this)(i+1,j)!=0){corres[i+1][j]=equiv[corres[i][j]];}
				if((*this)(i+1,j-1)!=0){corres[i+1][j-1]=equiv[corres[i][j]];}
			}
		}
	}


	//mise à jour des groupes grâce au tableau des équivalences
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			//cout<<equiv[corres[i][j]]<<endl;
			if(corres[i][j]!=-1){corres[i][j]=equiv[corres[i][j]]+1;} // +1 pour rammener -1 à 0
			else{corres[i][j]=0;}
		}
	}

	return corres;
}


/*void image::dispCompConn(const char* fic)const{
  set<int> groupes;
  image sortie(hauteur,largeur,255);
  image im_s(*this);
  im_s.seuiller(50);

  im_s.EcrireImagePGM("seuillage.pgm");

  int** conn=im_s.binConnexite8();

  for(int i=0;i<hauteur;i++){
  for(int j=0;j<largeur;j++){
  if(conn[i][j]!=0){sortie(i,j)=100+conn[i][j];groupes.insert(conn[i][j]);}
  else{sortie(i,j)=0;}
  //std::cout<<conn[i][j]<<endl;
  }
  }

  sortie.EcrireImagePGM(fic);
  cout<<"nombre d'objet "<<groupes.size()<<endl;
  }*/

int image::nbConnCom(int nconn,int seuil){
	set<int> groupes;
	image im_s(*this);
	im_s.seuiller(seuil);

	int** conn;
	if(nconn==8){
		conn=im_s.binConnexite8();
	}
	else{
		conn=im_s.binConnexite4();
	}

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(conn[i][j]!=0){groupes.insert(conn[i][j]);}
		}
	}

	for(int i=0;i<hauteur;i++)delete[] conn[i];
	delete[] conn;

	return groupes.size();
}


int** image::makeConnComp(int nconn,int seuil)const{
	set<int> groupes;
	image im_s(*this);
	im_s.seuiller(seuil);

	int** conn;
	if(nconn==8){
		conn=im_s.binConnexite8();
	}
	else{
		conn=im_s.binConnexite4();
	}

	return conn;
}


void image::writePgmItems(char * itemsName,int seuil,double** masque,int n){
	string name(itemsName);
	set<int> groupes;

	int** conn=makeConnComp(4,seuil);
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(conn[i][j]!=0){groupes.insert(conn[i][j]);}
		}
	}

	int k=0;
	std::set<int>::iterator it=groupes.begin();
	std::set<int>::iterator endIt=groupes.end();
	for(it;it!=endIt;it++){
		int i1=hauteur+1;
		int i2=-1;
		int j1=largeur+1;
		int j2=-1;
		for(int i=0;i<hauteur;i++){
			for(int j=0;j<largeur;j++){
				if(conn[i][j]==(*it)){
					if(i1>i)i1=i;
					if(i2<i)i2=i;
					if(j1>j)j1=j;
					if(j2<j)j2=j;
				}
			}
		}

		image* obIm = new image((*this),i1-1,i2+1,j1-1,j2+1,conn,(*it));
		item objet;
		objet.pIm=obIm;
		objet.load(masque,n,seuil);

		string name(itemsName);
		ostringstream ss;
		ss<<k;
		name+=ss.str();
		name+=".pgm";
		objet.writePgmItem(name.c_str());

		k++;
	}
}

image* image::duplique_elemStruc_bord(image elem_struct) const{
	image* sortie=new image(hauteur+2*elem_struct.hauteur,largeur+2*elem_struct.largeur,0);
	//cout<<"hauteur avant"<<hauteur<<" largeur avant"<<largeur<<endl;
	//cout<<"hauteur"<<sortie.hauteur<<" largeur"<<sortie.largeur<<endl;
	// On se place sur la premiere ligne de l'image courante
	for(int k=0;k<(*this).hauteur;k++){
		// On traite le 1er bord (bord gauche de l'image)
		int n=0;

		for(int i=elem_struct.hauteur;i<hauteur+elem_struct.hauteur;i++){
			for(int j=0;j<elem_struct.largeur;j++){
				(*sortie)(i,j) = (*this)(k,0);
			}
		}

		// On traite le 2�me bord (bord droite de l'image)
		for(int i=elem_struct.hauteur;i<hauteur+elem_struct.hauteur;i++){
			for(int j=sortie->largeur-elem_struct.largeur-1;j<sortie->largeur;j++){
				(*sortie)(i,j) = (*this)(k,hauteur-1);
			}
		}
	}

	// On traite le 3 �me bord (bord du haut de l'image)
	for(int i=0;i<elem_struct.hauteur;i++){
		for(int j=0;j<sortie->largeur;j++){
			(*sortie)(i,j) = (*sortie)(elem_struct.hauteur,j);
		}
	}

	// On traite le 4 �me bord (bord du bas de l'image)
	for(int i=sortie->hauteur-elem_struct.hauteur;i<sortie->hauteur;i++){
		for(int j=0;j<sortie->largeur;j++){
			(*sortie)(i,j) = (*sortie)(sortie->hauteur-elem_struct.hauteur-1,j);
		}
	}

	// On remplie l'int�rieur de l'image
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<hauteur;j++){
			(*sortie)(i+elem_struct.hauteur,j+elem_struct.largeur) = (*this)(i,j);
		}
	}
	sortie->valmax = valmax;

	return sortie;
}

image* image::duplique_elemStruc_bord(int taille /*image elem_struct*/) const{
   // image elem_struct = elemStruct(geo,taille);
    image* sortie=new image(hauteur+2*taille,largeur+2*taille,0);

    // On se place sur la premiere ligne de l'image courante
    for(int k=0;k<(*this).hauteur;k++){
      // On traite le 1er bord (bord gauche de l'image)
      int n=0;

	  for(int i=taille;i<hauteur+taille;i++){
		for(int j=0;j<taille;j++){
		      (*sortie)(i,j) = (*this)(k,0);
		}
	  }

    // On traite le 2�me bord (bord droite de l'image)
	  for(int i=taille;i<hauteur+taille;i++){
		for(int j=sortie->largeur-taille-1;j<sortie->largeur;j++){
		      (*sortie)(i,j) = (*this)(k,hauteur-1);
		}
	  }
    }

 // On traite le 3 �me bord (bord du haut de l'image)
    for(int i=0;i<taille;i++){
	  for(int j=0;j<sortie->largeur;j++){
		     (*sortie)(i,j) = (*sortie)(taille,j);
	  }
    }

 // On traite le 4 �me bord (bord du bas de l'image)
    for(int i=sortie->hauteur-taille;i<sortie->hauteur;i++){
	  for(int j=0;j<sortie->largeur;j++){
		     (*sortie)(i,j) = (*sortie)(sortie->hauteur-taille-1,j);
	  }
    }

 // On remplie l'int�rieur de l'image
    for(int i=0;i<hauteur;i++){
	    for(int j=0;j<hauteur;j++){
		  (*sortie)(i+taille,j+taille) = (*this)(i,j);
	    }
    }
    sortie->valmax = valmax;

    return sortie;
}


//-----------------------------------------------------------------------------//
image* image::dilatation(image elem_struct,bool Estdupliquer){
	 image* sortie;
	if(!Estdupliquer){
	sortie=(*this).duplique_elemStruc_bord(elem_struct);
	}
	else{
	     sortie=(this);
	}
	elem_struct.EcrireImagePGM("element_structurant.pgm");
	image copie(*sortie);
	int p=(elem_struct.hauteur-1)/2.0;
	for(int i=p;i<sortie->hauteur-p;i++){
	   for(int j=p;j<sortie->largeur-p;j++){
	     int maximum=copie(i,j);
	     for(int k=0;k<elem_struct.hauteur;k++){
	       for(int l=0;l<elem_struct.largeur;l++){
		 if(elem_struct(k,l)==1){
			maximum = max(maximum,copie(i+k-p,j+l-p)*elem_struct(k,l));
		 }
	       }
	     }
	     (*sortie)(i,j) = maximum ;
	   }
	}

	return sortie;
}
//-----------------------------------------------------------------------------//
image* image::erosion(image elem_struct,bool Estdupliquer){
	 image* sortie;
	if(!Estdupliquer){
	sortie=(*this).duplique_elemStruc_bord(elem_struct);
	}
	else{
	     sortie=(this);
	}
	elem_struct.EcrireImagePGM("element_structurant.pgm");
	// On fait une copie
	image copie(*sortie);
	int p=(elem_struct.hauteur-1)/2.0;
	for(int i=p;i<sortie->hauteur-p;i++){
	   for(int j=p;j<sortie->largeur-p;j++){
	     int minimum=copie(i,j);
	     for(int k=0;k<elem_struct.hauteur;k++){
	       for(int l=0;l<elem_struct.largeur;l++){
		 if(elem_struct(k,l)==1){
			minimum = min(minimum,copie(i+k-p,j+l-p)*elem_struct(k,l));
		 }
	       }
	     }
	     (*sortie)(i,j) = minimum ;
	   }
	}

	return sortie;
}

//-----------------------------------------------------------------------------//
image* image::ouverture(image elem_struct){
	image* im_erosion=erosion(elem_struct,false);
	image* sortie=im_erosion->dilatation(elem_struct,true);
	return sortie;
}

//-----------------------------------------------------------------------------//
image* image::fermeture(image elem_struct){
	image* im_dilatation=dilatation(elem_struct,false);
	image* sortie=im_dilatation->erosion(elem_struct,true);
	return sortie;
}

image* image::distanceT(double** masque,int n,int seuil)const{
	int p=(n-1)/2;
	image binaire(*this);
	binaire.seuiller(seuil);
	binaire.EcrireImagePGM("binaire.pgm");

	image* imS=new image(hauteur,largeur,0);
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(binaire(i,j)==255)(*imS)(i,j)=numeric_limits<int>::max();
		}
	}


	for(int i=p;i<hauteur-p;i++){
		for(int j=p;j<largeur-p;j++){
			if((*imS)(i,j)!=0){
				int minDist=numeric_limits<int>::max();
				for(int k=0;k<p;k++){
					for(int l=0;l<n;l++){
						if((*imS)(i-p+k,j-p+l)!=numeric_limits<int>::max()){
							if(minDist>(*imS)(i-p+k,j-p+l)+masque[k][l]){
								minDist=(*imS)(i-p+k,j-p+l)+masque[k][l];
							}
						}
					}
				}

				int k=p;
				for(int l=0;l<p;l++){
					if((*imS)(i-p+k,j-p+l)!=numeric_limits<int>::max()){
						if(minDist>(*imS)(i-p+k,j-p+l)+masque[k][l]){
							minDist=(*imS)(i-p+k,j-p+l)+masque[k][l];
						}
					}
				}

				(*imS)(i,j)=minDist;
			}
		}
	}


	for(int i=hauteur-p-1;i>=p;i--){
		for(int j=largeur-p-1;j>=p;j--){
			if((*imS)(i,j)!=0){
				int minDist=(*imS)(i,j);

				for(int l=p+1;l<n;l++){
					if((*imS)(i,j-p+l)!=numeric_limits<int>::max()){
						if(minDist >(*imS)(i,j-p+l)+masque[p][l]){
							minDist =(*imS)(i,j-p+l)+masque[p][l];
						}
					}
				}

				for(int k=p+1;k<n;k++){
					for(int l=0;l<n;l++){
						if((*imS)(i-p+k,j-p+l)!=numeric_limits<int>::max()){
							if(minDist>(*imS)(i-p+k,j-p+l)+masque[k][l]){
								minDist=(*imS)(i-p+k,j-p+l)+masque[k][l];
							}
						}
					}
				}

				(*imS)(i,j)=minDist;
			}
		}
	}

	imS->updateValmax();
	return imS;
}


image image::boule(double** masque,int n,int r){
	int p=(n-1)/2;
	int a=masque[0][p];
	image bl(2*(r/a + p) + 1 , 2*(r/a + p) + 1,0);
	int pBl=r/a+p;

	for(int i=0;i<bl.hauteur;i++){
		for(int j=0;j<bl.largeur;j++){
			bl(i,j)=numeric_limits<int>::max();
		}
	}

	bl(pBl,pBl)=0;

	for(int i=p;i<bl.hauteur-p;i++){
		for(int j=p;j<bl.largeur-p;j++){
			if(bl(i,j)!=0){
				int minDist=numeric_limits<int>::max();
				for(int k=0;k<p;k++){
					for(int l=0;l<n;l++){
						if(bl(i-p+k,j-p+l)!=numeric_limits<int>::max()){
							if(minDist>bl(i-p+k,j-p+l)+masque[k][l]){
								minDist=bl(i-p+k,j-p+l)+masque[k][l];
							}
						}
					}
				}

				int k=p;
				for(int l=0;l<p;l++){
					if(bl(i-p+k,j-p+l)!=numeric_limits<int>::max()){
						if(minDist>bl(i-p+k,j-p+l)+masque[k][l]){
							minDist=bl(i-p+k,j-p+l)+masque[k][l];
						}
					}
				}

				bl(i,j)=minDist;
				//assert(bl(i,j)!=numeric_limits<int>::max());
			}
		}
	}


	for(int i=bl.hauteur-p-1;i>=p;i--){
		for(int j=bl.largeur-p-1;j>=p;j--){
			if(bl(i,j)!=0){
				int minDist=bl(i,j);

				for(int l=p+1;l<n;l++){
					if(bl(i,j-p+l)!=numeric_limits<int>::max()){
						if(minDist >bl(i,j-p+l)+masque[p][l]){
							minDist =bl(i,j-p+l)+masque[p][l];
						}
					}
				}

				for(int k=p+1;k<n;k++){
					for(int l=0;l<n;l++){
						if(bl(i-p+k,j-p+l)!=numeric_limits<int>::max()){
							if(minDist>bl(i-p+k,j-p+l)+masque[k][l]){
								minDist=bl(i-p+k,j-p+l)+masque[k][l];
							}
						}
					}
				}

				bl(i,j)=minDist;
				//assert(bl(i,j)!=numeric_limits<int>::max());
			}
		}
	}

	for(int i=0;i<bl.hauteur;i++){
		for(int j=0;j<bl.largeur;j++){
			if(bl(i,j)>r)
				bl(i,j)=0;
			else
				bl(i,j)=1;
		}
	}

	return bl;
}

image* image::axeMedian(double** masque,int n,int seuil){
	bool incluse=true;
	image& DT = (*boulesMax(masque,n,seuil));
	DT.EcrireImagePGM("temp.pgm");
	int p=(n-1)/2;
	map<int,image> boules;
	map<int,image>::iterator itG;
	map<int,image>::iterator itP;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){//le centre de la grande boule est (i,j)
			int rayG=DT(i,j);

			if(rayG>=masque[p][p+1]){
				itG=boules.find(rayG);
				if(itG==boules.end()){
					boules[rayG]=boule(masque,n,rayG);
					itG=boules.find(rayG);
				}

				int hG=itG->second.hauteur;
				int pG=(hG-1)/2;
				for(int iG=0;iG<hG;iG++){
					for(int jG=0;jG<hG;jG++){

						int cPi=i-pG+iG;//le centre de la petite boule
						int cPj=j-pG+jG;
						int rayP=DT(cPi,cPj);
						if(rayP>=rayG){}
						else if(rayP<rayG && (cPi!=i || cPj!=j) && rayP!=0){
							itP=boules.find(rayP);
							if(itP==boules.end()){
								boules[rayP]=boule(masque,n,rayP);
								itP=boules.find(rayP);
							}

							if((itG->second).inclus((itP->second),cPi-i+pG,cPj-j+pG)){DT(cPi,cPj)=0;}
						}
					}
				}
			}
		}
	}

	DT.updateValmax();
	return &DT;
}


image* image::reconsAxeMed(double** masque,int n){
	image& cons=(*(new image(hauteur,largeur,0)));
	map<int,image> boules;
	map<int,image>::iterator itBoule;
	int p=(n-1)/2;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			int ray=(*this)(i,j);
			if(ray>0){
				cons(i,j)=1;
			}
			if(ray>=masque[p][p+1]){
				itBoule=boules.find(ray);
				if(itBoule==boules.end()){
					boules[ray]=boule(masque,n,ray);
					itBoule=boules.find(ray);
				}

				int hB=itBoule->second.hauteur;
				int pB=(hB-1)/2;
				for(int iB=0;iB<hB;iB++){
					for(int jB=0;jB<hB;jB++){
						if((itBoule->second)(iB,jB)==1)
							cons(i+iB-pB,j+jB-pB)=1;
					}
				}
			}
		}
	}

	cons.valmax=1;

	return &cons;
}


//-----------------------------------------------------------------------------//
//-------------------------Fonctions interm�diaires----------------------------//
//-----------------------------------------------------------------------------//
image image::elemCarre(int taille){
  image sortie(taille,taille,1);
  for(int i=0;i<taille;i++){
		for(int j=0;j<taille;j++){
			sortie(i,j)=1;
		}
	}
	return sortie;
}

//-----------------------------------------------------------------------------//
image image::elemCercle(int taille){
  assert(taille%2 == 1);
  image cercle(taille,taille,1);
  double rayon=taille/2;

  for(double i=0;i<taille;i++)
    for(double j=0;j<taille;j++)
      if((i-rayon)*(i-rayon)+(j-rayon)*(j-rayon)<rayon*rayon)cercle(i,j)=1;
      else cercle(i,j)=0;

  return cercle;
}

image image::elemTriangle(int taille){
   image sortie(taille,taille,1);
   for(double i=0;i<taille;i++){
     for(double j=i;j<taille;j++){
       sortie(i,j)=0;
     }
   }
   for(double i=1;i<taille;i++){
     for(double j=0;j<i;j++){
	     sortie(i,j)=1;
     }
   }
   return sortie;
}

image image::elemCroix(int taille,int epaisseur){
   image sortie(taille,taille,1);
   int a = taille - epaisseur;
   assert(a>0);
   assert(a%2 == 0);
   for(double i=0;i<taille;i++){
     for(double j=0;j<taille;j++){
       sortie(i,j)=0;
     }
   }
   int ind = (int)(taille/2.0+0.5) -1;
std::cout<<"ind : "<<ind<<endl;
std::cout<<"epaisseur : "<<epaisseur<<endl;
   // vertical //
   for(double i=0;i<taille;i++){
     for(double j=ind-epaisseur;j<=ind+epaisseur;j++){
	   sortie(i,j)=1;
     }
   }
    // horizontal //
   for(double i=ind-epaisseur;i<=ind+epaisseur;i++){
     for(double j=0;j<taille;j++){
	   sortie(i,j)=1;
     }
   }
   return sortie;
}

image image::elemStruct(geom geo,int taille){
    if(geo==carre){return elemCarre(taille);}
    if(geo==cercle){return elemCercle(taille);}
    if(geo==triangle){return elemTriangle(taille);}
}

//-----------------------------------------------------------------------------//
image* image::erosion(geom geo, int taille, bool Estdupliquer/*image elem_struct*/){
   image* sortie;
	if(!Estdupliquer){
	sortie=(*this).duplique_elemStruc_bord(taille/*elem_struct*/);
	}
	else{
	     sortie=(this);
	}
    image elem_struct = elemStruct(geo,taille);
   // On fait une copie
   image copie(*sortie);
   // On traite les bords
   int ind = (int)(elem_struct.hauteur/2.0+0.5);
   // boucle sur toute l'image
   for(int i=ind;i<sortie->hauteur-ind;i++){
	for(int j=ind;j<sortie->largeur-ind;j++){
		 // boucle sur l'image de l'�l�ment structurant
		 int minimum=copie(i,j);
		 int m=0;
		 for(int k=i-ind;k<ind+i-1;k++){
		      int n=0;
		      for(int l=j-ind;l<ind+j-1;l++){
			// On v�rifie qu'on est bien sur l'objet (�l�ment structurant)
			  if(elem_struct(m,n)==1){
			  minimum = min(minimum,copie(k,l)*elem_struct(m,n) );
			  }
			  n=n+1;
		      }
		 m=m+1;
		 }
	  (*sortie)(i,j) = minimum ;
	}
   }
return sortie;
}

// dupliquer sert a savoir si l'image a d�ja �tait dupliquer sur les bords par l'�l�ment structurant.
// Si celle-ci a �t� dupliqu�, il faut mettre cette variable a vrai sinon a faux
image* image::dilatation(geom geo, int taille, bool Estdupliquer/*image elem_struct*/){
	image* sortie;
	if(!Estdupliquer){
	sortie=(*this).duplique_elemStruc_bord(taille/*elem_struct*/);
	}
	else{
	     sortie=(this);
	}
	image elem_struct = elemStruct(geo,taille);
	image copie(*sortie);
	// On traite les bords
	int ind = (int)(elem_struct.hauteur/2.0+0.5);

	// boucle sur toute l'image
	for(int i=ind;i<sortie->hauteur-ind;i++){
		for(int j=ind;j<sortie->largeur-ind;j++){
			// boucle sur l'image de l'�l�ment structurant
			int maximum=copie(i,j);
			int m=0;
			for(int k=i-ind;k<ind+i-1;k++){
				int n=0;
				for(int l=j-ind;l<ind+j-1;l++){
				  // On v�rifie qu'on est bien sur l'objet (�l�ment structurant)
				    if(elem_struct(m,n)==1){
					maximum = max(maximum,copie(k,l)*elem_struct(m,n) );
				    }
					n=n+1;
				}
				m=m+1;
			}
			(*sortie)(i,j) = maximum ;
		}
	}
	return sortie;
}

//-----------------------------------------------------------------------------//
image* image::ouverture(geom g,int taille/*image elem_struct*/){
 image* im_erosion=erosion(g,taille,false);
 im_erosion->EcrireImagePGM("EROSION.pgm");
 image* sortie=im_erosion->dilatation(g,taille,true);
 return sortie;
}
//-----------------------------------------------------------------------------//
image* image::fermeture(geom g,int taille/*image elem_struct*/){
 image* im_dilatation=dilatation(g,taille,false);
 im_dilatation->EcrireImagePGM("DILATATION.pgm");
 image* sortie=im_dilatation->erosion(g,taille,true);
 return sortie;
}
//-----------------------------------------------------------------------------//


image& image::operator=(const image & im){
	valmax=im.valmax;
	if(buffer!=NULL){
		if(hauteur!=im.hauteur || largeur!=im.largeur){
			delete[] buffer;
			hauteur=im.hauteur;
			largeur=im.largeur;
			buffer=new int[hauteur*largeur];
		}
	}
	else{
		hauteur=im.hauteur;
		largeur=im.largeur;
		buffer=new int[hauteur*largeur];
	}

	int n=hauteur*largeur;
	for(int i=0;i<n;i++)buffer[i]=im.buffer[i];

	return (*this);
}



image& image::operator-(const image & im){
	assert(hauteur==im.hauteur);
	assert(largeur==im.largeur);
	image* sortie=new image(hauteur,largeur,0);
	for(int i=0;i<hauteur;i++){
	  for(int j=0;j<largeur;j++){
	      // std::cout<<"im : "<<(*this)(i,j);
	      // std::cout<<" | ouv: "<<im(i,j)<<endl;
	       (*sortie)(i,j)=(*this)(i,j)-im(i,j);
	       if((*sortie)(i,j)<0){(*sortie)(i,j)=0;};
	  }
	}
	sortie->updateValmax();
	return(*sortie);
}


bool image::max1dirLoc(int i,int j)const{
	int max=(*this)(i,j);
	if(((*this)(i-1,j)<max && max>(*this)(i+1,j)) ||
	   ((*this)(i,j-1)<max && max>(*this)(i,j+1)) ||
	   ((*this)(i+1,j+1)<max && max>(*this)(i-1,j-1)) ||
	   ((*this)(i+1,j-1)<max && max>(*this)(i-1,j+1)))return (*this)(i,j)!=0;

	return false;
}


bool image::inclus(const image & im,int i,int j)const{
	int ph=(im.hauteur-1)/2;
	int pl=(im.largeur-1)/2;

	int k,l;
	for(k=0;k<im.hauteur;k++){
		for(l=0;l<im.largeur;l++){
			if(im(k,l)!=0 && i+k-ph>=0 && i+k-ph<hauteur
			   && j+l-pl>=0 && j+l-pl<largeur && (*this)(i+k-ph,j+l-pl)==0)break;
		}
		if(l<im.largeur)break;
	}

	return l==im.largeur;
}

image* image::boulesMax(double** masque,int n,int seuil){
	bool fini=false;
	image* bin=new image((*this));
	bin->seuiller(seuil);
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if((*bin)(i,j)==255)(*bin)(i,j)=2;
		}
	}

	int ray=3;
	while(!fini){
		fini=true;
		image bl=boule(masque,n,ray);
		int pBl=(bl.hauteur-1)/2;

		for(int i=pBl;i<hauteur-pBl;i++){
			for(int j=pBl;j<largeur-pBl;j++){
				if((*bin)(i,j)==ray-1 && (*bin).inclus(bl,i,j)){
					fini=false;
					(*bin)(i,j)=ray;
				}
			}
		}

		ray++;
	}

	return bin;
}
