#include <bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    char data;
    TrieNode* children[26];
    bool isTerminal;
    int freq;   // frequency of complete word

    TrieNode(char ch) {
        data = ch;
        isTerminal = false;
        freq = 0;
        for(int i = 0; i < 26; i++)
            children[i] = NULL;
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    /* ================= INSERT ================= */
    void insert(string word) {
        insertword(root, word);
    }

    void insertword(TrieNode* root, string word) {
        if(word.length() == 0) {
            root->isTerminal = true;
            root->freq++;          // count frequency
            return;
        }

        int index = word[0] - 'a';
        TrieNode* child;

        if(root->children[index] != NULL)
            child = root->children[index];
        else {
            child = new TrieNode(word[0]);
            root->children[index] = child;
        }

        insertword(child, word.substr(1));
    }

    /* ================= SEARCH ================= */
    bool search(string word) {
        return searchword(root, word);
    }

    bool searchword(TrieNode* root, string word) {
        if(word.length() == 0)
            return root->isTerminal;

        int index = word[0] - 'a';
        if(root->children[index] == NULL)
            return false;

        return searchword(root->children[index], word.substr(1));
    }

    /* ================= PREFIX ================= */
    bool startsWith(string prefix) {
        return pref(root, prefix);
    }

    bool pref(TrieNode* root, string word) {
        if(word.length() == 0)
            return true;

        int index = word[0] - 'a';
        if(root->children[index] == NULL)
            return false;

        return pref(root->children[index], word.substr(1));
    }

    /* ================= AUTOCOMPLETE ================= */
    vector<string> autoComplete(string prefix, int k) {
        TrieNode* node = root;

        for(char c : prefix) {
            int index = c - 'a';
            if(node->children[index] == NULL)
                return {};
            node = node->children[index];
        }

        vector<pair<int,string>> results;
        string curr = prefix;
        dfs(node, curr, results);

        sort(results.begin(), results.end(),
             [](auto &a, auto &b) {
                if(a.first != b.first)
                    return a.first > b.first;
                return a.second < b.second;
             });

        vector<string> ans;
        for(int i = 0; i < min(k, (int)results.size()); i++)
            ans.push_back(results[i].second);

        return ans;
    }

    /* ================= DFS ================= */
    void dfs(TrieNode* root, string &curr,
             vector<pair<int,string>> &res) {
        if(root->isTerminal)
            res.push_back({root->freq, curr});

        for(int i = 0; i < 26; i++) {
            if(root->children[i] != NULL) {
                curr.push_back('a' + i);
                dfs(root->children[i], curr, res);
                curr.pop_back();
            }
        }
    }
    void deleteWord(string word) {
    deleteUtil(root, word, 0);
       }
       bool hasChildren(TrieNode* node) {
    for(int i = 0; i < 26; i++) {
        if(node->children[i] != NULL)
            return true;
    }
    return false;
}

bool deleteUtil(TrieNode* root, string word, int depth) {
    if(root == NULL)
        return false;

    if(depth == word.length()) {

        if(!root->isTerminal)
            return false;   

        root->isTerminal = false;
        root->freq = 0;    

        
        return !hasChildren(root);
    }

    int index = word[depth] - 'a';

    if(root->children[index] == NULL)
        return false;   

    bool shouldDeleteChild =
        deleteUtil(root->children[index], word, depth + 1);

    
    if(shouldDeleteChild) {
        delete root->children[index];
        root->children[index] = NULL;

    
        return (!root->isTerminal && !hasChildren(root));
    }

    return false;
}
};

/* ================= DRIVER ================= */
int main() {
    Trie t;

    t.insert("app");
    t.insert("app");
    t.insert("apple");
    t.insert("application");
    t.insert("apply");
    t.insert("bat");
    t.insert("ball");
    t.insert("ball");

    cout << t.search("app") << endl;      // 1
    cout << t.search("apps") << endl;     // 0
    cout << t.startsWith("ap") << endl;   // 1

    vector<string> res = t.autoComplete("app", 3);
    for(auto &s : res)
        cout << s << " ";
    cout << endl;

    return 0;
}