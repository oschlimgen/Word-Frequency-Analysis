#include <iostream>
#include <chrono>

#include "LinkedList.hpp"
#include "Analysis.cpp"



int main() {
  /*
  list<int> a;
  for(int i = 0; i < 20; ++i) {
    a.add(i);
  }
  std::cout << a.size() << ", " << a.back() << std::endl;

  for(int& val : a) {
    val *= 2;
  }
  std::cout << a.at(12) << ", " << a.back() << std::endl;

  a.resize(10, 0);
  int count = 0;
  for(int& val : a) {
    count++;
  }
  std::cout << count << ", " << a.back() << std::endl;

  a.insert(0, 100);
  std::cout << a[0] << ", " << a.back() << std::endl;

  a.resize(20, 1);
  a[a.size() - 1] = 3;
  count = 0;
  for(int& val : a) {
    count++;
  }
  std::cout << count << ", " << a.back() << std::endl;

  a.insert(20, 4);
  std::cout << a.at(20) << ", " << a.back() << std::endl;

  
  auto compare = [&](const int& a, const int& b) { return a > b; };
  a.sort();
  std::string line;
  for(const int& i : a) {
    line += std::to_string(i) + ',';
  }
  std::cout << line << std::endl;

  for(auto i = a.begin(); i != a.end();) {
    if(i.value() < 5) {
      a.remove(i);
    } else {
      ++i;
    }
  }
  std::cout << a.size() << ", " << a.back() << std::endl;
  */

  const std::string sections = "0123";

  const std::vector<Words::Replacement> replacements = {
    Words::Replacement('R', "r"),
    Words::Replacement('9', "u"),
    Words::Replacement('H', "@n"),
    Words::Replacement('P', "@l"),
    Words::Replacement('~', "Q"),
    Words::Replacement('q', "Q"),
    Words::Replacement('x', "g"),
    Words::Replacement('c', "I"),
    Words::Replacement('0', "Q"),
    Words::Replacement('F', "m"),
    Words::Replacement('T', "D"),
    Words::Replacement('Z', "zh"),
    Words::Replacement('j', "Y"),
    Words::Replacement('N', "Y"),
    Words::Replacement('U', "@") };
  
  const std::string vowels = {
    '@', 'I', '{', 'E', 'V',
    'i', 'u', 'Q', '2', '1',
    '5', '3', '#', '$', '6',
    '8', '7', '4' };
  const std::string consonants = {
    't', 'n', 'r', 's', 'd',
    'l', 'k', 'D', 'z', 'm',
    'h', 'v', 'w', 'p', 'Y',
    'b', 'f', 'S', 'g', 'J',
    '_' };

  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point end;
  int duration;

  if(StringFunctions::contains(sections, '0')) {
    Words input("data/CelexCountSylPron.txt");
    
    start = std::chrono::high_resolution_clock::now();
    input.read();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Reading File. Duration: " << duration << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    input.eliminate();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Removing Words. Duration: " << duration << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    input.sort();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time to sort: " << duration << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    input.reverse();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time to reverse: " << duration << "ms" << std::endl;

    
    input.checkLast();
    list<Words::Info>::const_iterator i = input.getData().begin();
    while(i + 1) {
      ++i;
    }
    std::cout << "New Final Word: " << i.node().word << std::endl;

    start = std::chrono::high_resolution_clock::now();
    input.replacePron(replacements);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Replacing Phonemes. Duration: " << duration << "ms" << std::endl;

    input.setPath("data/CuratedPronunciation.txt");
    input.write();

    std::cout << "Number of words: " << input.size() << std::endl;

    Syllables sylCounts("data/SyllableCounts.txt");

    start = std::chrono::high_resolution_clock::now();
    sylCounts.import(input);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Converting Words to Syllables. Duration: " << duration << "ms" << std::endl;

    int initialSylCount = sylCounts.size();
    start = std::chrono::high_resolution_clock::now();
    sylCounts.eliminate(vowels);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Removing Syllables. Removed " << initialSylCount - sylCounts.size() << ", Duration: " << duration << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    sylCounts.sort();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Sorting Syllables. Duration: " << duration << "ms" << std::endl;

    if(!sylCounts.getData().checkLast()) {
      throw std::runtime_error("After sorting not terminated by nullptr");
    }

    sylCounts.write();
  }


  Syllables sylCounts("data/SyllableCounts.txt");
  sylCounts.read();


  if(StringFunctions::contains(sections, '1')) {
    Phonemes phonemes("data/PhonemeCounts.txt");

    start = std::chrono::high_resolution_clock::now();
    phonemes.count(sylCounts);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Converting Syllables to Phonemes. Duration: " << duration << "ms" << std::endl;

    phonemes.sort();
    phonemes.write();


    Blends blends("data/BlendCounts.txt");

    start = std::chrono::high_resolution_clock::now();
    blends.count(sylCounts, consonants);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Converting Syllables to Blends. Duration: " << duration << "ms" << std::endl;

    blends.sort();
    blends.write();
  }

  if(StringFunctions::contains(sections, '2')) {
    Positional ps;

    start = std::chrono::high_resolution_clock::now();
    ps.count(sylCounts, consonants);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Counting Consonant Positions. Duration: " << duration << "ms" << std::endl;

    ps.setPath("data/StartPosFreqs.txt");
    ps.sortByStart();
    ps.write(true, false);

    ps.setPath("data/EndPosFreqs.txt");
    ps.sortByEnd();
    ps.write(false, true);
  }

  
  if(StringFunctions::contains(sections, '3')) {
    Overlap vOverlap("data/VowelOverlap.csv");

    start = std::chrono::high_resolution_clock::now();
    vOverlap.count(sylCounts, vowels);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Counting Vowel Overlap. Duration: " << duration << "ms" << std::endl;

    vOverlap.write();

    Overlap cOverlap("data/ConsonantOverlap.csv");

    start = std::chrono::high_resolution_clock::now();
    cOverlap.count(sylCounts, consonants);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Finished Counting Consonant Overlap. Duration: " << duration << "ms" << std::endl;

    cOverlap.write();
  }


  std::cout << "Hello World!" << std::endl;
  
  return 0;
}