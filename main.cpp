#include <iostream>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<string>
using namespace std;

int ROWS;
int COLUMNS;

struct Square{
  int f;
  int g;
  int h;
  int value;
  int x;
  int y;
  Square *parent;
};
    
//int rows = 5;
//int columns = 7;
void print(Square ***grid, char which)
{
  for(int i = 0; i < ROWS; i++)
  {
    for(int j = 0; j < COLUMNS; j++)
    {
      //cout << "R: " << i << "C: " << j << " ";
      if(which == 'p')
        cout << grid[i][j]->x << grid[i][j]->y << " ";
      if(which == 'v')
        cout << grid[i][j]->value << " ";
      if(which == 'h')
        cout << grid[i][j]->h << " ";
      if(which == 'g')
        cout << grid[i][j]->g << " ";
      if(which == 'f')
        cout << grid[i][j]->f << " ";
      if(which == 'b')
        if(grid[i][j]->parent != NULL)
          cout << grid[i][j]->parent->x << grid[i][j]->parent->y << " ";
        else
          cout << "00 ";
    }
    cout << endl;
 }
 cout << endl;
}

void set_distance(Square ***grid, int *start, int *finish)
{
  for(int i = 0; i < ROWS; i++)
  {
    for(int j = 0; j < COLUMNS; j++)
    {
      grid[i][j]->h = (abs(finish[0] - i)+abs(finish[1] - j))*10;
    }
  }
}


bool isin(Square *square, vector<Square *> list)
{
  for(int i = 0; i < list.size(); i++)
  {
    if(square == list[i])
      return true;
  }
  return false;
}
/*
void assign_g(Square ***grid, int *start, int *finish)
{
  for(int i = 0; i < ROWS; i++)
  {
    for(int j = 0; j < COLUMNS; j++)
    {
      cout << "I: " << i << " J: " << j << endl;
      //if((abs(start[0] - i) >= 1) && (abs(start[1] - j)) >= 1)
      //{
        //cout << "Adding 4's" << endl;
        cout << "S0-i: " << abs(start[0] - i) << " S1-j: " << abs(start[1] -j) << endl;
        if(!abs(start[0] - i))
        {
          cout << "case 1" << endl;
          grid[i][j]->g = (abs(start[1] - j)*10);
          //grid[i][j]->g = (abs(start[0] - i)*10)+(abs(start[1] - j)*10);
        }
        else if(!abs(start[1] -j))
        {
          cout << "case 2" << endl;
          grid[i][j]->g = (abs(start[0] - j)*10);
        }
        else if(abs(start[0] - i) == (abs(start[1] - j)))
        {
          cout << "case 3" << endl;
          grid[i][j]->g = (abs(start[0] - i) * 14);
        }
        else if(abs(start[0] - i) > abs(start[1]-j))
        {
          cout << "case 4" << endl;
          grid[i][j]->g += (abs(start[1]-j) * 4) + (abs(start[0] - i) * 10);
        }
        else
        {
          cout << "case 5" << endl;
          grid[i][j]->g += (abs(start[0] - i) * 4) + (abs(start[1] - j) * 10);
        }
      //}
    }
  } 
}
*/

void assign_g(Square ***grid, int *cur_square, int &cur_g)
{
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int a = cur_square[1]-1; a <= cur_square[1]+1; a++)
    {
      if(a == cur_square[1] && i == cur_square[0])
      {
      }
      else
      {
        if((i < ROWS && a < COLUMNS)&&(i >= 0 && a >=0))
        {
          cout << "I: " << i << " A: " << a << endl;
          if(i == cur_square[0] || a == cur_square[1])
          {
            if(grid[i][a]->g == 0)
            {
              grid[i][a]->g += (cur_g + 10);
            }
          }
          else
          {
            if(grid[i][a]->g == 0)
            {
              grid[i][a]->g += (cur_g + 14);
            }
          }
        }
      }
    }
  }
}
/*
  grid[cur_square[0]-1][cur_square[1]-1]->g += 14;
  grid[cur_square[0]-1][cur_square[1]]->g += 10;
  grid[cur_square[0]-1][cur_square[1]+1]->g += 14;
  grid[cur_square[0]][cur_square[1]-1]->g += 10;
  //grid[cur_square[0]][cur_square[1]]->g += 10;
  grid[cur_square[0]][cur_square[1]+1]->g += 10;
  grid[cur_square[0]+1][cur_square[1]-1]->g += 14;
  grid[cur_square[0]+1][cur_square[1]]->g += 10;
  grid[cur_square[0]+1][cur_square[1]+1]->g += 14;
}
*/

