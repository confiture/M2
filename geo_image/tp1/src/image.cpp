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

image::image(const image & im,int i1,int i2,int j1,int j2){
	valmax=im.valmax;
	hauteur=i2-i1+1;
	largeur=j2-j1+1;
	buffer=new int[hauteur*largeur];
	for(int i=i1;i<=i2;i++){
		for(int j=j1;j<=j2;j++){
			(*this)(i-i1,j-j1)=im(i,j);
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



int** image::connexite4()const{
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

	//finalUpdateEquiv(equiv,id);

	std::cout<<"corres[97][0] equiv a "<<equiv[corres[97][0]]<<std::endl;
	std::cout<<"corres[98][0] equiv a "<<equiv[corres[98][0]]<<std::endl;
	//std::cout<<"corres[99][2] equiv a "<<equiv[corres[99][1]]<<std::endl;

	//mise à jour des groupes grâce au tableau des équivalences
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			//cout<<equiv[corres[i][j]]<<endl;
			//std::cout<<" i "<<i<<" j "<<j<<"corres[i][j]"<<corres[i][j]<<endl;
			//if(corres[i][j]==1)exit(-1);
			if(corres[i][j]!=-1){corres[i][j]=equiv[corres[i][j]]+1;} // +1 pour rammener -1 à 0
			else{corres[i][j]=0;}
		}
	}

	return corres;
}

int** image::connexite8()const{
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


void image::dispCompConn(const char* fic)const{
	set<int> groupes;
	image sortie(hauteur,largeur,255);
	image im_s(*this);
	im_s.seuiller(50);

	im_s.EcrireImagePGM("seuillage.pgm");

	int** conn=im_s.connexite8();

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(conn[i][j]!=0){sortie(i,j)=100+conn[i][j];groupes.insert(conn[i][j]);}
			else{sortie(i,j)=0;}
			//std::cout<<conn[i][j]<<endl;
		}
	}

	sortie.EcrireImagePGM(fic);
	cout<<"nombre d'objet "<<groupes.size()<<endl;
}

int image::nbConnCom(int nconn,int seuil){
	set<int> groupes;
	image im_s(*this);
	im_s.seuiller(seuil);

	int** conn;
	if(nconn==8){
		conn=im_s.connexite8();
	}
	else{
		conn=im_s.connexite4();
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

void image::writePgmItems(char * itemsName,int seuil){
	string name(itemsName);
	set<int> groupes;
	image im_s(*this);
	im_s.seuiller(seuil);
	image neg(*this);
	neg.negatif();

	int** conn=im_s.connexite4();

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

		image temp((*this),i1-1,i2+1,j1-1,j2+1);
		image tempNeg(neg,i1-1,i2+1,j1-1,j2+1);
		//image tempNeg((*this),i1-1,i2+1,j1-1,j2+1);
		tempNeg.negatif();
		item objet;
		objet.pIm=&temp;
		objet.epaisseur=666;
		objet.nbTrous=tempNeg.nbConnCom(8,tempNeg.valmax-seuil)-1;

		string name(itemsName);
		ostringstream ss;
		ss<<k;
		name+=ss.str();
		name+=".pgm";
		objet.writePgmItem(name.c_str());
		string strneg("neg");
		strneg+=name;
		const char * tt=strneg.c_str();
		//tempNeg.EcrireImagePGM(tt);
		//tempNeg.dispCompConn(tt);

		image tempNegSeuil(tempNeg);
		tempNegSeuil.seuiller(tempNeg.valmax-seuil);
		//tempNegSeuil.EcrireImagePGM(tt);

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


image* image::dilatation(image elem_struct) const{
   image* sortie=(*this).duplique_elemStruc_bord(elem_struct);
    
   // On traite les bords
   int ind = (int)(elem_struct.hauteur/2.0+0.5);

   // boucle sur toute l'image
   for(int i=ind;i<sortie->hauteur-ind;i++){
	for(int j=ind;j<sortie->largeur-ind;j++){
		 // boucle sur l'image de l'�l�ment structurant
		 cout<<"ici 1"<<endl;
		 int maximum=(*sortie)(i,j);
		 cout<<"non 1"<<endl;
		 
		 int m=0;
		 for(int k=i-ind;k<ind+i-1;k++){
		      int n=0;
		      for(int l=j-ind;l<ind+j-1;l++){
			  cout<<"ici 2"<<endl;
			  maximum = max(maximum,(*sortie)(k,l)*elem_struct(m,n) );
			  cout<<"non 2"<<endl;
			  n=n+1;
		      }
		 m=m+1;
		 }
	  (*sortie)(i,j) = maximum ;	
	}
   }
return sortie;
}
