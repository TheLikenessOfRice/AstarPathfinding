#include <iostream>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<string>
#include<limits>
#include<algorithm>
#include<sys/stat.h>
using namespace std;

int ROWS;
int COLUMNS;

struct Square{
  int f;
  int g;
  int h;
  char value;
  int x;
  int y;
  Square *parent;
};

void print(Square ***grid, char which)
{
  for(int i = 0; i < ROWS; i++)
  {
    for(int j = 0; j < COLUMNS; j++)
    {
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


void clear_g(Square ***grid, int *cur_square)
{
  for(int i = cur_square[0]-1; i <= cur_square[0]+1; i++)
  {
    for(int a = cur_square[1]-1; a <= cur_square[1]+1; a++)
    {
      if((i < ROWS && a < COLUMNS)&&(i >= 0 && a >= 0))
      {
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
      if(list[i]->f < lowest->f)
      {
        lowest = list[i]; 
        position = i;
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
  for(int i = start[0]-1; i <= start[0]+1;i++)
  {
    for(int j = start[1]-1; j <= start[1]+1;j++)
    {
      if(i >= 0 && j >= 0)
      {
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
        if(isin(grid[i][j], cl) || (!isin(grid[i][j], ol) || (grid[i][j]->parent)) 
        || (i == cur_square[0] && j == cur_square[1]))
        {

        }
        else
        {
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


int find_wall(vector<Square *> cl, Square *wall)
{
  for(int i = 0; i < cl.size(); i++)
  {
    if(cl[i] == wall)
      return i;
  }
}


bool fileExists(const string &filename){
  struct stat buf;
  if(stat(filename.c_str(), &buf) != -1)
  {
    return true;
  }
  return false;
}


bool cin_int(int &variable, bool coord){
  if(!(cin >> variable)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
  }
  else{
    if(!coord){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return false;
  }
}


bool wall_input(bool &preload, int &wall_grid1, int &wall_grid2, int &wall_grid3,
int &wall_grid4, bool &long_wall, vector<string> &input, ifstream &m){

  long_wall = true;
  bool cin_error;
  bool bad_input;
 
  if(preload)
  {
    m >> wall_grid1;
    if(!wall_grid1){
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      preload = false;
    }
  }

  if(!preload){
    do{
      
      bad_input = false; 

      cin_error = cin_int(wall_grid1, true);

      if(!cin_error && wall_grid1 == 0){

        if(cin.get() == '\n')
          long_wall = false;

        else{
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        return true;
      }

      cin_error = cin_int(wall_grid2, true);

      if(cin.get() == '\n')
        long_wall = false;


      if(long_wall){

        cin_error = cin_int(wall_grid3, true);
        cin_error = cin_int(wall_grid4, true);

        if(wall_grid3 < 1 || wall_grid4 > ROWS || wall_grid4 < 1 || wall_grid4 > COLUMNS){
          cout << wall_grid1 << " " << wall_grid2 << " " << wall_grid3 << " " << wall_grid4 
          << " is outside the domain of the grid. Enter coordinates for a wall square: ";
          bad_input = true;
        }
      }

      if(cin_error)
        cout << "Invalid Input" << endl;

    }while(bad_input);
  }

  else{

    m >> wall_grid2;

    if(m.peek() == '\n')
      long_wall = false;

    if(long_wall){
      m >> wall_grid3;
      m >> wall_grid4;
    }

  }

  if(long_wall)
    input.push_back(to_string(wall_grid1) + " "  + to_string(wall_grid2) + " " + to_string(wall_grid3) + " " +to_string(wall_grid4) + '\n');

  else
    input.push_back(to_string(wall_grid1) + " " + to_string(wall_grid2) + '\n');

  return false;
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
  int wall_grid3;
  int wall_grid4;
  bool grid_size;
  ifstream m;
  vector<string> input;

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
    else{
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Incorrect input. Load map?(Y/N):  ";

    }
  }while(1);

  do
  {
    bool cin_error = false;
    if(!preload)
    {
      do{
        grid_size = false;

        cout << "Enter the number of rows: ";
        cin_error = cin_int(ROWS, false);

        if(ROWS < 2 || ROWS > 100)
            grid_size = true;

      }while(grid_size);
    }
    else
      m >> ROWS; 

    if(!preload){
      do{
        grid_size = false;

        cout << "Enter the number of columns: ";
        cin_error = cin_int(COLUMNS, false);

        if(COLUMNS < 2 || COLUMNS > 100)
          grid_size = true;

      }while(grid_size);
    }
    else
      m >> COLUMNS;

    if(!cin_error)
      break;
    else{
      cout << "Invalid input. Please try again." << endl;
    }
  }while(1);

  input.push_back(to_string(ROWS));
  input.push_back("\n");
  input.push_back(to_string(COLUMNS));
  input.push_back("\n");

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
      grid[i][j]->value = '.';
      grid[i][j]->parent = NULL;
    } 
  }

  print(grid, 'v');

  if(!preload)
    cout << "Enter the coordinates for the starting node(ex. '2 1' for 2 down 1 across): ";
  do
  {
    bool cin_error = false;
    if(!preload){
      cin_error = cin_int(s_start1, true);
      cin_error = cin_int(s_start2, true);
    }
    else{
      m >> s_start1;
      m >> s_start2;
    }
    if(cin_error){
      cout << "Invalid input. Please try again: ";
    }
    else if(s_start1 < 1 || s_start1 > ROWS || s_start2 < 1 || s_start2 > COLUMNS)
      cout << s_start1 << " " << s_start2 << " is outside the domain of the grid. Enter coordinates for the starting node: ";
    else
      break;
  }while(1);

  input.push_back(to_string(s_start1) + " ");
  input.push_back(to_string(s_start2));
  input.push_back("\n");

  grid[s_start1-1][s_start2-1]->value = 'S';
  start[0] = s_start1-1;
  start[1] = s_start2-1;
  cur_square = start;

  begin = grid[s_start1-1][s_start2-1];
  pcSquare = begin;

  cout << endl;
  print(grid, 'v');

  if(!preload)
    cout << "Enter the coordinates for the ending node: ";
  do
  {
    bool cin_error = false;
    if(!preload){
      cin_error = cin_int(s_end1, true);
      cin_error = cin_int(s_end2, true);
    }
    else{
      m >> s_end1;
      m >> s_end2;
    }
    if(cin_error){
      cout << "Invalid input. Please try again: ";
    }
    else if(s_end1 == s_start1 && s_end2 == s_start2)
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

  input.push_back(to_string(s_end1) + " ");
  input.push_back(to_string(s_end2));
  input.push_back("\n");


  grid[s_end1-1][s_end2-1]->value = 'E';
  finish[0] = s_end1-1;
  finish[1] = s_end2-1;

  end = grid[s_end1-1][s_end2-1];

  print(grid, 'v');

  do
  {
    wall_grid3 = 0;
    wall_grid4 = 0;
    bool cin_error = false;
    bool long_wall = true;


    cout << "Enter the coordinates to add or remove a wall('2 1' for 2 down and 1 across) or enter two coordinates for a section of wall('2 1 9 1') enter 0 to continue: ";

    bool zero_entered = wall_input(preload, wall_grid1, wall_grid2,
    wall_grid3, wall_grid4, long_wall, input, m);


    if(long_wall){
      if(wall_grid3 == 0 && wall_grid4 == 0)
        cin_error = true;
    }


    if(!zero_entered){

      if(cin_error){
        cout << "Invalid input. Please try again: ";
      }

      else if(wall_grid1 < 1 || wall_grid1 > ROWS || wall_grid2 < 1 || wall_grid2 > COLUMNS)
        cout << wall_grid1 << " " << wall_grid2 <<
        " is outside the domain of the grid. Enter coordinates for a wall square: ";

      else
      {
        if(wall_grid3 > 0 && wall_grid3 <= ROWS && wall_grid4 > 0 && wall_grid4 <= COLUMNS)
        {
          int difference;
          int adj_wall;
          if(wall_grid3 == wall_grid1)
          {
            //horizontal
            difference = abs(wall_grid4 - wall_grid2);
            if(wall_grid4 > wall_grid2)
              adj_wall = wall_grid2;
            else
              adj_wall = wall_grid4;
            for(int i = 0; i <= difference; i++)
            {
              if(grid[(wall_grid1-1)][(adj_wall-1) + i]->value == '.')
              {
                grid[(wall_grid1-1)][(adj_wall-1) + i]->value = 'W';   
                cl.push_back(grid[(wall_grid1-1)][(adj_wall-1) + i]);
              }
              else if(grid[(wall_grid1-1)][(adj_wall-1) + i]->value == 'W')
              {
                grid[(wall_grid1-1)][(adj_wall-1) + i]->value = '.';   
                ol.push_back(grid[(wall_grid1-1)][(adj_wall-1) + i]);
              }
            }
          }
          else{
            //vertical
            difference = abs(wall_grid3 - wall_grid1);
            if(wall_grid3 > wall_grid1)
              adj_wall = wall_grid1;
            else
              adj_wall = wall_grid3;
            for(int i = 0; i <= difference; i++)
            {
              if(grid[(adj_wall-1) + i][(wall_grid2-1)]->value == '.')
              {
                grid[(adj_wall-1) + i][(wall_grid2-1)]->value = 'W';   
                cl.push_back(grid[(adj_wall-1) + i][wall_grid2-1]);
              }
              else if(grid[(adj_wall-1) + i][(wall_grid2-1)]->value == 'W')
              {
                grid[(adj_wall-1) + i][(wall_grid2-1)]->value = '.';   
                ol.push_back(grid[(adj_wall-1) + i][wall_grid2-1]);
              }
            }
          }
        }


        else if(!isin(grid[wall_grid1-1][wall_grid2-1], cl))
        {
          grid[wall_grid1-1][wall_grid2-1]->value = 'W';
          cl.push_back(grid[wall_grid1-1][wall_grid2-1]);
        }

        else
        {
          grid[wall_grid1-1][wall_grid2-1]->value = '.';
          cl.erase(cl.begin() + find_wall(cl, grid[wall_grid1-1][wall_grid2-1]));
        }
        print(grid, 'v');
      }
    }
    else
      break;

  }while(1);

  if(!preload){
    char answer; 


    cout << "Do you want to save this grid layout?(Y/N): ";
    cin >> answer;
    answer = tolower(answer);
    if(answer == 'y')
    {
      string save_file;
      bool write_cont = false;
      do{
        do{
          cout << "Enter the name of the save file(Include .txt): ";
          cin >> save_file;

          save_file.erase(remove_if(save_file.begin(), save_file.end(), ::isspace),
          save_file.end());

          if(save_file.substr(save_file.length()-4) != ".txt")
          {

            cout << "Incorrect input. Please try again." << endl;
          }
          else
            break;
        }while(1);

        ifstream save_grid(filename.c_str());

        if(fileExists(save_file)){
          char overwrite;
          cout << "File already exists do you want to overwrite(Y/N)?: ";
          cin >> overwrite;
          overwrite = tolower(overwrite);
          if(overwrite == 'y'){
            if(remove(save_file.c_str()) != 0)
              cout << "Error deleting file" << endl;
            else{
              cout << "File overwritten" << endl;
              write_cont = true;
            }
          }
        }
        else
          write_cont = true;
        save_grid.close();
      }while(!write_cont);

      ofstream save_grid;

      save_grid.open(save_file.c_str(), ios::out);
      input.push_back("0");
      for(unsigned int i = 0; i < input.size();i++)
      {
        save_grid << input[i];
      }
    }
  }
  cin.ignore();
  cout << "Press Enter to continue" << endl;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  set_distance(grid, start, finish);

  ol.push_back(begin);

  add_new(grid, cur_square, ol, cl);

  
  if(ol.size() <= 1){
    cout << "No Solution" << endl;
    exit(0);
  }

  set_parent(cur_square, grid, ol, cl);

  cl.push_back(ol[0]);
  ol.erase(ol.begin());
  pl.push_back(begin);

  assign_g(grid, cur_square, cur_g);
  assign_f(grid, cur_square);



  while(lowest != end)
  {
    //Find the lowest node around the current node
    lowest = find_lowest(ol, position);

    //Edgecase: if there is nowhere left to go
    if(lowest == NULL)
    {
      break;
    }


    //Make sure there is the correct parent
    set_parent(cur_square, grid, ol, cl);
    check_parent(grid, cur_square, ol);

    //change current square
    cur_square[0] = lowest->x;
    cur_square[1] = lowest->y;



    //increment cur_g
    cur_g += (lowest->g - cur_g);


    //change parent for future lowest
    pcSquare = lowest;
    


    //send lowest to closed list so it cannot go back to that square
    cl.push_back(lowest); 

    //delete lowest from open list
    if(ol.size())
      ol.erase(ol.begin()+position);

    //add new nodes to open list based on new current square
    add_new(grid, cur_square, ol, cl); 
    

    if(ol.size() <= 1){
      cout << "No Solution" << endl;
      exit(0);
    }

    //assign new g and f values for the section
    assign_g(grid, cur_square, cur_g);


    assign_f(grid, cur_square);
  }

  for(Square *parents = end; parents != begin; parents = parents->parent)
  {
    if(parents)
    {
      if(parents != end)
      {
        parents->value = '=';
      }
    }
    else
    {
      break;
    }
  }
  end->value = 'E';

  print(grid, 'v');

  return 0;
}
