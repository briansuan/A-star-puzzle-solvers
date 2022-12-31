#include<string>
#include<vector>
#include<stack>
#include<iostream>
#include<stdlib.h> 
using namespace std;

/* Author: Brian Suan*/

// NOTE: 
/*****************************************************************************
* [S,M,L || || ] --> this means --> [TOP DISC, MIDDLE DISC, BOTTOM DISC]
* The leftmost character is always the top disc
*******************************************************************************/

// State has an attribute called "peg", which is represented by an array of strings of size 3
// the elements that go inside this array can be though of as the discs with sizes S, M, and L
// State also has an attribute called camefrom which is a similar array to peg
class state {
  public:
    string peg[3];      // An element in this arrat can be one of the following safe states: 
                        // SML, S, M, L, SM, ML, SL, or blank
    string camefrom[3]; // the parent on the solution path
    int g;              // cost so far
    int h;              // estimated cost to the goal
    int f;              // g+h
};

// Major data structures:
vector<state> frontier;  // Frontier nodes - will pick one of these to expand
vector<state> beenThere; // already expanded nodes


// ============= Helper Functions ================ //
// utility to add x to the beenthere
void addtoBeenThere(state x) {
  beenThere.push_back(x); 
}

// utility to add x to the frontier
void addtoFrontier(state x) {  
  frontier.push_back(x); 
}


// to remove state x from the frontier when it has been expanded
void removefromFrontier(state x) {   
  // this is an iterator, used to iterate over the state's peg attribute 
  vector<state>:: iterator it;
  // Iterate over the forntier vector to remove x
  for(it = frontier.begin(); it != frontier.end(); ++it) {
    if(it->peg[0] == x.peg[0] && it->peg[1] == x.peg[1] && it->peg[2] == x.peg[2]) {
      frontier.erase(it);
      break;
    }
  }
}



// Trace and display the solution path from goal to initial.
// Note that camefrom of each state should be used. (called from generate)
void tracePath(state goal) {

  // Loop through beenThere and check every state object to see if that specific state is the same as next's camefrom 
  for(int i=0; i < beenThere.size(); i++) {
    if(goal.camefrom[0] == beenThere[i].peg[0] && goal.camefrom[1] == beenThere[i].peg[1] && goal.camefrom[2] == beenThere[i].peg[2]) {
      cout << 
        "[" << beenThere[i].peg[2] << " || " << beenThere[i].peg[1] << " || " << beenThere[i].peg[0] << "]"
      << endl;
      tracePath(beenThere[i]);
      break;
    }
  }
}// end of tracePath  


// Check to see if next's items is already in frontier and return true or false.
//   If it is already in frontier, 
//   and if the next's f is better, 
//   update the frontier node to next.  (called from generate)
// =========== For Generating a new state to add to Frontier  ============= //
bool inFrontier(state next) {
  // if a state object in next is already in the frontier vector, return true, loop through frontier vector

  // Check the items of every state in frontier and compare with passed state's items
  for(int i=0; i < frontier.size(); i++) {
    // If the all elements from peg attribute of next and frontier are the same, 
    // if they are all the same, then the next state is already in the frontier 
    if(next.peg[0] == frontier[i].peg[0] && next.peg[1] == frontier[i].peg[1] && next.peg[2] == frontier[i].peg[2]) {
      if(next.f < frontier[i].f) {
        // if next is already in the frontier and it has a lower f value, 
        // update frontier node to next
        frontier[i].peg[0] = next.peg[0];
        frontier[i].peg[1] = next.peg[1];
        frontier[i].peg[2] = next.peg[2];
      }
      // Print out a message
      cout << "State already in Frontier..." << endl;
      return true;
    }
  }
  // Otherwise return false
  return false;
}


// Check if a state has already been visited by comparing it with items in beenThere
bool inBeenThere(state next) {
  // loop through beenThere to compare each item with the passed state
  for(int i=0; i < beenThere.size(); i++) {
    // if a state's items is equal to the items of a state in beenThere, return true, 
    // meaning that state has already been visited
    if(next.peg[0] == beenThere[i].peg[0] && next.peg[1] == beenThere[i].peg[1] && next.peg[2] == beenThere[i].peg[2]) {
      return true;
    }
  }
  // otherwise return false
  return false;
}


// Check is a state is unsafe, compare the passed state with the given states in the array
bool isUnsafe(state next) {
  // declare the unsafe states
  string unsafe[6] = {"MS", "LS", "LM", "LMS", "LSM", "MSL"};
  // compare each unsafe state with with the passed state
  for(int i =0; i < 6; i ++) {
    // return true if it is unsafe
    if(next.peg[0] == unsafe[i] || next.peg[1] == unsafe[i] || next.peg[2] == unsafe[i]) {
      return true;
    }
  }
  // otherwise return false
  return false;
}


