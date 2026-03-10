#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <stdexcept>

using namespace std;

//Utility Template//
template <typename T>
T toLowerCase(T s) {
    for (char &c : s)
        c = tolower(c);
    return s;
}

//Abstract Base Class//
class FileReader {
public:
    virtual bool readChunk(string &chunk) = 0;
    virtual ~FileReader() {}
};

//Buffered File Reader//
class BufferedFileReader : public FileReader {
private:
    ifstream file;
    size_t bufferSize;

public:
    BufferedFileReader(const string &path, size_t bufferKB) {
        bufferSize = bufferKB * 1024;
        file.open(path);
        if (!file)
            throw runtime_error("Failed to open file");
    }

    bool readChunk(string &chunk) override {
        chunk.clear();
        if (file.eof()) return false;

        chunk.resize(bufferSize);
        file.read(&chunk[0], bufferSize);
        chunk.resize(file.gcount());

        return !chunk.empty();
    }

    ~BufferedFileReader() {
        if (file.is_open()) file.close();
    }
};

//Tokenizer//
class Tokenizer {
private:
    string carry;

public:
    vector<string> tokenize(const string &data) {
        vector<string> tokens;
        string current = carry;
        carry.clear();

        for (char c : data) {
            if (isalnum(c)) {
                current += tolower(c);
            } else {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            }
        }

        carry = current;
        return tokens;
    }

    string flush() {
        string last = carry;
        carry.clear();
        return last;
    }
};

//Versioned Index//
class VersionedIndexer {
private:
    unordered_map<string, unordered_map<string, long long>> versions;

public:
    void addWord(const string &version, const string &word) {
        versions[version][word]++;
    }

    long long getWordCount(const string &version, const string &word) {
        return versions[version][word];
    }

    vector<pair<string, long long>> getTopK(const string &version, int k) {
    vector<pair<string, long long>> v(
        versions[version].begin(),
        versions[version].end()
    );

    sort(v.begin(), v.end(),
         [](const auto &a, const auto &b) {
             return a.second > b.second;
         });

    if ((int)v.size() > k)
        v.resize(k);

    return v;
    }
};

//Query Processor//
class QueryProcessor {
private:
    VersionedIndexer &indexer;

public:
    QueryProcessor(VersionedIndexer &idx) : indexer(idx) {}

    void wordQuery(const string &version, const string &word) {
        cout << "Version: " << version << "\n";
        cout << "Word count: "
             << indexer.getWordCount(version, toLowerCase(word)) << endl;
    }

    void diffQuery(const string &v1, const string &v2, const string &word) {
        long long c1 = indexer.getWordCount(v1, toLowerCase(word));
        long long c2 = indexer.getWordCount(v2, toLowerCase(word));
        cout << "Difference (v2 - v1): " << (c2 - c1) << endl;
    }

    void topKQuery(const string &version, int k) {
        auto res = indexer.getTopK(version, k);
        cout << "Top-"<< k <<" words in version "<< version <<": " << "\n";
        for (auto &p : res)
            cout << p.first << " -> " << p.second << endl;
    }
};

//Index Builder//
void buildIndex(const string &filePath,
                const string &version,
                size_t bufferKB,
                VersionedIndexer &indexer) {

    BufferedFileReader reader(filePath, bufferKB);
    Tokenizer tokenizer;
    string chunk;

    while (reader.readChunk(chunk)) {
        auto tokens = tokenizer.tokenize(chunk);
        for (auto &w : tokens)
            indexer.addWord(version, w);
    }

    string last = tokenizer.flush();
    if (!last.empty())
        indexer.addWord(version, last);
}

//Main//
int main(int argc, char *argv[]) {
    try {
        string file, file1, file2;
        string version, version1, version2;
        string query, word;
        int bufferKB = 256, topK = 0;

        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "--file") file = argv[++i];
            else if (arg == "--file1") file1 = argv[++i];
            else if (arg == "--file2") file2 = argv[++i];
            else if (arg == "--version") version = argv[++i];
            else if (arg == "--version1") version1 = argv[++i];
            else if (arg == "--version2") version2 = argv[++i];
            else if (arg == "--buffer") bufferKB = stoi(argv[++i]);
            else if (arg == "--query") query = argv[++i];
            else if (arg == "--word") word = argv[++i];
            else if (arg == "--top") topK = stoi(argv[++i]);
        }

        auto start = chrono::high_resolution_clock::now();

        VersionedIndexer indexer;

        if (!file.empty())
            buildIndex(file, version, bufferKB, indexer);

        if (!file1.empty() && !file2.empty()) {
            buildIndex(file1, version1, bufferKB, indexer);
            buildIndex(file2, version2, bufferKB, indexer);
        }

        QueryProcessor qp(indexer);
        
        

        if (query == "word")
            qp.wordQuery(version, word);
        else if (query == "diff")
            qp.diffQuery(version1, version2, word);
        else if (query == "top")
            qp.topKQuery(version, topK);

        auto end = chrono::high_resolution_clock::now();
        cout << "Buffer size: " << bufferKB << " KB\n";
        cout << "Execution time: "
             << chrono::duration<double>(end - start).count()
             << " seconds\n";
    }
    catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
