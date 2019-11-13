///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_DARK, DISK_LIGHT };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_DARK) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_LIGHT;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    // TODO: Write code for this function, including rewriting the return
    // statement, and then delete these comments.
  if(_colors.size() < 1){ // The vector should hold more than 1 element to compare the others
    return false;
  }
  // if there are more than 1 element than it will read this next if statement
  if(_colors[0] == DISK_LIGHT){  // if first index holds the DISK_LIGHT mentioned above with requirement
      for(int i =0; i < _colors.size(); i++){ // iterating through the whole std::vector<int>
        if(_colors[i] == _colors[i+2]){   // if even indexes are same and odd indexes are same we good
          // [0] [1] [2] [3]
          //  l   d   l   d then it means that its alternating
          return true;
        }
      }

    }
    else{
      return false;
    }
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the right (high indices) and all dark disks on the left (low
  // indices).
  bool is_sorted() const
  {
    // TODO: Write code for this function, including rewriting the return
    // statement, and then delete these comments.
    if(_colors.size() < 1){      // base case to mamke sure that there are more than 1 element
      return false;
    }
    for(int i =0; i <_colors.size(); i++){ // iterate through the whole vector
      int half = _colors.size()/2;         // divide the size in half
      if(_colors[half+i] == DISK_LIGHT){   // if array from the half till the end is light
        return true;                       // you got it
      }
      else{
        return false;                      // if not you didnt get it
      }
    }

  }

};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {

  // comments deleted
  int swapNum =0; // this will count the number of swaps in the function

  disk_state disk = before;   // this will hold the disk_state& before address to disk

  for(int i =0; i< disk.total_count(); i++){    // iterating through the whole array
    for(int j=0; j < disk.total_count()-1; j++){  // iterating thorugh the whole array -1 so the last
                                                  // one isnt comparing to the NULL
      if (disk.get(j) == DISK_LIGHT && disk.get(j+1) == DISK_DARK){
        // [0] [1] [2] [3] [4]
        //  l   d   l   d   l swap it & swapNum++ & j =1
        // [0] [1] [2] [3] [4]
        //  d   l   l   d   l swap index 1 and index 3 & swapNum++ & j =2
        // [0] [1] [2] [3] [4]
        //  d   d   l   l      and so on....
        disk.swap(j);
        swapNum++;
      }
    }
  }
  return sorted_disks(disk, swapNum++);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // Comments deleted on top
  disk_state disk = before;
  int counter =0;

  for(int i=0; i<disk.total_count()/2; i++)  { // most outerloop will iterate half the size
    // inner two forloop will each do other half to finish it.
    // for example: leftmost to right n/2 and rightmost to left n/2 they will complete the whole array
    // dividng the task in two therefore, outerloop only has to iterate half the size

    // left to middle forloop
    for(int left =0; left < disk.total_count() - i -1;left++){ // i is there to make sure this loop iterate HALF the size
      if(disk.get(left) == DISK_LIGHT && disk.get(left+1) == DISK_DARK){
        disk.swap(left);
        counter++;
        // [l] [d] [l] [d]| [] [] [] []
        // [0] [1] [2] [3]    swap 0 and 1  counter++
        // [d] [l] [l] [d]   and so on...

      }
    }
    // right to middle forloop
    for(int right = disk.total_count()-2; right > 0; right--){
      if(disk.get(right) ==DISK_LIGHT && disk.get(right+1) == DISK_DARK){
        disk.swap(right);
        counter++;
        // []  []  []  []  | [l] [d] [l] [d]    disk.total_count()-2 will point at index 6
        // [0] [1] [2] [3]   [4] [5] [6] [7]    count will point to the null char at the end of the array so -2 was used
        //                            l   d      swap them because we want all the dark on the leftmost
        //                       now points to 5 & 6 check if-statement and if applies swap again and so on.
      }
    }

  }

  return sorted_disks(disk, counter);
}
