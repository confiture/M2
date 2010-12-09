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
	}

	inline bool equalPos(const pixPPM & pix){
		return i==pix.i && j==pix.j;
	}

	int i;
	int j;
	double valR;
	double valG;
	double valB;
};
