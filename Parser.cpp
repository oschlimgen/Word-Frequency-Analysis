#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <fstream>

#include "LinkedList.hpp"
#include "StringFunctions.hpp"


#define deliminator '\\'


class Words {
public:
  class Info {
  public:
    std::string word;
    int freqCount;
    std::vector<std::string> syllables;
    std::vector<std::string> pronunciation;

    Info(const std::string& word, const int& freqCount, const std::vector<std::string>& syllables, const std::vector<std::string>& pronunciation) : word(word), freqCount(freqCount), syllables(syllables), pronunciation(pronunciation) {}
    Info(const std::string& wordSeg, const std::string& freqSeg, const std::string& sylSeg, const std::string& pronSeg) {
      word = wordSeg;
      if(!StringFunctions::isInteger(freqSeg)) throw std::invalid_argument("freqSeg argument of Info is not an integer.");
      freqCount = std::stoi(freqSeg);
      syllables = StringFunctions::split(sylSeg, '-');
      pronunciation = StringFunctions::split(pronSeg, '-');
    }

    bool operator==(const Info& other) const {
      return word == other.word;
    }
    bool operator<(const Info& other) const {
      if(word.length() == other.word.length()) {
        for(int i = 0; i < word.length(); ++i) {
          if(word[i] < other.word[i]) {
            return true;
          }
          if(word[i] > other.word[i]) {
            return false;
          }
        }
        return false;
      } else {
        return word.length() < other.word.length();
      }
    }
    bool operator>(const Info& other) const {
      return word > other.word;
    }

    std::string toString(const char& delim) const {
      std::string result;
      result += word + delim;
      result += std::to_string(freqCount) + delim;
      result += StringFunctions::toString(syllables, '-') + delim;
      result += StringFunctions::toString(pronunciation, '-');
      return result;
    }

    void removeQuotations() {
      for(std::string& syl : pronunciation) {
        if(StringFunctions::contains(syl, '\"')) {
          syl = StringFunctions::remove(syl, '\"');
        }
      }
    }
  };

  class Replacement {
  public:
    char c;
    std::string replacement;

