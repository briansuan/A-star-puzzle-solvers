#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<stdlib.h>  
using namespace std;

/* Author: Brian Suan*/

// State description class.
// Could have used struct.
// Items here is a string of L or R for FWDC 
// e.g. RRRR for the initial state
class state {
  public:
    string items;  // made up of L and R going from RRRR to LLLL 
    string camefrom; // the parent on the solution path
    int g;   // cost so far
    int h;   // estimated cost to the goal
    int f;   // g+h
};

// Major data structures:
std::vector<state> frontier;  // Frontier nodes - will pick one of these to expand
std::vector<state> beenThere; // already expanded nodes

// ========== Helper Functions =================== //

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
  vector<state>:: iterator it;

  for(it = frontier.begin(); it != frontier.end(); ++it) {
    if(it->items == x.items) {
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
    if(goal.camefrom == beenThere[i].items) {
      cout << beenThere[i].items << endl;
      tracePath(beenThere[i]);
      break;
    }
  }
}

//========= For Generating a new state to add to Frontier  =============//

// Check to see if next's items is already in frontier and return true or false.
//   If it is already in frontier, 
//   and if the next's f is better, 
//   update the frontier node to next.  (called from generate)
bool inFrontier(state next) {
  // if a state object in next is already in the frontier vector, return true, loop through frontier vector

  // Check the items of every state in frontier and compare with passed state's items
  for(int i=0; i < frontier.size(); i++) {
    // If passed state's items is the same with a state's items in frontier, return true
    if(next.items == frontier[i].items) {
      if(next.f < frontier[i].f) {
        frontier[i].items = next.items;
      }
      // Print out a message
      cout << "State alrrady in Frontier..." << endl;
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
    if(next.items == beenThere[i].items) {
      return true;
    }
  }
  // otherwise return false
  return false;
}

// Check is a state is unsafe, compare the passed state with the given states in the array
bool isUnsafe(state next) {
  // declare the unsafe states
  string unsafe[6] = {"RRLL", "LRRL", "LRRR", "LLRR", "RLLR", "RLLL"};
  // compare each unsafe state with with the passed state
  for(int i =0; i < 6; i ++) {
    // return true if it is unsafe
    if(next.items == unsafe[i]) {
      return true;
    }
  }
  // otherwise return false
  return false;
}

// Try to add next to frontier but stop if LLLL (called from generateAll)
void generate(state next) { 

  cout << "Trying to add: " << next.items << endl; 

  // the goal is reached
  if (next.items  == "LLLL") { 
    cout << ">>Reached: " << next.items << endl;
    // added this to label the goal path
    cout << endl << "Initial to goal path: " << endl;
    tracePath(next);  // display the solution path
    exit(0); 
  }

  
  // else compute h and then f for next
  int h = 0;// start point for h; g is already set
  // ** update h and then f

  for(int i=1; i < next.items.length(); i++) {
    // the h function is how many items are still on the right side
    if(next.items[i] == 'R') {
      // if an item is on the right side, add 1 to h
      h += 1;
    }
  }

  // set h to the state's h attribute
  next.h = h;
  next.f = next.g + next.h;

  // check is a state is unsafe
  if (isUnsafe(next)){
    
  }
  // check is a state is already visited
  else if (inBeenThere(next)){
    
  }
  // check if a state is already in the frontier, add it to frontier if not
  else if (!inFrontier(next)){
    addtoFrontier(next); // add next to Frontier 
  }
  
}  



// Generate all new states from current (called from main)
void generateAll(state current){
  state next;  // a generated one

  // each next will be modified from current for ease
  current.g = current.g + 1; // to update g to be used for next
  // storing the parent so that we can produce the solution path
  current.camefrom = current.items; 

  // check all possibilities as follows..

  // move F alone to L
  next = current;  // starting point of next is current
  if (current.items[0] == 'R') { 
    next.items[0]='L'; 
    generate(next);
  }

  next = current; // starting point of next
  // move F alone to R
  if (current.items[0] == 'L') { 
    next.items[0]='R'; 
    generate(next);
  }

  next = current; // starting point of next
  // move FW to L
  if (current.items[0] == 'R' && current.items[1] == 'R') { 
    next.items[0]='L'; 
    next.items[1] = 'L'; 
    generate(next);
  } 

  next = current; // starting point of next
  // move FW to R
  if (current.items[0] == 'L' && current.items[1] == 'L') { 
    next.items[0]='R'; 
    next.items[1] = 'R'; 
    generate(next);
  }  
  
  next = current; // starting point of next
  // move FD to L
  if (current.items[0] == 'R' && current.items[2] == 'R') { 
    next.items[0]='L'; 
    next.items[2] = 'L'; 
    generate(next);
  }  

  next = current; // starting point of next
  // move FD to R
  if (current.items[0] == 'L' && current.items[2] == 'L') { 
    next.items[0]='R'; 
    next.items[2] = 'R'; 
    generate(next);
  }  

  next = current; // starting point of next
  // move FC to L
  if (current.items[0] == 'R' && current.items[3] == 'R') { 
    next.items[0]='L'; 
    next.items[3] = 'L'; 
    generate(next);
  }  

  next = current; // starting point of next
  // move FC to R 
  if (current.items[0] == 'L' && current.items[3] == 'L') { 
    next.items[0] = 'R'; 
    next.items[3] = 'R'; 
    generate(next);
  }  
}


// Find the best f state out of the frontier and return it (called from main)
state bestofFrontier(){ 

  // choose a state's f value to start with and set that as the current best f
  int f_best = frontier[0].f;
  // choose a state from the frontier vector
  state best_frontier = frontier[0];
  // loop through the frontier to see which in the frontier has the best f
  for(int i=1; i < frontier.size(); i++){
    // compare the current "best" f with a given stat's f from the frontier
    if(frontier[i].f < f_best){
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
void displayFrontier(){
  for (int k = 0; k < frontier.size(); k++){ 
    cout << frontier[k].items << " ";  
    cout << "g = " << frontier[k].g << " ";
    cout << "h = " << frontier[k].h << " ";
    cout << "f = " << frontier[k].f << endl; 
  }
}

// ================= Main ====================== //

int main(){
  state  current = {"RRRR","", 0, 3, 3};  // initial
  addtoFrontier(current);

  char ans;
  while ( ans != 'n'){
    removefromFrontier(current);
    addtoBeenThere(current);

    cout << ">>>Expand:" << current.items << endl; 
    generateAll(current);  // new states are added to frontier

    cout << "Frontier is:" << endl;
    displayFrontier();

    current = bestofFrontier(); // pick the best state out of frontier
    cout << "\nEnter y to expand the next state or n to quit: ";
    cin >> ans;
  }
}

