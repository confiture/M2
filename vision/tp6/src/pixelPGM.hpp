#include <list>
class pixPGM{
public:

	inline pixPGM(int i,int j,double val){
		this->i=i;
		this->j=j;
		this->val=val;
	}

	inline pixPGM(){}

	inline pixPGM& operator=(const pixPGM & pix){
		i=pix.i;
		j=pix.j;
		val=pix.val;
		return (*this);
	}

	inline bool equalPos(const pixPGM & pix){
		return i==pix.i && j==pix.j;
	}

	inline static double distance2(const pixPGM & pix_A, const pixPGM & pix_B){
	  double res_i = (pix_A.i-pix_B.i)*(pix_A.i-pix_B.i);
	  double res_j = (pix_A.j-pix_B.j)*(pix_A.j-pix_B.j);
	  double res_val = (pix_A.val-pix_B.val)*(pix_A.val-pix_B.val);
	  double res = res_i + res_j  + res_val;
	  return res;
	}

	inline  pixPGM operator+(const pixPGM & pix){
		pixPGM res;
		res.i = i + pix.i;
		res.j = j + pix.j;
		res.val = val + pix.val;
		return res;
	}

	inline pixPGM operator/(int nb){
		pixPGM res;
		res.i = (double)i / nb+0.5;
		res.j = (double)j / nb+0.5;
		res.val = val / nb;
	return res;
	}

	inline static pixPGM moyenne(const std::list<pixPGM> & Lpix){
	  std::list<pixPGM>::const_iterator it=Lpix.begin();
	  pixPGM moy;
	  pixPGM pix(0,0,0);
	  int nb_pix = 0;
	  for(it;it!=Lpix.end();it++){
	    pix = pix+(*it);
	    nb_pix++;
	  }
	moy = pix/nb_pix;
	return moy;
	}
	
	


	int i;
	int j;
	double val;
};
