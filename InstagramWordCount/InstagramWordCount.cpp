#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using std::string;

string POSTSTART = "<div class=\"_3-95 _2pim _a6-h _a6-i\">"; //all posts begin with this div class
string POSTEND = "</div>";


//returns the index of the closest forward post in a given starting post
//returns -1 if there are no more posts in the string
int findPostIndex(int startIndex, string &file) {
    for (int i = startIndex; i < file.size() - POSTSTART.size() + 1; i++) {
        if (file[i] == '<') {
            string temp = file.substr(i, POSTSTART.size());
            if (temp == POSTSTART) {
                return i += POSTSTART.size();
            }
        }
    }

    return -1;
}

//returns the post as a string, if fails returns empty string
string readPost(int startIndex, string &file) {
    string ans;

    for (int i = startIndex; i < file.size() - POSTEND.size() + 1; i++) {
        ans.push_back(file[i]);
        if (file[i] == '<') {
            string temp = file.substr(i, POSTEND.size());
            if (temp == POSTEND) {
                ans.pop_back();
                return ans;
            }
        }
    }

    return "";
}


//merge all posts into one string
string mergePosts(string &file) {
    string ans;

    for (int i = 0; i < file.size();) {
        int next = findPostIndex(i, file);
        if (next == -1)
            break;
        string post = readPost(next, file);
        ans += post;
        i = next + post.size();
    }

    return ans;
}

//in a given string with no extra spaces, returns the first word at an index
string readWord(int startIndex, string& file) {
    string ans;
    for (int i = startIndex; i < file.size(); i++) {
        if (file[i] + 0 >= 97 && file[i] + 0 <= 122) {
            ans.push_back(file[i]);
        }
        else if (file[i] + 0 == -30) {
            ans.push_back('\'');
            i += 2;   // since the apostrophe is non-ascii character, it takes up 3 spaces and has a decimal code of -30
        }
        else if (file[i] + 0 >= 65 && file[i] + 0 <= 90) {
            ans.push_back(file[i] += 32);
        }
        else
            break;
    }

    return ans;
}


//returns file as a single string
string getFile(std::ifstream &file) {
    string line, all;

    for (int i = 0; i < 221; i++) {
        std::getline(file, line);
        all += line;
    }

    return all;
}

int main() {
    std::unordered_map<string, int> wordCount;
    std::ifstream File("posts_1.html");
    string file = getFile(File);
    File.close();

    file = mergePosts(file);

    std::ofstream output;
    output.open("output.txt");

    for (int i = 0; i < file.size(); i++) {
        string temp = readWord(i, file);
        if(temp != "")
            wordCount[temp]++;
        i += temp.size();
    }

    std::vector<std::pair<string, int>> sorted(wordCount.begin(), wordCount.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& left, auto& right) {
        return left.second < right.second;                                          //adds hashmap to pair vector and sorts it by occurences
    });

    for (int i = sorted.size() - 1; i >= 0; i--) {
        if (sorted[i].second >= 0) {
            output << sorted[i].first << ": " << sorted[i].second << "\n";
        }
    }

    output.close();
    return 0;
}
