class Drone
{
	public:
	
	Drone();
	
	int s;
	double x, y, alt;
	
	void err(char *s);
	void drop(char *AT);
	void tempo(double ms);

	void takeOff();
	void land();

	void left(double ms);
	void right(double ms);
	void forward(double ms);
	void backward(double ms);
	void up(double ms);
	void down(double ms);
	void rotRight(double ms);
	void rotLeft(double ms);
};