void clear_g(Square ***grid, int *cur_square)
{
  cout << "clearing g" << endl;
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int a = cur_square[1]-1; a <= cur_square[1]+1; a++)
    {
      if((i < ROWS && a < COLUMNS)&&(i >= 0 && a >= 0))
      {
        cout << "I: " << i << " A: " << a << endl;
        grid[i][a]->g = 0;
      }
    }
  }
}

void assign_f(Square ***grid, int *cur_square)
{
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int j = cur_square[1]-1; j <= cur_square[1]+1; j++)
    {
      //if(i != cur_square[0] && j != cur_square[1])
      if(i >= 0 && i < ROWS && j >= 0 && j < COLUMNS)
        grid[i][j]->f = (grid[i][j]->h + grid[i][j]->g);
    }
  }
}

Square *find_lowest(vector<Square *> list, int &position)
{
  if(list.size() != 0)
  {
    Square *lowest = list[0];
    position = 0;
    for(int i = 1; i < list.size(); i++)
    {
      cout << "IF: " << list[i]->f << " LF: " << lowest->f << endl;
      if(list[i]->f < lowest->f)
      {
        lowest = list[i]; 
        position = i;
        cout << "P: " << position << endl;
      }
    }
    return lowest;
  }
  else
  {
    return NULL;
  }
}

void add_new(Square ***grid, int *start, vector<Square *> &ol, vector<Square *> &cl)
{
  cout << "Adding new" << endl;
  //int start[2] = {2,1};
  for(int i = start[0]-1; i <= start[0]+1;i++)
  {
    for(int j = start[1]-1; j <= start[1]+1;j++)
    {
      if(i >= 0 && j >= 0)
      {
        cout << "I: " << i << " J: " << j << endl;
        if((i < ROWS && j < COLUMNS)&&(i >= 0 && j >= 0))
        {
          if((!isin(grid[i][j], cl)) && (!isin(grid[i][j], ol)))
          {
            ol.push_back(grid[i][j]);
          }
        }
      }
    }
  }
}

void print_list(vector<Square *> list)
{
  cout << "List" << endl;
  for(int i = 0; i < list.size(); i++)
  {
    cout << "X: " << list[i]->x << " Y: " << list[i]->y << endl;
  }
}

void set_parent(int *cur_square, Square ***grid, vector<Square *> ol, vector<Square *> cl)
{
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int j = cur_square[1]-1; j <= cur_square[1]+1; j++)
    {
      if(i >= 0 && i < ROWS && j >= 0 && j < COLUMNS)
      {
        cout << endl;
        cout << "I: " << i << " J: " << j << endl;
        cout << grid[i][j]->parent << " " << isin(grid[i][j], ol) << endl;
        cout << "checking for parent" << endl;
        if(isin(grid[i][j], cl) || (!isin(grid[i][j], ol) || (grid[i][j]->parent)) || (i == cur_square[0] && j == cur_square[1]))
        {
          cout << "cant parent" << endl;
        }
        else
        {
          cout << "parent set" << endl;
          grid[i][j]->parent = grid[cur_square[0]][cur_square[1]];
        }
      }
    }
  }
}

void check_parent(Square ***grid, int *cur_square, vector<Square *> ol)
{
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int j = cur_square[1]-1; j <= cur_square[1]+1; j++)
    {
      if(i >= 0 && i < ROWS && j >= 0 && j < COLUMNS)
      {
        if(isin(grid[i][j], ol) && (i != cur_square[0] && j != cur_square[1]) && grid[i][j])
        {
          if(i != cur_square[0] && j != cur_square[1])
          {
            if(grid[i][j]->g > (grid[cur_square[0]][cur_square[1]]->g+14))
            {
              grid[i][j]->parent = grid[cur_square[0]][cur_square[1]];
            }
          }
          else
          {
            if(grid[i][j]->g > (grid[cur_square[0]][cur_square[1]]->g+10))
            {
              grid[i][j]->parent = grid[cur_square[0]][cur_square[1]];
            }
          }              
        }
      }
    }
  }
}

