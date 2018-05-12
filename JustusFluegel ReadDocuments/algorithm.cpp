using namespace std;

// Example program
#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>

int main() {
  std::vector<std::vector<string>> markers ( ("00:30:00", "1")
                                           , ("00:40:00", "1")
                                           );

  // cut off decimals are intended

  string ffmpeg_str = "ffmpeg -i in.ts -filter_complex \\\n";

  for (int i; i < (markers.size() / 2); i++) {
      int startIndex = i * 2;
      int endIndex = i * 2 + 1;

      string startTime = markers.at(startIndex).at(0);
      string endTime = markers.at(endIndex).at(0);

      ffmpeg_str = ffmpeg_str + "[0:v]trim=start=[" + startTime + ":end=[" + endTime + "][a]; \\\n";
  }

  cout << ffmpeg_str;

  //cout << markers.at(0).at(0);
}


// string calculateCut(int iteration) {
// //     startIndex = iteration * 2 + 1;
// //     endIndex = iteration * 2 + 2;

// //     return "Hello";
// // }
