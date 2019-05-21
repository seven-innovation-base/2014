#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <time.h>

using namespace std;


//class:MazePos-----------------------------------------
//�Թ�ͨ��������
class MazePos
{
public:
 int wx,ly; //���X,Y���� 
 int path; //�������;0:�տ�,-1:ǽ��,1:����·��
 bool pass; //�Ƿ�������(��ǽ��������);false:û��,true:������
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
//����ջԪ������
class SElemType
{
public:
 int ord; //�Թ�ͨ����·���ϵ����
 MazePos seat;  //ͨ�������Թ��е�λ������
 int di;  //�Ӵ�ͨ����������һͨ����ķ���
    //0:��Ч,1:��,2:��,3:��,4:��
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
//��ͨ������ɵ��Թ���ͼ
#define MAPWIDTH 10
#define MAPHEIGHT 10

typedef struct MazeMap
{
 //��ͨ������󹹳�
 MazePos mazemap[MAPWIDTH][MAPHEIGHT];
}MazeMap;
//End:MazeMap---------------------------------------


//struct::MazeWay----------------------------------------
//��������·������Ԫ������
typedef struct MazeWay
{
 int wx,ly;
}MazeWay;
//End:MazeWay--------------------------------------------


//Class:Maze----------------------------------------
//������,�Թ�·���������
class Maze
{
public:
 Maze(int width=MAPWIDTH,int height=MAPHEIGHT); //�����Թ���ͼ
 ~Maze();
 void DoMaze(); //�ҳ�����·������ʾ
private:
 bool InitOK; //��ʼ���ɹ���־
 MazeMap map; //�Թ���ͼ
 MazePos start,end; //�Թ�����ںͳ���
 bool FindPath(); //��Ҫ����,Ѱ�ҳ���·��
 list<MazeWay> mazeway; //��ų���·������ʱ����
 void RandMap(); //��������Թ���ͼ����
 bool CreateMap(bool init); //�ڳ�ʼ���ҵ�����·����������Ӧ��ͼ����
 bool pass(MazePos curpos); //��ǰ·��ͨ�����Ƿ��ͨ(���ǲ���δ�����Ŀտ�)
 MazePos NextPos(MazePos curpos,int di); //ȡ�õ�ǰͨ���鵱ǰ�����ϵ���һ��ͨ����
 bool Invalide(SElemType e); //ʹ��ǰͨ������Ϊ����ͨ
 void DisplayMaze(); //��ʾ�Թ���ͼ

};

Maze::Maze(int width,int height)
{
 //
 //��������Թ���ͼ
 CreateMap(true);
 //��ʾ��ͼ
 DisplayMaze();
}

Maze::~Maze()
{
 //Add codes here
}

bool Maze::FindPath ()
{
 //
 //Ѱ�ҳ���,�����ɳ���·������
 if(InitOK)
 {
  //MazeStack mstack;
  stack<SElemType,list<SElemType> > mstack;
  MazePos curpos=start; 
  int curstep=1; //�����Ĳ���
  MazeWay mw; //����·����Ԫ��
  unsigned mwsize=mazeway.size (); //Ϊ��ʾ���й��̶���
  do
  {
   if(pass(curpos))
   {
    //�����ǰλ�ÿ�ͨ(����δ�߹��Ŀտ�)

    //��װջԪ��,����ǰλ�ý�ջ
    SElemType e;
    e.ord =curstep;
    e.seat =curpos;
    e.di =1;
    mstack.push (e);

    //���浱ǰλ�õ�����·������
    mw.wx =e.seat .wx ;
    mw.ly =e.seat .ly ;
    mazeway.push_back (mw);


    //����ǳ���,�����
    if(curpos==end)
     return true;

    //��Ȼ�ͽ��õ���һ��ͨ����
    curpos=NextPos(curpos,e.di );
    curstep++;
   }
   else
   {
    //��ǰλ�ò���ͨ,����ջ���ҵ���һ��λ��
    if(!mstack.empty())
    {
     SElemType e;
     e=mstack.top ();
     mstack.pop ();
     
     //��������·������
     mazeway.pop_back ();

     while((e.di==0 || e.di ==4) && !mstack.empty ())
     {
      Invalide(e);  //��ǿ�ͨ���鲻��ͨ��
      e=mstack.top ();
      mstack.pop (); //�˻�һ��

      //��������·������
      mazeway.pop_back ();

     }
     if(mstack.empty ())
      return false;
     else if( e.di<5)
     {
      e.di++;
      e.di=e.di%5;

      mstack.push (e);

      //���浱ǰλ�õ�����·������
      mw.wx =e.seat .wx ;
      mw.ly =e.seat .ly ;
      mazeway.push_back (mw);

      curpos=NextPos(e.seat ,e.di );
     }
     
    }
   }
   ///*//��ʾ���й���
   if(mwsize!=mazeway.size () )
   {
    CreateMap(false);
    DisplayMaze();
    mwsize=mazeway.size ();
    Sleep(800); //Ҫ����windows.hͷ�ļ�
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
 //ͨ��MazePos���Ͳ������ݵ���Ϣ���MazeMap map;
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
 //ֻ�����ɴ����ϵ����µ��Թ���ͼ
 MazeWay curway; //������ɵĵ�ǰ������ĳ���·����(���mw)
 list<MazeWay> mw; //������ɵĳ���·��(��curway���)
 list<MazeWay>::iterator iter; //����������

 curway.wx =0;
 curway.ly =1;
 mw.push_back (curway);

 curway.wx ++;
 mw.push_back (curway);

 srand(time(0)); //ȡ�õ�ǰʱ����Ϊ���������

 while(curway.wx <MAPWIDTH-1 && curway.ly <MAPHEIGHT-1)
 {
  if(rand()%2==1)
  {
   //�������X�����ϵ�·����
   curway.wx ++;
   
   mw.push_back (curway);
  }
  else
  {
   //�������Y�����ϵ�·����
   curway.ly ++;
   mw.push_back (curway);
  }
 }

 srand(time(0));
 for(int y=0;y<MAPHEIGHT;y++)
 {
  for(int x=0;x<MAPWIDTH;x++)
  {
   //���ÿ��ͨ����
   map.mazemap [x][y].wx =x;
   map.mazemap [x][y].ly =y;
   map.mazemap [x][y].pass =false;
   if(x==0||y==0||x==MAPWIDTH-1||y==MAPHEIGHT-1)
   {
    //��������ǽ��
    map.mazemap [x][y].path =-1;
    //map.mazemap [x][y].pass =true;
   }
   else
   {
    if(rand()%10>=6) //��ֵԽС,ǽ��Խ��
    {
     map.mazemap [x][y].path =-1; //�������ǽ��
     //map.mazemap [x][y].pass =true;
    }
    else
    {
     map.mazemap [x][y].path =0; //���ɿյ�ͨ����
     //map.mazemap [x][y].pass =false;
    }
   } 
  }
 }
 //���ɳ���·��
 for(iter=mw.begin ();iter!=mw.end ();iter++)
 {
  map.mazemap [(*iter).wx ][(*iter).ly ].path =0;
  //map.mazemap [(*iter).wx ][(*iter).ly ].pass =false;
 }
 //������ںͳ���

 start=map.mazemap [mw.front ().wx][mw.front ().ly];
 end=map.mazemap [mw.back ().wx][mw.back ().ly];
 //��ʼ���ɹ�
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
 //ͨ��SElemType���Ͳ������ݵ���Ϣ����MazeMap map;
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
      cout<<"��";break; //ǽ��ͼ��
   case 0:
      cout<<"  ";break; //�տ�ͼ��
   case 1:
      cout<<"==";break; //����·��ͼ��
   }
    
  }
  cout<<endl;
 }
 cout<<endl; 
}
//End:Maze----------------------------------------

//main--------------------------------------------
//������,�Թ������ʾ
int main(int argc, char* argv[])
{
 //
 cout<<"������������ɵ��Թ�:"<<endl;
 Maze mymaze; //�����Թ�
 cout<<"���������ʾ�Թ��ⷨ!"<<endl;
// system("pause");
 mymaze.DoMaze (); //���ɳ���·��
 cout<<"��ʾ����."<<endl;
 system("pause");
 return 0;
}