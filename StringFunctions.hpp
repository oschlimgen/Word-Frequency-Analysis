#pragma once


#include <string>
#include <vector>


namespace StringFunctions {
  /* Returns true if the string contains character c. */
  bool contains(const std::string& str, const char& c) {
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it == c) {
        return true;
      }
    }
    return false;
  }

  /* Returns true if the string is empty or its only characters are c. */
  bool onlyContains(const std::string& str, const char& c) {
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it != c) {
        return false;
      }
    }
    return true;
  }

  bool onlyAlphabetical(const std::string& str) {
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(!std::isalpha(*it)) {
        return false;
      }
    }
    return true;
  }

  std::string remove(const std::string& str, const char& c) {
    std::string result;
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it != c) {
        result += *it;
      }
    }
    return result;
  }

  std::string replace(const std::string& str, const char& c, const char& replacement) {
    std::string result;
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it == c) {
        result += replacement;
      } else {
        result += *it;
      }
    }
    return result;
  }
  std::string replace(const std::string& str, const char& c, const std::string& replacement) {
    std::string result;
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it == c) {
        result += replacement;
      } else {
        result += *it;
      }
    }
    return result;
  }

  std::string tolower(const std::string& str) {
    std::string result;
    for(auto it = str.begin(); it != str.end(); ++it) {
      result += std::tolower(*it);
    }
    return result;
  }

  bool isInteger(const std::string& str) {
    if(str.length() == 0) return false;

    auto it = str.begin();
    if(*it == '-') {
      if(str.length() == 1) return false;
      ++it;
    }

    for(; it != str.end(); ++it) {
      if(*it < '0' || *it > '9') {
        return false;
      }
    }

    return true;
  }

  std::vector<std::string> split(const std::string& str, const char& delim) {
    std::vector<std::string> result;

    std::string seg = "";
    for(auto it = str.begin(); it != str.end(); ++it) {
      if(*it == delim) {
        result.push_back(seg);
        seg = "";
      } else {
        seg += *it;
      }
    }
    result.push_back(seg);

    return result;
  }

  std::string toString(const std::vector<std::string>& data, const char& delim) {
    if(data.size() == 0) return "";

    auto it = data.begin();
    std::string result = *it;
    ++it;
    for(; it != data.end(); ++it) {
      result += (delim + *it);
    }

    return result;
  }
}
