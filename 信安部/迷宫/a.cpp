#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <time.h>

using namespace std;


//class:MazePos-----------------------------------------
//迷宫通道块类型
class MazePos
{
public:
 int wx,ly; //块的X,Y坐标 
 int path; //块的类型;0:空块,-1:墙壁,1:出口路径
 bool pass; //是否曾经过(对墙壁无意义);false:没有,true:曾经过
 bool operator==(const MazePos pos)
 {
  return (wx==pos.wx && ly==pos.ly );
 };
 MazePos operator=(const MazePos pos)
 {
  
  wx=pos.wx;
  ly=pos.ly;
  pass=pos.pass;
  path=pos.path;
  return *this;
 };
};
//End:MazePos---------------------------------------


//class:SElemType-----------------------------------------
//辅助栈元素类型
class SElemType
{
public:
 int ord; //迷宫通道块路径上的序号
 MazePos seat;  //通道块在迷宫中的位置坐标
 int di;  //从此通道块走向下一通道块的方向
    //0:无效,1:东,2:南,3:西,4:北
 bool operator==(const SElemType et)
 {
  return (seat==et.seat); 
 };
 SElemType operator=(const SElemType et)
 {
  ord =et.ord ;
  seat =et.seat ;
  di =et.di ;
  return (*this);
 };
};
//End:SElemType--------------------------------------


//struct:MazeMap-------------------------------------
//由通道块组成的迷宫地图
#define MAPWIDTH 10
#define MAPHEIGHT 10

typedef struct MazeMap
{
 //由通道块矩阵构成
 MazePos mazemap[MAPWIDTH][MAPHEIGHT];
}MazeMap;
//End:MazeMap---------------------------------------


//struct::MazeWay----------------------------------------
//辅助出口路径链表元素类型
typedef struct MazeWay
{
 int wx,ly;
}MazeWay;
//End:MazeWay--------------------------------------------


//Class:Maze----------------------------------------
//主体类,迷宫路径问题求解
class Maze
{
public:
 Maze(int width=MAPWIDTH,int height=MAPHEIGHT); //生成迷宫地图
 ~Maze();
 void DoMaze(); //找出出口路径并显示
private:
 bool InitOK; //初始化成功标志
 MazeMap map; //迷宫地图
 MazePos start,end; //迷宫的入口和出口
 bool FindPath(); //主要函数,寻找出口路径
 list<MazeWay> mazeway; //存放出口路径的临时链表
 void RandMap(); //随机生成迷宫地图函数
 bool CreateMap(bool init); //在初始和找到出口路径后生成相应地图函数
 bool pass(MazePos curpos); //当前路径通道块是否可通(即是不是未经过的空块)
 MazePos NextPos(MazePos curpos,int di); //取得当前通道块当前方向上的下一个通道块
 bool Invalide(SElemType e); //使当前通道块标记为不可通
 void DisplayMaze(); //显示迷宫地图

};

Maze::Maze(int width,int height)
{
 //
 //随机生成迷宫地图
 CreateMap(true);
 //显示地图
 DisplayMaze();
}

Maze::~Maze()
{
 //Add codes here
}

bool Maze::FindPath ()
{
 //
 //寻找出口,并生成出口路径链表
 if(InitOK)
 {
  //MazeStack mstack;
  stack<SElemType,list<SElemType> > mstack;
  MazePos curpos=start; 
  int curstep=1; //经过的步数
  MazeWay mw; //出口路径块元素
  unsigned mwsize=mazeway.size (); //为显示运行过程而设
  do
  {
   if(pass(curpos))
   {
    //如果当前位置可通(即是未走过的空块)

    //封装栈元素,将当前位置进栈
    SElemType e;
    e.ord =curstep;
    e.seat =curpos;
    e.di =1;
    mstack.push (e);

    //保存当前位置到出口路径链表
    mw.wx =e.seat .wx ;
    mw.ly =e.seat .ly ;
    mazeway.push_back (mw);


    //如果是出口,则结束
    if(curpos==end)
     return true;

    //不然就将得到下一个通道块
    curpos=NextPos(curpos,e.di );
    curstep++;
   }
   else
   {
    //当前位置不可通,则在栈内找到下一个位置
    if(!mstack.empty())
    {
     SElemType e;
     e=mstack.top ();
     mstack.pop ();
     
     //调整出口路径链表
     mazeway.pop_back ();

     while((e.di==0 || e.di ==4) && !mstack.empty ())
     {
      Invalide(e);  //标记刻通道块不能通过
      e=mstack.top ();
      mstack.pop (); //退回一步

      //调整出口路径链表
      mazeway.pop_back ();

     }
     if(mstack.empty ())
      return false;
     else if( e.di<5)
     {
      e.di++;
      e.di=e.di%5;

      mstack.push (e);

      //保存当前位置到出口路径链表
      mw.wx =e.seat .wx ;
      mw.ly =e.seat .ly ;
      mazeway.push_back (mw);

      curpos=NextPos(e.seat ,e.di );
     }
     
    }
   }
   ///*//显示运行过程
   if(mwsize!=mazeway.size () )
   {
    CreateMap(false);
    DisplayMaze();
    mwsize=mazeway.size ();
    Sleep(800); //要包含windows.h头文件
   }
   //*
  }while(!mstack.empty ());
 }
 return false;
}

