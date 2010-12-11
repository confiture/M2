#include "image.hpp"

using namespace std;


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


image::image(const image & im,int i1,int j1,int n,int m){
	valmax=im.valmax;
	hauteur=n;
	largeur=m;
	buffer=new double[hauteur*largeur];
	for(int i=i1;i<=i1+n-1;i++){
		for(int j=j1;j<=j1+m-1;j++){
			(*this)(i-i1,j-j1)=im(i,j);
		}
	}
}


double& image::operator()(int i,int j){
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);
	return buffer[i*largeur+j];
}

double image::operator()(int i,int j)const{
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);
	return buffer[i*largeur+j];
}

int image::getHauteur(){
	return hauteur;
}

int image::getLargeur(){
	return largeur;
}

int image::getValmax(){
	return valmax;
}

void image::updateValmax(){
	for(int i=0;i<hauteur*largeur;i++){
		if(buffer[i]>valmax)valmax=buffer[i];
	}
}

image::image(const image & im1, const image & im2){
	// Initialisation de la hauteur maximal et de la valmax
	int max_hauteur = im1.hauteur;
	if(max_hauteur<im2.hauteur){max_hauteur=im2.hauteur;}
	double valmax = im1.valmax;
	if(valmax<im2.valmax){valmax=im2.valmax;}

	this->buffer = new double[max_hauteur* (im1.largeur+im2.largeur)];
	this->hauteur = max_hauteur;
	this->largeur = im1.largeur + im2.largeur;
	this->valmax=valmax;

	// Boucles sur la 1ère image
	for(int i=0;i<im1.hauteur;i++){
		for(int j=0;j<im1.largeur;j++){
			(*this)(i,j) = im1(i,j);
		}
	}
	// Boucles sur la 2ème image
	for(int i=0;i<im2.hauteur;i++){
		for(int j=0;j<im2.largeur;j++){
			(*this)(i,j+im1.largeur) = im2(i,j);
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

int image::EcrireImagePGM(const char* nomFichier)const{
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


double image::median(int p,int pix_i,int pix_j){
	int pos = (p-1)/2;
	int k,l;
	double tab[p*p];

	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			tab[(k-pix_i+pos)*p + (l+pos-pix_j)]=(*this)(k,l);
		}
	}

	return medianTab(tab,p*p);
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

image* image::medianFilter(int p){
	int n=p/2;
	image* sortie=new image(hauteur,largeur,0);
	double max=0;

	for(int i=n;i<hauteur-n;i++){
		for(int j=n;j<largeur-n;j++){
			(*sortie)(i,j)=this->median(p,i,j);
			if(sortie->valmax<(*sortie)(i,j))sortie->valmax=(*sortie)(i,j);
		}
	}

	return sortie;
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
	double pixC = (*this)(i,j);
	if((*this)(i,j-1)<pixC && (*this)(i+1,j-1)<=pixC
	   && (*this)(i+1,j)<=pixC && (*this)(i+1,j+1)<=pixC
	   && (*this)(i,j+1)<=pixC && (*this)(i-1,j+1)<pixC
	   && (*this)(i-1,j)<pixC && (*this)(i-1,j-1)<pixC){
		return true;
	}
	return false;
}

std::list<pixel> image::best_interest_points(int n,int winn,int winp)const{
	image im(*this);
	im.elim_neg();

	image temp(im);
	temp.recadre(0,255);
	temp.EcrireImagePGM("temprec.pgm");

	list<pixel> lpixel;
	for(int i=winn;i<im.hauteur-winn;i++){
		for(int j=winp;j<im.largeur-winp;j++){
			//cout<<i<<" "<<j<<endl;
			if(im.maxLoc(i,j)){
				//cout<<"maxloc"<<endl;
				pixel p(i,j,im(i,j));
				lpixel.push_back(p);
			}
		}
	}

	lpixel.sort();

	list<pixel> lpixsortie;
	std::list<pixel>::iterator it=lpixel.end();
	it--;
	for(int i=0;i<n;i++){
		if(it==lpixel.begin())break;
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
	int epais=1;
	int grand=4;
//int epais=5;
//int grand=15;
	//la verticale
	//cout<<"i "<<i<<endl;
	//cout<<"j "<<j<<endl;
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


void image::drawLine(int xi,int yi,int xf,int yf,int color){
	//if(xi==xf){
	//	for(int j=yi;j<=yf;j++)(*this)(xi,j)=color;
	//	return;
	//}

	/*
	int x,y ;
	double a,b ;
	a =(double) (yf-yi)/(xf-xi) ;
	b = yi - a * xi ;
	for ( x = xi ; x <= xf ; x++ ) {
		y =(int) (a * x + b) ;
		(*this)(x,y)=color;
	}
	*/

	double val;
	double realVal;
	double erreur;
	double a=(double)(xf-xi)/(yf-yi);
	double b=xi-a*yi;
	//int x=xi;
	for(int y=yi;y<=yf;y++){
		val=a*y+b;
		(*this)((int)val,y)=255;
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

void image::GaussFilterObj(){
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

	(*this)=(*sortie);

	delete sortie;
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


double image::ssd(int i1,int j1,const image & comp,int i2,int j2,int n, int p)const{
	double res=0;
	for(int i=-n;i<=n;i++){
		for(int j=-p;j<=p;j++){
			res+=((*this)(i1+i,j1+j)-comp(i2+i,j2+j))*((*this)(i1+i,j1+j)-comp(i2+i,j2+j));
		}
	}

	return res;
}

double image::moyenne(int i_pix,int j_pix, int n, int p)const{
	double moy = 0 ;
	int nb_pix = 0;
	for(int i=-n;i<=n;i++){
		for(int j=-p;j<=p;j++){
			moy+=(*this)(i_pix+i,j_pix+j);
			nb_pix = nb_pix + 1;
		}
	}
	moy = moy/(double)nb_pix;
	return moy;
}

double image::sigma(int i_pix,int j_pix, int n, int p)const{
	double som = 0;
	double res = 0;
	double moy = moyenne(i_pix,j_pix,n,p);
	for(int i=-n;i<=n;i++){
		for(int j=-p;j<=p;j++){
			som+=((*this)(i_pix+i,j_pix+j)-moy)*((*this)(i_pix+i,j_pix+j)-moy);
		}
	}
	double fact = 1.0 / ( (double)(2*n+1)*(2*p+1) );
	res = sqrt(fact * som);
	return res;
}


double image::zncc(int i1,int j1,const image & comp,int i2,int j2,int n, int p)const{
	double som = 0;
	double res = 0;
	double moy1 = moyenne(i1,j1,n,p);
	double moy2 = comp.moyenne(i2,j2,n,p);
	double sigma1 = sigma(i1,j1,n,p);
	double sigma2 = comp.sigma(i2,j2,n,p);

	for(int i=-n;i<=n;i++){
		for(int j=-p;j<=p;j++){
			som+=((*this)(i1+i,j1+j)-moy1) * ((comp(i2+i,j2+j)-moy2));
		}
	}

	res = (1.0/(sigma1*sigma2)) * som ;
	return res;
}

pixel** image::matchPoints(const image & comp,int nbpoints,int winn,int winp,
                           double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{
	pixel** corres=new pixel*[nbpoints];
	for(int i=0;i<nbpoints;i++)corres[i]=new pixel[2];


	double currentScore;
	pixel Pix;

	std::list<pixel> thisBest=best_interest_points(nbpoints,winn,winp);
	std::list<pixel> compBest=comp.best_interest_points(nbpoints,winn,winp);

	std::list<pixel>::iterator thisIt=thisBest.begin();
	std::list<pixel>::iterator thisItEnd=thisBest.end();
	std::list<pixel>::iterator compItEnd=compBest.end();
	int i=0;
	for(thisIt;thisIt!=thisItEnd;thisIt++){
		double extrScore;
		if(sim){
			extrScore=-numeric_limits<double>::infinity();
			//extrScore=-10000000;
		}
		else{extrScore=numeric_limits<double>::infinity();}

		std::list<pixel>::iterator compIt=compBest.begin();
		for(compIt;compIt!=compItEnd;compIt++){

			currentScore=(this->*score)(thisIt->_i,thisIt->_j,comp,compIt->_i,compIt->_j,winn,winp);
			//cout<<"currentScore" <<currentScore<<endl;
			if(currentScore<extrScore && !sim || currentScore>extrScore && sim){
				extrScore=currentScore;
				Pix._i=compIt->_i;Pix._j=compIt->_j;
				Pix._val=(compIt->_val);
			}
		}

		corres[i][0]=(*thisIt);
		corres[i][1]=Pix;
		std::cout<<thisIt->_i<<" "<<thisIt->_j<<" val "<<thisIt->_val<<"-->"<<Pix._i<<" "<<Pix._j<<" val"
			" val "<<Pix._val<<" score "<<currentScore<<std::endl;

		i++;
	}

	return corres;
}

pixel** image::dblMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                              double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	pixel** corres1=matchPoints(comp,nbpoints,winn,winp,score,sim);
	pixel** corres2=comp.matchPoints(*this,nbpoints,winn,winp,score,sim);

	pixel** corres=new pixel*[nbpoints];

	int k=0;
	for(int i=0;i<nbpoints;i++){
		for(int j=0;j<nbpoints;j++){
			if(corres1[i][0].equalPos(corres2[j][1]) && corres2[j][0].equalPos(corres1[i][1])){
				corres[k]=new pixel[2];
				corres[k][0]=corres1[i][0];
				corres[k][1]=corres1[i][1];
				k++;
				break;
			}
		}
	}
	corres[k]=NULL;

	for(int i=0;i<nbpoints;i++){
		delete[] corres1[i];
		delete[] corres2[i];
	}
	delete[] corres1;
	delete[] corres2;

	return corres;
}

image* image::drawMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                           double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	pixel** corres=matchPoints(comp,nbpoints,winn,winp,score,sim);

	image* join=new image(*this,comp);

	for(int i=0;i<nbpoints;i++){
		join->drawLine(corres[i][0]._i,corres[i][0]._j,
		               corres[i][1]._i,largeur+corres[i][1]._j,255);
	}

	return join;
}

image* image::drawDblMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                       double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	pixel** corres=dblMatchPoints(comp,nbpoints,winn,winp,score,sim);

	image* join=new image(*this,comp);

	for(int i=0;i<nbpoints;i++){
		if(corres[i]!=NULL){
			join->drawLine(corres[i][0]._i,corres[i][0]._j,
			               corres[i][1]._i,largeur+corres[i][1]._j,255);
		}
		else{
			break;
		}
	}

	return join;
}

image* image::makeDepth(const image & comp,int winn,int winp,
                        double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	image* sortie=new image(hauteur,largeur,0);
	double extrScore;
	double currentScore;
	int jCorres;

	//(*this).EcrireImagePGM("temp1.pgm");
	//comp.EcrireImagePGM("temp2.pgm");
	for(int i=winn;i<hauteur-winn;i++){
		for(int j=winp;j<largeur-winp;j++){
			if(sim){
				extrScore=-numeric_limits<double>::infinity();
			}
			else{
				extrScore=numeric_limits<double>::infinity();
			}

			for(int jj=winp;jj<largeur-winp;jj++){
				currentScore=(this->*score)(i,j,comp,i,jj,winn,winp);
				if(currentScore<extrScore && !sim || currentScore>extrScore && sim){
					extrScore=currentScore;
					jCorres=jj;
				}
			}

			if(j==jCorres){(*sortie)(i,j)=1.1;}
			else{(*sortie)(i,j)=1.0/(absf(j-jCorres));
				assert((*sortie)(i,j)>0);}

		}
	}

	sortie->valmax = 1.1;

	return sortie;
}

int** image::matchProfPoints(const image & comp,int winn,int winp,
                        double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	int** corres=new int*[hauteur];
	for(int i=0;i<hauteur;i++)corres[i]=new int[largeur];

	double extrScore;
	double currentScore;
	int jCorres;

	(*this).EcrireImagePGM("temp1.pgm");
	comp.EcrireImagePGM("temp2.pgm");
	for(int i=winn;i<hauteur-winn;i++){
		for(int j=winp;j<largeur-winp;j++){
			if(sim){
				extrScore=-numeric_limits<double>::infinity();
			}
			else{
				extrScore=numeric_limits<double>::infinity();
			}

			for(int jj=winp;jj<largeur-winp;jj++){
				currentScore=(this->*score)(i,j,comp,i,jj,winn,winp);
				if(currentScore<extrScore && !sim || currentScore>extrScore && sim){
					extrScore=currentScore;
					jCorres=jj;
				}
			}

			corres[i][j]=jCorres;
		}
	}

	return corres;
}

image* image::dblMatchProfPoints(const image & comp,int winn,int winp,
                         double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const{

	int** corresg=this->matchProfPoints(comp,winn,winp,score,sim);
	int** corresd=comp.matchProfPoints((*this),winn,winp,score,sim);
	image* sortie=new image(hauteur,largeur,0);

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if(corresd[i][corresg[i][j]]!=j){(*sortie)(i,j)=0;}
			else{
				if(j==corresg[i][j]){(*sortie)(i,j)=1.1;}
				else{(*sortie)(i,j)=1.0/(absd(j-corresg[i][j]));assert((*sortie)(i,j)>0);}
			}
		}
		std::cout<<i/((double)hauteur)*100<<"%"<<std::endl;
		std::cout.flush();
	}

	for(int i=0;i<hauteur;i++){
		delete[] corresd[i];
		delete[] corresg[i];
	}
	delete[] corresd;
	delete[] corresg;

	return sortie;
}

void image::transInterpol(double di,double dj){
	int deci;
	if(di>0){deci=(int)(di+0.5);}
	else{deci=(int)(di-0.5);}

	int decj;
	if(dj>0){decj=(int)(dj+0.5);}
	else{decj=(int)(dj-0.5);}

	image trans=image(hauteur,largeur,0);

	int i_inf=0;
	int i_sup=hauteur;
	int j_inf=0;
	int j_sup=largeur;

	if(di>0){i_sup-=deci;}
	else if(di<0){i_inf=-deci;}
	if(dj>0){j_sup-=decj;}
	else if(dj<0){j_inf=-decj;}

	for(int i=i_inf;i<i_sup;i++){
		for(int j=j_inf;j<j_sup;j++){
			trans(i+deci,j+decj)=(*this)(i,j);
		}
	}

	trans.valmax=valmax;

	trans.EcrireImagePGM("temp.pgm");

	(*this)=trans;

	double decRel_i=di;
	double incr=-0.5;
	if(di<0)incr=0.5;
	while(absd(decRel_i+incr)<=0.5)decRel_i+=incr;

	double decRel_j=dj;
	incr=-0.5;
	if(dj<0)incr=0.5;
	while(absd(decRel_j+incr)<=0.5)decRel_j+=incr;

	int cas;
	if(decRel_i>=0 && decRel_j>=0)cas=0;//en bas à droite
	if(decRel_i< 0 && decRel_j>=0)cas=1;//en haut à droite
	if(decRel_i< 0 && decRel_j< 0)cas=2;//en haut à gauche
	if(decRel_i>=0 && decRel_j< 0)cas=3;//en bas à gauche

	double d0,d1,d2,d3;
	switch(cas){
	case 0:
		d0=sqrt(decRel_i*decRel_i+decRel_j*decRel_j);
		d1=sqrt((decRel_i-1)*(decRel_i-1)+decRel_j*decRel_j);
		d2=sqrt((decRel_i-1)*(decRel_i-1)+(decRel_j-1)*(decRel_j-1));
		d3=sqrt(decRel_i*decRel_i+(decRel_j-1)*(decRel_j-1));
		break;
	case 1:
		d0=sqrt(decRel_i*decRel_i+decRel_j*decRel_j);
		d1=sqrt(decRel_i*decRel_i+(decRel_j-1)*(decRel_j-1));
		d2=sqrt((decRel_i+1)*(decRel_i+1)+(decRel_j-1)*(decRel_j-1));
		d3=sqrt((decRel_i+1)*(decRel_i+1)+decRel_j*decRel_j);
		break;
	case 2:
		d0=sqrt(decRel_i*decRel_i+decRel_j*decRel_j);
		d1=sqrt((decRel_i+1)*(decRel_i+1)+decRel_j*decRel_j);
		d2=sqrt((decRel_i+1)*(decRel_i+1)+(decRel_j+1)*(decRel_j+1));
		d3=sqrt(decRel_i*decRel_i+(decRel_j+1)*(decRel_j+1));
		break;
	case 3:
		d0=sqrt(decRel_i*decRel_i+decRel_j*decRel_j);
		d1=sqrt(decRel_i*decRel_i+(decRel_j+1)*(decRel_j+1));
		d2=sqrt((decRel_i-1)*(decRel_i-1)+(decRel_j+1)*(decRel_j+1));
		d3=sqrt((decRel_i-1)*(decRel_i-1)+decRel_j*decRel_j);
		break;
	}

	double sumDist=d0+d1+d2+d3;
	switch(cas){
	case 0:
		for(int i=1;i<hauteur-1;i++){
			for(int j=1;j<largeur-1;j++){
				(*this)(i,j)=(trans(i,j)*d0+trans(i+1,j)*d1+trans(i+1,j+1)*d2+trans(i,j+1)*d3)/sumDist;
			}
		}
		break;
	case 1:
		for(int i=1;i<hauteur-1;i++){
			for(int j=1;j<largeur-1;j++){
				(*this)(i,j)=(trans(i,j)*d0+trans(i,j+1)*d1+trans(i-1,j+1)*d2+trans(i-1,j)*d3)/sumDist;
			}
		}
		break;
	case 2:
		for(int i=1;i<hauteur-1;i++){
			for(int j=1;j<largeur-1;j++){
				(*this)(i,j)=(trans(i,j)*d0+trans(i-1,j)*d1+trans(i-1,j-1)*d2+trans(i,j-1)*d3)/sumDist;
			}
		}
		break;
	case 3:
		for(int i=1;i<hauteur-1;i++){
			for(int j=1;j<largeur-1;j++){
				(*this)(i,j)=(trans(i,j)*d0+trans(i,j-1)*d1+trans(i+1,j*1)*d2+trans(i+1,j)*d3)/sumDist;
			}
		}
		break;
	}

	EcrireImagePGM("temp2.pgm");

	updateValmax();
}

double* image::Kanade(const image & T,int cornerI,int cornerJ,double eps)const{
	image Tom=T;
	int h=T.hauteur;
	int w=T.largeur;
	image Io((*this),cornerI,cornerJ,h,w);

	Io.GaussFilterObj();

	Io.EcrireImagePGM("Io.pgm");

	assert(T.hauteur==Io.hauteur);
	assert(T.largeur==Io.largeur);
	image* gradJ = Io.contourX();
	image* gradI = Io.contourY();
	image gradJ2 = (*gradJ)*(*gradJ);
	image gradI2 = (*gradI)*(*gradI);
	image gradIJ = (*gradI)*(*gradJ);

	double** mat = new double*[2];
	mat[0] = new double[2];mat[1] = new double[2];
	mat[0][0]=mat[0][1]=mat[1][0]=mat[1][1]=0;

	int n=Io.hauteur*Io.largeur;
	for(int i=1;i<h-1;i++){
		for(int j=1;j<w-1;j++){
			mat[0][0] += gradI2(i,j);
			mat[0][1] += gradIJ(i,j);
			mat[1][0] += gradIJ(i,j);
			mat[1][1] += gradJ2(i,j);
		}
	}

	double** matC = invMat22(mat);
	delete[] mat[0];delete mat[1];delete[] mat;

	double delta[2];
	double vec[2];
	double *  Delta = new double[2];
	Delta[0]=Delta[1]=0;

	int k=0;
	do{
		Tom.GaussFilterObj();
		vec[0]=vec[1]=0;
		for(int i=0;i<n;i++){
			vec[0]+=gradI->buffer[i]*(Tom.buffer[i]-Io.buffer[i]);
			vec[1]+=gradJ->buffer[i]*(Tom.buffer[i]-Io.buffer[i]);
		}

		matMVec22(matC,vec,delta);

		Delta[0]+=delta[0];
		Delta[1]+=delta[1];

		Tom=T;
		Tom.transInterpol(Delta[0],Delta[1]);

		assert(Tom.hauteur==Io.hauteur);
		assert(Tom.largeur==Io.largeur);

		std::ostringstream ss;
		ss<<k<<".pgm";
		std::string TomS("tom");
		TomS+=ss.str();

		image erreur(Io-Tom);
		erreur.recadre(0,255);

		assert(erreur.hauteur==Io.hauteur);
		assert(erreur.largeur==Io.largeur);

		erreur.EcrireImagePGM(TomS.c_str());

// 		cout<<delta[0]<<" "<<delta[1]<<endl;
// 		cout<<Delta[0]<<" "<<Delta[1]<<endl<<"============="<<endl;

		k++;
	}
	while(k<50 && (absd(delta[0])>=eps || absd(delta[1])>=eps));

	return Delta;
}

image& image::operator=(const image & im){
	valmax=im.valmax;
	if(buffer!=NULL){
		if(hauteur!=im.hauteur || largeur!=im.largeur){
			delete[] buffer;
			hauteur=im.hauteur;
			largeur=im.largeur;
			buffer=new double[hauteur*largeur];
		}
	}
	else{
		hauteur=im.hauteur;
		largeur=im.largeur;
		buffer=new double[hauteur*largeur];
	}

	int n=hauteur*largeur;
	for(int i=0;i<n;i++)buffer[i]=im.buffer[i];

	return (*this);
}

image operator-(const image & im1,const image & im2){
    image res(im1.hauteur,im1.largeur,0);
    for(int i=0;i<im1.hauteur*im1.largeur;i++){
      res.buffer[i]=im1.buffer[i]-im2.buffer[i];
    }

    res.updateValmax();
    return res;
}

image operator*(const image & im1,const image & im2){
	assert(im1.hauteur==im2.hauteur);
	assert(im1.largeur==im2.largeur);
	image im(im1.hauteur,im1.largeur,0);
	int n=im1.hauteur*im1.largeur;

	for(int i=0;i<n;i++)im.buffer[i]=im1.buffer[i]*im2.buffer[i];

	im.updateValmax();
	return im;
}

double residu(const image & im1,const image & im2){
	double res=0;
	image diff=im1-im2;
	int n=im1.hauteur*im1.largeur;
	for(int i=0;i<n;i++)res+=diff.buffer[i];

	return res;
}

