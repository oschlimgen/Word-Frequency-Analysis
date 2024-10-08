#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <fstream>

#include "LinkedList.hpp"
#include "StringFunctions.hpp"

#include "Parser.cpp"


#ifdef deliminator
  #undef deliminator
#endif
#define deliminator ','


class Phonemes {
public:
  class Info {
  public:
    char sound;
    int freqCount;

    Info(const char& sound, const int& freqCount) : sound(sound), freqCount(freqCount) {}

    Info(const std::string& soundSeg, const std::string& freqSeg) {
      sound = soundSeg.at(0);
      if(!StringFunctions::isInteger(freqSeg)) throw std::invalid_argument("freqSeg argument of Info is not an integer.");
      freqCount = std::stoi(freqSeg);
    }

    std::string toString(const char& delim) const {
      std::string result;
      result += sound;
      result += delim;
      result += std::to_string(freqCount);
      return result;
    }
  };

private:
  std::string filePath;
  list<Info> data;

public:
  Phonemes() : filePath("Phonemes.txt") {}
  Phonemes(const std::string& path) {
    setPath(path);
  }

  void clear() {
    data.clear();
  }

  std::string getPath() const { return filePath; }
  void setPath(const std::string& path) { filePath = path; }

  int size() const { return data.size(); }
  const list<Info>& getData() const { return data; }
  const Info& getInfoAt(const int& i) const { return data.at(i); }

  void count(const Syllables& syllables) {
    std::unordered_map<char,int> phonemeCounts;

    data.clear();

    const list<Syllables::Info>& sylList = syllables.getData();
    for(const Syllables::Info& i : sylList) {
      for(const char& p : i.pronunciation) {
        phonemeCounts[p] += i.freqCount;
      }
    }

    for(const std::pair<char, int>& i : phonemeCounts) {
      data.add(Info(i.first, i.second));
    }
  }

