#include "image.hpp"

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
	image * transInter=im.transInterpol(6.6,8.8);

	transInter->EcrireImagePGM("resultTrans.pgm");
}

int main(int argc, char* argv[]){
	//testExtract(argv[1]);
	//testOpEg(argv[1]);

	//testBidon();
	testTransInter(argv[1]);

	return 0;
}

