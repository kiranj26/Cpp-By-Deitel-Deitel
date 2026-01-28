

#include <iostream>

using std::cout;
using std::endl;

/*
  Linear Search Normal : return searcg index if the key is found else -1
*/

int linearSearch(int arr[], int arrSize, int key)
{
  for(int i = 0; i < arrSize ; i++)
  {
    if (arr[i] == key)
    {
      return i+1;
    }
  }
  return -1;
}

int linearSearchRecursive(int arr[], int start, int end, int key)
{
  if (start == end)
  {
    return -1; // end of array no element found
  }
  if (arr[start] == key)
  {
    return start + 1;
  }

  return (linearSearchRecursive(arr, start + 1, end,key));
}
int main() {

    int arr[] = {1,2,3,4,5,6,7,8,9};
    int key = 9;
    cout << "The key " << key << " is preset in array at index " << linearSearchRecursive(arr, 0,9, key) << endl;
    return 0;
}
