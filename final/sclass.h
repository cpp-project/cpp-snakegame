# include "vector.h"
#include <ctime>
using namespace std;

int map[30][60];

int count_growth; // growth 갯수
int count_poison; // poison 갯수
int count_gate; // gate 갯수

int max_count; // MAX 도달 횟수
int num_1; // 맵에서 1 갯수

char com_1; //v표시 해줄거
char com_2;
char com_3;
char com_4;

int mission_1; // 미션 설정
int mission_2;
int mission_3;
int mission_4;

//변수명 바꿀지 정해야함 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
time_t nowGameT;
time_t startAT;
time_t nowAT;
time_t startPT;
time_t nowPT;
int itemT = 6;



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
		Snake(int _height, int _width)
		: height(_height), width(_width)
        {   init(map);
			growth_item(map);
			poison_item(map);
            check_wall(map);
            get_gate(wall1);
			body.push_back(Vector(width/2, height/2));
            body.push_back(Vector(width/2+1, height/2));
            body.push_back(Vector(width/2+2, height/2));
			set_direction(0);
			exit = false;
			speed = 100000;
			level = 1;
			mission_1 = 1;
			mission_4 = 5;
			mission_2 = 1;
			mission_3 = 1;
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

		void growth_item(int map[30][60]){ // growth 놓기
			startAT = time(NULL);

			int x = rand()%width;
			int y = rand()%height;
			growth = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(growth==body[i])growth_item(map);
            if(map[y][x]!=0) growth_item(map);

		}


		void poison_item(int map[30][60]){  // poison 위치 설정
			startPT = time(NULL);

      int x,y;
			x = rand()%width;
			y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])poison_item(map);

            if(map[y][x]!=0) poison_item(map);

		}

		void game_end(){ //3이하로 뱀 크기 떨어지면 죽음
			if(body.size()<3){exit = true;}
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

		//맵 다음단계 되면 다시 3개로 줄여줌
		void change_body(){

			for(int i=0; i<body.size()-3; i++){
				reduceBodySize(body.size());
			}

		}

		void make_move(){ // 이동
			if(!exit)body[0]+=direction;

			for(unsigned int i = 1; i<body.size(); ++i)
				if(body[i]==body[0]){exit = true; break;}
		}

		void body_move(){
			for(unsigned int i=body.size()-1; i>0; --i)
				increaseBodySize(i);
		}

		/*아이템을 먹을때마다 startT 갱신해줌.
		메인 while문에서 계속 checkForGrowth을 호출해주며
		nowT를 갱신해줌. 만약 nowT - startT가 5초 이상이
		될 경우 dropGrowth함수를 재호출해서 새로 아이템 생성*/
		void checkForGrowth(){

			nowAT = time(NULL);

			if(nowAT - startAT > itemT){
				growth_item(map);
			}
			if(body[0]==growth)
			{
				count_growth++; //수정
				points++;
				if(points>best)best = points;
				growth_item(map);
				increaseBodySize(body.size());
			}
		}

		void checkForPoison(){ //길이 줄이고 포인트 감소

			nowPT = time(NULL);

			if(nowPT - startPT > itemT){
				poison_item(map);
			}

			if(body[0]==poison)
			{	count_poison++; //수정
				points--;
				poison_item(map);
				reduceBodySize(body.size());
			}
		}

		 void checkForwall(){ // 벽에 닿으면 죽음
		    for(int i=0; i<num_1; i++){
		        if(body[0]==wall1[i] && body[0] != gate[0] && body[0]!= gate[1]){
			exit = true;
						}

		    }
		}

        void check_gate(){ // 몸이 다 통과할때까지 머리 방향 설정
            if(body[0] == gate[0]){
                body[0] = gate[1];
                count_gate++;
                set_direction(gate_direction(gate[1]));
            }

            else if(body[0] == gate[1]){
				count_gate++;
                body[0] = gate[0];
                set_direction(gate_direction(gate[0]));
            }

        }

		void change_gate(){ // 다 통과해야 게이트 바뀌도록
			Vector A = direction;
			for(int i=0; i<body.size(); i++){
				A = A + direction;

			}
            if(body[0] == gate[0]+A){
				get_gate(wall1);
            }

            else if(body[0] == gate[1]+A){
				get_gate(wall1);
            }

    }


        int gate_direction(Vector gate){ //벽 없는 쪽으로 머리 방향 설정
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

		void mission(){ // 미션 완료시 v표시
			if(mission_1 <= count_growth){
				com_1 = 'v';
			}
			else com_1 = ' ';
			if(mission_2 <= count_poison){

				com_2 = 'v';
			}
			else com_2 = ' ';
			if(mission_3 <= count_gate){

				com_3 = 'v';
			}
			else com_3 = ' ';
			if(mission_4 <= body.size()){

				com_4 = 'v';
			}
			else com_4 = ' ';
		}
		int mission_complete(){
			if(com_2 == 'v' && com_1 == 'v' && com_3 == 'v' && com_4 == 'v' ) {
				level++;
				return level;
			}
			return level;
		}

		void checkPoints(){ // 미션 완료시 맵 바꾸고 미션 스코어 변경
			switch(mission_complete()){
				case 2: {
					mission_1 =2;
					mission_4 =4;
					mission_2 =2;
					mission_3 =2;
					change_map(map2);
					break;}
				case 3: {
					mission_1 =3;
					mission_4 =5;
					mission_2 =3;
					mission_3 =3;
					change_map(map3); break;}
				case 4: {
					mission_1 =4;
					mission_4 =6;
					mission_2 =4;
					mission_3 =4;
					change_map(map4); break;}
				case 5: {
					mission_1 =5;
					mission_4 =7;
					mission_2 =5;
					mission_3 =5;
					change_map(map5); break;}
			}
		}

        char* check_map(){ // 맵바꾸기
            if(level == 1){return get_table(map1);}
            if(level == 2){return get_table(map2);}
			if(level == 3){return get_table(map3);}
            if(level == 4){return get_table(map4);}
			if(level == 5){return get_table(map5);}
            return get_table(map5);
        }

		void increaseBodySize(uint i){
			if(i >= body.size())
				body.push_back(body.back());
			else
				body[i] = body[i - 1];
		}

		void reduceBodySize(uint i){
			if(i >= body.size())
				body.pop_back();
			else
				body[i] = body[i - 1];
		}

		int getBodySize(){return body.size();}
		int get_points() {return points;}
		int get_level() {return level;}
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
