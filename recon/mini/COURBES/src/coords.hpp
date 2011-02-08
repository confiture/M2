class coords{
public:

	coords(int i,int j){
		this->i=i;
		this->j=j;
	}

	bool operator<(const coords & c){
		if(i<c.i)return true;
		if(i==c.i && j<c.j)return true;

		return false;
	}


private:
	int i;
	int j;
};