  void sort() {
    auto compare = [&](const Info& a, const Info& b) { return a.freqCount > b.freqCount; };
    data.sort(compare);
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;
      file << "Phoneme" << deliminator << "Count" << std::endl;

      for(const Info& i : data) {
        file << i.toString(deliminator) << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};


class Blends {
public:
  class Info {
  public:
    std::string blend;
    int freqCount;

    Info(const std::string& blend, const int& freqCount) : blend(blend), freqCount(freqCount) {}

    Info(const std::string& blendSeg, const std::string& freqSeg) {
      blend = blendSeg;
      if(!StringFunctions::isInteger(freqSeg)) throw std::invalid_argument("freqSeg argument of Info is not an integer.");
      freqCount = std::stoi(freqSeg);
    }

    std::string toString(const char& delim) const {
      std::string result;
      result += blend;
      result += delim;
      result += std::to_string(freqCount);
      return result;
    }
  };

private:
  std::string filePath;
  list<Info> data;

public:
  Blends() : filePath("Blends.txt") {}
  Blends(const std::string& path) {
    setPath(path);
  }

  void clear() {
    data.clear();
  }

  std::string getPath() const { return filePath; }
  void setPath(const std::string& path) { filePath = path; }

  int size() const { return data.size(); }
  const list<Info>& getData() const { return data; }
  const Info& getInfoAt(const int& i) const { return data.at(i); }

  void count(const Syllables& syllables, const std::string& consonants) {
    std::unordered_map<std::string,int> blendCounts;

    data.clear();

    std::string blend;
    const list<Syllables::Info>& sylList = syllables.getData();
    for(const Syllables::Info& i : sylList) {
      for(const char& p : i.pronunciation) {
        if(StringFunctions::contains(consonants, p)) {
          blend += p;
        } else {
          if(blend.length() > 1) {
            blendCounts[blend] += i.freqCount;
          }
          blend = "";
        }
      }

      if(blend.length() > 1) {
        blendCounts[blend] += i.freqCount;
      }
      blend = "";
    }

    for(const std::pair<std::string, int>& i : blendCounts) {
      data.add(Info(i.first, i.second));
    }
  }

  void sort() {
    auto compare = [&](const Info& a, const Info& b) { return a.freqCount > b.freqCount; };
    data.sort(compare);
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;
      file << "Blend" << deliminator << "Count" << std::endl;

      for(const Info& i : data) {
        file << i.toString(deliminator) << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};


class Overlap {
public:
  class Info {
  public:
    char a;
    char b;
    int freqCount;

    Info(const char& a, const char& b, const int& freqCount) : a(a), b(b), freqCount(freqCount) {}

    Info(const std::string& aSeg, const std::string& bSeg, const std::string& freqSeg) {
      a = aSeg.at(0);
      b = bSeg.at(0);
      if(!StringFunctions::isInteger(freqSeg)) throw std::invalid_argument("freqSeg argument of Info is not an integer.");
      freqCount = std::stoi(freqSeg);
    }

    std::string toString(const char& delim) const {
      std::string result;
      result += a;
      result += delim;
      result += b;
      result += delim;
      result += std::to_string(freqCount);
      return result;
    }
  };

private:
  std::string filePath;
  std::vector<std::vector<Info>> data;

public:
  Overlap() : filePath("Overlap.txt") {}
  Overlap(const std::string& path) {
    setPath(path);
  }

  void clear() {
    data.clear();
  }

  std::string getPath() const { return filePath; }
  void setPath(const std::string& path) { filePath = path; }

  int size() const { return data.size(); }
  const std::vector<std::vector<Info>>& getData() const { return data; }
  const Info& getInfoAt(const int& i, const int& j) const { return data.at(i).at(j); }

  void count(const Syllables& syllables, const std::string& phonemes) {
    std::unordered_map<char, int> phonemeNums;

    clear();

    int count = 0;
    for(const char& c : phonemes) {
      phonemeNums[c] = count;
      ++count;
    }

    for(const char& a : phonemes) {
      std::vector<Info> v;
      for(const char& b : phonemes) {
        v.push_back(Info(a, b, 0));
      }
      data.push_back(v);
    }

    const list<Syllables::Info>& sylList = syllables.getData();
    for(const Syllables::Info& syl : sylList) {
      for(const char& i : syl.pronunciation) {
        if(StringFunctions::contains(phonemes, i)) {
          for(const char& p : phonemes) {
            std::string modifiedSyl = StringFunctions::replace(syl.pronunciation, i, p);
            data.at(phonemeNums.at(i)).at(phonemeNums.at(p)).freqCount += syllables.getSylFreq(modifiedSyl);
          }
        }
      }
    }

    for(int i = 0; i < data.size(); ++i) {
      for(int j = i; j < data.size(); ++j) {
        data.at(i).at(j).freqCount = (data.at(i).at(j).freqCount + data.at(j).at(i).freqCount) / 2;
      }
      for(int j = 0; j < i; ++j) {
        data.at(i).at(j).freqCount = data.at(j).at(i).freqCount;
      }
    }
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;

      std::string firstLine = "Overlap";
      for(const Info& i : data.at(0)) {
        firstLine += deliminator;
        firstLine += i.b;
      }
      file << firstLine << std::endl;

      for(const std::vector<Info>& vec : data) {
        std::string line = std::string() + vec.at(0).a;
        for(const Info& i : vec) {
          line += (deliminator + std::to_string(i.freqCount));
        }
        file << line << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};


class Positional {
public:
  class Info {
  public:
    char sound;
    int startFreq;
    int endFreq;
    float percentStart;

    Info(const char& sound, const int& startFreq, const int& endFreq, const float& percentStart) : sound(sound), startFreq(startFreq), endFreq(endFreq), percentStart(percentStart) {}

    std::string toString(const char& delim) const {
      std::string result;
      result += sound;
      result += delim;
      result += std::to_string(startFreq);
      result += delim;
      result += std::to_string(endFreq);
      result += delim;
      result += std::to_string(percentStart * 100.0f);
      result += '%';
      return result;
    }
  };

private:
  std::string filePath;
  list<Info> data;

public:
  Positional() : filePath("Positional.txt") {}
  Positional(const std::string& path) {
    setPath(path);
  }

  void clear() {
    data.clear();
  }

  std::string getPath() const { return filePath; }
  void setPath(const std::string& path) { filePath = path; }

  int size() const { return data.size(); }
  const list<Info>& getData() const { return data; }
  const Info& getInfoAt(const int& i) const { return data.at(i); }

  void count(const Syllables& syllables, const std::string& consonants) {
    std::unordered_map<char, int> startCounts;
    std::unordered_map<char, int> endCounts;

    clear();

    for(const char& c : consonants) {
      startCounts.insert(std::make_pair(c, 0));
      endCounts.insert(std::make_pair(c, 0));
    }

    const list<Syllables::Info>& sylList = syllables.getData();
    for(const Syllables::Info& syl : sylList) {
      bool vowel = false;
      for(const char& i : syl.pronunciation) {
        if(StringFunctions::contains(consonants, i)) {
          if(!vowel) {
            startCounts.at(i) += syl.freqCount;
          } else {
            endCounts.at(i) += syl.freqCount;
          }
        } else {
          vowel = true;
        }
      }
    }

    for(const char& c : consonants) {
      int start = startCounts.at(c);
      int end = endCounts.at(c);
      float percent = (float)start / (float)(start + end);
      data.add(Info(c, start, end, percent));
    }
  }

  void sortByStart() {
    auto compare = [&](const Info& a, const Info& b) { return a.startFreq > b.startFreq; };
    data.sort(compare);
  }

  void sortByEnd() {
    auto compare = [&](const Info& a, const Info& b) { return a.endFreq > b.endFreq; };
    data.sort(compare);
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;

      std::string firstLine = "Positional Counts,Starting,Ending";
      file << firstLine << std::endl;

      for(const Info& i : data) {
        std::string line = i.toString(deliminator);
        file << line << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }

  void write(const bool& start, const bool& end) const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;

      std::string firstLine = "Positional Counts,Starting,Ending";
      file << firstLine << std::endl;

      for(const Info& i : data) {
        std::string line = std::string() + i.sound;
        if(start) {
          line += (deliminator + std::to_string(i.startFreq));
        }
        if(end) {
          line += (deliminator + std::to_string(i.endFreq));
        }

        if(start) {
          line += (deliminator + std::to_string(i.percentStart * 100.0f) + '%');
        } else if(end) {
          line += (deliminator + std::to_string((1.0f - i.percentStart) * 100.0f) + '%');
        }
        file << line << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};

