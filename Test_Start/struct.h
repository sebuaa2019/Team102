struct Request{
	int unuse;
	float thredhold;
};

struct SrvFlw{
	int unuse;
	struct Request request;
};

class Follow{
	public:
		Follow();
		bool call(SrvFlw srv);

	private:

};