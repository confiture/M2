#include "image.hpp"

using namespace std;
//TEST//
double& image::operator()(int i,int j){
	return buffer[i*largeur+j];
}

double image::operator()(int i,int j)const{
	return buffer[i*largeur+j];
}

void image::updateValmax(){
	for(int i=0;i<hauteur*largeur;i++){
		if(buffer[i]>valmax)valmax=buffer[i];
	}
}

image::image(int hauteur, int largeur,int valmax){
	this->buffer = new double[hauteur*largeur];
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
	for(int i=0;i<hauteur*largeur;i++)buffer[i]=valmax;
}

image::image(const image & im){
	this->buffer = new double[im.hauteur*im.largeur];
	this->hauteur=im.hauteur;
	this->largeur=im.largeur;
	this->valmax=im.valmax;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=im(i,j);
		}
	}
}

image::image(char* nomFichier){
	/* Ouverture */
	FILE* ifp = fopen(nomFichier,"r");

	/* Lecture du Magic number */
	int ich1 = getc( ifp );
	if ( ich1 == EOF )
		std::cerr<<"EOF / read error reading magic number"<<std::endl;
	int ich2 = getc( ifp );
	if ( ich2 == EOF )
		std::cerr<<"EOF / read error reading magic number"<<std::endl;
	/*if(ich2 != '2'){
	  std::cerr<<" wrong ifp format "<<std::endl;
	  exit(-1);
	  }*/


	/*Lecture des dimensions*/
	largeur = pm_getint( ifp );
	hauteur = pm_getint( ifp );
	valmax = pm_getint( ifp );

	buffer = new double[hauteur*largeur];

	/*Lecture*/
	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			(*this)(i,j)=pm_getrawbyte(ifp);
			//
			//cerr<<(*this)(i,j)<<" ";
			//cerr<<buffer[i*largeur+j];
			//exit(-1);
		}
	}
	/* fermeture */
	fclose(ifp);
}

int image::EcrireImagePGM(char* nomFichier)const{
	char str[50];
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
			//cerr<<(i,j)<<" ";
			// exit(-1);
			int pix=(int)((*this)(i,j)+0.5);
			os<<pix;
			//cerr<<(*this)(i,j)<<" ";
			os<<std::endl;
			//sprintf(str,"%u ",(*this)(i,j));
			//os<<str;
			//cerr<<str<<" ";
		}
	}
	fb.close();
	return 1;

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

/**
 *On applique le filtre filter de dimensions p*p sur la positions (pix_i,pix_j)
 *à partir de l'image courante sur l'image de sortie passée en paramètre.
 */
void image::ApplyFilter(int p,double** filter,int pix_i,int pix_j,image & sortie){
	int pos = (p-1)/2;
	int k,l;
	double val=0;
	sortie(pix_i,pix_j)=0;
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			val+=(double)(*this)(k,l)*filter[k+pos-pix_i][l+pos-pix_j];
		}
	}
	sortie(pix_i,pix_j)=val;
}



image* image::HarrisFilter(double alpha){
	image* gauss=GaussFilter();
	image* Ix2=gauss->contourX2();
	image* Iy2=gauss->contourY2();
	image* Ixy=gauss->contourXY();

	image* Ix2g=Ix2->GaussFilter();
	image* Iy2g=Iy2->GaussFilter();
	image* Ixyg=Ixy->GaussFilter();

	image tempix2g(*Ix2g);
	image tempiy2g(*Iy2g);
	image temp(*Ixyg);
	tempix2g.recadre(0,255);
	tempiy2g.recadre(0,255);
	temp.recadre(0,255);
	tempix2g.EcrireImagePGM("Ix2g.pgm");
	tempiy2g.EcrireImagePGM("Iy2g.pgm");
	temp.EcrireImagePGM("Ixyg.pgm");

	delete gauss;delete Ix2;delete Iy2;delete Ixy;

	image* sortie=new image(hauteur,largeur,0);
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*Ix2g)(i,j)*(*Iy2g)(i,j)-(*Ixyg)(i,j)*(*Ixyg)(i,j)-alpha*((*Ix2g)(i,j)+(*Iy2g)(i,j))*((*Ix2g)(i,j)+(*Iy2g)(i,j));
			//if((*sortie)(i,j)<0)(*sortie)(i,j)=0;
			if((*sortie)(i,j)>sortie->valmax)sortie->valmax=(*sortie)(i,j);
		}
	}

	return sortie;
}

void image::elim_neg(){
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if((*this)(i,j)<0)(*this)(i,j)=0;
		}
	}
}

bool image::maxLoc(int i,int j){
	int pixC = (*this)(i,j);
	if((*this)(i,j-1)<pixC && (*this)(i+1,j-1)<=pixC
	   && (*this)(i+1,j)<=pixC && (*this)(i+1,j+1)<=pixC
	   && (*this)(i,j+1)<=pixC && (*this)(i-1,j+1)<pixC
	   && (*this)(i-1,j)<pixC && (*this)(i-1,j-1)<pixC){
		return true;
	}
	return false;
}

std::list<pixel> image::best_interest_points(int n)const{
	image im(*this);
	im.elim_neg();

	list<pixel> lpixel;
	for(int i=1;i<im.hauteur-1;i++){
		for(int j=1;j<im.largeur-1;j++){
			if(im.maxLoc(i,j)){
				pixel p(i,j,im(i,j));
				lpixel.push_back(p);
			}
		}
	}

	lpixel.sort();

	list<pixel> lpixsortie;
	std::list<pixel>::iterator it=lpixel.end();
	for(int i=0;i<n;i++){
		lpixsortie.push_back(*it);
		it--;
	}

	return lpixsortie;
}

