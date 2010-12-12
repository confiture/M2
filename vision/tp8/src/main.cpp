#include "image.hpp"

using namespace std;

void testExtract(char * fic){
	image im(fic);
	image extract(im,50,50,100,50);

	extract.EcrireImagePGM("result.pgm");
}

void testOpEg(char * fic){
	image im(fic);
	image egal=im;

	egal.EcrireImagePGM("resultEg.pgm");
}

void testBidon(){
	double a=-1.7;
	std::cout<<(int)a<<std::endl;
}

void testTransInter(double i,double j,char * fic,char * ficS,char * diff){
	image im(fic);
	image imInit(fic);
	im.transInterpol(i,j);

	im.EcrireImagePGM(ficS);
	image erreur=im-imInit;
	erreur.recadre(0,255);
	erreur.EcrireImagePGM(diff);
}

void testInvMat(){
	double** mat = new double*[2];
	mat[0] = new double[2];mat[1] = new double[2];
	mat[0][0]=mat[0][1]=mat[1][0]=mat[1][1]=0;

	mat[0][0]=1;mat[0][1]=2;mat[1][0]=3;mat[1][1]=4;

	double ** matInv = invMat22(mat);

	cout<<matInv[0][0]<<" "<<matInv[0][1]<<endl
	    <<matInv[1][0]<<" "<<matInv[1][1]<<endl;

	double*  vec = new double[2];
	vec[0]=2;
	vec[1]=7;

	double*  vecS = new double[2];
	matMVec22(matInv,vec,vecS);

	cout<<"===mulitiplication==="<<endl
	    <<vecS[0]<<endl<<vecS[1]<<endl;
}

void runKanade(char * fic,char * ficMod,char * ficS,int cornerI,int cornerJ){
	image im(fic);
	image model(ficMod);

	double* delta=im.Kanade(model,cornerI,cornerJ,0.001);
	int n1=(int)(delta[0]+0.5)+cornerI;
	int m1=(int)(delta[1]+0.5)+cornerJ;

	im.drawBox(n1,m1,n1+model.getHauteur(),m1+model.getLargeur(),1,255);
	im.EcrireImagePGM(ficS);
	if(absd(delta[0])<0.0001){
		cout<<0<<endl;
	}
	else{
		cout<<delta[0]<<endl;
	}
	if(absd(delta[1])<0.0001){
		cout<<0<<endl;
	}
	else{
		cout<<delta[1]<<endl;
	}
	//std::cout<<delta[0]<<endl<<delta[1]<<endl;
}

void testDiff(char * fic1,char * fic2,int row,int col){
	image im1(fic1);
	image im2(fic2);

	image Io(im1,row,col,im2.getHauteur(),im2.getLargeur());

	image im(Io-im2);
	im.recadre(0,255);

	im.EcrireImagePGM("diff.pgm");
}

void testDrawBox(char * fic1,int n1,int m1,int n2,int m2){
	image im(fic1);
	im.drawBox(n1,m1,n2,m2,1,255);

	im.EcrireImagePGM("testBox.pgm");
}

int main(int argc, char* argv[]){
	//testExtract(argv[1]);
	//testOpEg(argv[1]);

	//testBidon();

// 	float i;
// 	float j;
// 	sscanf(argv[1],"%f",&i);
// 	sscanf(argv[2],"%f",&j);
// 	testTransInter(i,j,argv[3],argv[4],argv[5]);

	//testInvMat();

	float i;
	float j;
	sscanf(argv[4],"%f",&i);
	sscanf(argv[5],"%f",&j);
	runKanade(argv[1],argv[2],argv[3],((int)(i+0.5)),((int)(j+0.5)));

	//testDiff(argv[1],argv[2],atoi(argv[3]),atoi(argv[4]));

	//testDrawBox(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));

	return 0;
}