// Generate a new state
void generate(state next) { 

  cout << 
    "Trying to add: " << "[" << next.peg[0] << " || " << next.peg[1] << " || " << next.peg[2] << "]"
  << endl; 

  // the goal is reached
  if (next.peg[2]  == "SML") { 
    cout 
      << "\n***GOAL REACHED!***: " << "[" << next.peg[0] << " || " << next.peg[1] << " || " << next.peg[2] << "]\n" 
    << endl;
    // added this to label the goal path
    cout << endl << "INITIAL to GOAL path: " << endl;
    cout << "Initial: [SML" << " || " << "" << " || " << "" << "]" << endl;
    tracePath(next);  // display the solution path
    exit(0); 
  }

  
  // else compute h and then f for next
  int h = next.peg[0].length() + next.peg[1].length();// start point for h; g is already set


  // set h to the state's h attribute
  next.h = h;
  // f(n) = g(n) + h(n)
  next.f = next.g + next.h;

  // check is a state is unsafe
  if (isUnsafe(next)) {
    cout << "[" << next.peg[0] << " || " << next.peg[1] << " || " << next.peg[2] << "]" << " IS AN ILLEGAL MOVE! State was not added to frontier..." << endl;
  }
  // check is a state is already visited
  else if (inBeenThere(next)) {
    
  }
  // check if a state is already in the frontier, add it to frontier if not
  else if (!inFrontier(next)) {
    addtoFrontier(next); // add next to Frontier 
  }
}  


// // Generate all new states from current (called from main)
void generateAll(state current) {

  state next;  // a generated one
  string new_order; // to store the new order of discs on a given peg

  // each next will be modified from current for ease
  current.g = current.g + 1; // to update g to be used for next
  // storing the parent so that we can produce the solution path
  for(int i=0; i < 3; i++) {
    current.camefrom[i] = current.peg[i];
  } 

  // starting point of next is current
  next = current;           
  // top disk from peg 1 to peg 2
  if(current.peg[0].length() != 0) {
    // Take the first character of string in the first peg
    // and concatenate it to the string in the second peg
    new_order = current.peg[0][0] + current.peg[1];
    // set the resulting string as the value for the second peg
    next.peg[1] = new_order;
    // remove the first character from the first peg
    next.peg[0].replace(0,1,"");
    generate(next);
  } 
  // starting point of next is current
  next = current;
  // top disk from peg 1 to peg 3
  if(current.peg[0].length() != 0) {
    new_order = current.peg[0][0] + current.peg[2];
    next.peg[2] = new_order;
    next.peg[0].replace(0,1,"");
    generate(next);
  } 
  // starting point of next is current
  next = current;
  // top disk from peg 2 to peg 1
  if(current.peg[1].length() != 0) {
    new_order = next.peg[1][0] + next.peg[0];
    next.peg[0] = new_order;
    next.peg[1].replace(0,1,"");
    generate(next);
  }
  // starting point of next is current
  next = current;
  // top disk from peg 2 to peg 3
  if(current.peg[1].length() != 0) {
    new_order = next.peg[1][0] + next.peg[2];
    next.peg[2] = new_order;
    next.peg[1].replace(0,1,"");
    generate(next);
  }
  // starting point of next is current
  next = current;
  // top disk from peg 3 to peg 1
  if(current.peg[2].length() != 0) {
    new_order = next.peg[2][0] + next.peg[0];
    next.peg[0] = new_order;
    next.peg[2].replace(0,1,"");
    generate(next);
  }
  // starting point of next is current
  next = current;
  // top disk from peg 3 to peg 2
  if(current.peg[2].length() != 0) {
    new_order = next.peg[2][0] + next.peg[1];
    next.peg[1] = new_order;
    next.peg[2].replace(0,1,"");
    generate(next);
  }
}


// Find the best f state out of the frontier and return it (called from main)
state bestofFrontier() {
  // choose a state's f value to start with and set that as the current best f
  int f_best = frontier[0].f;
  // choose a state from the frontier vector
  state best_frontier = frontier[0];
  // loop through the frontier to see which in the frontier has the best f
  for(int i=1; i < frontier.size(); i++) {
    // compare the current "best" f with a given stat's f from the frontier
    if(frontier[i].f < f_best) {
      // if that given state has a better f, 
      // set that state's f as the the new f_best
      f_best = frontier[i].f;
      // set the current frontier as the best frontier 
      best_frontier = frontier[i];
    }
  }
  // return the best frontier
  return best_frontier;
}


// Display the states in the frontier  (called from main)
void displayFrontier() {
  for (int k = 0; k < frontier.size(); k++) {
    cout << 
      "[" << frontier[k].peg[0] << " || " << frontier[k].peg[1] << " || " << frontier[k].peg[2] << "]" 
    << " ";  
    cout << "g = " << frontier[k].g << " ";
    cout << "h = " << frontier[k].h << " ";
    cout << "f = " << frontier[k].f << endl; 
  }
}


// ================= Main ====================//
int main() {
  cout << "\nINITIAL:\n*****[SML" << " || " << "" << " || " << "" << "]*****\n";
  // Initial state and f, g, h values
  state  current = {
    {"SML","",""},   
    {"", "", ""},       
    0, 3, 3
  };  
  // add initial state to frontier
  addtoFrontier(current);

  char ans;
  while (ans != 'n') {
    // remove the current state from the frontier
    removefromFrontier(current);    
    // add the current state to been there   
    addtoBeenThere(current);

    cout << 
      "\n>>>Expand:" << "[" << current.peg[0] << " || " << current.peg[1] << " || " << current.peg[2] << "]" 
    << endl; 
    // generateAll possible moves from this current state
    generateAll(current);  // new states are added to frontier

    cout << "Frontier is:" << endl;
    
    displayFrontier();

    // pick the best state out of frontier
    current = bestofFrontier(); 
    cout << "Best frontier: \n" << "f = " << current.f << " g = "<< current.g << " h = " <<current.h << endl;
    cout << "\nEnter y to expand the next state or n to quit: ";
    cin >> ans;
  }
}