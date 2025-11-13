#include <bits/stdc++.h>
using namespace std;

// ------------------- Trie Node -------------------
struct TrieNode {
    bool isEnd;
    unordered_map<char, TrieNode*> children;

    TrieNode() {
        isEnd = false;
    }
};

// ------------------- Trie Class -------------------
class Trie {
    TrieNode* root;
public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a word into the Trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
    }

    // Load all words from a file (like words.txt)
    void loadWords(const string& filename) {
        ifstream file(filename);
        string word;
        while (file >> word) {
            insert(word);
        }
        file.close();
    }

    // DFS to collect all words that start with the given prefix
    void dfs(TrieNode* node, string prefix, vector<string>& result) {
        if (result.size() >= 10) return; // limit to 10 suggestions
        if (node->isEnd) result.push_back(prefix);

        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            dfs(it->second, prefix + it->first, result);
        }
    }

    // Get top suggestions for a given prefix
    vector<string> getSuggestions(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                return {};
            }
            node = node->children[c];
        }
        vector<string> result;
        dfs(node, prefix, result);
        return result;
    }
};

// ------------------- MAIN FUNCTION -------------------
int main() {
    Trie trie;
    trie.loadWords("words.txt");  // make sure words.txt is in same folder

    string prefix;
    ofstream out("web/output.txt"); // output file inside 'web' folder

    cout << "🔍 Text Autocomplete Project (type 'exit' to stop)\n";

    while (true) {
        cout << "\nEnter prefix: ";
        cin >> prefix;

        if (prefix == "exit") break;

        vector<string> suggestions = trie.getSuggestions(prefix);

        // Write suggestions to file
        out << "Prefix: " << prefix << "\n";
        if (suggestions.empty()) {
            out << "No suggestions found.\n";
        } else {
            out << "Suggestions:\n";
            for (auto& word : suggestions)
                out << "- " << word << "\n";
        }
        out << "----------------------\n";
        out.flush();

        cout << "✅ Suggestions saved to web/output.txt\n";
    }

    out.close();
    return 0;
}