/*
void determine_parent(Square *&pSquare, int *cur_square, Square ***grid, vector<Square *> ol)
{
  cout << "stuck here old mate" << endl;
  bool in = false;
  for(int i = cur_square[0]-1; i <= cur_square[0]+1;i++)
  {
    for(int j = cur_square[1]-1; j <= cur_square[1]+1;j++)
    {
      if(i < 5 && j < 7 && i >= 0 && j >= 0)
      {
        if(isin(grid[i][j], ol))
        {
          in = true;
        }
      }  
    }
  }
  cout << "in: ";
  if(in)
    cout << "true" << endl;

  else 
    cout << "false" << endl;
  int new_square[2];
  if(in == false)
  {
    new_square[0] = pSquare->x;
    new_square[1] = pSquare->y;
  }

  if(!in)
  {
    determine_parent(pSquare->parent, new_square, grid, ol);
  }

  else if(in && (pSquare == NULL))
  {
    cout << "Cannot Solve: Dead End" << endl;
    exit(0);
  }
  else if(in)
  {
    pSquare = grid[cur_square[0]][cur_square[1]];
  }
}

*/

/*
void determine_parent(Square *pSquare, int *cur_square, Square ***grid, vector<Square*> pl)
{
  int lowest = 0;
  for(int i = cur_square[0]-1; i <= cur_square[0]+1;i++)
  {
    for(int j = cur_square[1]-1; j <= cur_square[1]+1;j++)
    {
      if(i < 5 && j < 7 && i >= 0 && j >= 0)
      {
        if(grid[i][j] != pSquare && isin(grid[i][j], pl) )
        {
          cout << "Found Lower" << endl;
          grid[cur_square[0]][cur_square[1]]->parent = grid[i][j];
          return;
        }
      }  
    }
  }
}
bool inlist(Square *square, vector<Square *> list, int &lowest)
{
  for(int i = 0; i < list.size(); i++)
  {
    if(square == list[i])
      
      return true;
  }
  return false;
}
*/

int find_wall(vector<Square *> cl, Square *wall)
{
  for(int i = 0; i < cl.size(); i++)
  {
    if(cl[i] == wall)
      return i;
  }
}