    Replacement(const char& c, const std::string& replacement) : c(c), replacement(replacement) {}
  };

private:
  std::string filePath;
  list<Info> data;

public:
  Words() : filePath("Words.txt") {}
  Words(const std::string& path) {
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

  // REMOVE
  void checkLast() {
    if(!data.checkLast()) {
      throw std::runtime_error("Linked list not terminated by nullptr");
    }
  }
  void sort() {
    // struct Compare { bool operator()(Words::Info a, Words::Info b) const { return a.word < b.word; } };
    data.sort();
  }
  void reverse() {
    data.reverse();
  }

  void eliminate() {
    Info* last = &data.back();
    for(list<Info>::iterator i = data.begin(); i != data.end();) {
      bool elim = false;
      const std::string& word = i->word;

      if(i->freqCount == 0) {
        elim = true;
      } else if(!StringFunctions::onlyAlphabetical(word)) {
        elim = true;
      } else if(word != StringFunctions::tolower(word)) {
        elim = true;
      } else if(word == last->word) { // If the word is the same as the last word evaluated
        if(i->freqCount > last->freqCount) { // If the variation of the word is more common
          i->removeQuotations();
          *last = *i; // Replace the pronunciation with the more common one
        }
        elim = true;
      }

      if(elim) {
        data.remove(i);
      } else {
        i->removeQuotations();
        last = &(*i);
        ++i;
      }
    }
  }

  void replacePron(const std::vector<Replacement>& replacements) {
    for(Info& i : data) {
      for(std::string& syl : i.pronunciation) {
        for(const Replacement& r : replacements) {
          syl = StringFunctions::replace(syl, r.c, r.replacement);
        }
      }
    }
  }

  void read() {
    std::ifstream file(filePath);

    if(file.is_open()) {
      std::string line;
      int lineNum = 0;
      bool firstLine = true;
      data.clear();

      while(std::getline(file, line)) {
        lineNum++;
        if(line == "") continue;
        if(line.length() > 1 && line[0] == '#' && line[1] == '#') continue;
        if(line[line.length() - 1] == '\r') {
          line.pop_back();
        }
        if(StringFunctions::onlyContains(line, ' ')) continue;

        if(firstLine) {
          firstLine = false;
          continue;
        }
        
        std::vector<std::string> segments = StringFunctions::split(line, deliminator);

        if(segments.size() != 4) throw std::runtime_error("Wrong number of segments in line " + std::to_string(lineNum) + ". Segments found: " + std::to_string(segments.size()));
        if(!StringFunctions::isInteger(segments[1])) throw std::runtime_error("Second segment of line " + std::to_string(lineNum) + " is not an integer.");
        Info info = Info(segments[0], segments[1], segments[2], segments[3]);

        data.add(info);
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;
      file << "Word\\Cob\\WordSyl\\PhonSylCLX" << std::endl;

      for(const Info& i : data) {
        file << i.toString(deliminator) << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};


class Syllables {
public:
  class Info {
  public:
    std::string pronunciation;
    int freqCount;

    Info(const std::string& pronunciation, const int& freqCount) : pronunciation(pronunciation), freqCount(freqCount) {}

    Info(const std::string& pronSeg, const std::string& freqSeg) {
      pronunciation = pronSeg;
      if(!StringFunctions::isInteger(freqSeg)) throw std::invalid_argument("freqSeg argument of Info is not an integer.");
      freqCount = std::stoi(freqSeg);
    }

    std::string toString(const char& delim) const {
      return (pronunciation + delim + std::to_string(freqCount));
    }
  };

private:
  std::string filePath;
  list<Info> data;
  std::unordered_map<std::string, int> counts;

public:
  Syllables() : filePath("Syllables.txt") {}
  Syllables(const std::string& path) {
    setPath(path);
  }

  void clear() {
    data.clear();
    counts.clear();
  }

  std::string getPath() const { return filePath; }
  void setPath(const std::string& path) { filePath = path; }

  int size() const { return data.size(); }
  const list<Info>& getData() const { return data; }
  const Info& getInfoAt(const int& i) const { return data.at(i); }
  int getSylFreq(const std::string& syl) const {
    auto it = counts.find(syl);
    if(it == counts.end()) return 0;
    return it->second;
  }

  void import(const Words& words) {
    clear();

    const list<Words::Info>& wordList = words.getData();
    for(const Words::Info& i : wordList) {
      for(const std::string& pron : i.pronunciation) {
        counts[pron] += i.freqCount;
      }
    }

    for(const std::pair<std::string, int>& i : counts) {
      data.add(Info(i.first, i.second));
    }
  }

  void eliminate(const std::string& vowels) {
    for(list<Info>::iterator i = data.begin(); i != data.end();) {
      int vowelCount = 0;
      for(const char& c : i->pronunciation) {
        if(StringFunctions::contains(vowels, c)) {
          vowelCount++;
        }
      }

      if(vowelCount != 1) {
        data.remove(i);
      } else {
        ++i;
      }
    }
  }

  void sort() {
    auto compare = [&](const Info& a, const Info& b) { return a.freqCount > b.freqCount; };
    data.sort(compare);
  }

  void read() {
    std::ifstream file(filePath);

    if(file.is_open()) {
      std::string line;
      int lineNum = 0;
      bool firstLine = true;
      data.clear();

      while(std::getline(file, line)) {
        lineNum++;
        if(line == "") continue;
        if(line.length() > 1 && line[0] == '#' && line[1] == '#') continue;
        if(line[line.length() - 1] == '\r') {
          line.pop_back();
        }
        if(StringFunctions::onlyContains(line, ' ')) continue;

        if(firstLine) {
          firstLine = false;
          continue;
        }
        
        std::vector<std::string> segments = StringFunctions::split(line, deliminator);

        if(segments.size() != 2) throw std::runtime_error("Wrong number of segments in line " + std::to_string(lineNum) + ". Segments found: " + std::to_string(segments.size()));
        if(!StringFunctions::isInteger(segments[1])) throw std::runtime_error("Second segment of line " + std::to_string(lineNum) + " is not an integer.");
        
        Info info(segments[0], segments[1]);
        data.add(info);
        counts[info.pronunciation] = info.freqCount;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }

  void write() const {
    std::ofstream file(filePath);
    if(file.is_open()) {
      file << "## This file was generated by code using data from another file." << std::endl;
      file << std::endl;
      file << "Syllable\\Count" << std::endl;

      for(const Info& i : data) {
        file << i.toString(deliminator) << std::endl;
      }

      file.close();
    } else {
      throw std::invalid_argument("File path not valid.");
    }
  }
};

