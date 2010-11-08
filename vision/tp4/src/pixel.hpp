class pixel{
public:

	inline pixel(int i,int j,int val){
		_i=i;
		_j=j;
		_val=val;
	}

	inline friend bool operator<(const pixel & p1,const pixel & p2){
		return p1._val<p2._val;
	}

	int _i;
	int _j;
	int _val;

private:

};
