#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <streambuf>
#include "image.hpp"
//class image;

class item{
public:
	image im;
	int epaisseur;
	int nbTrous;

	void writePgmItem(const char * fic);

};

#endif
