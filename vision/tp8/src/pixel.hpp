class pixel{
public:

	inline pixel(int i,int j,double val){
		_i=i;
		_j=j;
		_val=val;
	}

	inline pixel(){}

	inline friend bool operator<(const pixel & p1,const pixel & p2){
		return p1._val<p2._val;
	}

	inline pixel& operator=(const pixel & pix){
		_i=pix._i;
		_j=pix._j;
		_val=pix._val;

		return (*this);
	}

	inline bool equalPos(const pixel & pix){
		return _i==pix._i && _j==pix._j;
	}

	int _i;
	int _j;
	double _val;

private:

};
