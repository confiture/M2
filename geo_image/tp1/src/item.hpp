#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <streambuf>
class image;

class item{
public:
	image* pIm;
	int epaisseur;
	int nbTrous;

	void writePgmItem(const char * fic);

};

#endif
