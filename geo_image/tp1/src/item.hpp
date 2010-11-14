#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <streambuf>

class image;

class item{
public:
	void writePgmItem(const char * fic);

	image * pIm;
	int epaisseur;
	int nbTrous;
private:
};

#endif