void image::drawPts(const std::list<pixel> & Lpix, int col){
	std::list<pixel>::const_iterator it=Lpix.begin();
	for(it;it!=Lpix.end();it++){
		drawCross(it->_i,it->_j,col);
	}
}


void image::drawCross(int i,int j,int color){
	int epais=0.5;
	int grand=2;

	//la verticale
	for(int ii=i-grand;ii<=i+grand;ii++){
		for(int jj=j-epais;jj<=j+epais;jj++){
			if(ii>=0 && ii<hauteur && jj<largeur && jj>=0){
					(*this)(ii,jj)=color;
				}
		}
	}

	//l'horizontale
	for(int jj=j-grand;jj<=j+grand;jj++){
		for(int ii=i-epais;ii<=i+epais;ii++){
			if(ii>=0 && ii<hauteur && jj<largeur && jj>=0){
					(*this)(ii,jj)=color;
				}
		}
	}
}

image* image::GaussFilter(){
	image* sortie = new image(hauteur,largeur,0);
	double** filter=new double*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new double[3];
	filter[0][0]=filter[0][2]=filter[2][0]=filter[2][2]=1.0/16;
	filter[0][1]=filter[1][0]=filter[1][2]=filter[2][1]=2.0/16;
	filter[1][1]=4.0/16;

	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
			if(sortie->valmax<(*sortie)(i,j))sortie->valmax=(*sortie)(i,j);
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;


	return sortie;
}

image* image::contourX(){
	image* sortie = new image(hauteur,largeur,0);
	double** filter=new double*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new double[3];
	filter[0][0]=filter[2][0]=-1.0;
	filter[0][2]=filter[2][2]=1.0;
	filter[1][0]=-2.0;
	filter[1][2]=2.0;
	filter[0][1]=filter[1][1]=filter[2][1]=0.0;
	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
			//cerr<<sortie[i*cols+j]<<endl;
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;

	sortie->updateValmax();
	return sortie;
}

image* image::contourX2(){
	image* sortie=contourX();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortie)(i,j)*(*sortie)(i,j);
		}
	}

	sortie->updateValmax();
	return sortie;
}

image* image::LissageGaussienX2(){
	image* Ix2=contourX2();
	image* sortie = Ix2->GaussFilter();

	delete Ix2;
	return sortie;
}

image* image::LissageGaussienY2(){
	image* Iy2=contourY2();
	image* sortie = Iy2->GaussFilter();
	return sortie;
}

image* image::LissageGaussienXY(){
	image* Ixy=contourXY();
	image* sortie = Ixy->GaussFilter();
	return sortie;
}
image* image::contourY2(){
	image* sortie=contourY();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortie)(i,j)*(*sortie)(i,j);
		}
	}

	sortie->updateValmax();
	return sortie;
}

image* image::contourXY(){
	image* sortie = new image(hauteur,largeur,0);
	image* sortieX=contourX();
	image* sortieY=contourY();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortieX)(i,j)*(*sortieY)(i,j);
		}
	}

	sortie->updateValmax();
	return sortie;
}

image* image::contourY(){
	image* sortie = new image(hauteur,largeur,0);
	double** filter=new double*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new double[3];
	filter[0][0]=filter[0][2]=-1.0;
	filter[2][0]=filter[2][2]=1.0;
	filter[0][1]=-2.0;
	filter[2][1]=2.0;
	filter[1][0]=filter[1][1]=filter[1][2]=0.0;
	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
			//cerr<<sortie[i*cols+j]<<endl;
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;

	sortie->updateValmax();
	return sortie;
}


double image::ssd(int i1,int j1,const image & comp,int i2,int j2,int n, int p){
	double res=0;
	for(int i=-n;i<=n;i++){
		for(int j=-p;j<=p;j++){
			res+=((*this)(i1+i,j1+j)-comp(i2+i,j2+j))*((*this)(i1+i,j1+j)-comp(i2+i,j2+j));
		}
	}

	return res;
}

void image::matchPoints(const image & comp,int nbpoints,int winn,int winp,
                        double (*score)(int,int,const image &,int,int,int,int))const{
	double currentScore;
	pixel minPix;

	std::list<pixel> thisBest=best_interest_points(nbpoints);
	std::list<pixel> compBest=comp.best_interest_points(nbpoints);

	std::list<pixel>::iterator thisIt=thisBest.begin();
	std::list<pixel>::iterator thisItEnd=thisBest.end();
	std::list<pixel>::iterator compItEnd=compBest.end();

	for(thisIt;thisIt!=thisItEnd;thisIt++){
		double minScore=1000000;
		std::list<pixel>::iterator compIt=compBest.begin();
		for(compIt;compIt!=compItEnd;compIt++){
			minScore=numeric_limits<double>::infinity();
			currentScore=score(thisIt->_i,thisIt->_j,comp,compIt->_i,compIt->_j,winn,winp);
			if(currentScore<minScore){
				minScore=currentScore;
				minPix._i=compIt->_i;minPix._j=compIt->_j;
				minPix._val=minScore;
			}
		}
		std::cout<<thisIt->_i<<" "<<thisIt->_j<<"-->"<<minPix._i<<" "<<minPix._j<<" "
		         <<minPix._val<<std::endl;
	}
}

