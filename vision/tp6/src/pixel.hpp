#include <list>
class pixPPM{
public:

	inline pixPPM(int i,int j,double valR,double valG,double valB){
		this->i=i;
		this->j=j;
		this->valR=valR;
		this->valG=valG;
		this->valB=valB;
	}

	inline pixPPM(){}

	inline pixPPM& operator=(const pixPPM & pix){
		i=pix.i;
		j=pix.j;
		valR=pix.valR;
		valG=pix.valG;
		valB=pix.valB;
		
		return (*this);
	}

	inline bool equalPos(const pixPPM & pix){
		return i==pix.i && j==pix.j;
	}
	
	inline double distance2(const pixPPM & pix_A, const pixPPM & pix_B){
	  double res_i = (pix_A.i-pix_B.i)*(pix_A.i-pix_B.i);
	  double res_j = (pix_A.j-pix_B.j)*(pix_A.j-pix_B.j);
	  double res_R = (pix_A.valR-pix_B.valR)*(pix_A.valR-pix_B.valR);
	  double res_G = (pix_A.valG-pix_B.valG)*(pix_A.valG-pix_B.valG);
	  double res_B = (pix_A.valB-pix_B.valB)*(pix_A.valB-pix_B.valB);
	  double res = res_i + res_j  + res_R + res_G + res_B;
	  return res;
	}
	
	inline  pixPPM operator+(const pixPPM & pix){
		pixPPM res;
		res.i = i + pix.i;
		res.j = j + pix.j;
		res.valR = valR + pix.valR;
		res.valG = valG + pix.valG;
		res.valB = valB + pix.valB;
		return res;
	}
	
	inline pixPPM moyenne(const std::list<pixPPM> & Lpix){
	  std::list<pixPPM>::const_iterator it=Lpix.begin();
	  pixPPM moy(0,0,0,0,0);
	  int nb_pix = 1;
	  for(it;it!=Lpix.end();it++){
	    moy = moy+(*it);
	    nb_pix++;
	  }
	
	return moy;
	}
	
	
	int i;
	int j;
	double valR;
	double valG;
	double valB;
};
