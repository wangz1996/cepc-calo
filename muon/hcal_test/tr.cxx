class A
{
	int a=1;
public:
	void pr()
	{
		cout<<a<<endl;
	}
	void c()
	{
		a=2;
	}
};
void tr()
{
	A a;
	a.c();
	a.pr();
}
