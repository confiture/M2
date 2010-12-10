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

void testTransInter(char * fic){
	image im(fic);
	im.transInterpol(-6.6,-8.8);

	im.EcrireImagePGM("resultTrans.pgm");
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

int main(int argc, char* argv[]){
	//testExtract(argv[1]);
	//testOpEg(argv[1]);

	//testBidon();
	testTransInter(argv[1]);

	//testInvMat();

	return 0;
}

