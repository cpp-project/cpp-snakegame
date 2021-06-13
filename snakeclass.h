# include "vector.h"

using namespace std;

int count_growth; // growth 갯수
int count_poison; // poison 갯수
int count_gate; // gate 갯수

class Snake
{
	private:
		vector<Vector> body;
		Vector growth; //growth 추가
		Vector poison; //poison 추가
		Vector wall1;
		Vector direction;
		int points;
		int level;
		int best;
		const int height;
		const int width;
		bool exit;
		int speed;
		char* table;

	public:
		Snake(int _height, int _width, int_best)
		: best(_best), height(_height), width(_width)
        {
			growth_item(map);
			poison_item(map);

			body.push_back(Vector(width/2, height/2));
			set_direction(0);
			exit = false;
			speed = 80;
			level = 1;
			points = 0;
			srand(time(NULL));
			table = new char[height*width];
		}
		~Snake()
		{delete [] table;}


		void growth_item(){ // growth 놓기
			count_growth++;

			int x = rand()%width;
			int y = rand()%height;
			growth = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(growth==body[i])growth_item();

		}


		void poison_item(){  // poison 위치 설정
			count_poison++;

			int x = rand()%width;
			int y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])poison_item();

		}

		void set_direction(int d){ //방향 설정
			switch(d)
			{
				case 0:direction = Vector(0, -1); break;
				case 1:direction = Vector(1, 0); break;
				case 2:direction = Vector(0, 1); break;
				case 3:direction = Vector(-1, 0); break;
			}
		}

		void make_move()
		{
			exit = false;
			if(body[0].getX()<0 || body[0].getX()>=width)
				exit = true;
			else if(body[0].getX()==0)
			{	if(direction==Vector(-1,0))
					exit = true;
			}
			else if(body[0].getX()==(width-1))
			{	if(direction==Vector(1,0))
					exit = true;
			}

			if(body[0].getY()<0 || body[0].getY()>=height)
				exit = true;
			else if(body[0].getY()==0)
			{
				if(direction==Vector(0,-1))
					exit = true;
			}
			else if(body[0].getY()==(height-1))
			{
				if(direction==Vector(0,1))
					exit = true;
			}

			if(!exit)body[0]+=direction;

			for(unsigned int i = 1; i<body.size(); ++i)
				if(body[i]==body[0]){exit = true; break;}
		}
		void body_move()
		{
			for(unsigned int i=body.size()-1; i>0; --i)
				snakeSwap(i);
		}
		void checkForGrowth()
		{
			if(body[0]==apple)
			{
				points++;
				if(points>best)best = points;
				growth_item();
				snakeSwap(body.size());
			}
		}
		void checkForPoison() // 길이 줄이고 포인트 감소
		{
			if(body[0]==poison)
			{
				points--;
				poison_item();
				snakeSwap2(body.size());
			}
		}
		void checkPoints()
		{
			switch(points){
				case 3: level = 2; speed = 98; break;
				case 5: level = 3; speed = 85; break;
				case 7: level = 4; speed = 80; break;
				case 9: level = 5; speed = 60; break;
				case 50: level = 6; speed = 80; break;
				case 60: level = 7; speed = 65; break;
				case 70: level = 8; speed = 50; break;
				case 80: level = 9; speed = 40; break;
				case 90: level = 10; speed = 30; break;
				case 100: level = 999; speed = 15; break;
			}
		}
		void snakeSwap(uint i)
		{
			if(i >= body.size())
				body.push_back(body.back());
			else
				body[i] = body[i - 1];
		}

		void snakeSwap2(uint i)
		{
			if(i >= body.size())
				body.pop_back();
			else
				body[i] = body[i - 1];
		}

		int get_points() {return points;}
		int get_level() {return level;}
		int get_Best() {return best;)}
		bool get_exit() {return exit;}
		void set_exit(bool e) {exit = e;}
		int get_speed() {return speed;}
		int get_height() {return height;}
		int get_width() {return width;}
		char* get_table()
		{
			std::memset(table, ' ', height*width);

			table[body[0].getY()*width+body[0].getX()] = 'h';
			for(unsigned int i=1; i<body.size(); ++i)
				table[body[i].getY()*width+body[i].getX()] = 'b';

			table[growth.getY()*width+growth.getX()] = 'a';
			table[poison.getY()*width+poison.getX()] = 'x';  //poison

	for(unsigned int j=0; j<1800; j++)
          {
              if(map2[j]==1) table[j] = '1' ;
              else if(map2[j]==2) table[j] = '2' ;
          }


		int get_direction(){
			if(direction.getX()==1)
				return 1;
			else if(direction.getX()==-1)
				return 3;
			else if(direction.getY()==-1)
				return 0;
			else
				return 2;
		}
};
