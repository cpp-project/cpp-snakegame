# include "vector_step4.h"
#include <ctime>
#define MAXlen 10
using namespace std;
int map[30][60];


int count_growth; // growth 갯수
int count_poison; // poison 갯수
int count_gate; // gate 갯수

time_t nowGameT;
time_t startAT;
time_t nowAT;
time_t startPT;
time_t nowPT;
int itemT = 5;




class Snake
{
	private:
		vector<Vector> body;
		Vector growth; //growth 추가
		Vector poison; //poison 추가
		Vector wall1[1000];
    Vector gate[2];
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
        { init(map);
			growth_item(map);
			poison_item(map);

			body.push_back(Vector(width/2, height/2));
			body.push_back(Vector(width/2+1, height/2));
      body.push_back(Vector(width/2+2, height/2));
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

		void init(int map[30][60]){
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = map1[i][j];
                }

            }
        }


        void change_map(int next_map[30][60]){ //맵 change
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = next_map[i][j];
                }
            }

            check_wall(next_map);

        }

        void get_gate(Vector wall[]){ //gate 위치 정해주기
            int A = rand()%num_1;
            int B = rand()%num_1;
            if(B == A) get_gate(wall1);
            gate[0] = wall1[A];
            gate[1] = wall1[B];
        }


		void growth_item(){ // growth 놓기

			startAT = time(NULL);

			count_growth++;

			int x = rand()%width;
			int y = rand()%height;
			growth = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(growth==body[i])growth_item();

		}


		void poison_item(){  // poison 위치 설정
			startPT = time(NULL);
			count_poison++;

			int x = rand()%width;
			int y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])poison_item();

		}

		void check_wall(int map[30][60]){ //벽 위치 벡터배열로 생성
			num_1 =0;
			for(int i=0; i<30; i++){
            for(int j=0; j<60; j++)
                if(map[i][j]==1){
                wall1[num_1++] = Vector(j,i);
            }
        }
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

		void checkForwall()
		{
            for(int i=0; i<wallC; i++){
                if(body[0]==wall1[i] && body[0] != gate[0] && body[0]!= gate[1])
                    exit = true;
            }
		}

        void check_gate()
        {
            if(body[0] == gate[0]){
                body[0] = gate[1];

                set_direction(gate_direction(gate[1]));

                get_gate(wall1);
            }

            else if(body[0] == gate[1]){
                body[0] = gate[0];
                setDirection(gateDirection(gate[0]));
                get_gate(wall1);
            }

        }


        int gate_direction(Vector gate){
            Vector A = gate + Vector(0,-1) ;
            if(map[A.getY()][A.getX()]==0) return 0;
            Vector B = gate + Vector(1,0) ;
            if(map[B.getY()][B.getX()]==0) return 1;
            Vector C = gate + Vector(0,1) ;
            if(map[C.getY()][C.getX()]==0) return 2;
            Vector D = gate + Vector(-1,0) ;
            if(map[D.getY()][D.getX()]==0) return 3;

            return -1;
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
		char* checkMap(){
					 if(level == 1){return get_table(map1);}
					 if(level == 2){return get_table(map2);}
					 return 0;
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
		char* get_table(int map[30][60])
		{
			std::memset(table, ' ', height*width);

			table[body[0].getY()*width+body[0].getX()] = 'h';
			for(unsigned int i=1; i<body.size(); ++i)
				table[body[i].getY()*width+body[i].getX()] = 'b';

			table[growth.getY()*width+growth.getX()] = 'a';
			table[poison.getY()*width+poison.getX()] = 'x';  //poison

			for(unsigned int i=0; i<30; i++)
		            {
		                for(int j=0; j<60; j++){
		                    if(map[i][j]==1) table[i*width+j] = '1' ;
		                    else if(map[i][j]==2) table[i*width+j] = '2' ;
		                    else if (map[i][j]==3) table[i*width+j] = '3';
		                }

		            }
		             table[gate[0].getY()*width+ gate[0].getX()] = 'G';
		            table[gate[1].getY()*width+ gate[1].getX()] = 'G';

					return table;
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