int main()
{
  Square ***grid;
  Square *lowest = NULL;
  Square *pcSquare;
  Square *begin;
  Square *end;
  

  vector<Square*> cl;
  vector<Square*> ol;
  vector<Square*> pl;
  int start[2];
  int finish[2];
  int *cur_square;
  int position = 0;
  int grid_num;
  int cur_g = 0;
  char map;
  string filename;
  bool preload;
  int s_start1;
  int s_start2;
  int s_end1;
  int s_end2;
  int wall_grid1;
  int wall_grid2;

  cout << "Do you want to load a map?(Y/N): ";
  do
  {
    cin >> map;
    if(map == 'y' || map == 'Y')
    {
      preload = true;
      cout << "Enter the name of the file: ";
      do
      {
        cin >> filename;
       
        ifstream m;
        m.open(filename.c_str(), ios::in);

        if(!m)
          cout << "This file does not exist. Enter filename: ";
        else
          break;

      }while(1);

      break;
    }
    else if(map == 'n' || map == 'N')
    {
      preload = false;
      break;
    }
    else
      cout << "Incorrect input. Load map?(Y/N):  ";
  }while(1);

  if(!preload)
  {
    cout << "Enter the number of rows: ";
    cin >> ROWS;
    cout << "Enter the number of columns: ";
    cin >> COLUMNS;

    //instantiate the grid
    grid = new Square**[ROWS];
    for(int i = 0; i < ROWS;i++)
    {
      grid[i] = new Square*[COLUMNS];
    }

    for(int i = 0; i < ROWS; i++)
    {
      for(int j = 0; j < COLUMNS; j++)
      {
        grid[i][j] = new Square;
        grid[i][j]->g = 0;
        grid[i][j]->f = 0;
        grid[i][j]->x = i;
        grid[i][j]->y = j;
        grid[i][j]->value = 0;
        grid[i][j]->parent = NULL;
      } 
    }

    cout << "R: " << ROWS  << " C: " << COLUMNS << endl;

    print(grid, 'v');
    cout << "Enter the coordinates for the starting node(ex. '2 1' for 2 down 1 across): ";

    do
    {
      cin >> s_start1;
      cin >> s_start2;
      if(s_start1 < 1 || s_start1 > ROWS || s_start2 < 1 || s_start2 > COLUMNS)
        cout << s_start1 << " " << s_start2 << " is outside the domain of the grid. Enter coordinates for the starting node: ";
      else
        break;
    }while(1);

    grid[s_start1-1][s_start2-1]->value = 1;
    start[0] = s_start1-1;
    start[1] = s_start2-1;
    cur_square = start;

    begin = grid[s_start1-1][s_start2-1];
    pcSquare = begin;


    print(grid, 'v');
    cout << "Enter the coordinates for the ending node: ";
    do
    {
     cin >> s_end1;
     cin >> s_end2;
     if(s_end1 == s_start1 && s_end2 == s_start2)
     {
      cout << "Ending coordinates cannot equal starting coordinates. Please enter the coordinates for the ending node: ";
     }
     else
     {

      if(s_end1 < 1 || s_end1 > ROWS || s_end2 < 1 || s_end2 > COLUMNS)
        cout << s_end1 << " " << s_end2 << " is outside the domain of the grid. Enter coordinates of the ending node: ";
      else
        break;
     }
    }while(1);

    grid[s_end1-1][s_end2-1]->value = 2;
    finish[0] = s_end1-1;
    finish[1] = s_end2-1;

    end = grid[s_end1-1][s_end2-1];


    do
    {
      print(grid, 'v');
      cout << "Enter the coordinates for a wall or enter the coordinates to change a wall node to a standard grid(enter 0 to continue): ";
      cin >> wall_grid1;
      if(wall_grid1 == 0)
        break;
      cin >> wall_grid2;
      if(wall_grid1 < 1 || wall_grid1 > ROWS || wall_grid2 < 1 || wall_grid2 > COLUMNS)
        cout << wall_grid1 << " " << wall_grid2 << "is outside the domain of the grid. Enter coordinates for a wall square: ";
      else
      {
        if(!isin(grid[wall_grid1-1][wall_grid2-1], cl))
        {
          grid[wall_grid1-1][wall_grid2-1]->value = 3;
          cl.push_back(grid[wall_grid1-1][wall_grid2-1]);
        }
        else
        {
          grid[wall_grid1-1][wall_grid2-1]->value = 0;
          cl.erase(cl.begin() + find_wall(cl, grid[wall_grid1-1][wall_grid2-1]));
        }
      }
    }while(1);
  }

/*
  for(int i = 0; i < ROWS; i++)
  {
    for(int j = 0; j < COLUMNS; j++)
    {
      grid[i][j] = new Square;
      grid[i][j]->g = 0;
      grid[i][j]->f = 0;
      grid[i][j]->x = i;
      grid[i][j]->y = j;
      grid[i][j]->value = 0;
      grid[i][j]->parent = NULL;
    } 
 }
 */

  /*
  cout << "Which grid do you want?(1 or 2)" << endl;
  cin >> grid_num;


  if(grid_num == 1)
  {
    start[0] = 2;
    start[1] = 1;
    finish[0] = 2;
    finish[1] = 5;
    cur_square = start;
    pcSquare = grid[2][1];
    begin = grid[2][1];
    end = grid[2][5];
    cout << "OEND: " << end << endl;
 
    grid[2][1]->value = 1;
    grid[1][3]->value = 3;
    grid[2][3]->value = 3;
    grid[3][3]->value = 3;
    grid[2][5]->value = 2;

    cl.push_back(grid[1][3]);
    cl.push_back(grid[2][3]);
    cl.push_back(grid[3][3]);
  }
  else if(grid_num == 2)
  {
    start[0] = 4;
    start[1] = 3;
    finish[0] = 2;
    finish[1] = 1;
    cur_square = start;
    pcSquare = grid[4][3];
    begin = grid[4][3];
    end = grid[2][1];

    grid[4][3]->value = 1;
    grid[2][1]->value = 2;
    grid[3][0]->value = 3;
    grid[3][1]->value = 3;
    grid[3][2]->value = 3;
    grid[3][3]->value = 3;
    grid[3][4]->value = 3;
    grid[2][3]->value = 3;
    grid[1][3]->value = 3;

    cl.push_back(grid[3][0]);
    cl.push_back(grid[3][1]);
    cl.push_back(grid[3][2]);
    cl.push_back(grid[3][3]);
    cl.push_back(grid[3][4]);
    cl.push_back(grid[2][3]);
    cl.push_back(grid[1][3]);
  }
  */
  cout << "V:" << endl;
  print(grid, 'v');

  set_distance(grid, start, finish);
  
  ol.push_back(begin);

  add_new(grid, cur_square, ol, cl);

  set_parent(cur_square, grid, ol, cl);

  cl.push_back(ol[0]);
  ol.erase(ol.begin());
  pl.push_back(begin);

  cout << "start_g: " << cur_g << endl; 
  //assign_g(grid, start, finish);
  assign_g(grid, cur_square, cur_g);
  cout << "cur_g_after_start: " << cur_g << endl;
  assign_f(grid, cur_square);
  


  while(lowest != end)
  {
    cout << endl;
    cout << endl;

    cout << "Starting New Search" << endl;

    //print open list
    cout << "OL: ";
    print_list(ol);

    //print closed list
    cout << "CL: ";
    print_list(cl);

    //print parent grid values
    cout << "P: " << endl;
    print(grid, 'b');

    cout << "F: " << endl; 
    print(grid, 'f');

    //Find the lowest node around the current node
    lowest = find_lowest(ol, position);
    cout << "LOW: " << lowest << " END: " << end << endl;

    //Edgecase: if there is nowhere left to go
    if(lowest == NULL)
    {
      cout << "list is empty" << endl;
      break;
    }
    cout << "LX: " << lowest->x << " " << lowest->y << endl;

    //pl.push_back(lowest);

    //Make sure there is the correct parent
    set_parent(cur_square, grid, ol, cl);
    check_parent(grid, cur_square, ol);
    //determine_parent(pcSquare, cur_square, grid, ol);


    //clear g values
    //clear_g(grid, cur_square);

    //change current square
    cur_square[0] = lowest->x;
    cur_square[1] = lowest->y;
    

    //increment cur_g
    cur_g += (lowest->g - cur_g);
    
    //set lowest equal to last lowest
    //lowest->parent = pcSquare;
    lowest->value = 6;

    //change parent for future lowest
    pcSquare = lowest;


    cout << cur_square[0] << " " << cur_square[1] << endl;

    //send lowest to closed list so it cannot go back to that square
    cl.push_back(lowest); 

    //delete lowest from open list
    if(ol.size())
      ol.erase(ol.begin()+position);
//    print_list(ol);
//    print_list(cl);

    //add new nodes to open list based on new current square
    add_new(grid, cur_square, ol, cl); 
    cout << "V:" << endl;
    print(grid, 'v');

    //assign new g and f values for the section
    cout << "CG: " << cur_g << endl;
    assign_g(grid, cur_square, cur_g);
    cout << "LOW G: " << lowest->g << " cur_g after: " << cur_g << endl;


    assign_f(grid, cur_square);
    cout << "G:" << endl;
    print(grid, 'g');
  }

  print(grid, 'b');

  cout << "go to parents" << endl;
  int pValue = 7;
  for(Square *parents = end; parents != begin; parents = parents->parent)
  {
    cout << "go back" << endl;
    if(parents)
    {
      parents->value = pValue;
      pValue++;
    }
    else
    {
      cout << "parents == " << parents << endl;
      break;
    }
  }

  cout << "H: " << endl;
  print(grid, 'h');
  cout << "G: " << endl;
  print(grid, 'g');
  cout << "F: " << endl;
  print(grid, 'f');
  cout << "XY: " << endl;
  print(grid, 'p');
  cout <<  "V: " << endl;
  print(grid, 'v');

  return 0;
}