MazePos Maze::NextPos(MazePos curpos,int di)
{
 //
 MazePos pos;
 switch(di)
 {
 case 1:
  pos=map.mazemap [curpos.wx+1][curpos.ly ] ;
  break;
 case 2:
  pos=map.mazemap [curpos.wx][curpos.ly+1 ] ;
  break;
 case 3:
  pos=map.mazemap [curpos.wx-1][curpos.ly] ;
  break;
 case 4:
  pos=map.mazemap [curpos.wx][curpos.ly-1] ;
  break;
 }
 return pos;
}

bool Maze::pass(MazePos curpos)
{
 //
 //通过MazePos类型参数传递的信息检查MazeMap map;
 if(curpos.wx <0 || 
  curpos.wx >=MAPWIDTH || 
  curpos.ly <0 || 
  curpos.ly >=MAPHEIGHT)
  return false;
 return (map.mazemap [curpos.wx ][curpos.ly ].path ==0 && 
   map.mazemap [curpos.wx ][curpos.ly ].pass ==false );
  
}

void Maze::DoMaze ()
{
 //
 if(!InitOK)
  return;
 if(FindPath())
 {
  CreateMap(false);
  DisplayMaze();
 }
 else
 {
  cout<<endl<<"NO PATH!"<<endl;
 }
}

void Maze::RandMap ()
{
 //
 //只能生成从左上到右下的迷宫地图
 MazeWay curway; //随机生成的当前正处理的出口路径块(组成mw)
 list<MazeWay> mw; //随机生成的出口路径(由curway组成)
 list<MazeWay>::iterator iter; //容器适配器

 curway.wx =0;
 curway.ly =1;
 mw.push_back (curway);

 curway.wx ++;
 mw.push_back (curway);

 srand(time(0)); //取得当前时间作为随机数种子

 while(curway.wx <MAPWIDTH-1 && curway.ly <MAPHEIGHT-1)
 {
  if(rand()%2==1)
  {
   //生成随机X坐标上的路径块
   curway.wx ++;
   
   mw.push_back (curway);
  }
  else
  {
   //生成随机Y坐标上的路径块
   curway.ly ++;
   mw.push_back (curway);
  }
 }

 srand(time(0));
 for(int y=0;y<MAPHEIGHT;y++)
 {
  for(int x=0;x<MAPWIDTH;x++)
  {
   //填充每个通道块
   map.mazemap [x][y].wx =x;
   map.mazemap [x][y].ly =y;
   map.mazemap [x][y].pass =false;
   if(x==0||y==0||x==MAPWIDTH-1||y==MAPHEIGHT-1)
   {
    //生成四周墙壁
    map.mazemap [x][y].path =-1;
    //map.mazemap [x][y].pass =true;
   }
   else
   {
    if(rand()%10>=6) //数值越小,墙壁越多
    {
     map.mazemap [x][y].path =-1; //随机生成墙壁
     //map.mazemap [x][y].pass =true;
    }
    else
    {
     map.mazemap [x][y].path =0; //生成空的通道块
     //map.mazemap [x][y].pass =false;
    }
   } 
  }
 }
 //生成出口路径
 for(iter=mw.begin ();iter!=mw.end ();iter++)
 {
  map.mazemap [(*iter).wx ][(*iter).ly ].path =0;
  //map.mazemap [(*iter).wx ][(*iter).ly ].pass =false;
 }
 //生成入口和出口

 start=map.mazemap [mw.front ().wx][mw.front ().ly];
 end=map.mazemap [mw.back ().wx][mw.back ().ly];
 //初始化成功
 InitOK=true;
}

bool Maze::CreateMap (bool init)
{
 //
 if(init)
 {
  RandMap();
 }
 else
 {
  for(int y=0;y<MAPHEIGHT;y++)
   for(int x=0;x<MAPWIDTH;x++)
   {
    if(map.mazemap [x][y].path ==0)
     map.mazemap [x][y].pass =0;
   }
  list<MazeWay>::iterator  iter;
  for(iter=mazeway.begin ();iter!=mazeway.end ();iter++)
  {
   map.mazemap [(*iter).wx][(*iter).ly ].path =1; 
  }
 }
 return true;
}

bool Maze::Invalide (SElemType e)
{
 //
 //通过SElemType类型参数传递的信息修正MazeMap map;
 if(e.seat .wx<0 ||
  e.seat .wx>=MAPWIDTH ||
  e.seat .ly<0 ||
  e.seat .ly>=MAPHEIGHT)
  return false;
 map.mazemap [e.seat .wx][e.seat .ly ].pass =true; 
 return true;
}

void Maze::DisplayMaze ()
{
 //
 cout<<endl;
 for(int y=0;y<MAPHEIGHT;y++)
 {
  for(int x=0;x<MAPWIDTH;x++)
  {
   switch (map.mazemap [x][y].path)
   {
   case -1:
      cout<<"█";break; //墙壁图案
   case 0:
      cout<<"  ";break; //空块图案
   case 1:
      cout<<"==";break; //出口路径图案
   }
    
  }
  cout<<endl;
 }
 cout<<endl; 
}
//End:Maze----------------------------------------

//main--------------------------------------------
//主函数,迷宫求解演示
int main(int argc, char* argv[])
{
 //
 cout<<"下面是随机生成的迷宫:"<<endl;
 Maze mymaze; //生成迷宫
 cout<<"按任意键演示迷宫解法!"<<endl;
// system("pause");
 mymaze.DoMaze (); //生成出口路径
 cout<<"演示结束."<<endl;
 system("pause");
 return 0;
}